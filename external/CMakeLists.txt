#------------------------------------------------------------------------------------------------------------
# -- Some Extra Settings For Maintainability
set(SOURCES_DIR "${EXTERNAL_DIR}/sources")
#
set(GLFW_SOURCE_DIR "${SOURCES_DIR}/glfw")
set(GLFW_BINARY_DIR "${EXTERNAL_DIR}/glfw")
#
set(GLM_SOURCE_DIR "${SOURCES_DIR}/glm")
set(GLM_BINARY_DIR "${EXTERNAL_DIR}/glm")
#
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
#
# --- GLEW
#
#  find library
find_library(
    myglew glew 
    NAMES glew2 GLEW GLEW2 
    # PATHS "${CMAKE_CURRENT_SOURCE_DIR}/glew"
    # PATH_SUFFIXES "lib/Release/x64" "lib/Release/x86"   
    DOC "glew library"
)
if(${myglew} STREQUAL myglew-NOTFOUND)
# if(NOT ${myglew})
    message(FATAL_ERROR "no glew, sad day")
else()
    message(STATUS "glew found! as ${myglew}")
    # list(APPEND EXTRA_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/glew/include")
    list(APPEND EXTRA_LIB_DIRS ${myglew})
endif()
#
#------------------------------------------------------------------------------------------------------------
#
# --- OpenGL
#
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    # pass
else()
    message(FATAL_ERROR "no OpenGL, sad day")
endif()
#
#------------------------------------------------------------------------------------------------------------
#
# --- GLFW
#
if(NOT EXISTS "${GLFW_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
#
find_package(glfw3 REQUIRED PATHS "${GLFW_BINARY_DIR}" NO_DEFAULT_PATH)
#
if(glfw3_FOUND)
    message(STATUS "Found glfw3 ")
    # append in local scope
    list(APPEND EXTRA_LINKS glfw3)
    # set in parent scope
    set(EXTRA_LINKS "${EXTRA_LINKS}" PARENT_SCOPE)
else()
    message(FATAL_ERROR "no glfw3, sad day")
endif()
#
#------------------------------------------------------------------------------------------------------------
#
# --- GLM
#
if(NOT EXISTS "${GLM_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "The glm submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
#
find_package(glm REQUIRED PATHS "${GLM_BINARY_DIR}" NO_DEFAULT_PATH)
if(glm_FOUND)
    message(STATUS "GLM found at ${GLM_BINARY_DIR}")
    # append in local scope
    list(APPEND EXTRA_INCLUDE_DIRS "${GLM_BINARY_DIR}/include")
    # set in parent scope
    set(EXTRA_INCLUDE_DIRS "${EXTRA_INCLUDE_DIRS}" PARENT_SCOPE)
else()
    message(FATAL_ERROR "no glm, sad day")
endif()