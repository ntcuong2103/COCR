#ifndef MAIN_TAB_WIDGET_H
#define MAIN_TAB_WIDGET_H

#include <QWidget>

namespace Ui {
    class MainTabWidget;
}
class WelcomeWidget;

class PaintWidget;

class View2DWidget;

class View3DWidget;

class ImageWidget;

class CameraWidget;

class OCRThread;

class QHBoxLayout;

class MainTabWidget : public QWidget {
Q_OBJECT

public:
    explicit MainTabWidget(QWidget *parent = nullptr);

    ~MainTabWidget();

private:
    Ui::MainTabWidget *ui;
    WelcomeWidget *welcomeWidget;
    PaintWidget *paintWidget;
    View2DWidget *view2DWidget;
    View3DWidget *view3DWidget;
    ImageWidget *imageWidget;
    CameraWidget *cameraWidget;
    OCRThread *ocrThread;
    bool is2DLastUsed;
    bool isMolLatest;
    bool isOCRBtnClicked;
    bool isAgreementChecked;
    bool isFromDraw;
    enum class DataSource {
        CAMERA, IMAGE, PAINT
    } lastSource;

    void setRecentlyUsedViewer(bool is2D);

    const char *KEY_IS_2D_LAST_USED = "main_tab_widget/is_2d_last_used";
protected:
    void resizeEvent(QResizeEvent *e) override;

    void closeEvent(QCloseEvent *e) override;

private slots:

    void setAgreementChecked(bool isChecked);

    void handleTabChange(int index);

    void onOcrJobReady();

    void doOCR(const QList<QList<QPointF>> &_script);

    void doOCR(const QImage &_image);

private:
    // 2D、3D 窗体会存在大量图元，采取识别时构造、后台时析构调的策略
    QHBoxLayout *v2dLayout, *v3dLayout, *camLayout;

    void safeAttach2DWidget();

    void safeDelete2DWidget();

    void safeAttach3DWidget();

    void safeDelete3DWidget();

    // 拍照窗体存在跨平台兼容问题，也是懒加载
    void safeAttachCamWidget();

    void safeDeleteCamWidget();
};

#endif // MAIN_TAB_WIDGET_H