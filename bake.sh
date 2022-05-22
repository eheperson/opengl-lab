#!/bin/bash

rootDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


buildDir="${rootDir}/out/build"
installDir="${rootDir}/out/app"
cmakeCache="${buildDir}/CmakeCache.txt"


# clear cmake cache before configure
if [ -f ${cmakeCache} ]; then
    rm ${cmakeCache}
fi

# delete and re-create build directory at every time
if [ ! -d ${buildDir} ]; then
  mkdir ${buildDir}
# else
#   rm -r ${buildDir}
#   mkdir ${buildDir}
fi

# delete and re-create install directory at every time
if [ ! -d ${installDir} ]; then
  mkdir ${installDir}
else
  rm -r ${installDir}
  mkdir ${installDir}
fi

# -----------------------------------
#       CONFIGURE
# -----------------------------------
echo "" && echo " Configure Begin" &&echo ""

# alternatives
# cmake -G "MinGW Makefiles" -DUSE_EXAMPLELIB=ON -S ${rootDir} -B ${buildDir}

# cmake -DUSE_ADDER=OFF -S . -B ${buildDir}
cmake -DCMAKE_INSTALL_PREFIX=${installDir} -S ${rootDir} -B ${buildDir}

echo "" && echo " Configure End" && echo ""


# -----------------------------------
#       BUILD
# -----------------------------------
echo "" && echo " Build Begin" && echo ""

# alternatives
# cd ${buildDir} && make && cd ${rootDir}
# cd ${buildDir} && mingw32-make && cd ${rootDir}

cd ${buildDir} && cmake --build . -v && cd ${rootDir}

# if cmake version below than 3.13 use below
# cd ${buildDir} && ./configure --prefix=${installDir} && make && cd ${rootDir}

echo "" && echo " Build End" && echo ""

# -----------------------------------
#        INSTALL
# -----------------------------------
echo "" && echo " Install Begin" && echo ""

# cd ${buildDir} && cmake --install . --prefix ${installDir} && cd ${rootDir}
cd ${buildDir} && cmake --install . && cd ${rootDir}
# if cmake version below than 3.13 use below
# cd ${buildDir} && sudo make install  && cd ${rootDir}

echo "" && echo " Install End " && echo ""

# -----------------------------------
#        PACKING
# -----------------------------------
echo "" && echo " Packing Begin" && echo ""
#
cd ${buildDir}  && cpack -C Debug --verbose cd ${rootDir}
# To create an archive of the full source tree you would type:
        # cpack --config CPackSourceConfig.cmake
echo "" && echo " Packing End " && echo ""