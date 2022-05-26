
add_executable(01-hellogl 01-hellogl.cpp)

set(EXECUTABLES 01-hellogl)

set(EXTRA_INCLUDE_DIRS "${EXTRA_INCLUDE_DIRS}" PARENT_SCOPE)
set(EXTRA_LIB_DIRS "${EXTRA_LIB_DIRS}" PARENT_SCOPE)
set(EXTRA_LINKS "${EXTRA_LINKS}" PARENT_SCOPE)

target_include_directories(${EXECUTABLES} PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(${EXECUTABLES} PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(${EXECUTABLES} PUBLIC  ${EXTRA_LINKS})
# target_link_libraries(${EXECUTABLES} PUBLIC OpenGL glfw GLEW  ${EXTRA_LINKS} ${CMAKE_DL_LIBS})

# install(TARGETS ${MODULE_1} DESTINATION lib)
# install(FILES ${MODULES_DIR}/${MODULE_1}/${MODULE_1}.h DESTINATION include)
