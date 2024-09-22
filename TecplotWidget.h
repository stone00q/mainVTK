#ifndef TECPLOTWIDGET_H
#define TECPLOTWIDGET_H

#include <QWidget>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkIdList.h>
#include <vtkCellType.h>
#include <ctime>

#include <vtkMultiBlockDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>

#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>

#include <vtkContourFilter.h>

#include <vtkArrayCalculator.h>
#include <vtkSphereWidget.h>
#include <vtkNamedColors.h>
#include <vtkPointSource.h>
#include <vtkStreamTracer.h>
#include <vtkRungeKutta4.h>

#include <vtkArrowSource.h>
#include <vtkMaskPoints.h>
#include <vtkGlyph3D.h>

#include <vtkGradientFilter.h>
#include <vtkDoubleArray.h>

#include <cctype>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);
class TecplotReader {
public:
    TecplotReader(){}
    ~TecplotReader(){}
    vtkMultiBlockDataSet* ReadTecplotData(const std::string& fileName);
private:
    // Helper functions:
    void pointsReader(int pointId, const std::string& line, int varNum, std::vector<vtkSmartPointer<vtkFloatArray>>& zoneData, vtkPoints* thePoints);
    void cellsReader(const std::string& cellType, const std::string& line, vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid);
};
class CutPlane
{
public:
    CutPlane(){}
    ~CutPlane(){}
    bool GetInputDataStatus();
    void SetInputData(vtkUnstructuredGrid* inputData,QVTKInteractor* qvtkInteractor,vtkRenderer* renderer);
    void SetCutPlaneWidget(); // 设置widget
    void CloseCutPlaneWidget(); // 设置widget不可见
    void DeleteCutPlaneWidget();
    void AddCutPlane(std::string& name,std::map<std::string,vtkActor*>& actorsList,std::map<std::string,bool>& actorsStatus);
private:
    vtkSmartPointer<vtkUnstructuredGrid> ug;
    vtkSmartPointer<QVTKInteractor> qvtkInteractor;
    vtkSmartPointer<vtkRenderer> renderer;

    vtkSmartPointer<vtkImplicitPlaneWidget2> cutPlaneWidget;
    vtkSmartPointer<vtkImplicitPlaneRepresentation> cutPlaneRep;
    bool widgetStatus = false;//看widget是否打开过,0为未打开
    bool dataStatus = false;//是否设置过数据
    //bool cutterStatus = false; //是否有新建过cutter
    int cutActorNum = 0; //有多少个默认名字的actor,只增不减，不重复命名
};
class ColorMap
{
public:
    ColorMap() {}
    ~ColorMap(){}
    bool SetColorMapObject(std::string name, std::map<std::string,vtkActor*>& actorsList);  //选中操作对象
    bool SetSolidColor(std::map<std::string,vtkScalarBarActor*>& barsList, QColor RGBA = QColor(211, 211, 211));
    bool SetColorMapVariable(std::string selectedVar, std::map<std::string,vtkSmartPointer<vtkLookupTable> >& lutsList, std::map<std::string,vtkScalarBarActor*>& barsList,vtkRenderer* renderer);
private:
    vtkActor* m_selectedActor;
    std::string m_objName;
    bool m_selectedStatus = false; //记录是否有设置过选中操作对象，没有就不能setsolidcolor
};
class Contour
{
 // 对一个contour进行新建、setinputdata、加入contourlist、actor列表和renderer，全都默认命名方式
public:
    Contour(){}
    ~Contour(){}
    void Initialize(std::string contourName,std::string derivedName,std::map<std::string,vtkActor*>& actorsList,vtkRenderer* renderer);
    std::string AddContour(std::string& contourDerived); // 添加一个contour,并返回默认命名
    double* SetActiveProperty(std::string propertyName); // 设置，返回这个属性的range
    int AddEntry(double value);  // 设置等值线/面抽取值，返回eid
    bool EditEntry(int entryId,double value);
    bool RemoveEntry(int entryId);   // 删除后entryid会前移
private:
    vtkDataSet* m_data;
    int m_valueNum = 0;//entry数量
    //需要new的
    vtkSmartPointer<vtkContourFilter> m_contourFilter;
};
class Glyph{
    //对一个glyph进行新建，默认命名方式
public:
    Glyph(){}
    ~Glyph(){}
    void Initialize(std::string glyphName,std::string derivedName,std::map<std::string,vtkActor*>& actorsList,vtkRenderer* renderer);
    void SetGlyphVector(std::string vectorName);
    void SetGlyphSourceTipLength(double tipLength);  //箭头尖的长度
    void SetGlyphSourceTipRadius(double tipRadius);  //箭头尖的半径
    void SetGlyphSourceShaftRadius(double shaftRadius);  // 箭头杆的半径
    void SetGlyphSourceScaleFactor(double scaleFactor);  //factor大小
    void SetGlyphPointsNumber(int pointsNumber); //
private:
    bool InputState = false;//记录是否初始化过
    vtkDataSet* m_data;
    vtkSmartPointer<vtkArrowSource> m_arrowSource;
    vtkSmartPointer<vtkMaskPoints> m_maskPoints;
    vtkSmartPointer<vtkGlyph3D> m_glyphFilter;
};
class StreamTracer
{
public:
    StreamTracer(){}
    ~StreamTracer(){}
    void Initialize(std::string streamTracerName,std::string derivedName,std::map<std::string,vtkActor*>& actorsList,vtkRenderer* renderer);
private:
   vtkDataSet* m_data;
};

