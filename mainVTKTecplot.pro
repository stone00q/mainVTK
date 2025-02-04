QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += D:/VTK/VTK8.2-VS2015-OCCT7.7/VTK8.2-install/include/vtk-8.2
# INCLUDEPATH += D:/VTK/OpenCASCADE_7.7.0/install/inc
LIBS += -L$$quote("D:/VTK/VTK8.2-VS2015-OCCT7.7/VTK8.2-install/lib/")
LIBS += -L$$quote("D:/VTK/OpenCASCADE_7.7.0/install/win64/vc14/lib")
CONFIG(debug, debug|release) {
LIBS+=\
vtkChartsCore-8.2d.lib\
vtkCommonColor-8.2d.lib\
vtkCommonComputationalGeometry-8.2d.lib\
vtkCommonCore-8.2d.lib\
vtkCommonDataModel-8.2d.lib\
vtkCommonExecutionModel-8.2d.lib\
vtkCommonMath-8.2d.lib\
vtkCommonMisc-8.2d.lib\
vtkCommonSystem-8.2d.lib\
vtkCommonTransforms-8.2d.lib\
vtkDICOMParser-8.2d.lib\
vtkDomainsChemistry-8.2d.lib\
vtkDomainsChemistryOpenGL2-8.2d.lib\
vtkdoubleconversion-8.2d.lib\
vtkexodusII-8.2d.lib\
vtkexpat-8.2d.lib\
vtkFiltersAMR-8.2d.lib\
vtkFiltersCore-8.2d.lib\
vtkFiltersExtraction-8.2d.lib\
vtkFiltersFlowPaths-8.2d.lib\
vtkFiltersGeneral-8.2d.lib\
vtkFiltersGeneric-8.2d.lib\
vtkFiltersGeometry-8.2d.lib\
vtkFiltersHybrid-8.2d.lib\
vtkFiltersHyperTree-8.2d.lib\
vtkFiltersImaging-8.2d.lib\
vtkFiltersModeling-8.2d.lib\
vtkFiltersParallel-8.2d.lib\
vtkFiltersParallelImaging-8.2d.lib\
vtkFiltersPoints-8.2d.lib\
vtkFiltersProgrammable-8.2d.lib\
vtkFiltersSelection-8.2d.lib\
vtkFiltersSMP-8.2d.lib\
vtkFiltersSources-8.2d.lib\
vtkFiltersStatistics-8.2d.lib\
vtkFiltersTexture-8.2d.lib\
vtkFiltersTopology-8.2d.lib\
vtkFiltersVerdict-8.2d.lib\
vtkfreetype-8.2d.lib\
vtkGeovisCore-8.2d.lib\
vtkgl2ps-8.2d.lib\
vtkglew-8.2d.lib\
vtkGUISupportQt-8.2d.lib\
vtkGUISupportQtSQL-8.2d.lib\
vtkhdf5-8.2d.lib\
vtkhdf5_hl-8.2d.lib\
vtkImagingColor-8.2d.lib\
vtkImagingCore-8.2d.lib\
vtkImagingFourier-8.2d.lib\
vtkImagingGeneral-8.2d.lib\
vtkImagingHybrid-8.2d.lib\
vtkImagingMath-8.2d.lib\
vtkImagingMorphological-8.2d.lib\
vtkImagingSources-8.2d.lib\
vtkImagingStatistics-8.2d.lib\
vtkImagingStencil-8.2d.lib\
vtkInfovisCore-8.2d.lib\
vtkInfovisLayout-8.2d.lib\
vtkInteractionImage-8.2d.lib\
vtkInteractionStyle-8.2d.lib\
vtkInteractionWidgets-8.2d.lib\
vtkIOAMR-8.2d.lib\
vtkIOAsynchronous-8.2d.lib\
vtkIOCityGML-8.2d.lib\
vtkIOCore-8.2d.lib\
vtkIOEnSight-8.2d.lib\
vtkIOExodus-8.2d.lib\
vtkIOExport-8.2d.lib\
vtkIOExportOpenGL2-8.2d.lib\
vtkIOExportPDF-8.2d.lib\
vtkIOGeometry-8.2d.lib\
vtkIOImage-8.2d.lib\
vtkIOImport-8.2d.lib\
vtkIOInfovis-8.2d.lib\
vtkIOLegacy-8.2d.lib\
vtkIOLSDyna-8.2d.lib\
vtkIOMINC-8.2d.lib\
vtkIOMovie-8.2d.lib\
vtkIONetCDF-8.2d.lib\
vtkIOOCCT-8.2d.lib\
vtkIOParallel-8.2d.lib\
vtkIOParallelXML-8.2d.lib\
vtkIOPLY-8.2d.lib\
vtkIOSegY-8.2d.lib\
vtkIOSQL-8.2d.lib\
vtkIOTecplotTable-8.2d.lib\
vtkIOVeraOut-8.2d.lib\
vtkIOVideo-8.2d.lib\
vtkIOXML-8.2d.lib\
vtkIOXMLParser-8.2d.lib\
vtkjpeg-8.2d.lib\
vtkjsoncpp-8.2d.lib\
vtklibharu-8.2d.lib\
vtklibxml2-8.2d.lib\
vtklz4-8.2d.lib\
vtklzma-8.2d.lib\
vtkmetaio-8.2d.lib\
vtkNetCDF-8.2d.lib\
vtkogg-8.2d.lib\
vtkParallelCore-8.2d.lib\
vtkpng-8.2d.lib\
vtkproj-8.2d.lib\
vtkpugixml-8.2d.lib\
vtkRenderingAnnotation-8.2d.lib\
vtkRenderingContext2D-8.2d.lib\
vtkRenderingContextOpenGL2-8.2d.lib\
vtkRenderingCore-8.2d.lib\
vtkRenderingFreeType-8.2d.lib\
vtkRenderingGL2PSOpenGL2-8.2d.lib\
vtkRenderingImage-8.2d.lib\
vtkRenderingLabel-8.2d.lib\
vtkRenderingLOD-8.2d.lib\
vtkRenderingOpenGL2-8.2d.lib\
vtkRenderingQt-8.2d.lib\
vtkRenderingVolume-8.2d.lib\
vtkRenderingVolumeOpenGL2-8.2d.lib\
vtksqlite-8.2d.lib\
vtksys-8.2d.lib\
vtktheora-8.2d.lib\
vtktiff-8.2d.lib\
vtkverdict-8.2d.lib\
vtkViewsContext2D-8.2d.lib\
vtkViewsCore-8.2d.lib\
vtkViewsInfovis-8.2d.lib\
vtkViewsQt-8.2d.lib\
vtkzlib-8.2d.lib\
}else{
LIBS+=\
vtkChartsCore-8.2.lib\
vtkCommonColor-8.2.lib\
vtkCommonComputationalGeometry-8.2.lib\
vtkCommonCore-8.2.lib\
vtkCommonDataModel-8.2.lib\
vtkCommonExecutionModel-8.2.lib\
vtkCommonMath-8.2.lib\
vtkCommonMisc-8.2.lib\
vtkCommonSystem-8.2.lib\
vtkCommonTransforms-8.2.lib\
vtkDICOMParser-8.2.lib\
vtkDomainsChemistry-8.2.lib\
vtkDomainsChemistryOpenGL2-8.2.lib\
vtkdoubleconversion-8.2.lib\
vtkexodusII-8.2.lib\
vtkexpat-8.2.lib\
vtkFiltersAMR-8.2.lib\
vtkFiltersCore-8.2.lib\
vtkFiltersExtraction-8.2.lib\
vtkFiltersFlowPaths-8.2.lib\
vtkFiltersGeneral-8.2.lib\
vtkFiltersGeneric-8.2.lib\
vtkFiltersGeometry-8.2.lib\
vtkFiltersHybrid-8.2.lib\
vtkFiltersHyperTree-8.2.lib\
vtkFiltersImaging-8.2.lib\
vtkFiltersModeling-8.2.lib\
vtkFiltersParallel-8.2.lib\
vtkFiltersParallelImaging-8.2.lib\
vtkFiltersPoints-8.2.lib\
vtkFiltersProgrammable-8.2.lib\
vtkFiltersSelection-8.2.lib\
vtkFiltersSMP-8.2.lib\
vtkFiltersSources-8.2.lib\
vtkFiltersStatistics-8.2.lib\
vtkFiltersTexture-8.2.lib\
vtkFiltersTopology-8.2.lib\
vtkFiltersVerdict-8.2.lib\
vtkfreetype-8.2.lib\
vtkGeovisCore-8.2.lib\
vtkgl2ps-8.2.lib\
vtkglew-8.2.lib\
vtkGUISupportQt-8.2.lib\
vtkGUISupportQtSQL-8.2.lib\
vtkhdf5-8.2.lib\
vtkhdf5_hl-8.2.lib\
vtkImagingColor-8.2.lib\
vtkImagingCore-8.2.lib\
vtkImagingFourier-8.2.lib\
vtkImagingGeneral-8.2.lib\
vtkImagingHybrid-8.2.lib\
vtkImagingMath-8.2.lib\
vtkImagingMorphological-8.2.lib\
vtkImagingSources-8.2.lib\
vtkImagingStatistics-8.2.lib\
vtkImagingStencil-8.2.lib\
vtkInfovisCore-8.2.lib\
vtkInfovisLayout-8.2.lib\
vtkInteractionImage-8.2.lib\
vtkInteractionStyle-8.2.lib\
vtkInteractionWidgets-8.2.lib\
vtkIOAMR-8.2.lib\
vtkIOAsynchronous-8.2.lib\
vtkIOCityGML-8.2.lib\
vtkIOCore-8.2.lib\
vtkIOEnSight-8.2.lib\
vtkIOExodus-8.2.lib\
vtkIOExport-8.2.lib\
vtkIOExportOpenGL2-8.2.lib\
vtkIOExportPDF-8.2.lib\
vtkIOGeometry-8.2.lib\
vtkIOImage-8.2.lib\
vtkIOImport-8.2.lib\
vtkIOInfovis-8.2.lib\
vtkIOLegacy-8.2.lib\
vtkIOLSDyna-8.2.lib\
vtkIOMINC-8.2.lib\
vtkIOMovie-8.2.lib\
vtkIONetCDF-8.2.lib\
vtkIOOCCT-8.2.lib\
vtkIOParallel-8.2.lib\
vtkIOParallelXML-8.2.lib\
vtkIOPLY-8.2.lib\
vtkIOSegY-8.2.lib\
vtkIOSQL-8.2.lib\
vtkIOTecplotTable-8.2.lib\
vtkIOVeraOut-8.2.lib\
vtkIOVideo-8.2.lib\
vtkIOXML-8.2.lib\
vtkIOXMLParser-8.2.lib\
vtkjpeg-8.2.lib\
vtkjsoncpp-8.2.lib\
vtklibharu-8.2.lib\
vtklibxml2-8.2.lib\
vtklz4-8.2.lib\
vtklzma-8.2.lib\
vtkmetaio-8.2.lib\
vtkNetCDF-8.2.lib\
vtkogg-8.2.lib\
vtkParallelCore-8.2.lib\
vtkpng-8.2.lib\
vtkproj-8.2.lib\
vtkpugixml-8.2.lib\
vtkRenderingAnnotation-8.2.lib\
vtkRenderingContext2D-8.2.lib\
vtkRenderingContextOpenGL2-8.2.lib\
vtkRenderingCore-8.2.lib\
vtkRenderingFreeType-8.2.lib\
vtkRenderingGL2PSOpenGL2-8.2.lib\
vtkRenderingImage-8.2.lib\
vtkRenderingLabel-8.2.lib\
vtkRenderingLOD-8.2.lib\
vtkRenderingOpenGL2-8.2.lib\
vtkRenderingQt-8.2.lib\
vtkRenderingVolume-8.2.lib\
vtkRenderingVolumeOpenGL2-8.2.lib\
vtksqlite-8.2.lib\
vtksys-8.2.lib\
vtktheora-8.2.lib\
vtktiff-8.2.lib\
vtkverdict-8.2.lib\
vtkViewsContext2D-8.2.lib\
vtkViewsCore-8.2.lib\
vtkViewsInfovis-8.2.lib\
vtkViewsQt-8.2.lib\
vtkzlib-8.2.lib\
}
SOURCES += \
    TecplotWidget.cpp \
    class_reader.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    TecplotWidget.h \
    class_reader.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
