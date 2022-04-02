# this file is used to config 3rdparty cmake options
set(NCNN_BUILD_BENCHMARK OFF CACHE INTERNAL "" FORCE)
set(NCNN_BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(NCNN_BUILD_TOOLS OFF CACHE INTERNAL "" FORCE)
set(NCNN_INSTALL_SDK OFF CACHE INTERNAL "" FORCE)
set(NCNN_SIMPLEOMP OFF CACHE INTERNAL "" FORCE)
# enable vulkan for android
if (ANDROID)
    set(NCNN_SYSTEM_GLSLANG OFF CACHE INTERNAL "" FORCE)
    set(NCNN_VULKAN ON CACHE INTERNAL "" FORCE)
else ()
    set(NCNN_VULKAN OFF CACHE INTERNAL "" FORCE)
endif ()
set(NCNN_SHARED_LIB ${BUILD_SHARED_LIBS} CACHE INTERNAL "" FORCE)
if (WASM)
    set(NCNN_THREADS ON CACHE INTERNAL "" FORCE)
    set(NCNN_SSE2 OFF CACHE INTERNAL "" FORCE)
else ()
    set(NCNN_THREADS ON CACHE INTERNAL "" FORCE)
endif ()