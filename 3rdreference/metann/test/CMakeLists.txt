project(test_MetaNN LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/cmake/Utils.cmake)

GET_ALL_SUB_DIR_NAME(PROJECT_SOURCE_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SOURCE)
foreach (PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR_LIST})
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_SOURCE_DIR} TEST_SOURCE)
endforeach ()

addExecutable(${PROJECT_NAME} "${TEST_SOURCE}")
target_include_directories(${PROJECT_NAME} PRIVATE ${MetaNN_INCLUDE_DIR})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
