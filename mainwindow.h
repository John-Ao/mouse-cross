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
    QTimer *timer;
    HDC hdcScreen, hdcMem;
    HBITMAP hbmCanvas;
    HGDIOBJ hbmOld;
    HBRUSH brush;
    CURSORINFO cur_info;
    ICONINFO icon_info;

    void drawCursor();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif  // MAINWINDOW_H
