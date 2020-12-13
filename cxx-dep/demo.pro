QT += core gui quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 # console

# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    

msvc {
    RC_FILE = demo.rc
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8 /openmp
}

LIB_PREFIX = C:/static
BOOST_PREFIX = $${LIB_PREFIX}/boost1.74.0
OPENCV_PREFIX = $${LIB_PREFIX}/opencv4.5.0
RDKIT_PREFIX = $${LIB_PREFIX}/rdkit2020.09.1
TORCH_PREFIX = $${LIB_PREFIX}/libtorch1.7.0
COORDGEN_PREFIX = $${LIB_PREFIX}/coordgen1.4.2

INCLUDEPATH += $${LIB_PREFIX}/include\
                $${COORDGEN_PREFIX}/include \
                $${BOOST_PREFIX}/include/boost-1_74 \
                $${OPENCV_PREFIX}/include \
                $${RDKIT_PREFIX}/include/rdkit \
                $${RDKIT_PREFIX}/include \
                $${TORCH_PREFIX}/include \
                $${TORCH_PREFIX}/include/torch/csrc/api/include

LIBS += \
        $${BOOST_PREFIX}/lib/libboost_atomic-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_chrono-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_container-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_context-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_contract-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_coroutine-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_date_time-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_exception-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_fiber-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_filesystem-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_graph-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_iostreams-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_locale-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_log_setup-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_log-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_math_c99f-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_math_c99l-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_math_c99-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_math_tr1f-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_math_tr1l-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_math_tr1-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_nowide-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_prg_exec_monitor-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_program_options-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_random-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_regex-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_serialization-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_stacktrace_noop-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_stacktrace_windbg_cached-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_stacktrace_windbg-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_system-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_test_exec_monitor-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_thread-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_timer-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_type_erasure-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_unit_test_framework-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_wave-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_wserialization-vc142-mt-s-x64-1_74.lib \
        $${BOOST_PREFIX}/lib/libboost_zlib-vc142-mt-s-x64-1_74.lib \
        $${TORCH_PREFIX}/lib/torch.lib  \
        $${TORCH_PREFIX}/lib/c10.lib  \  
        $${TORCH_PREFIX}/lib/torch_cpu.lib  \ 
        $${TORCH_PREFIX}/lib/clog.lib  \ 
        $${TORCH_PREFIX}/lib/cpuinfo.lib  \ 
        $${TORCH_PREFIX}/lib/dnnl.lib  \ 
        $${TORCH_PREFIX}/lib/libprotobuf.lib  \ 
        $${TORCH_PREFIX}/lib/libprotobuf-lite.lib  \ 
        $${TORCH_PREFIX}/lib/libprotoc.lib  \ 
        $${TORCH_PREFIX}/lib/mkldnn.lib  \ 
        $${RDKIT_PREFIX}/lib/Abbreviations.lib \
        $${RDKIT_PREFIX}/lib/Alignment.lib \
        $${RDKIT_PREFIX}/lib/Catalogs.lib \
        $${RDKIT_PREFIX}/lib/ChemicalFeatures.lib \
        $${RDKIT_PREFIX}/lib/ChemReactions.lib \
        $${RDKIT_PREFIX}/lib/ChemTransforms.lib \
        $${RDKIT_PREFIX}/lib/CIPLabeler.lib \
        $${RDKIT_PREFIX}/lib/DataStructs.lib \
        $${RDKIT_PREFIX}/lib/Depictor.lib \
        $${RDKIT_PREFIX}/lib/Deprotect.lib \
        $${RDKIT_PREFIX}/lib/Descriptors.lib \
        $${RDKIT_PREFIX}/lib/DistGeometry.lib \
        $${RDKIT_PREFIX}/lib/DistGeomHelpers.lib \
        $${RDKIT_PREFIX}/lib/EigenSolvers.lib \
        $${RDKIT_PREFIX}/lib/FileParsers.lib \
        $${RDKIT_PREFIX}/lib/FilterCatalog.lib \
        $${RDKIT_PREFIX}/lib/Fingerprints.lib \
        $${RDKIT_PREFIX}/lib/FMCS.lib \
        $${RDKIT_PREFIX}/lib/ForceField.lib \
        $${RDKIT_PREFIX}/lib/ForceFieldHelpers.lib \
        $${RDKIT_PREFIX}/lib/FragCatalog.lib \
        $${RDKIT_PREFIX}/lib/GraphMol.lib \
        $${RDKIT_PREFIX}/lib/hc.lib \
        $${RDKIT_PREFIX}/lib/InfoTheory.lib \
        $${RDKIT_PREFIX}/lib/MMPA.lib \
        $${RDKIT_PREFIX}/lib/MolAlign.lib \
        $${RDKIT_PREFIX}/lib/MolCatalog.lib \
        $${RDKIT_PREFIX}/lib/MolChemicalFeatures.lib \
        $${RDKIT_PREFIX}/lib/MolDraw2D.lib \
        $${RDKIT_PREFIX}/lib/MolEnumerator.lib \
        $${RDKIT_PREFIX}/lib/MolHash.lib \
        $${RDKIT_PREFIX}/lib/MolStandardize.lib \
        $${RDKIT_PREFIX}/lib/MolTransforms.lib \
        $${RDKIT_PREFIX}/lib/O3AAlign.lib \
        $${RDKIT_PREFIX}/lib/Optimizer.lib \
        $${RDKIT_PREFIX}/lib/PartialCharges.lib \
        $${RDKIT_PREFIX}/lib/RDGeneral.lib \
        $${RDKIT_PREFIX}/lib/RDGeometryLib.lib \
        $${RDKIT_PREFIX}/lib/RDStreams.lib \
        $${RDKIT_PREFIX}/lib/ReducedGraphs.lib \
        $${RDKIT_PREFIX}/lib/RGroupDecomposition.lib \
        $${RDKIT_PREFIX}/lib/ScaffoldNetwork.lib \
        $${RDKIT_PREFIX}/lib/ShapeHelpers.lib \
        $${RDKIT_PREFIX}/lib/SimDivPickers.lib \
        $${RDKIT_PREFIX}/lib/SmilesParse.lib \
        $${RDKIT_PREFIX}/lib/Subgraphs.lib \
        $${RDKIT_PREFIX}/lib/SubstructLibrary.lib \
        $${RDKIT_PREFIX}/lib/SubstructMatch.lib \
        $${RDKIT_PREFIX}/lib/TautomerQuery.lib \
        $${RDKIT_PREFIX}/lib/Trajectory.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/opencv_world450.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/opencv_img_hash450.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/IlmImf.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/libopenjp2.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/libwebp.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/zlib.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/libpng.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/ittnotify.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/libprotobuf.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/libjpeg-turbo.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/libtiff.lib \
        $${OPENCV_PREFIX}/x64/vc16/staticlib/quirc.lib \
        $${COORDGEN_PREFIX}/lib/coordgen.lib

