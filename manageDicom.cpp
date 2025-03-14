#include "manageDicom.h"
#include <Qmessagebox.h>
#include <QDebug>

#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkImageActor.h"
#include "vtkImageReslice.h"
#include "vtkImageMapToColors.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageFlip.h"
#include "QVTKOpenGLNativeWidget.h"
#include "vtkDICOMImageReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkImageData.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkAxesActor.h"
#include "vtkTextActor.h"
#include <vtkSliderRepresentation.h>
#include <vtkProperty2D.h>
#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkreslicecursorcallback.h"
#include "vtkTextProperty.h"  
#include "vtkImageMapper3D.h"
#include <itkImage.h>
#include "itkImageToVTKImageFilter.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkPNGImageIOFactory.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include "vtkImageSliceMapper.h"
#include "vtkImageSlice.h"


#include<vector>
#include<string>
#include <vtkSliderWidget.h>
#include <vtkImageViewer2.h>
#include <vtkSliderRepresentation2D.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkImageMapper.h>
#include <vtkImageSliceMapper.h>
using namespace std;

class vtkSliderCallback : public vtkCommand
{
public:
    static vtkSliderCallback* New()
    {
        return new vtkSliderCallback;
    }
    virtual void Execute(vtkObject* caller, unsigned long, void*)
    {
        vtkSliderWidget* sliderWidget =
            reinterpret_cast<vtkSliderWidget*>(caller);
        this->viewer1->SetSlice(static_cast<vtkSliderRepresentation*>(sliderWidget->GetRepresentation())->GetValue());
        this->viewer2->SetSlice(static_cast<vtkSliderRepresentation*>(sliderWidget->GetRepresentation())->GetValue());
        this->viewer3->SetSlice(static_cast<vtkSliderRepresentation*>(sliderWidget->GetRepresentation())->GetValue());
        // 更新切片显示
    }
    //vtkImageActor* imageActor = nullptr;
    //vtkRenderWindow* renderWindow = nullptr;
	vtkSmartPointer<vtkImageViewer2> viewer1 = nullptr;
    vtkSmartPointer<vtkImageViewer2> viewer2 = nullptr;
    vtkSmartPointer<vtkImageViewer2> viewer3 = nullptr;
};

manageDicom::manageDicom(QWidget *parent):
    QMainWindow(parent)
{

}

manageDicom::~manageDicom(){

}

void manageDicom::initUi(Ui::MainWindow *mutalUi){
    ui = mutalUi;
    qDebug() << "初始化成功！";
}

// 使用ITK读取DICOM图片，输入类型为文件夹
Input3dImageType::Pointer manageDicom::read3dImage(QString path){

    reader3d->SetImageIO(gdcmImageIO);
    nameGenerator->SetUseSeriesDetails(true);
    nameGenerator->AddSeriesRestriction("0008|0021");
    nameGenerator->SetDirectory(path.toStdString().c_str());

    using SeriesIdContainer = vector<string>;
    const vector<string> & seriesUID = nameGenerator->GetSeriesUIDs();
    auto seriesItr = seriesUID.begin();
    auto seriesEnd = seriesUID.end();

    using FileNamesContainer = vector<string>;
    FileNamesContainer fileNames;
    string seriesIdentifier;
    while (seriesItr != seriesEnd)
    {
        seriesIdentifier = seriesItr->c_str();
        fileNames = nameGenerator->GetFileNames(seriesIdentifier);
        ++seriesItr;
    }
    reader3d->SetFileNames(fileNames);
    reader3d->Update();
    return reader3d->GetOutput();

}

vtkSmartPointer<vtkImageData> manageDicom::itkToVtk(Input3dImageType::Pointer image){

    // 定义 ITK 到 VTK 的图像转换过滤器
    typedef itk::ImageToVTKImageFilter<Input3dImageType> itkTovtkFilterType;
    itkTovtkFilterType::Pointer itkTovtkImageFilter = itkTovtkFilterType::New();
    itkTovtkImageFilter->SetInput(image); // 设置图像数据从 ITK 转向 VTK
    try{
        itkTovtkImageFilter->Update();
    }
    catch(itk::ExceptionObject){
        qDebug() << "Itk to Vtk failed!";
    }
    // 创建 vtkImageFlip 过滤器
    vtkSmartPointer<vtkImageFlip> imageVTK = vtkSmartPointer<vtkImageFlip>::New();
    imageVTK->SetInputData(itkTovtkImageFilter->GetOutput()); // 设置输入数据
    imageVTK->SetFilteredAxes(1); // 设置要翻转的轴
    imageVTK->Update();

    return imageVTK->GetOutput();

}

