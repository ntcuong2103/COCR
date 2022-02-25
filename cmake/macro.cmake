# https://stackoverflow.com/questions/7787823
macro(GET_SUB_DIR_NAME RESULT CURRENT_PATH)
    file(GLOB CHILDREN RELATIVE ${CURRENT_PATH} ${CURRENT_PATH}/*)
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
    string(REGEX REPLACE "(.*\\/)(.*)(\\/)" "\\2" ${RESULT} ${CURRENT_PATH})
endmacro()