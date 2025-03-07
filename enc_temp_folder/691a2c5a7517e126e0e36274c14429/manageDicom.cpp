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
        this->viewer->SetSlice(static_cast<vtkSliderRepresentation*>(sliderWidget->GetRepresentation())->GetValue());


        // 更新切片显示
    }
    //vtkImageActor* imageActor = nullptr;
    //vtkRenderWindow* renderWindow = nullptr;
	vtkSmartPointer<vtkImageViewer2> viewer = nullptr;
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
    // 将 VTK 渲染窗口与 Qt OpenGLWidget 关联
    //ui->openGLWidget->setRenderWindow(renderWindow1);
    //ui->openGLWidget_2->setRenderWindow(renderWindow2);
    //ui->openGLWidget_3->setRenderWindow(renderWindow3);
    //ui->openGLWidget_4->setRenderWindow(renderWindow4);
}

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
    //vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();

    //reader->SetInputData(itkToVtk(image));
    //reader->Update();

    //if (reader->GetOutput() == nullptr) {
    //    QMessageBox::warning(this,"错误","无法读取 DICOM 数据，请检查文件夹路径是否正确。");
    //    return;
    //}
    //// 创建四个渲染器
    //vtkSmartPointer<vtkRenderer> ren[4];
    //for (int i = 0; i < 4; i++) {
    //    ren[i] = vtkSmartPointer<vtkRenderer>::New();
    //}

    //// 初始化交互器
    //vtkSmartPointer<vtkRenderWindowInteractor> interactor1 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //vtkSmartPointer<vtkRenderWindowInteractor> interactor2 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //vtkSmartPointer<vtkRenderWindowInteractor> interactor3 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //vtkSmartPointer<vtkRenderWindowInteractor> interactor4 = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    //renderWindow1->SetInteractor(interactor1);
    //renderWindow2->SetInteractor(interactor2);
    //renderWindow3->SetInteractor(interactor3);
    //renderWindow4->SetInteractor(interactor4);

    //// 创建 CellPicker 和 Property
    //vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    //picker->SetTolerance(0.005);

    //vtkSmartPointer<vtkProperty> ipwProp = vtkSmartPointer<vtkProperty>::New();

    //// 创建 ImagePlaneWidget
    //vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];
    //int imageDims[3];
    //reader->GetOutput()->GetDimensions(imageDims);

    //for (int i = 0; i < 3; i++) {
    //    planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
    //    planeWidget[i]->SetInteractor(interactor1);
    //    planeWidget[i]->SetPicker(picker);
    //    planeWidget[i]->RestrictPlaneToVolumeOn();
    //    double color[3] = { 0, 0, 0 };
    //    color[i] = 1;
    //    planeWidget[i]->GetPlaneProperty()->SetColor(color);
    //    planeWidget[i]->SetTexturePlaneProperty(ipwProp);
    //    planeWidget[i]->TextureInterpolateOff();
    //    planeWidget[i]->SetResliceInterpolateToLinear();
    //    planeWidget[i]->SetInputConnection(reader->GetOutputPort());
    //    planeWidget[i]->SetPlaneOrientation(i);
    //    planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
    //    planeWidget[i]->DisplayTextOn();
    //    planeWidget[i]->SetDefaultRenderer(ren[3]);
    //    planeWidget[i]->SetWindowLevel(1358, -27);
    //    planeWidget[i]->On();
    //    planeWidget[i]->InteractionOn();
    //}

    //planeWidget[1]->SetLookupTable(planeWidget[0]->GetLookupTable());
    //planeWidget[2]->SetLookupTable(planeWidget[0]->GetLookupTable());

    //// 创建 ResliceCursor 和回调函数
    //vtkSmartPointer<vtkResliceCursorCallback> cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
    //vtkSmartPointer<vtkResliceCursor> resliceCursor = vtkSmartPointer<vtkResliceCursor>::New();
    //resliceCursor->SetCenter(reader->GetOutput()->GetCenter());
    //resliceCursor->SetThickMode(0);
    //resliceCursor->SetThickness(10, 10, 10);
    //resliceCursor->SetImage(reader->GetOutput());

    //vtkSmartPointer<vtkResliceCursorWidget> resliceCursorWidget[3];
    //vtkSmartPointer<vtkResliceCursorLineRepresentation> resliceCursorRep[3];

    //double viewUp[3][3] = { { 0, 0, -1 }, { 0, 0, 1 }, { 0, 1, 0 } };
    //for (int i = 0; i < 3; i++) {
    //    resliceCursorWidget[i] = vtkSmartPointer<vtkResliceCursorWidget>::New();
    //    resliceCursorWidget[i]->SetInteractor(interactor1);

    //    resliceCursorRep[i] = vtkSmartPointer<vtkResliceCursorLineRepresentation>::New();
    //    resliceCursorWidget[i]->SetRepresentation(resliceCursorRep[i]);
    //    resliceCursorRep[i]->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(resliceCursor);
    //    resliceCursorRep[i]->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);

    //    const double minVal = reader->GetOutput()->GetScalarRange()[0];
    //    if (vtkImageReslice* reslice = vtkImageReslice::SafeDownCast(resliceCursorRep[i]->GetReslice())) {
    //        reslice->SetBackgroundColor(minVal, minVal, minVal, minVal);
    //    }

    //    resliceCursorWidget[i]->SetDefaultRenderer(ren[i]);
    //    resliceCursorWidget[i]->SetEnabled(1);

    //    ren[i]->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    //    double camPos[3] = { 0, 0, 0 };
    //    camPos[i] = 1;
    //    ren[i]->GetActiveCamera()->SetPosition(camPos);
    //    ren[i]->GetActiveCamera()->ParallelProjectionOn();
    //    ren[i]->GetActiveCamera()->SetViewUp(viewUp[i][0], viewUp[i][1], viewUp[i][2]);
    //    ren[i]->ResetCamera();
    //    cbk->IPW[i] = planeWidget[i];
    //    cbk->RCW[i] = resliceCursorWidget[i];
    //    resliceCursorWidget[i]->AddObserver(vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);
    //    double range[2];
    //    reader->GetOutput()->GetScalarRange(range);
    //    resliceCursorRep[i]->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
    //    planeWidget[i]->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
    //    resliceCursorRep[i]->SetLookupTable(resliceCursorRep[0]->GetLookupTable());
    //    planeWidget[i]->GetColorMap()->SetLookupTable(resliceCursorRep[0]->GetLookupTable());
    //}

    //// 设置背景颜色
    //ren[0]->SetBackground(0, 0, 0);
    //ren[1]->SetBackground(0, 0, 0);
    //ren[2]->SetBackground(0, 0, 0);
    //ren[3]->SetBackground(0, 0, 0);

    //// 设置视图布局
    //ren[0]->SetViewport(0, 0, 0.5, 0.5);
    //ren[1]->SetViewport(0.5, 0, 1, 0.5);
    //ren[2]->SetViewport(0, 0.5, 0.5, 1);
    //ren[3]->SetViewport(0.5, 0.5, 1, 1);

    //// 初始化交互器
    //interactor1->Initialize();
    //interactor2->Initialize();
    //interactor3->Initialize();
    //interactor4->Initialize();

}

