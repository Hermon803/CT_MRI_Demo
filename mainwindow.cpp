#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>

// VTK Headers
#include <vtkSmartPointer.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkImageReslice.h>
#include <vtkImageMapper.h>
#include <vtkActor2D.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkImageShiftScale.h>

// VTK Readers
#include <vtkDICOMImageReader.h>
#include <vtkNIFTIImageReader.h>

// VTK Writers
#include <vtkNIFTIImageWriter.h>
#include <vtkDICOMWriter.h>

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mdicom(new manageDicom)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    connect(this,&MainWindow::mutualUi,mdicom,&manageDicom::initUi);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mdicom;
}

void MainWindow::createActions()
{
    loadAction = new QAction("加载文件", this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::on_loadFile);

    saveAction = new QAction("保存文件", this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::on_saveFile);

    saveAsAction = new QAction("另存为",this);
    connect(saveAsAction,&QAction::triggered,this,&MainWindow::on_saveAsFile);
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = this->menuBar()->addMenu("文件(&F)");
    fileMenu->addAction(loadAction);
    fileMenu->addAction(saveAction);
}

void MainWindow::on_loadFile()
{
    // 打开文件选择对话框
    QString selectedPath = QFileDialog::getOpenFileName(
        this,
        "打开医学图像文件",
        "",
        "DICOM文件 (*.dcm *.dicom);;NIfTI文件 (*.nii *.nii.gz);;文件夹 (*)"
        );

    if(selectedPath.isEmpty()) {
        QMessageBox::warning(this, "错误", "空路径！请重新选择文件夹或文件");
        return;
    }

    currentFilePath = selectedPath;

    // 判断是否是文件夹
    if(QDir(selectedPath).exists()) {
        // 如果是文件夹，调用 create4View 函数
        mdicom->create4View(currentFilePath);
        isDicom = true;
    } else {
        // 如果是文件，判断文件类型
        if(currentFilePath.endsWith(".dcm", Qt::CaseInsensitive) ||
            currentFilePath.endsWith(".dicom", Qt::CaseInsensitive)) {
            // 如果是 DICOM 文件，调用 create4View 函数
            mdicom->create4View(currentFilePath);
            isDicom = true;
        } else if(currentFilePath.endsWith(".nii") ||
                   currentFilePath.endsWith(".nii.gz")) {
            // 如果是 NIfTI 文件，调用相应的处理函数
            isDicom = false;
            // TODO: 添加对 NIfTI 文件的处理逻辑
            vtkSmartPointer<vtkNIFTIImageReader> reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
            reader->SetFileName(currentFilePath.toStdString().c_str());
            reader->Update();
            // TODO: 继续处理 NIfTI 文件的逻辑
        } else {
            // 如果不是支持的文件类型，显示错误信息
            QMessageBox::warning(this, "错误", "不支持的文件类型！请重新选择文件");
            return;
        }
    }
}

void MainWindow::on_saveFile()
{

}

void MainWindow::on_saveAsFile(){
    // Todo
}




