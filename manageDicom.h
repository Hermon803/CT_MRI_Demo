#ifndef MANAGEDICOM_H
#define MANAGEDICOM_H

#include <QMainWindow>
#include "./ui_mainwindow.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"

#include "itkGDCMSeriesFileNames.h"
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkPNGImageIOFactory.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>


typedef itk::Image< short, 3 > Input3dImageType;
typedef itk::Image< short, 3 > Output3dImageType;

class manageDicom : public QMainWindow{
    Q_OBJECT
public:
    manageDicom(QWidget *parent = nullptr);
    ~manageDicom();

public:
    void create4View(QString dicomDirPath);
public slots:
    void initUi(Ui::MainWindow *);

private slots:

    //void loadDicom();
    //void saveDicom();
private:
    Input3dImageType::Pointer read3dImage(QString path);
    vtkSmartPointer<vtkImageData> itkToVtk(Input3dImageType::Pointer image);
private:
    itk::ImageSeriesReader<Input3dImageType>::Pointer reader3d = itk::ImageSeriesReader<Input3dImageType>::New();
    itk::GDCMSeriesFileNames::Pointer nameGenerator = itk::GDCMSeriesFileNames::New();
    itk::GDCMImageIO::Pointer gdcmImageIO = itk::GDCMImageIO::New();
    // 初始化 VTK 渲染窗口
    vtkSmartPointer<vtkRenderWindow> renderWindow1 = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow2 = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow3 = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow4 = vtkSmartPointer<vtkRenderWindow>::New();

private:
    Ui::MainWindow *ui;
};

#endif // MANAGEDICOM_H
