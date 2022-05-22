# RPP-P (Rec-Play-Pause Pico)
 Template repository for cross-platform cpp development based on CMake build system Resources

> **NOTE :** This repository is tested only on MacOS and debian based Linux Distributions. Will be updated for Windows in the future.

### Who is this repository for?
* If project based on CMakeLists build system.
* If project contains only few source files like `main.cpp`, `mylib.cpp` etc.
* If your project requires 3Rd dependencies like `SDL2`.
* If project requires develop extra libraries  like `TestLib.h`. add if them requires 3Rd dependencies.
* If you need a C++ development laboratuary to test simple libraries requires 3Rd dependencies.

### Requirements:
* Cmake
* Make (optional)
* Git
* GNU Compiler Collection (gcc, g++ etc.)
* Any optional 3Rd dependency

### Extras : 
* `License.txt`, you can license your software by editing this file.

---

## Project Structure
### Zero Files/Dirs :
* **`main.cpp` file :** Your C++ file.
* **`rppnConfig.h.in` file :** Configuration file of the project.
* **`bake.sh` file :**  Baker file for tests.
* **`modules` directory :**  Contains your extra modules.
* **`external` directory :**  Contains your external dependencies source codes and binaries.

### Created Files/Dirs :
* **`out/app` directory :** Installation directory of the CMake
* **`out/build` directory :** Build output of the CMake 
* **`out/package` directory :** Package output of the CMake 

## Preparing Development Environment
```
    # clone the repo
    git clone git@github.com:eheperson/rpp-n.git

    # change access rights of compile.sh file
    chmod +x bake.sh
```

---

## Development

### Step Zero (Adding Mode 3Rd dependency): 

As a step zero, you must link your 3Rd dependency to the project. There are 2 different options here for that purpose:

* Get the builded binaries and link them.
* Get the source code, build the source code then, link the binaries.

Here are the steps of building 3Rd dependenc from source and link it 
(*Example to build and link SDL2 module*) :

1. If your 3Rd dependency on GitHub, you can add it easily as a git project submodule:
```
    cd ./external/sources
    git submodule add git@github.com:libsdl-org/SDL.git

    # or just use git clone command
    cd ./external/sources
    git clone git@github.com:libsdl-org/SDL.git
```

2. Create a build folder outside of the source code directory :
```
    # create build folder
    mkdir -p ../sdl2_build
```

3. Configure the 3Rd dependency to prepare it for cmake build:
```
    cd ./SDL

    # configure
    cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=../../sdl2 \
      -S . \
      -B ../sdl2_build 
```

4. Start build process of 3Rd dependency : 
```
    cd ../ # corresponds to ./external/sources

    # option 1
    cmake --build ./sdl2_build -v

    # option 2
    cd ./sdl2_build
    make -j7
```

4. Start install process of 3Rd dependency : 
```
    cd ../ # correspond to ./external directory
    
    # option 1
    cmake --install ./sources/sdl2_build --prefix sdl2

    # option 2 
    cd ./sources/sdl2_build
    make install
```

> If you  dont want build the 3Rd dependencies from source and have binaries of the 3Rd dependency, just create new folder in the `./external/` dir and put them here.

## Start to development (After the step zero)

* Use the `bake.sh` file for developement. 
* Purpose of the `bake.sh` file is handling all *build*, *install* and *packing* procedures in isolated directory named as `out/`.
* `CMAKE_INSTALL_PREFIX` option is modified to install targets in `/out` directory.

### Steps

1. Write your code.
2. Edit the `CMakeLists.txt` file for proper linking, compiling and installing.
3. Run the `bake.sh` file.

---

## Production

* After your development is done, you can follow the steps below to build and pack your application.

### 1. Configure : 

```
    # run this command at the root directory of the project
    cmake -S . -B ./out/build
```

### 2. Build : 

```
    # option 1 : building by cmake
    cd ./out/build && cmake --build . -v 

    # option 2 : building by make itself 
    # ( there is no any major difference between option1 and option 2)
    cd ./out/build && make 

    # alternatively you can use -j parameters with make command to speed up build process
    cd ./out/build && make -j7
```

### 3. Install : 

The project will configured to install common unix install directories like : `usr/slare/local`, `/usr/local` etc.

> you may require run install commands by `sudo`, because installing process will install your project to the one of the root directories.

```
    # option 1 :
    cd ./out/build && cmake --install .

    # option 2 : 
    cd ./out/build && make install
```

### 4. Packing : 

```
    cd ./out/build && cpack -C Release
```

## Adding more .cpp files :

In the `'CMakeLists.txt'` there are an example of adding a new executable file.
As a first step, create an executable like `'temp.cpp'`.