void manageDicom::create4View(QString dicomDirPath){

    // 使用ITK读入DICOM图片
    Input3dImageType::Pointer image = read3dImage(dicomDirPath);
    show3dImage(image);
}

void manageDicom::show3dImage(Input3dImageType::Pointer image)  
{  
    const vtkSmartPointer<vtkImageData> DICOM_IMAGE = itkToVtk(image);
    // 创建图像交互器并绑定到Qt窗口

    vtkSmartPointer<vtkRenderWindowInteractor> iren1 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> iren2 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> iren3 = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	iren1->SetRenderWindow(ui->openGLWidget->renderWindow());
    iren2->SetRenderWindow(ui->openGLWidget_2->renderWindow());
    iren3->SetRenderWindow(ui->openGLWidget_3->renderWindow());
    
    // 创建图像查看器并绑定到Qt窗口
    vtkSmartPointer<vtkImageViewer2> viewer1 = vtkSmartPointer<vtkImageViewer2>::New();
    viewer1->SetRenderWindow(ui->openGLWidget->renderWindow());
    viewer1->SetInputData(DICOM_IMAGE);

    vtkSmartPointer<vtkImageViewer2> viewer2 = vtkSmartPointer<vtkImageViewer2>::New();
    viewer2->SetRenderWindow(ui->openGLWidget_2->renderWindow());
    viewer2->SetInputData(DICOM_IMAGE);

    vtkSmartPointer<vtkImageViewer2> viewer3 = vtkSmartPointer<vtkImageViewer2>::New();
    viewer3->SetRenderWindow(ui->openGLWidget_3->renderWindow());
    viewer3->SetInputData(DICOM_IMAGE);


    // 配置图像显示属性
    viewer1->SetColorLevel(500);
    viewer1->SetColorWindow(2000);
    viewer1->SetSlice(40);
    viewer1->SetSliceOrientationToXZ();
    viewer1->GetRenderer()->SetBackground(0, 0, 0);

    viewer2->SetColorLevel(500);
    viewer2->SetColorWindow(2000);
    viewer2->SetSlice(40);
    viewer2->SetSliceOrientationToXY();
    viewer2->GetRenderer()->SetBackground(0, 0, 0);

    viewer3->SetColorLevel(500);
    viewer3->SetColorWindow(2000);
    viewer3->SetSlice(40);
    viewer3->SetSliceOrientationToYZ();
    viewer3->GetRenderer()->SetBackground(0, 0, 0);
    // 配置交互器样式（保持原有相机操作）
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    iren1->SetInteractorStyle(style);
    iren2->SetInteractorStyle(style);
    iren3->SetInteractorStyle(style);
    // 设置交互器（必须在创建控件前完成且必须在样式设置后调用）
    viewer1->SetupInteractor(iren1);
    viewer2->SetupInteractor(iren2);
    viewer3->SetupInteractor(iren3);

    // 创建滑动条控件
    vtkSmartPointer<vtkSliderRepresentation2D> sliderRep =
        vtkSmartPointer<vtkSliderRepresentation2D>::New();
    sliderRep->SetMinimumValue(viewer1->GetSliceMin());
    sliderRep->SetMaximumValue(viewer1->GetSliceMax());
    sliderRep->SetValue(viewer1->GetSlice());

    // 滑动条视觉样式配置
    sliderRep->GetSliderProperty()->SetColor(1, 0, 0);
    sliderRep->GetTitleProperty()->SetColor(1, 0, 0);
    sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
    sliderRep->GetPoint1Coordinate()->SetValue(0.1, 0.1);  // 左侧10%位置
    sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
    sliderRep->GetPoint2Coordinate()->SetValue(0.9, 0.1); // 右侧90%位置

    // 创建滑动条控件
    vtkSmartPointer<vtkSliderWidget> sliderWidget =
        vtkSmartPointer<vtkSliderWidget>::New();
    sliderWidget->SetInteractor(iren1);
    sliderWidget->SetRepresentation(sliderRep);
    sliderWidget->SetAnimationModeToAnimate();
    sliderWidget->EnabledOn();

    // 配置回调函数
    vtkSmartPointer<vtkSliderCallback> callback =
        vtkSmartPointer<vtkSliderCallback>::New();
    callback->viewer1 = viewer1;
    callback->viewer2 = viewer2;
    callback->viewer3 = viewer3;
    sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback);


    // 初始化渲染（关键步骤）
    viewer1->Render();
    viewer2->Render();
    viewer3->Render();

	iren1->Initialize();
    iren1->Start(); 

    iren2->Initialize();
    iren2->Start();

    iren3->Initialize();
    iren2->Start();
}

