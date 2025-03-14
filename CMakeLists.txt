cmake_minimum_required(VERSION 3.16)

project(CT_MRI_Demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 检查是否已通过环境变量/命令行定义路径
if(NOT DEFINED VTK_DIR)
  set(VTK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/VTK-9.4.1/Degug/lib/cmake/vtk-9.4" CACHE PATH "VTK安装路径")
endif()

if(NOT DEFINED ITK_DIR)
  set(ITK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/ITK-5.4.2/Degug/lib/cmake/itk-5.4" CACHE PATH "ITK安装路径")
endif()

find_package(VTK 9.4 REQUIRED)
find_package(ITK 5.4 REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CT_MRI_Demo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        manageDicom.h
        manageDicom.cpp
        vtkreslicecursorcallback.h vtkreslicecursorcallback.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CT_MRI_Demo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CT_MRI_Demo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CT_MRI_Demo
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CT_MRI_Demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                         Qt${QT_VERSION_MAJOR}::OpenGL
                                        ${VTK_LIBRARIES}
                                    ${ITK_LIBRARIES})

# Link WS2_32.lib and Psapi.lib on Windows

target_link_libraries(CT_MRI_Demo PRIVATE
    Ws2_32
    Psapi)


# VTK autoninit is needed
vtk_module_autoinit(TARGETS CT_MRI_Demo MODULES ${VTK_LIBRARIES})


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CT_MRI_Demo)
endif()
set_target_properties(CT_MRI_Demo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CT_MRI_Demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CT_MRI_Demo)
endif()
