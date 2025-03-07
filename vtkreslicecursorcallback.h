#ifndef VTKRESLICECURSORCALLBACK_H
#define VTKRESLICECURSORCALLBACK_H


#include "vtkCommand.h"
#include "vtkImagePlaneWidget.h"
#include "vtkResliceCursorWidget.h"
#include"vtkAutoInit.h"
#include <vtkOrientationMarkerWidget.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

class vtkResliceCursorCallback : public vtkCommand
{
public:
    vtkResliceCursorCallback();
    static vtkResliceCursorCallback* New()
    {
        return new vtkResliceCursorCallback;
    }
    virtual void Execute(vtkObject* caller, unsigned long, void* callData) override;

    vtkImagePlaneWidget* IPW[3];
    vtkResliceCursorWidget* RCW[3];

};

#endif // VTKRESLICECURSORCALLBACK_H