```
# Step 1 : adding executable (CMakeLists.txt line:56)
add_executable(temp temp.cpp)

# Step 2 : adding binary tree to the search path (CMakeLists.txt line:60)
# if you will not use rppnConfig.h in your executable, this step is unnecessary.
target_include_directories(temp PUBLIC "${PROJECT_BINARY_DIR}") 

# Step 3 : linking new executable with our modules (CMakeLists.txt line:77, line:78, line:79)
target_include_directories(temp PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(temp PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(temp PUBLIC ${EXTRA_LINKS})

# Step 4 : install the executable (CMakeLists.txt line:85)
install(TARGETS temp DESTINATION bin)
```

> Optionally, you can add new executable as a dependency of main.cpp like : 
> ```
> add_executable(${CMAKE_PROJECT_NAME} main.cpp temp.cpp)   
> ```

---

## Adding more modules :

Yo do not have to make any changes in `'./CMakeLists.txt'` file. go to the `'/modules'` directory and create new folder with your new module name first.


To the purpose of maintainability, test module renamed as `'${MODULE_1}'` in the `'modules/CMakeLists.txt'` file. If you want to add new module easily: 

* **Step 1 :** just use the existing lines on `'modules/CMakeLists.txt'`  by copy and paste, change the required fields then : 
```
    option(USE_MODULE_2 "Build module 2" ON)
    #
    if(USE_MODULE_2)
        # To make use  of the new library we will add an add_subdirectory() call
        # add_subdirectory(${MODULE_2})
        message(STATUS "Module 2 Found")
        add_definitions(-DUSE_MODULE_2)
        #
        set(MODULE_2 MyNewModule)
        set(EXECUTABLE_FILE_2 MyNewModule/myNewExecutable.cxx)
        #
        add_library(${MODULE_2} ${EXECUTABLE_FILE_2})
        # link test module to find 3Rd dependencies
        target_include_directories(${MODULE_2} PUBLIC ${EXTRA_INCLUDE_DIRS}) 
        target_link_directories(${MODULE_2} PUBLIC ${EXTRA_LIB_DIRS})
        target_link_libraries(${MODULE_2} PUBLIC ${EXTRA_LINKS})
        #
        # append in local scope
        list(APPEND EXTRA_INCLUDE_DIRS "${MODULES_DIR}/${MODULE_2}")
        list(APPEND EXTRA_LIB_DIRS "${MODULE_2}")
        list(APPEND EXTRA_LINKS ${MODULE_2})
        # set in parent scope
        set(EXTRA_INCLUDE_DIRS "${EXTRA_INCLUDE_DIRS}" PARENT_SCOPE)
        set(EXTRA_LIB_DIRS "${EXTRA_LIB_DIRS}" PARENT_SCOPE)
        set(EXTRA_LINKS "${EXTRA_LINKS}" PARENT_SCOPE)
        #
        install(TARGETS ${MODULE_2} DESTINATION lib)
        install(FILES ${MODULES_DIR}/${MODULE_2}/${MODULE_2}.h DESTINATION include)
    endif()
```

* **Step 3 :** be sure your new module structure looks like : 
``` 
    \MyNewModule
        MyNewModule.h
        myNewExecutable.cxx
```

## Adding more 3Rd dependencies:
* Check the `external/CMakeLists.txt` module, there are 3 implemented steps to add new 3Rd dependency.
* The procedure does not differs so much but linking steps could be trickey because of the finding dependency specific cmake variables like `SDL2_BINARY_DIR` and `EXTRA_INCLUDE_DIRS`. 
* Check the  `external/ext_dep/lib/cmake` directory of your `ext_deb` to spot the pre-defined cmake variables of your 3Rd dependency.
* If your source code does not have `CMakeLists.txt` file in it's root directory, do not implement step2, it will throw cmake `FATAL_ERROR`. 
* Purpose of the step 2 is that, check the added submodules if they are installed by checking the `CMakeLists.txt` file inside it.
## Extra Info:

* If your new module has more than 1 executables, you can link them like below,  Cmake will interpred them as dependencies.: 
```
    add_library(${MODULE_2} ${EXECUTABLE_FILE_2} {EXECUTABLE_FILE_4} {EXECUTABLE_FILE_5} ... )
```

* If your multiple files does not dependent, you can use this approach and you can import it then : 
```
    add_library(other_module other_executable.cpp)
    target_include_directories(other_module PUBLIC ${EXTRA_INCLUDE_DIRS}) 
    target_link_directories(other_module PUBLIC ${EXTRA_LIB_DIRS})
    target_link_libraries(other_module PUBLIC ${EXTRA_LINKS})
    install(TARGETS other_executable DESTINATION lib)
```

* You can add more than 1 header files also, just follow same steps and do not forget add new installation rule to the : 
    ```
        install(FILES ${MODULES_DIR}/${MODULE_2}/${MODULE_3}.h DESTINATION include)
    ```



