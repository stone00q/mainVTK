#ifndef VTKTECPLOTWIDGET_H
#define VTKTECPLOTWIDGET_H

#include <QWidget>

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


#include <cctype>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
//关于cut只有一个cutplanewidget，但是可以创建多个actor。注意：整个过程不包括更新渲染
class vtkCutPlane{
public:
    vtkCutPlane();
    ~vtkCutPlane(){}
    bool GetInputdataStatus();
    void SetInputData(vtkUnstructuredGrid* inputData,QVTKInteractor* qvtkInteractor,vtkRenderer* renderer);
    void SetCutPlaneWidget();//设置widget
    void CloseCutPlaneWidget();//设置widget不可见
    void RemoveCutPlaneWidget();
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
class vtkColorMap
{
public:
    vtkColorMap(){}
    ~vtkColorMap(){}
    bool SetColorMapObject(std::string name, std::map<std::string,vtkActor*>& actorsList);  //选中操作对象
    bool SetSolidColor(std::map<std::string,vtkScalarBarActor*>& barsList, QColor RGBA = QColor(211, 211, 211));
    bool SetColorMapVariable(std::string selectedVar, std::map<std::string,vtkSmartPointer<vtkLookupTable> >& lutsList, std::map<std::string,vtkScalarBarActor*>& barsList,vtkRenderer* renderer);
    // barStaus负责记录是否是第一次打开
private:
    vtkActor* selectedActor;
    std::string objName;
    bool selectedStatus = false; //记录是否有设置过选中操作对象，没有就不能setsolidcolor
};
class vtkContour
{//对一个contour进行新建、setinputdata、加入contourlist、actor列表和renderer，全都默认命名方式
public:
    vtkContour(){}
    ~vtkContour(){}
    void Initialize(std::string contourName,std::string derivedName,std::map<std::string,vtkActor*>& actorsList,vtkRenderer* renderer);
    std::string AddContour(std::string& contourDerived);//添加一个contour,并返回默认命名
    double* SetActiveProperty(std::string propertyName);//设置,返回这个属性的range
    int AddEntry(double value); //设置等值线/面抽取值，返回eid
    bool EditEntry(int entryId,double value);
    bool RemoveEntry(int entryId);//删除后entryid会前移

private:
    vtkDataSet* data;
    int valueNum = 0;//总共多少条等值线/面
    //需要new的
    vtkSmartPointer<vtkContourFilter> contour;
};
class vtkTecplotWidget:public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit vtkTecplotWidget(QWidget *parent = 0);
    ~vtkTecplotWidget();

    void SetInputData(vtkMultiBlockDataSet* inputData);

    int GetPropertiesNumber();
    std::string GetPropertyName(int id);

    /***设置、获取渲染器背景颜色***/
    void SetBackgroundColor(QColor color =QColor(0, 139, 139));
    QColor GetBackgroundColor();

    /***设置basicActor、basicMapper的颜色***/

    QColor GetSolidColor();
    void SetSolidVisible(bool flag);

    /***设置颜色映射变量***/
    bool SetColorMapObject(QString name); //输入进行颜色映射的actor名字
    void SetSolidColor(QColor  RGBA = QColor(211, 211, 211));
    bool SetColorMapVariable(QString name); //进行颜色映射

   // void SetColorMapVariable(std::string selectedVar);
    void SetColorMappingFlag(bool flag = false);
    //Cut
    void SetCutPlaneWidget(bool flag=true);
    QString AddCutPlane(QString name="");//默认命名方式，返回名字cutPlane*,并且其他actor都会设置为不可见
    bool SetCutPlaneVisible(QString name, bool flag=true);
    bool DelCutPlane(QString name);
    //void SetCutPlane(QPoint origin, QVector normal);
    //QPoint GetCutOrigin();
    //QVector GetCutNormal();
    void SetCutColorMapVariable(std::string varName);

    //StreamLine
    void SetStreamTrancerSeed(int PointNum=100);
    void SetStreamTrancerApply();

    bool SetVelocity(QString u, QString v, QString w);
    bool SetVelocity(QString vel);

    /*等值线/面，旨在showcontour才会进行渲染*/
    QString AddContour(QString contourDerived);
    double* SetContourBy(QString contourName,QString propertyName);
    int AddEntry(QString contourName,double value);
    bool EditEntry(QString contourName,int entryId,double value);
    bool RemoveEntry(QString contourName,int entryId);
    void ShowContour(QString contourName);//注意，显示某个contour的时候，默认会把其他的都关掉
    //bool RemoveContour(QString contourName);//关闭某个contour
private:
    //共享
    vtkMultiBlockDataSet* multiBlock;
    vtkUnstructuredGrid* unstructuredGrid;
    vtkPointData* pointData;
    vtkDataArray* scalars;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    vtkSmartPointer<QVTKInteractor> qvtkInteractor;

    //截面的法平面
    //vtkSmartPointer<vtkPlaneWidget> cutPlaneWidget;
    //vtkSmartPointer<vtkImplicitPlaneWidget2> cutPlaneWidget;
    //vtkSmartPointer<vtkImplicitPlaneRepresentation> cutPlaneRep;

    //流线
    vtkSmartPointer<vtkSphereWidget> seedSphereWidget;
    //渲染管理actor
    //std::map<std::string,vtkDataSetMapper*> mappersMap;
    std::map<std::string,vtkActor*> actorsMap;
    std::map<std::string,vtkLookupTable*> lutsMap;
    std::map<std::string,vtkScalarBarActor*> barActorsMap;

    //actorslist保存所有打开过的actor，每次新建就在里面，删除就挪走
    //actorsststus保存actor状态，1可见，0不可见
    std::map<std::string,vtkActor*> actorsList;
    std::map<std::string,bool> actorsStatus;
    std::map<std::string,vtkSmartPointer<vtkLookupTable> > lutsList;
    std::map<std::string,vtkScalarBarActor*> barsList;
    //std::map<std::string,vtkSmartPointer<vtkContour> > contoursList;
    std::map<std::string,vtkContour*> contoursList;
    int varNum;
    int seedPointNum;
    int contourNum = 0;

    vtkCutPlane cutPlane;
    vtkColorMap colorMap;

};


#endif // VTKTECPLOTWIDGET_H





















































