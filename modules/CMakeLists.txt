set(EXTRA_INCLUDE_DIRS "${EXTRA_INCLUDE_DIRS}" PARENT_SCOPE)
set(EXTRA_LIB_DIRS "${EXTRA_LIB_DIRS}" PARENT_SCOPE)
set(EXTRA_LINKS "${EXTRA_LINKS}" PARENT_SCOPE)

add_executable(00-openglZero 00-openglZero.cpp)        
target_include_directories(00-openglZero PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(00-openglZero PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(00-openglZero PUBLIC  ${EXTRA_LINKS})

add_executable(01-hellogl 01-hellogl.cpp)        
target_include_directories(01-hellogl PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(01-hellogl PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(01-hellogl PUBLIC  ${EXTRA_LINKS})

add_executable(02-firstTriangle 02-firstTriangle.cpp)
target_include_directories(02-firstTriangle PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(02-firstTriangle PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(02-firstTriangle PUBLIC  ${EXTRA_LINKS})

add_executable(03-matricies 03-matricies.cpp)
target_include_directories(03-matricies PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(03-matricies PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(03-matricies PUBLIC  ${EXTRA_LINKS})

add_executable(03-matricies-2 03-matricies-2.cpp)
target_include_directories(03-matricies-2 PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(03-matricies-2 PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(03-matricies-2 PUBLIC  ${EXTRA_LINKS})

add_executable(04-coloredCube 04-coloredCube.cpp)
target_include_directories(04-coloredCube PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(04-coloredCube PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(04-coloredCube PUBLIC  ${EXTRA_LINKS})

add_executable(05-texturedCube 05-texturedCube.cpp)
target_include_directories(05-texturedCube PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(05-texturedCube PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(05-texturedCube PUBLIC  ${EXTRA_LINKS})

add_executable(06-controls 06-controls.cpp)
target_include_directories(06-controls PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(06-controls PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(06-controls PUBLIC  ${EXTRA_LINKS})

add_executable(07-objLoading 07-objLoading.cpp)
target_include_directories(07-objLoading PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(07-objLoading PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(07-objLoading PUBLIC  ${EXTRA_LINKS})

# target_link_libraries(${EXECUTABLES} PUBLIC OpenGL glfw GLEW  ${EXTRA_LINKS} ${CMAKE_DL_LIBS})

# install(TARGETS ${MODULE_1} DESTINATION lib)
# install(FILES ${MODULES_DIR}/${MODULE_1}/${MODULE_1}.h DESTINATION include)
