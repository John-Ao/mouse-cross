#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    Ui::MainWindow *ui;
    QTimer *timerPos, *timerIcon;
    HDC hdcScreen, hdcMem;
    HBITMAP hbmCanvas;
    HGDIOBJ hbmOld;
    HBRUSH brush;
    CURSORINFO cur_info;
    ICONINFO icon_info;
    int cur_x, cur_y, cur_ox = 32, cur_oy = 32;

    void updatePos();
    void updateIcon();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif  // MAINWINDOW_H
