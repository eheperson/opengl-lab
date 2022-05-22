option(USE_MODULE_1 "Build module 1" ON)
#
if(USE_MODULE_1)
    # To make use  of the new library we will add an add_subdirectory() call
    # add_subdirectory(${MODULE_1})
    message(STATUS "Module 1 Found")
    add_definitions(-DUSE_MODULE_1)
    #
    set(MODULE_1 TestLib)
    set(EXECUTABLE_FILE_1 TestLib/greetings.cxx)
    #
    add_library(${MODULE_1} ${EXECUTABLE_FILE_1})
    # link test module to find 3Rd dependencies
    target_include_directories(${MODULE_1} PUBLIC ${EXTRA_INCLUDE_DIRS}) 
    target_link_directories(${MODULE_1} PUBLIC ${EXTRA_LIB_DIRS})
    target_link_libraries(${MODULE_1} PUBLIC ${EXTRA_LINKS})
    #
    # append in local scope
    list(APPEND EXTRA_INCLUDE_DIRS "${MODULES_DIR}/${MODULE_1}")
    list(APPEND EXTRA_LIB_DIRS "${MODULE_1}")
    list(APPEND EXTRA_LINKS ${MODULE_1})
    # set in parent scope
    set(EXTRA_INCLUDE_DIRS "${EXTRA_INCLUDE_DIRS}" PARENT_SCOPE)
    set(EXTRA_LIB_DIRS "${EXTRA_LIB_DIRS}" PARENT_SCOPE)
    set(EXTRA_LINKS "${EXTRA_LINKS}" PARENT_SCOPE)
    #
    install(TARGETS ${MODULE_1} DESTINATION lib)
    install(FILES ${MODULES_DIR}/${MODULE_1}/${MODULE_1}.h DESTINATION include)
endif()

# add new modules below