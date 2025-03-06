#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "manageDicom.h"

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkGenericOpenGLRenderWindow.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QAction;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:

signals:
    void mutualUi(Ui::MainWindow *); // 共享Ui指针
private slots:
    void on_loadFile();
    void on_saveFile();
    void on_saveAsFile();

private:
    void createActions();
    void createMenus();

private:
    QAction *loadAction;
    QAction *saveAction;
    QAction *saveAsAction;
private:

private:
    manageDicom *mdicom;
    QString currentFilePath;

    bool isDicom = false;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
