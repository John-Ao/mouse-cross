#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow* self = nullptr;
int dpi;

LRESULT CALLBACK MouseProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    if (nCode >= 0 and wParam == WM_MOUSEMOVE) {
        auto pt = ((MSLLHOOKSTRUCT*)lParam)->pt;
        self->move({(int)pt.x * 96 / dpi - 15, (int)pt.y * 96 / dpi - 15});
    }
    return CallNextHookEx(0, nCode, wParam, lParam);
}

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
}

MainWindow::~MainWindow() {
    delete ui;
    if (hook) {
        UnhookWindowsHookEx(hook);
    }
}
