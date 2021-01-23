#include "mol3d.hpp"
#include "mol3d_builder.hpp"
#include "mol3dwindow.hpp"
#include "openbabel_util.hpp"
#include <QApplication>
#include <QWidget>
#include <Qt3DCore/QEntity>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN64
    _putenv("BABEL_DATADIR=C:/static/openbabel-3.1.1/data");
#endif
    QApplication a(argc, argv);
    auto molUtil = std::make_shared<MolUtilOpenBabelImpl>();
    //C1C(C=C)CC(C#C)C(C#C)C1
    auto mol = molUtil->fromFormat("C1C(C=C)CC(C#C)C(C#C)C1", "smi");
    auto rootEntity = new Qt3DCore::QEntity();
    auto molBuilder = new Mol3DBuilder(rootEntity, mol);
    if (!molBuilder->build())exit(-1);
    auto view = new Mol3DWindow(rootEntity);
    auto container = QWidget::createWindowContainer(view);
    container->showMaximized();
//    qDebug() << mol->atomsNum() << "," << mol->bondsNum();
//    std::cout << molUtil->getFormat(*mol, "pdb") << std::endl;
    return a.exec();
}