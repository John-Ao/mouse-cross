#include "mainwindow.h"

#include <windows.h>

#include <QImage>
#include <QPixmap>
// #include <QTimer>

#include "ui_mainwindow.h"

int dpi;
int cur_w, cur_h, cur_x, cur_y, cur_ox, cur_oy;
HHOOK hook = nullptr;
MainWindow* self = nullptr;
// QTimer* timer = nullptr;

template <class T>
int scale(T x) {
    return (int)x * 96 / dpi;
}

void drawCursor() {
    // Get your device contexts.
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    // Create the bitmap to use as a canvas.
    HBITMAP hbmCanvas = CreateCompatibleBitmap(hdcScreen, cur_w, cur_h);
    // Select the bitmap into the device context.
    HGDIOBJ hbmOld = SelectObject(hdcMem, hbmCanvas);

    // 获取鼠标信息
    cur_w = GetSystemMetrics(SM_CXCURSOR);
    cur_h = GetSystemMetrics(SM_CYCURSOR);
    CURSORINFO ci;
    ci.cbSize = sizeof(ci);
    GetCursorInfo(&ci);
    ICONINFO ii;
    GetIconInfo(ci.hCursor, &ii);
    cur_x = ci.ptScreenPos.x;
    cur_y = ci.ptScreenPos.y;
    cur_ox = ii.xHotspot;
    cur_oy = ii.yHotspot;

    // 废弃方案：截图并在截图上绘制
    // SetStretchBltMode(hdcMem, HALFTONE); // 用于选择拉伸填充方式
    // StretchBlt(hdcMem, 0, 0, cur_w, cur_h, hdcScreen, pt_x - cur_ox, pt_y - cur_oy, cur_w, cur_h, SRCCOPY);

    // 填充纯色底色
    RECT rect = {0, 0, cur_w, cur_h};
    FillRect(hdcMem, &rect, CreateSolidBrush(0x0000FF00));
    DrawIcon(hdcMem, 0, 0, ci.hCursor);

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

    // 释放资源
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmCanvas);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    // 绘制
    self->ui->label->resize({scale(cur_w), scale(cur_h)});
    self->ui->label->setPixmap(QPixmap::fromImage(img));
    self->move({scale(cur_x - cur_ox), scale(cur_y - cur_oy)});
    self->raise();
}

LRESULT CALLBACK MouseProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    if (nCode >= 0 and wParam == WM_MOUSEMOVE) {
        drawCursor();
    }
    return CallNextHookEx(0, nCode, wParam, lParam);
}

// void alwaysOnTop() {
//     self->raise();
// }

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    SetProcessDPIAware();
    dpi = (int)GetDpiForSystem();
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    self = this;
    if (!(hook = SetWindowsHookExA(WH_MOUSE_LL, MouseProc, nullptr, 0))) {
        qDebug() << "Failed to hook!";
    }
    // timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, alwaysOnTop);
    // timer->start(10);
    ui->label->setScaledContents(true);
    drawCursor();
    // 废弃方案：设置本窗口不被截屏捕捉，但这样也没法被parsec捕捉了
    // SetWindowDisplayAffinity((HWND)winId(), WDA_EXCLUDEFROMCAPTURE);
}

MainWindow::~MainWindow() {
    delete ui;
    if (hook) {
        UnhookWindowsHookEx(hook);
    }
}
