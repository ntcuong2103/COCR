function(generateOpenBabelQrc OUTPUT_PATH)
    file(GLOB_RECURSE OPENBABEL_DATA_LIST RELATIVE ${DEV_ASSETS_DIR} ${DEV_ASSETS_DIR}/openbabel/**)

    set(OPENBABEL_QRC_BUFFER "<RCC>\n<qresource prefix=\"/\">\n")

    foreach (OPENBABEL_DATA ${OPENBABEL_DATA_LIST})
        string(REPLACE ${DEV_ASSETS_DIR}/ "" OPENBABEL_DATA_RELATIVE_PATH ${OPENBABEL_DATA})
        string(APPEND OPENBABEL_QRC_BUFFER "<file compress=\"9\" compress-algo=\"best\">")
        string(APPEND OPENBABEL_QRC_BUFFER ${OPENBABEL_DATA_RELATIVE_PATH})
        string(APPEND OPENBABEL_QRC_BUFFER "</file>\n")
    endforeach ()

    string(APPEND OPENBABEL_QRC_BUFFER "</qresource>\n</RCC>")

    if (EXISTS ${OUTPUT_PATH})
        file(READ ${OUTPUT_PATH} OLD_OPENBABEL_QRC_BUFFER)
        if (NOT ${OPENBABEL_QRC_BUFFER} STREQUAL ${OLD_OPENBABEL_QRC_BUFFER})
            file(WRITE ${OUTPUT_PATH} ${OPENBABEL_QRC_BUFFER})
        endif ()
    else ()
        file(WRITE ${OUTPUT_PATH} ${OPENBABEL_QRC_BUFFER})
    endif ()
    unset(OPENBABEL_DATA_LIST)
    unset(OPENBABEL_QRC_BUFFER)
    unset(OLD_OPENBABEL_QRC_BUFFER)
endfunction()