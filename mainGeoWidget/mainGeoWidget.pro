QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += D:/VTK/VTK8.2-VS2015-OCCT7.7/VTK8.2-install/include/vtk-8.2
LIBS += -L$$quote("D:/VTK/VTK8.2-VS2015-OCCT7.7/VTK8.2-install/lib/")
CONFIG(debug, debug|release) {
LIBS+=\
vtkCommonColor-8.2d.lib\
vtkCommonComputationalGeometry-8.2d.lib\
vtkCommonCore-8.2d.lib\
vtkCommonDataModel-8.2d.lib\
vtkCommonExecutionModel-8.2d.lib\
vtkCommonMath-8.2d.lib\
vtkCommonMisc-8.2d.lib\
vtkCommonSystem-8.2d.lib\
vtkCommonTransforms-8.2d.lib\
vtkFiltersCore-8.2d.lib\
vtkFiltersExtraction-8.2d.lib\
vtkFiltersGeneral-8.2d.lib\
vtkFiltersGeneric-8.2d.lib\
vtkFiltersGeometry-8.2d.lib\
vtkfreetype-8.2d.lib\
vtkGUISupportQt-8.2d.lib\
vtkGUISupportQtSQL-8.2d.lib\
vtkInteractionStyle-8.2d.lib\
vtkInteractionWidgets-8.2d.lib\
vtkIOCore-8.2d.lib\
vtkIOOCCT-8.2d.lib\
vtkRenderingContextOpenGL2-8.2d.lib\
vtkRenderingCore-8.2d.lib\
vtkRenderingFreeType-8.2d.lib\
vtkRenderingOpenGL2-8.2d.lib\
vtkRenderingQt-8.2d.lib
}else{
LIBS+=\
vtkCommonColor-8.2.lib\
vtkCommonComputationalGeometry-8.2.lib\
vtkCommonCore-8.2.lib\
vtkCommonDataModel-8.2.lib\
vtkCommonExecutionModel-8.2.lib\
vtkCommonMath-8.2.lib\
vtkCommonMisc-8.2.lib\
vtkCommonSystem-8.2.lib\
vtkCommonTransforms-8.2.lib\
vtkFiltersCore-8.2.lib\
vtkFiltersExtraction-8.2.lib\
vtkFiltersGeneral-8.2.lib\
vtkFiltersGeneric-8.2.lib\
vtkFiltersGeometry-8.2.lib\
vtkfreetype-8.2.lib\
vtkGUISupportQt-8.2.lib\
vtkGUISupportQtSQL-8.2.lib\
vtkInteractionStyle-8.2.lib\
vtkInteractionWidgets-8.2.lib\
vtkIOCore-8.2.lib\
vtkIOGeometry-8.2.lib\
vtkIOOCCT-8.2.lib\
vtkRenderingCore-8.2.lib\
vtkRenderingFreeType-8.2.lib\
vtkRenderingOpenGL2-8.2.lib\
vtkRenderingQt-8.2.lib
}
SOURCES += \
    GeometryWidget.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    GeometryWidget.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