INCLUDEPATH += C:/Users/xgd/source/COCR/cxx/src

HEADERS += C:/Users/xgd/source/COCR/cxx/src/atom.hpp \
        C:/Users/xgd/source/COCR/cxx/src/isomer.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_classifier.hpp \
        C:/Users/xgd/source/COCR/cxx/src/bignumber.hpp \
        C:/Users/xgd/source/COCR/cxx/src/mol.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_crnn.hpp \
        C:/Users/xgd/source/COCR/cxx/src/bond.hpp \
        C:/Users/xgd/source/COCR/cxx/src/opencv_util.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_enet.hpp \
        C:/Users/xgd/source/COCR/cxx/src/cocr_types.hpp \
        C:/Users/xgd/source/COCR/cxx/src/polya.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_module.hpp \
        C:/Users/xgd/source/COCR/cxx/src/config.hpp \
        C:/Users/xgd/source/COCR/cxx/src/qml_sketchitem.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_mv3.hpp \
        C:/Users/xgd/source/COCR/cxx/src/couch_data.hpp \
        C:/Users/xgd/source/COCR/cxx/src/qt_util.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_transform.hpp \
        C:/Users/xgd/source/COCR/cxx/src/dataset_cifar.hpp \
        C:/Users/xgd/source/COCR/cxx/src/rdkit_util.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_util.hpp \
        C:/Users/xgd/source/COCR/cxx/src/dataset_couch.hpp \
        C:/Users/xgd/source/COCR/cxx/src/solver.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_yolov4.hpp \
        C:/Users/xgd/source/COCR/cxx/src/fraction.hpp \
        C:/Users/xgd/source/COCR/cxx/src/std_util.hpp \
        C:/Users/xgd/source/COCR/cxx/src/xxxxx.hpp \
        C:/Users/xgd/source/COCR/cxx/src/hw.hpp \
        C:/Users/xgd/source/COCR/cxx/src/timer.hpp \
        C:/Users/xgd/source/COCR/cxx/src/hw_mol.hpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_activation.hpp

SOURCES += C:/Users/xgd/source/COCR/cxx/src/atom.cpp \
        C:/Users/xgd/source/COCR/cxx/src/isomer.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_classifier.cpp \
        C:/Users/xgd/source/COCR/cxx/src/bignumber.cpp \
        C:/Users/xgd/source/COCR/cxx/src/mol.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_crnn.cpp \
        C:/Users/xgd/source/COCR/cxx/src/bond.cpp \
        C:/Users/xgd/source/COCR/cxx/src/opencv_util.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_enet.cpp \
        C:/Users/xgd/source/COCR/cxx/src/cocr_types.cpp \
        C:/Users/xgd/source/COCR/cxx/src/polya.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_module.cpp \
        C:/Users/xgd/source/COCR/cxx/src/config.cpp \
        C:/Users/xgd/source/COCR/cxx/src/qml_sketchitem.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_mv3_large.cpp \
        C:/Users/xgd/source/COCR/cxx/src/couch_data.cpp \
        C:/Users/xgd/source/COCR/cxx/src/qt_util.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_mv3_small.cpp \
        C:/Users/xgd/source/COCR/cxx/src/dataset_cifar.cpp \
        C:/Users/xgd/source/COCR/cxx/src/rdkit_util.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_transform.cpp \
        C:/Users/xgd/source/COCR/cxx/src/dataset_couch.cpp \
        C:/Users/xgd/source/COCR/cxx/src/std_util.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_util.cpp \
        C:/Users/xgd/source/COCR/cxx/src/hw.cpp \
        C:/Users/xgd/source/COCR/cxx/src/timer.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_yolov4.cpp \
        C:/Users/xgd/source/COCR/cxx/src/hw_mol.cpp \
        C:/Users/xgd/source/COCR/cxx/src/torch_activation.cpp \
        C:/Users/xgd/source/COCR/cxx/core/main.cpp

RESOURCES += qml.qrc \
            demo.qrc \
            res/res.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
