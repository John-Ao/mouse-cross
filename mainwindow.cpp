#include "mainwindow.h"

#include <QImage>
#include <QPixmap>

#include "ui_mainwindow.h"

int dpi;

template <class T>
int scale(T x) {
    return (int)x * 96 / dpi;
}

void MainWindow::drawCursor() {
    // 获取鼠标信息
    int cur_w, cur_h, cur_x, cur_y, cur_ox, cur_oy;
    cur_w = GetSystemMetrics(SM_CXCURSOR);
    cur_h = GetSystemMetrics(SM_CYCURSOR);
    if (!cur_w || !cur_h) {
        qDebug() << "Failed to get cursor size\n";
        return;
    }
    cur_info = {sizeof(CURSORINFO), 0, 0, 0};
    if (!GetCursorInfo(&cur_info)) {
        qDebug() << "Failed to get cursor info\n";
        return;
    }
    if (!GetIconInfo(cur_info.hCursor, &icon_info)) {
        qDebug() << "Failed to get icon info\n";
        return;
    }
    // 坑：需要释放资源
    DeleteObject(icon_info.hbmColor);
    DeleteObject(icon_info.hbmMask);
    cur_x = cur_info.ptScreenPos.x;
    cur_y = cur_info.ptScreenPos.y;
    cur_ox = icon_info.xHotspot;
    cur_oy = icon_info.yHotspot;

    // 废弃方案：截图并在截图上绘制
    // SetStretchBltMode(hdcMem, HALFTONE); // 用于选择拉伸填充方式
    // StretchBlt(hdcMem, 0, 0, cur_w, cur_h, hdcScreen, pt_x - cur_ox, pt_y - cur_oy, cur_w, cur_h, SRCCOPY);

    // 填充纯色底色
    RECT rect = {0, 0, cur_w, cur_h};
    FillRect(hdcMem, &rect, CreateSolidBrush(0x0000FF00));
    DrawIcon(hdcMem, 0, 0, cur_info.hCursor);

    // 将底色替换为透明
    auto img = QImage::fromHBITMAP(hbmCanvas).convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < img.height(); ++y) {
        auto line = (uint32_t*)img.scanLine(y);
        // qDebug() << line[0] << '\n';
        for (int x = 0; x < img.width(); ++x) {
            if (line[x] == 0xFF00FF00) {
                line[x] = 0;
            }
        }
    }

    // 绘制
    ui->label->resize({scale(cur_w), scale(cur_h)});
    ui->label->setPixmap(QPixmap::fromImage(img));
    move({scale(cur_x - cur_ox), scale(cur_y - cur_oy)});
    raise();
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->label->setScaledContents(true);

    SetProcessDPIAware();
    dpi = (int)GetDpiForSystem();

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // 废弃方案：设置本窗口不被截屏捕捉，但这样也没法被parsec捕捉了
    // SetWindowDisplayAffinity((HWND)winId(), WDA_EXCLUDEFROMCAPTURE);

    hdcScreen = GetDC(NULL);  // Get your device contexts.
    hdcMem = CreateCompatibleDC(hdcScreen);
    hbmCanvas = CreateCompatibleBitmap(hdcScreen, 60, 60);  // Create the bitmap to use as a canvas.
    hbmOld = SelectObject(hdcMem, hbmCanvas);               // Select the bitmap into the device context.

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::drawCursor);
    timer->start(10);
}

MainWindow::~MainWindow() {
    timer->stop();
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmCanvas);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    delete ui;
    delete timer;
}
