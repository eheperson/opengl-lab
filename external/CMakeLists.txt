#------------------------------------------------------------------------------------------------------------
# -- Some Extra Settings For Maintainability
set(SOURCES_DIR "${EXTERNAL_DIR}/sources")
#
set(SDL2_SOURCE_DIR "${SOURCES_DIR}/SDL")
set(SDL2_BINARY_DIR "${EXTERNAL_DIR}/sdl2")
#
# (Add more 3Rd dependency Step 1)
# set(EXAMPLE_DEPENDENCY_SOURCE_DIR "${SOURCES_DIR}/EXAMPLE_DEPENDENCY")
# set(EXAMPLE_DEPENDENCY_BINARY_DIR "${EXTERNAL_DIR}/example_dep")

#------------------------------------------------------------------------------------------------------------
# download all git submodules of they do not downloaded
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
#
#------------------------------------------------------------------------------------------------------------
# --- Check SDL2 Module
if(NOT EXISTS "${SDL2_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "The sdl2 submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
# --- Find SDL2 Builded Binaries
find_package(SDL2 REQUIRED PATHS "${SDL2_BINARY_DIR}" NO_DEFAULT_PATH)
if(SDL2_FOUND)
    # append in local scope
    list(APPEND EXTRA_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
    # list(APPEND EXTRA_LIB_DIRS ${xxxx})
    list(APPEND EXTRA_LINKS ${SDL2_LIBRARIES})
    # set in parent scope
    set(EXTRA_INCLUDE_DIRS "${EXTRA_INCLUDE_DIRS}" PARENT_SCOPE)
    # set(EXTRA_LIB_DIRS "${EXTRA_LIB_DIRS}" PARENT_SCOPE)
    set(EXTRA_LINKS "${EXTRA_LINKS}" PARENT_SCOPE)
    # status messages
    message(STATUS "SDL2 Libs : ${SDL2_LIBRARIES}")
    message(STATUS "SDL2 Include Dirs : ${SDL2_INCLUDE_DIRS}")
    # install 3Rd package
    install(FILES "${SDL2_BINARY_DIR}/bin/sdl2-config" DESTINATION bin)
    install(DIRECTORY "${SDL2_BINARY_DIR}/include/" DESTINATION include)
    install(DIRECTORY ${SDL2_BINARY_DIR}/lib/ DESTINATION lib
        REGEX "/cmake$" EXCLUDE
        REGEX "/pkgconfig$" EXCLUDE
    )    
else()
    message(FATAL_ERROR "no sdl2, sad day")
endif()
#
#------------------------------------------------------------------------------------------------------------
# --- Check EXAMPLE_DEPENDENCY Module : (Add more 3Rd dependency Step 2)
if(NOT EXISTS "${EXAMPLE_DEPENDENCY_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "The example dependency submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
# --- Find SDL2 Builded Binaries (Add more 3Rd dependency Step 3)
find_package(EXAMPLE_DEPENDENCY REQUIRED PATHS "${EXAMPLE_DEPENDENCY_BINARY_DIR}" NO_DEFAULT_PATH)
if(EXAMPLE_DEPENDENCY_FOUND)
    # append in local scope
    list(APPEND EXTRA_INCLUDE_DIRS ${EXAMPLE_DEPENDENCY_INCLUDE_DIRS})
    # list(APPEND EXTRA_LIB_DIRS ${xxxx})
    list(APPEND EXTRA_LINKS ${EXAMPLE_DEPENDENCY_LIBRARIES})
    # set in parent scope
    set(EXTRA_INCLUDE_DIRS "${EXTRA_INCLUDE_DIRS}" PARENT_SCOPE)
    # set(EXTRA_LIB_DIRS "${EXTRA_LIB_DIRS}" PARENT_SCOPE)
    set(EXTRA_LINKS "${EXTRA_LINKS}" PARENT_SCOPE)
    # status messages
    message(STATUS "EXAMPLE_DEPENDENCY Libs : ${EXAMPLE_DEPENDENCY_LIBRARIES}")
    message(STATUS "EXAMPLE_DEPENDENCY Include Dirs : ${EXAMPLE_DEPENDENCY_INCLUDE_DIRS}")
    # install 3Rd package
    install(FILES "${EXAMPLE_DEPENDENCY_BINARY_DIR}/bin/sdl2-config" DESTINATION bin)
    install(DIRECTORY "${EXAMPLE_DEPENDENCY_BINARY_DIR}/include/" DESTINATION include)
    install(DIRECTORY ${EXAMPLE_DEPENDENCY_BINARY_DIR}/lib/ DESTINATION lib
        REGEX "/cmake$" EXCLUDE
        REGEX "/pkgconfig$" EXCLUDE
    )    
else()
    message(FATAL_ERROR "no example dependency, sad day")
endif()