void manageDicom::show3dImage(Input3dImageType::Pointer image)  
{  
    vtkSmartPointer<vtkImageViewer2> viewer =
        vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputData(itkToVtk(image));
    //设置基本属性
    viewer->SetSize(640, 480);
    viewer->SetColorLevel(500);
    viewer->SetColorWindow(2000);
    viewer->SetSlice(40);
    viewer->SetSliceOrientationToXY();
    viewer->SetRenderWindow(ui->openGLWidget->renderWindow());
    viewer->GetRenderer()->SetBackground(0, 0, 0);
    viewer->Render();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> rwi =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //设置交互属性
    viewer->SetupInteractor(rwi);
    //viewer->GetRenderer()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);

    vtkSmartPointer<vtkSliderRepresentation2D> sliderRep =
        vtkSmartPointer<vtkSliderRepresentation2D>::New();
    sliderRep->SetMinimumValue(viewer->GetSliceMin());
    sliderRep->SetMaximumValue(viewer->GetSliceMax());
    sliderRep->SetValue(5.0);
    sliderRep->GetSliderProperty()->SetColor(1, 0, 0);//red
    sliderRep->GetTitleProperty()->SetColor(1, 0, 0);//red
    sliderRep->GetLabelProperty()->SetColor(1, 0, 0);//red
    sliderRep->GetSelectedProperty()->SetColor(0, 1, 0);//green
    sliderRep->GetTubeProperty()->SetColor(1, 1, 0);//yellow
    sliderRep->GetCapProperty()->SetColor(1, 1, 0);//yellow
    sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
    sliderRep->GetPoint1Coordinate()->SetValue(40, 40);
    sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
    sliderRep->GetPoint2Coordinate()->SetValue(500, 40);
    vtkSmartPointer<vtkSliderWidget> sliderWidget =
        vtkSmartPointer<vtkSliderWidget>::New();
    sliderWidget->SetInteractor(rwi);
    sliderWidget->SetRepresentation(sliderRep);
    sliderWidget->SetAnimationModeToAnimate();
    sliderWidget->EnabledOn();

    vtkSmartPointer<vtkSliderCallback> callback =
        vtkSmartPointer<vtkSliderCallback>::New();
    callback->viewer = viewer;

    sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback);
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    double axesSize[3] = { 100,100,100 };
    axes->SetTotalLength(axesSize);
    axes->SetConeRadius(0.1);
    axes->SetShaftTypeToLine();
    axes->SetAxisLabels(false);

    viewer->GetRenderer()->AddActor(axes);
    viewer->Render();

    viewer->GetRenderer()->ResetCamera();
    viewer->Render();
    rwi->Initialize();
    
    rwi->Start();
   
}

