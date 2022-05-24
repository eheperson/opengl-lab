
add_executable(hellogl hellogl.cpp)

list(APPEND EXECUTABLES hellogl)


set(EXTRA_INCLUDE_DIRS "${EXTRA_INCLUDE_DIRS}" PARENT_SCOPE)
set(EXTRA_LIB_DIRS "${EXTRA_LIB_DIRS}" PARENT_SCOPE)
set(EXTRA_LINKS "${EXTRA_LINKS}" PARENT_SCOPE)

target_include_directories(${EXECUTABLES} PUBLIC ${EXTRA_INCLUDE_DIRS}) 

target_include_directories(${EXECUTABLES} PUBLIC ${EXTRA_INCLUDE_DIRS}) 
target_link_directories(${EXECUTABLES} PUBLIC ${EXTRA_LIB_DIRS})
target_link_libraries(${EXECUTABLES} PUBLIC ${EXTRA_LINKS})

# install(TARGETS ${MODULE_1} DESTINATION lib)
# install(FILES ${MODULES_DIR}/${MODULE_1}/${MODULE_1}.h DESTINATION include)
