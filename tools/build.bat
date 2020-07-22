cd 3rdparty/rapidjson && git checkout v1.1.0
cd ../..
mkdir lib\rapidjson build\rapidjson
cd build\rapidjson
cmake ../../3rdparty/rapidjson -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/rapidjson -DBUILD_TESTING=OFF -DRAPIDJSON_BUILD_TESTS=OFF -DRAPIDJSON_BUILD_DOC=OFF -DRAPIDJSON_BUILD_EXAMPLES=OFF
cmake --build . -j 8 --target install --config Release
cd ../..


cd 3rdparty/eigen && git checkout 3.3.7
cd ../..
mkdir lib\eigen build\eigen
cd build\eigen
cmake ../../3rdparty/eigen -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/eigen -DBUILD_TESTING=OFF
cmake --build . -j 8 --target install --config Release
cd ../..


cd 3rdparty/json && git checkout v3.7.3
cd ../..
mkdir lib\json build\json
cd build\json
cmake ../../3rdparty/json -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/json
cmake --build . -j 8 --target install --config Release
cd ../..


cd 3rdparty/FunctionalPlus && git checkout v0.2.70
cd ../..
mkdir lib\FunctionalPlus build\FunctionalPlus
cd build\FunctionalPlus
cmake ../../3rdparty/FunctionalPlus -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/FunctionalPlus
cmake --build . -j 8 --target install --config Release
cd ../..


cd 3rdparty/frugally-deep && git checkout v0.14.30
cd ../..
mkdir lib\frugally-deep build\frugally-deep
cd build\frugally-deep
cmake ../../3rdparty/frugally-deep -DFunctionalPlus_DIR=../../lib/FunctionalPlus/lib/cmake/FunctionalPlus -Dnlohmann_json_DIR=../../lib/json/lib/cmake/nlohmann_json -DEigen3_DIR=../../lib/eigen/share/eigen3/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/frugally-deep
cmake --build . -j 8 --target install --config Release
cd ../..


cd 3rdparty/zlib && git checkout v1.2.11
cd ../..
mkdir lib\zlib build\zlib
cd build\zlib
cmake ../../3rdparty/zlib -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/zlib -DBUILD_TESTING=OFF
cmake --build . -j 8 --target install --config Release
cd ../..


cd 3rdparty/libxml2 && git checkout 1e7851b
cd ../..
mkdir lib\libxml2 build\libxml2
cd build\libxml2
cmake ../../3rdparty/libxml2 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/libxml2 -DBUILD_TESTING=OFF
cmake --build . -j 8 --target install --config Release
cd ../..


REM MSVC下为了照顾OpenBabel，统一做静态编译，Qt源码静态编译命令参考如下：
REM ..\qt-everywhere-src-5.15.0\configure.bat -static -prefix "path to/COCR/lib/Qt/5.15.0/msvc2019_64" -confirm-license -opensource -debug-and-release -platform win32-msvc -nomake examples -nomake tests -mp
cd 3rdparty/openbabel && git checkout openbabel-3-1-1
cd ../..
mkdir lib\openbabel build\openbabel
cd build\openbabel
cmake ../../3rdparty/openbabel -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/openbabel -DBUILD_TESTING=OFF -DRAPIDJSON_INCLUDE_DIRS=../../lib/rapidjson/include/rapidjson -DBUILD_GUI=OFF -DWITH_INCHI=OFF -DWITH_STATIC_INCHI=OFF -DOPENBABEL_USE_SYSTEM_INCHI=OFF -DWITH_MAEPARSER=OFF -DZLIB_LIBRARY_RELEASE=../../lib/zlib/lib/zlib.lib -DZLIB_INCLUDE_DIR=../../lib/zlib/include -DLIBXML2_INCLUDE_DIR=../../lib/libxml2/include/libxml2 -DLIBXML2_LIBRARY=../../lib/libxml2/lib/libxml2.lib -DWITH_STATIC_LIBXML=OFF -DBUILD_SHARED=OFF
cmake --build . -j 8 --target install --config Debug && move ..\..\lib\openbabel\bin\openbabel-3.lib ..\..\lib\openbabel\bin\openbabel-3d.lib
cmake --build . -j 8 --target install --config Release
cd ../..


cd 3rdparty/opencv && git checkout 4.3.0
cd ../..
mkdir lib\opencv build\opencv
cd build\opencv
cmake ../../3rdparty/opencv -DCMAKE_INSTALL_PREFIX=../../lib/opencv -DBUILD_opencv_python_tests=OFF -DBUILD_opencv_python_bindings_generator=OFF -DBUILD_JAVA=OFF -DBUILD_opencv_java_bindings_generator=OFF -DBUILD_TESTS=OFF -DBUILD_PREF_TESTS=OFF -DWITH_ADE=OFF -DWITH_FFMPEG=OFF -DWITH_IPP=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_opencv_world=OFF
cmake --build . -j 8 --target install --config Debug
cmake --build . -j 8 --target install --config Release
cd ../..


REM config gtest
cd 3rdparty/googletest && git checkout master && cd ../..
mkdir -p lib\googletest build\googletest && cd build\googletest
cmake ../../3rdparty/googletest -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/googletest
cmake --build . -j 8 --target install --config Release && cd ../..
