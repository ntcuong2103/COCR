#include "view3d_widget.h"
#include "chem/jmol_manager.hpp"
#include "chem/jmol.hpp"
#include "3d/entity_base.hpp"
#include "ui_view3d_widget.h"
#include "mol3d_widget.hpp"
#include "format_dialog.h"

View3DWidget::View3DWidget(QWidget *parent)
        : QWidget(parent), ui(new Ui::View3DWidget) {
    ui->setupUi(this);

    BaseEntity::SetView3DWidget(this);

    auto formatVec = GetWritableFormats();
    for (auto &format:formatVec) {
        if (std::string::npos != format.find("Read-only")) { continue; }
        auto suffix = format.substr(0, format.find(" "));
        if (!xgd::JMol::IsValidWritableFormat(suffix)) { continue; }
        ui->format_box->addItem(QString::fromStdString(suffix));
    }
    connect(ui->format_box, &QComboBox::currentTextChanged, [&](const QString &formatName) {
        currentFormat = formatName.toStdString();
    });
    ui->format_box->setCurrentText("smi");

    connect(ui->format_btn, &QToolButton::clicked, this, &View3DWidget::showFormatDialog);
    connect(ui->expand_btn, &QToolButton::clicked, this, &View3DWidget::switchSuperAtomState);
    connect(ui->reset_btn, &QToolButton::clicked, this, &View3DWidget::reformatInputState);

    auto l = new QHBoxLayout();
    mol3DWidget = new Mol3DWidget(ui->container);
    l->addWidget(mol3DWidget);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);
    ui->container->setLayout(l);
}

View3DWidget::~View3DWidget() {
    delete ui;
}

void View3DWidget::syncMolToScene(std::shared_ptr<xgd::JMol> _mol) {
    mol3DWidget->syncMolToScene(_mol);
}

void View3DWidget::startWaitHint() {
    mol3DWidget->startWaitHint();
}

void View3DWidget::onAtomPicked(const size_t &_aid) {
    ui->pick_edit->setText(mol3DWidget->makeAtomInfo(_aid));
}

void View3DWidget::onBondPicked(const size_t &_bid) {
    ui->pick_edit->setText(mol3DWidget->makeBondInfo(_bid));
}


void View3DWidget::showFormatDialog() {
    qDebug() << __FUNCTION__ << currentFormat.c_str();
    auto formatDialog = new FormatDialog(this);
    formatDialog->resize(size() * 0.8);
    formatDialog->setModal(true);
    formatDialog->setWindowTitle(QString::fromStdString(currentFormat));
    auto mol = xgd::JMolManager::GetInstance().getCurrentMol();
    if (mol) {
        try {
            QString content = QString::fromStdString(mol->writeAs(currentFormat));
            formatDialog->setFormatContent(content);
        } catch (...) {
            formatDialog->setFormatContent(tr("Format algorithm failed on your molecule"));
        }
    } else {
        formatDialog->setFormatContent(tr("No molecule found"));
    }
    formatDialog->exec();
}


void View3DWidget::switchSuperAtomState() {
    std::shared_ptr<xgd::JMol> mol;
    if (++expBtnClickTimes % 2) {
        mol = xgd::JMolManager::GetInstance().getFullHydrogenExpandedMol();
    } else {
        mol = xgd::JMolManager::GetInstance().getFullHydrogenInputMol();
    }
    syncMolToScene(mol);
}

void View3DWidget::reformatInputState() {
    auto mol = xgd::JMolManager::GetInstance().getCurrentMol();
    syncMolToScene(mol);
}
