#define SCALE 2.0
#define PI radians(180.0)
#define TAU (PI*2.0)
#define CL(x,a,b) smoothstep(0.0,1.0,(2.0/3.0)*(x-a)/(b-a)+(1.0/6.0))*(b-a)+a // https://www.shadertoy.com/view/Ws3Xzr
const int CHARACTERS[14] = int[14](31599,9362,31183,31207,23524,29671,29679,30994,31727,31719,1488,448,2,3640); float digitIsOn( int digit, vec2 id ) { if ( id.x < .0 || id.y < .0 || id.x > 2. || id.y > 4. ) return .0; return floor( mod( float( CHARACTERS[ int( digit ) ] ) / pow( 2., id.x + id.y * 3. ), 2. ) ); } float digitSign( float v, vec2 id ) { return digitIsOn( 10 - int( ( sign( v ) - 1. ) * .5 ), id ); } int digitCount( float v ) { return int( floor( log( max( v, 1. ) ) / log( 10. ) ) ); } float digitFirst( vec2 uv, float scale, float v, int decimalPlaces ) { vec2 id = floor( uv * scale ); if ( .0 < digitSign( v, id ) ) return 1.; v = abs( v ); int digits = digitCount( v ); float power = pow( 10., float( digits ) ); float offset = floor( .1 * scale ); id.x -= offset; float n; for ( int i = 0 ; i < 33 ; i++, id.x -= offset, v -= power * n, power /= 10. ) { n = floor( v / power ); if ( .0 < digitIsOn( int( n ), id ) ) return 1.; if ( i == digits ) { id.x -= offset; if ( .0 < digitIsOn( int( 12 ), id ) ) return 1.; } if ( i >= digits + decimalPlaces ) return .0; } return .0; } float digitFirst( vec2 uv, float scale, float v ) { return digitFirst( uv, scale, v, 3 ); } vec3 digitIn( vec3 color, vec3 fontColor, vec2 uv, float scale, float v ) { float f = digitFirst( uv, scale, v ); return mix( color, fontColor, f ); } vec3 digitIn( vec3 color, vec2 uv, float scale, float v ) { return digitIn( color, vec3(1.), uv, scale, v ); } 
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 m = vec2((iMouse.xy-0.5*iResolution.xy)*2.0/iResolution.y);
    float st = radians(-31.0); // start time
    float t = (iMouse.z > 0.0) ? atan(m.x, -m.y): st+(iTime*TAU)/3600.0;
    float n = (cos(t) > 0.0) ? sin(t): 1.0/sin(t);
    float z = pow(500.0, n); // autozoom
    z = clamp(z, CL(z, 1e-16, 1e-15), CL(z, 1e+17, 1e+18)); // clamp to prevent black screen
    vec2 uv = (fragCoord-0.5*iResolution.xy)/iResolution.y*SCALE*z;
    float ls = (iTime*TAU)/5.0; // light animation speed
    float a = atan(uv.x, -uv.y); // screen arc
    float i = a/TAU; // spiral increment 0.5 per 180°
    float r = pow(length(uv), 0.5/n)-i; // archimedean at 0.5
    float cr = ceil(r); // round up radius
    float wr = cr+i; // winding ratio
    float vd = (cr*TAU+a) / (n*2.0); // visual denominator
    float vd2 = vd*2.0;
    vec3 col = vec3(sin(wr*vd+ls)); // blend it
    col *= pow(sin(fract(r)*PI), floor(abs(n*2.0))+5.0); // smooth edges
    col *= sin(vd2*wr+PI/2.0+ls*2.0); // this looks nice
    col *= 0.2+abs(cos(vd2)); // dark spirals
	vec3 g = mix(vec3(0), vec3(1), pow(length(uv)/z, -1.0/n)); // dark gradient
	col = min(col, g); // blend gradient with spiral
    vec3 rgb = vec3( cos(vd2)+1.0, abs(sin(t)), cos(PI+vd2)+1.0 );
    col += (col*2.0)-(rgb*0.5); // add color
    if (iMouse.z > 0.0) // on mouse click
    {
        uv = (fragCoord-0.5*iResolution.xy)/iResolution.y;
        col = digitIn(col, vec3(1), (uv*2.0)-m, 44.0, n); // show value
    }
    fragColor = vec4(col, 1.0);
} 