project(app_qml LANGUAGES CXX)
include(${CMAKE_SOURCE_DIR}/cmake/Utils.cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_COMPONENTS Core Quick)
find_package(Qt5 COMPONENTS ${QT_COMPONENTS} REQUIRED)

if (NOT BUILD_PRIVATE)
    set(QML_SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)
    writeQrcFileBySearchDirectory("${QML_SRC_FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/qml" "${CMAKE_CURRENT_SOURCE_DIR}")
else ()
    unset(QML_SRC_FILE)
endif ()

set(PROJECT_SOURCE ${QML_SRC_FILE})
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src PROJECT_SOURCE)

# add exe
if (ANDROID)
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCE})
else ()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})
endif ()

if (BUILD_PRIVATE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "QML_PREFIX=\"${CMAKE_CURRENT_SOURCE_DIR}/qml/\"")
else ()
    target_compile_definitions(${PROJECT_NAME} PRIVATE "QML_PREFIX=\"qrc:/qml/\"")
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${PROJECT_NAME}
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

unset(QT_COMPONENT_TARGETS)
foreach (QT_COMPONENT ${QT_COMPONENTS})
    list(APPEND QT_COMPONENT_TARGETS Qt5::${QT_COMPONENT})
endforeach ()

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_COMPONENT_TARGETS})

if (WASM)
    copyQtWasmTemplates("${CMAKE_CURRENT_SOURCE_DIR}/wasm/*" "${PROJECT_NAME}" "${CMAKE_CURRENT_BINARY_DIR}")
endif ()