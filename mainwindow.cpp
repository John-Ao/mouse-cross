#include "mainwindow.h"

#include <QImage>
#include <QPixmap>
#include <QProcess>

#include "ui_mainwindow.h"

int dpi;
const int cur_w = 64, cur_h = 64;
const RECT rect_ = {0, 0, cur_w, cur_h};

template <class T>
int scale(T x) {
    return (int)x * 96 / dpi;
}

void restart() {
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    qApp->quit();
}

void MainWindow::updatePos() {
    if (!cur_w) {
        return;
    }
    POINT pos;
    if (!GetCursorPos(&pos)) {
        return;
    }
    move({scale(pos.x - cur_ox), scale(pos.y - cur_oy)});
    raise();
}

void MainWindow::updateIcon() {
    cur_info.cbSize = sizeof(cur_info);
    if (!GetCursorInfo(&cur_info)) {
        return;
    }

    // 获取图标中心
    if (!GetIconInfo(cur_info.hCursor, &icon_info)) {
        return;
    }
    DeleteObject(icon_info.hbmColor);
    DeleteObject(icon_info.hbmMask);
    cur_ox = icon_info.xHotspot;
    cur_oy = icon_info.yHotspot;

    // 填充纯色底色
    FillRect(hdcMem, &rect_, brush);
    DrawIcon(hdcMem, 0, 0, cur_info.hCursor);

    // 将底色替换为透明
    auto img = QImage::fromHBITMAP(hbmCanvas).convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < img.height(); ++y) {
        auto line = (uint32_t*)img.scanLine(y);
        for (int x = 0; x < img.width(); ++x) {
            if (line[x] == 0xFF00FF00) {
                line[x] = 0;
            }
        }
    }

    // 绘制
    ui->label->setPixmap(QPixmap::fromImage(img));
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->label->setScaledContents(true);

    SetProcessDPIAware();
    dpi = (int)GetDpiForSystem();
    ui->label->resize({scale(cur_w), scale(cur_h)});

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    hdcScreen = GetDC(NULL);  // Get your device contexts.
    hdcMem = CreateCompatibleDC(hdcScreen);
    hbmCanvas = CreateCompatibleBitmap(hdcScreen, cur_w, cur_h);  // Create the bitmap to use as a canvas.
    hbmOld = SelectObject(hdcMem, hbmCanvas);                     // Select the bitmap into the device context.
    brush = CreateSolidBrush(0x0000FF00);

    timerPos = new QTimer(this);
    timerIcon = new QTimer(this);
    connect(timerPos, &QTimer::timeout, this, &MainWindow::updatePos);
    connect(timerIcon, &QTimer::timeout, this, &MainWindow::updateIcon);
    timerPos->start(10);
    timerIcon->start(50);
}

MainWindow::~MainWindow() {
    timerPos->stop();
    timerIcon->stop();
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmCanvas);
    DeleteObject(brush);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    delete ui;
    delete timerPos;
    delete timerIcon;
}
