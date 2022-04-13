# https://stackoverflow.com/questions/7787823
macro(GET_SUB_DIR_NAME RESULT CURRENT_PATH)
    file(GLOB CHILDREN RELATIVE ${CURRENT_PATH} LIST_DIRECTORIES true ${CURRENT_PATH}/*)
    set(DIR_LIST "")
    foreach (CHILD ${CHILDREN})
        if (IS_DIRECTORY ${CURRENT_PATH}/${CHILD})
            list(APPEND DIR_LIST ${CHILD})
        endif ()
    endforeach ()
    set(${RESULT} ${DIR_LIST})
    unset(DIR_LIST)
    unset(CHILDREN)
endmacro()

macro(GET_ALL_SUB_DIR_NAME RESULT CURRENT_PATH)
    file(GLOB_RECURSE CHILDREN RELATIVE ${CURRENT_PATH} LIST_DIRECTORIES true ${CURRENT_PATH}/*)
    set(DIR_LIST "")
    foreach (CHILD ${CHILDREN})
        if (IS_DIRECTORY ${CURRENT_PATH}/${CHILD})
            list(APPEND DIR_LIST ${CHILD})
        endif ()
    endforeach ()
    set(${RESULT} ${DIR_LIST})
    unset(DIR_LIST)
    unset(CHILDREN)
endmacro()

macro(PATH_2_DIR_NAME RESULT CURRENT_PATH)
    string(REGEX REPLACE "(.*\\/)(.*)(\\/)" "\\2" ${RESULT} "${CURRENT_PATH}")
endmacro()

function(findQtModules)
    GET_SUB_DIR_NAME(QT_MODULE_DIR_LIST ${Qt5_DIR}/../)
    foreach (QT_MODULE ${QT_MODULE_DIR_LIST})
        set(${QT_MODULE}_DIR ${Qt5_DIR}/../${QT_MODULE} CACHE INTERNAL "")
    endforeach ()
endfunction()

function(writeFileIfChanged OUTPUT_PATH OUTPUT_BUFFER)
    if (EXISTS ${OUTPUT_PATH})
        file(READ ${OUTPUT_PATH} OLD_OUTPUT_BUFFER)
        if (NOT "${OUTPUT_BUFFER}" STREQUAL "${OLD_OUTPUT_BUFFER}")
            file(WRITE ${OUTPUT_PATH} "${OUTPUT_BUFFER}")
        endif ()
    else ()
        file(WRITE ${OUTPUT_PATH} "${OUTPUT_BUFFER}")
    endif ()
endfunction()

function(copyQtWasmTemplates TEMPLATE_ROOT APP_NAME OUTPUT_ROOT)
    file(GLOB WASM_TEMPLATES ${TEMPLATE_ROOT})
    add_custom_target(${APP_NAME}_wasm_template)
    add_dependencies(${APP_NAME} ${APP_NAME}_wasm_template)
    foreach (WASM_TEMPLATE ${WASM_TEMPLATES})
        get_filename_component(WASM_TEMPLATE_NAME ${WASM_TEMPLATE} NAME)
        if (${WASM_TEMPLATE_NAME} STREQUAL "index.html") # replace APP_NAME placeholder to real ${APP_NAME}
            file(READ ${WASM_TEMPLATE} OLD_INDEX_HTML_BUFFER)
            string(REPLACE "APP_NAME" "${APP_NAME}" INDEX_HTML_BUFFER "${OLD_INDEX_HTML_BUFFER}")
            writeFileIfChanged(${OUTPUT_ROOT}/${WASM_TEMPLATE_NAME} "${INDEX_HTML_BUFFER}")
        else () # just copy
            add_custom_command(TARGET ${APP_NAME}_wasm_template POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                    copy ${WASM_TEMPLATE} ${OUTPUT_ROOT})
        endif ()
    endforeach ()
endfunction()

function(writeQrcFileBySearchDirectory OUTPUT_PATH SEARCH_DIR RELATIVE_DIR)
    file(GLOB_RECURSE QRC_FILE_LIST RELATIVE ${RELATIVE_DIR} ${SEARCH_DIR}/**)

    set(QRC_BUFFER "<RCC>\n<qresource prefix=\"/\">\n")
    foreach (QRC_FILE ${QRC_FILE_LIST})
        string(APPEND QRC_BUFFER "<file compress=\"9\" compress-algo=\"best\">")
        string(APPEND QRC_BUFFER "${QRC_FILE}")
        string(APPEND QRC_BUFFER "</file>\n")
    endforeach ()
    string(APPEND QRC_BUFFER "</qresource>\n</RCC>")

    writeFileIfChanged(${OUTPUT_PATH} "${QRC_BUFFER}")
endfunction()

function(addExecutable TARGET_NAME TARGET_SOURCE)
    # include windows exe info
    if (MSVC)
        list(APPEND TARGET_SOURCE ${DEV_ASSETS_DIR}/windows/leafxy.rc)
    endif ()
    add_executable(${TARGET_NAME} ${TARGET_SOURCE})
    set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "d")
    if (MSVC)
        target_include_directories(${TARGET_NAME} PUBLIC ${DEV_ASSETS_DIR}/windows)
    endif ()
    target_include_directories(${TARGET_NAME} PRIVATE ${DEV_HEADER_DIR})
    target_compile_options(${TARGET_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/bigobj>)
endfunction()

function(addLibrary TARGET_NAME TARGET_SOURCE)
    # include windows dll info
    if (MSVC)
        list(APPEND TARGET_SOURCE ${DEV_ASSETS_DIR}/windows/3rd.rc)
    endif ()
    add_library(${TARGET_NAME} ${TARGET_SOURCE})
    set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "d")
    include(GenerateExportHeader)
    generate_export_header(${TARGET_NAME})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    if (MSVC)
        target_include_directories(${TARGET_NAME} PUBLIC ${DEV_ASSETS_DIR}/windows)
    endif ()
    target_include_directories(${TARGET_NAME} PRIVATE ${DEV_HEADER_DIR})
    target_compile_options(${TARGET_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/bigobj>)
endfunction()

macro(CHECK_OPENCV TARGET_NAME)
    if (NOT ${OpenCV_FOUND})
        message(WARNING "OpenCV not found, ${TARGET_NAME} will not be built")
        return()
    endif ()
endmacro()

macro(CHECK_CONTINUE_OPENCV TARGET_NAME)
    if (NOT ${OpenCV_FOUND})
        message(WARNING "OpenCV not found, ${TARGET_NAME} will not be built")
        continue()
    endif ()
endmacro()

function(linkOpenCV TARGET_NAME)
    target_link_libraries(${TARGET_NAME} PRIVATE ${OpenCV_LIBS})
    target_include_directories(${TARGET_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
endfunction()

macro(CHECK_LIBTORCH TARGET_NAME)
    if (NOT ${Torch_FOUND})
        message(WARNING "Torch not found, ${TARGET_NAME} will not be built")
        return()
    endif ()
endmacro()

macro(CHECK_CONTINUE_LIBTORCH TARGET_NAME)
    if (NOT ${Torch_FOUND})
        message(WARNING "Torch not found, ${TARGET_NAME} will not be built")
        continue()
    endif ()
endmacro()

function(linkLibtorch TARGET_NAME)
    target_link_libraries(${TARGET_NAME} PRIVATE ${TORCH_LIBRARIES})
    target_include_directories(${TARGET_NAME} PRIVATE ${TORCH_INCLUDE_DIRS})
endfunction()

macro(CHECK_BOOST TARGET_NAME)
    if (NOT ${Boost_FOUND})
        message(WARNING "Boost not found, ${TARGET_NAME} will not be built")
        return()
    endif ()
endmacro()

macro(CHECK_CONTINUE_BOOST TARGET_NAME)
    if (NOT ${Boost_FOUND})
        message(WARNING "Boost not found, ${TARGET_NAME} will not be built")
        continue()
    endif ()
endmacro()

function(linkBoost TARGET_NAME)
    target_link_directories(${TARGET_NAME} PRIVATE ${Boost_LIB_DIR})
    target_link_libraries(${TARGET_NAME} PRIVATE ${Boost_LIBRARIES})
    target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIR})
endfunction()

function(linkOpenBabel TARGET_NAME)
    add_dependencies(${TARGET_NAME} openbabel)
    target_link_libraries(${TARGET_NAME} PRIVATE openbabel)
    target_include_directories(${TARGET_NAME} PRIVATE ${openbabel_INCLUDE_DIR})
endfunction()

function(linkCoordgenlibs TARGET_NAME)
    add_dependencies(${TARGET_NAME} coordgenlibs)
    target_link_libraries(${TARGET_NAME} PRIVATE coordgenlibs)
    target_include_directories(${TARGET_NAME} PRIVATE ${coordgenlibs_INCLUDE_DIR})
endfunction()

function(linkMaeparser TARGET_NAME)
    add_dependencies(${TARGET_NAME} maeparser)
    target_link_libraries(${TARGET_NAME} PRIVATE maeparser)
    target_include_directories(${TARGET_NAME} PRIVATE ${maeparser_INCLUDE_DIR})
endfunction()

function(linkInchi TARGET_NAME)
    add_dependencies(${TARGET_NAME} inchi)
    target_link_libraries(${TARGET_NAME} PRIVATE inchi)
    target_include_directories(${TARGET_NAME} PRIVATE ${inchi_INCLUDE_DIR})
endfunction()

function(linkNcnn TARGET_NAME)
    add_dependencies(${TARGET_NAME} ncnn)
    target_link_libraries(${TARGET_NAME} PRIVATE ncnn)
    target_include_directories(${TARGET_NAME} PRIVATE ${ncnn_INCLUDE_DIR})
endfunction()

macro(CHECK_QT TARGET_NAME QT_COMPONENTS)
    foreach (QT_COMPONENT ${QT_COMPONENTS})
        if (NOT ${Qt5${QT_COMPONENT}_FOUND})
            message(WARNING "Qt5${QT_COMPONENT} not found, ${TARGET_NAME} will not be built")
            return()
        endif ()
    endforeach ()
endmacro()

macro(CHECK_CONTINUE_QT TARGET_NAME QT_COMPONENTS)
    foreach (QT_COMPONENT ${QT_COMPONENTS})
        if (NOT ${Qt5${QT_COMPONENT}_FOUND})
            message(WARNING "Qt5${QT_COMPONENT} not found, ${TARGET_NAME} will not be built")
            continue()
        endif ()
    endforeach ()
endmacro()

function(linkQt TARGET_NAME QT_COMPONENTS)
    foreach (QT_COMPONENT ${QT_COMPONENTS})
        target_link_libraries(${TARGET_NAME} PRIVATE Qt5::${QT_COMPONENT})
        target_include_directories(${TARGET_NAME} PRIVATE ${Qt5${QT_COMPONENT}_INCLUDE_DIRS})
    endforeach ()
endfunction()

function(linkOpenMP TARGET_NAME)
    if (ANDROID)
        target_link_libraries(${TARGET_NAME} PRIVATE -static-openmp)
    elseif (IOS OR APPLE)
        target_link_libraries(${TARGET_NAME} PRIVATE OpenMP::OpenMP_CXX)
    endif ()
endfunction()

function(makeLibrary DIRECTORY_NAME LIBRARY_NAME)
    unset(DIRS)
    unset(LIBRARY_SOURCE)
    GET_ALL_SUB_DIR_NAME(DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY_NAME}/src)
    foreach (DIR ${DIRS} .)
        aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY_NAME}/src/${DIR} LIBRARY_SOURCE)
    endforeach ()
    addLibrary(${LIBRARY_NAME} "${LIBRARY_SOURCE}")
    set(${LIBRARY_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY_NAME}/include CACHE INTERNAL "")
    target_include_directories(${LIBRARY_NAME} PRIVATE ${${LIBRARY_NAME}_INCLUDE_DIR})
    target_compile_definitions(${LIBRARY_NAME} PRIVATE
            "TEST_SAMPLES_PATH=\"${DEV_ASSETS_DIR}/\"")

    unset(DIRS)
    unset(TEST_LIBRARY_SOURCE)
    GET_ALL_SUB_DIR_NAME(DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY_NAME}/test)
    foreach (DIR ${DIRS} .)
        aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY_NAME}/test/${DIR} TEST_LIBRARY_SOURCE)
    endforeach ()
    addExecutable(test_${LIBRARY_NAME} "${TEST_LIBRARY_SOURCE}")
    target_include_directories(test_${LIBRARY_NAME} PRIVATE ${${LIBRARY_NAME}_INCLUDE_DIR})
    target_link_libraries(test_${LIBRARY_NAME} PRIVATE ${LIBRARY_NAME})
    target_compile_definitions(test_${LIBRARY_NAME} PRIVATE
            "TEST_SAMPLES_PATH=\"${DEV_ASSETS_DIR}/\"")
    add_test(NAME test_${LIBRARY_NAME} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_${LIBRARY_NAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

function(addLibraryDeps ORIGIN THIRD_PARTY)
    add_dependencies(${ORIGIN} ${THIRD_PARTY})
    target_link_libraries(${ORIGIN} PUBLIC ${THIRD_PARTY})
    target_include_directories(${ORIGIN} PUBLIC ${${THIRD_PARTY}_INCLUDE_DIR})
endfunction()