class TecplotWidget:public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit TecplotWidget(QWidget *parent = 0);
    ~TecplotWidget();

    void SetFileName(QString fileName);
    int GetNumberOfProperty();
    QString GetPropertyName(int id); // !!!这个还没实现，后面需要改一下代码，让返回一个list才行。

    /****设置、获取渲染框背景颜色****/
    void SetBackgroundColor(QColor color = QColor(0,139,139));
    QColor GetBackgroundColor();

    /****设置basicActor颜色****/
    QColor GetSolidColor();     // ！！！ 没实现+1
    void SetSolidColor(bool flag);  // ！！！没实现+1，后面可能不需要了

    /****设置颜色映射变量****/
    bool SetColorMapObject(QString name);  // 输入进行颜色映射的actor名字
    void SetSolidColor(QColor RGBA = QColor(211,211,211));
    bool SetColorMapVariable(QString name);  // 进行颜色映射
    void SetColorMappingFlag(bool flag = true);  // !!!没实现，后面需要补写

    /****设置cut****/
    void SetCutPlaneWidget(bool flag=true);
    QString AddCutPlane(QString name="");//默认命名方式，返回名字cutPlane*,并且其他actor都会设置为不可见
    bool SetCutPlaneVisible(QString name, bool flag=true);
    bool DeleteCutPlane(QString name);
    //void SetCutPlane(QPoint origin, QVector normal);
    //QPoint GetCutOrigin();
    //QVector GetCutNormal();
    void SetCutColorMapVariable(std::string varName);  // !!!暂时没实现

    /****等值线/面，只在showcontour才会进行渲染****/
    QString AddContour(QString contourDerived);
    double* SetContouredBy(QString contourName,QString propertyName);
    int AddEntry(QString contourName,double value);
    bool EditEntry(QString contourName,int entryId,double value);
    bool RemoveEntry(QString contourName,int entryId);
    void ShowContour(QString contourName);//注意，显示某个contour的时候，默认会把其他的都关掉

    /****矢量图形化****/
    QString AddGlyph(QString glyphDerived);
    void SetGlyphVector(QString glyphName,QString vectorName);
    void SetGlyphSourceTipLength(QString glyphName, double tipLength);  //箭头尖的长度
    void SetGlyphSourceTipRadius(QString glyphName, double tipRadius);  //箭头尖的半径
    void SetGlyphSourceShaftRadius(QString glyphName,double shaftRadius);  // 箭头杆的半径
    void SetGlyphSourceScaleFactor(QString glyphName, double scaleFactor);  //factor大小
    void SetGlyphPointsNumber(QString glyphName,int pointsNumber); //
    void ShowGlyph(QString glyphName);

    /****Q值计算****/
    void CalculateQCriterion(QString actorName);
private:

    //共享的
    vtkMultiBlockDataSet* m_multiBlock;
    vtkUnstructuredGrid* m_unstructuredGrid;
    vtkPointData* m_pointData;
    vtkDataArray* m_scalars;

    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    vtkSmartPointer<QVTKInteractor> m_qvtkInteractor;

    //渲染管理actor
    //actorslist保存所有打开过的actor，每次新建就在里面，删除就挪走
    //actorsststus保存actor状态，1可见，0不可见
    std::map<std::string,vtkActor*> m_actorsList;
    std::map<std::string,bool> m_actorsStatus;
    std::map<std::string,vtkSmartPointer<vtkLookupTable> > m_lutsList;
    std::map<std::string,vtkScalarBarActor*> m_barsList;
    //std::map<std::string,vtkSmartPointer<vtkContour> > contoursList;
    std::map<std::string,Contour*> m_contoursList;
    std::map<std::string,Glyph*> m_glyphsList;
    int m_varNum;
    int m_seedPointNum;
    int m_contourNum = 0;
    int m_glyphNum = 0;

    CutPlane m_cutPlane;
    ColorMap m_colorMap;
    TecplotReader m_reader;
};

#endif // TECPLOTWIDGET_H
