#include "vtkTecplotWidget.h"

vtkTecplotWidget::vtkTecplotWidget(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
    // 初始化渲染器和渲染窗口等所有功能共享部分
    this->multiBlock=vtkSmartPointer<vtkMultiBlockDataSet>::New();
    this->unstructuredGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
    this->pointData=vtkSmartPointer<vtkPointData>::New();
    //this->scalars=vtkSmartPointer<vtkDataArray>::New();
    //scalars=nullptr;
    this->varNum = 0;

    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->renderWindow->AddRenderer(renderer);
    this->SetRenderWindow(renderWindow);

    //获取交互器
    this->qvtkInteractor=this->GetInteractor();
    //this->cutPlaneWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
    //this->cutPlaneRep=vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();

    this->seedSphereWidget=vtkSmartPointer<vtkSphereWidget>::New();
    seedPointNum=100;
    cutPlane = vtkCutPlane();
}

vtkTecplotWidget::~vtkTecplotWidget()
{

}

void vtkTecplotWidget::SetInputData(vtkMultiBlockDataSet* inputData)
{
    if (inputData == nullptr) {
        std::cerr << "Input data is nullptr." << std::endl;
        return;
    }
    this->multiBlock = inputData;
    this->unstructuredGrid=vtkUnstructuredGrid::SafeDownCast(this->multiBlock->GetBlock(0));

    //添加一个速度向量
    auto calculator=vtkSmartPointer<vtkArrayCalculator>::New();
    calculator->SetInputData(this->unstructuredGrid);
    calculator->AddScalarArrayName("u");
    calculator->AddScalarArrayName("v");
    calculator->AddScalarArrayName("w");
    calculator->SetResultArrayName("velocity");
    calculator->SetFunction("u*iHat + v*jHat + w*kHat");
    calculator->Update();

    this->unstructuredGrid=calculator->GetUnstructuredGridOutput();
    this->pointData = this->unstructuredGrid->GetPointData();
    varNum = pointData->GetNumberOfArrays();
    //开始basic渲染
    auto basicMapper=vtkSmartPointer<vtkDataSetMapper>::New();
    auto basicActor=vtkSmartPointer<vtkActor>::New();
    actorsList["basicActor"]=basicActor;
    this->actorsStatus["basicActor"]=true;
    basicMapper->SetInputData(unstructuredGrid);
    basicActor->SetMapper(basicMapper);
    this->renderer->AddActor(basicActor);
    this->renderWindow->Render();

}

int vtkTecplotWidget::GetPropertiesNumber()
{

    int num = this->unstructuredGrid->GetPointData()->GetNumberOfArrays();
    for(int i=0;i<num;i++)
    {
        cout <<endl <<this->unstructuredGrid->GetPointData()->GetArrayName(i);
    }
    return num;
}

void vtkTecplotWidget::SetBackgroundColor(QColor color)
{
    //？？默认颜色0-255
    int r=color.red();
    int g=color.green();
    int b=color.blue();

    double normalizedR = r / 255.0;
    double normalizedG = g / 255.0;
    double normalizedB = b / 255.0;

    this->renderer->SetBackground(normalizedR, normalizedG, normalizedB);
    //？？是否每次都要设置后都要重新渲染
    this->renderWindow->Render();
}

QColor vtkTecplotWidget::GetBackgroundColor()
{
    double* rgb = renderer->GetBackground();
    int r = static_cast<int>(rgb[0] * 255);
    int g = static_cast<int>(rgb[1] * 255);
    int b = static_cast<int>(rgb[2] * 255);
    return QColor(r, g, b);
}

/*void vtkTecplotWidget::SetSolidColor(QColor  RGBA)
{
    //颜色转换
    int r=RGBA.red();
    int g=RGBA.green();
    int b=RGBA.blue();
    double normalizedR = r / 255.0;
    double normalizedG = g / 255.0;
    double normalizedB = b / 255.0;

    auto actor=actorsMap["basicActor"];
    vtkProperty* property=actor->GetProperty();
    property->SetColor(normalizedR,normalizedG,normalizedB);
    this->renderWindow->Render();
}*/

QColor vtkTecplotWidget::GetSolidColor()
{
    auto actor=actorsMap["basicActor"];
    vtkProperty* property=actor->GetProperty();
    double* rgb=property->GetColor();
    int r = static_cast<int>(rgb[0] * 255);
    int g = static_cast<int>(rgb[1] * 255);
    int b = static_cast<int>(rgb[2] * 255);
    return QColor(r, g, b);
}

/*void vtkTecplotWidget::SetColorMapVariable(std::string selectedVar)
{
    int selectedID = this->unstructuredGrid->GetPointData()->SetActiveScalars(selectedVar.c_str());
    this->scalars=this->pointData->GetArray(selectedID);

    //设置颜色映射相关
    if (lutsMap.find("basicLut") == lutsMap.end())
    {
        auto basicLut=vtkSmartPointer<vtkLookupTable>::New();
        basicLut->SetTableRange(this->scalars->GetRange());  //设置颜色映射范围
        basicLut->SetNumberOfColors(256);
        basicLut->Build();
        lutsMap["basicLut"] = basicLut;
        auto basicScalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
        basicScalarBarActor->SetLookupTable(basicLut);
        barActorsMap["basicScalarBarActor"]=basicScalarBarActor;

        auto mapper=mappersMap["basicMapper"];
        mapper->SetScalarRange(this->scalars->GetRange());
        mapper->SetLookupTable(basicLut);
        this->renderer->AddActor(basicScalarBarActor);

    }else
    {
        auto basicLut=lutsMap["basicLut"];
        basicLut->SetTableRange(this->scalars->GetRange());  //设置颜色映射范围
        basicLut->SetNumberOfColors(256);
        basicLut->Build();
        auto basicScalarBarActor=barActorsMap["basicScalarBarActor"];
        basicScalarBarActor->SetLookupTable(basicLut);

        auto mapper=mappersMap["basicMapper"];
        mapper->SetScalarRange(this->scalars->GetRange());
        mapper->SetLookupTable(basicLut);
        this->renderer->AddActor(basicScalarBarActor);
    }
}

void vtkTecplotWidget::SetColorMappingFlag(bool flag)//对于颜色映射paraview一remove就是整个actor remove
{
    //falg为true就执行颜色映射
    if(!flag && barActorsMap.find("basicScalarBarActor") != barActorsMap.end())
    {
        auto basicMapper = mappersMap["basicMapper"];
        basicMapper->ScalarVisibilityOff();  // 关闭颜色映射
        basicMapper->SetLookupTable(nullptr);  // 清除Lookup Table

        // 从renderer中移除ScalarBarActor
        if(barActorsMap.find("basicScalarBarActor") != barActorsMap.end())
        {
            this->renderer->RemoveActor(barActorsMap["basicScalarBarActor"]);
        }

        // 清空或恢复lutsMap与barActorsMap中的对象
        this->lutsMap.clear();
        this->barActorsMap.clear();
    }else if(flag)
    {
        this->renderWindow->Render();
    }
}*/


bool vtkTecplotWidget::SetColorMapObject(QString name)
{
    std::string objName = name.toStdString();
    return colorMap.SetColorMapObject(objName,this->actorsList);
}
void vtkTecplotWidget::SetSolidColor(QColor RGBA)
{
    colorMap.SetSolidColor(this->barsList, RGBA);
    this->renderer->Render();
}
bool vtkTecplotWidget::SetColorMapVariable(QString name)
{
    std::string varName = name.toStdString();
    bool flag = colorMap.SetColorMapVariable(varName,this->lutsList,this->barsList,this->renderer);
    this->renderer->Render();
    return flag;
}

void vtkTecplotWidget::SetCutPlaneWidget(bool flag)
{//所有cut相关操作都需要先确认是否设置的输入
    bool InputdataStatus= this->cutPlane.GetInputdataStatus();
    if(!InputdataStatus)
    {
        this->cutPlane.SetInputData(this->unstructuredGrid,this->qvtkInteractor,this->renderer);
    }
    if(flag)
    {
        this->cutPlane.SetCutPlaneWidget();
    }else{
        this->cutPlane.CloseCutPlaneWidget();
    }
    this->renderer->Render();
}

QString vtkTecplotWidget::AddCutPlane(QString name)
{
    if(!this->cutPlane.GetInputdataStatus())
    {
        this->cutPlane.SetInputData(this->unstructuredGrid,this->qvtkInteractor,this->renderer);
    }
    std::string tmpName=name.toStdString();
    this->cutPlane.AddCutPlane(tmpName,actorsList,actorsStatus);
    this->renderer->Render();
    return name.fromStdString(tmpName);
}

bool vtkTecplotWidget::SetCutPlaneVisible(QString name,bool flag)
{
    std::string tmpName = name.toStdString();
    if(actorsList.count(tmpName) == 0) return false;
    vtkActor* actor = actorsList[tmpName];  //?不new真的可以用吗
    actor->SetVisibility(flag);
    actorsStatus[tmpName] = flag;
    this->renderer->Render();
    return true;
}

bool vtkTecplotWidget::DelCutPlane(QString name)
{
    std::string tmpName = name.toStdString();
    if(actorsList.count(tmpName) == 0) return false;
    vtkActor* actor = actorsList[tmpName];
    this->renderer->RemoveActor(actor);
    this->actorsList.erase(tmpName);
    this->actorsStatus.erase(tmpName);
    this->renderer->Render();
    return true;
}
void vtkTecplotWidget::SetStreamTrancerSeed(int pointNum)
{
    this->seedPointNum=pointNum;
    auto colors=vtkSmartPointer<vtkNamedColors>::New();
    this->seedSphereWidget->PlaceWidget(this->unstructuredGrid->GetBounds());
    this->seedSphereWidget->SetPlaceFactor(0.5);
    this->seedSphereWidget->SetRepresentationToWireframe();
    this->seedSphereWidget->GetSphereProperty()->SetColor(colors->GetColor3d("BurlyWood").GetData());
    this->seedSphereWidget->GetSphereProperty()->SetOpacity(0.5);  // 设置透明度
    this->seedSphereWidget->SetInteractor(this->qvtkInteractor);
    this->seedSphereWidget->On();
}

void vtkTecplotWidget::SetStreamTrancerApply()
{
    double center[3];
    this->seedSphereWidget->GetCenter(center);
    double radius=this->seedSphereWidget->GetRadius();

    //？？下面这些代码都需要判断之前是否new过，即该功能是否打开过且未关闭
    //在球体内部生成指定数量的点
    auto pointSource=vtkSmartPointer<vtkPointSource>::New();
    pointSource->SetCenter(center);
    pointSource->SetRadius(radius);
    pointSource->SetNumberOfPoints(this->seedPointNum);
    pointSource->Update();

    this->pointData->SetActiveVectors("velocity");
    auto streamTracer=vtkSmartPointer<vtkStreamTracer>::New();
    streamTracer->SetInputData(this->unstructuredGrid);
    streamTracer->SetSourceConnection(pointSource->GetOutputPort());
    streamTracer->SetMaximumPropagation(100);
    streamTracer->SetIntegrationDirectionToBoth();
    streamTracer->SetIntegrator(vtkSmartPointer<vtkRungeKutta4>::New());

    // 生成流线
    vtkNew<vtkPolyDataMapper> streamMapper;
    streamMapper->SetInputConnection(streamTracer->GetOutputPort());

    vtkNew<vtkActor> streamActor;
    streamActor->SetMapper(streamMapper);
    //streamActor->GetProperty()->SetColor(1.0, 1.0, 0.0);  // 流线颜色

    auto basicActor=actorsMap["basicActor"];
    basicActor->SetVisibility(0);

    this->renderer->AddActor(streamActor);
    this->renderer->Render();

}


QString vtkTecplotWidget::AddContour(QString contourDerived)
{//默认命名方式,此时只是完成了contour的搭建和管理，但是并未设置某个contour的具体值
    this->contourNum++;
    std::string contourName = "Contour" + std::to_string(this->contourNum);
    vtkContour* contour = new vtkContour();
    this->contoursList[contourName] = contour; //一个名字contourname对应一个vtkcontour指针去管理
    std::string derivedName = contourDerived.toStdString();
    contour->Initialize(contourName,derivedName,this->actorsList,this->renderer);
    //此时，actorslist里添加了这个contourname对应的actor
    this->actorsStatus[contourName] = true;
    QString name = QString::fromStdString(contourName);
    return name;
}
double* vtkTecplotWidget::SetContourBy(QString contourName,QString propertyName)
{
    vtkContour* contour = this->contoursList[contourName.toStdString()];
    double* range = contour->SetActiveProperty(propertyName.toStdString());
    return range;
}
int vtkTecplotWidget::AddEntry(QString contourName,double value)
{
    vtkContour* contour = this->contoursList[contourName.toStdString()];
    int entryId =contour->AddEntry(value);
    return entryId;
}
bool vtkTecplotWidget::EditEntry(QString contourName,int entryId,double value)
{
    vtkContour* contour = this->contoursList[contourName.toStdString()];
    bool flag = contour->EditEntry(entryId,value);
    return flag;
}
bool vtkTecplotWidget::RemoveEntry(QString contourName,int entryId)
{//注意，remove一个entry后，该entry后面的所有id都要往前挪一位
    vtkContour* contour = this->contoursList[contourName.toStdString()];
    bool flag = contour->RemoveEntry(entryId);
    return flag;
}
void vtkTecplotWidget::ShowContour(QString contourName)
{//注意apply之后，默认会关闭其他所有的actor显示
    for(auto& pair:this->actorsList)
    {//打开的其他actor关闭
        if(pair.first!=contourName.toStdString()){
            vtkActor* actor = pair.second;
            actor->VisibilityOff();
            actorsStatus[pair.first] = false;
        }
    }
    this->renderer->Render();
}
/*************************************************************************************************
 *************************************************************************************************
 *************************************************************************************************
 *********************class vtkCutPlane的实现 ******************************************
 *************************************************************************************************
 *************************************************************************************************
 **************************************************************************************************/
vtkCutPlane::vtkCutPlane(){}
bool vtkCutPlane::GetInputdataStatus()
{
    return this->dataStatus;
}
void vtkCutPlane::SetInputData(vtkUnstructuredGrid* inputData,QVTKInteractor* qvtkInteractor,vtkRenderer* renderer)
{
    this->ug = inputData;
    this->qvtkInteractor=qvtkInteractor;
    this->renderer=renderer;

    this->dataStatus=true;
}
void vtkCutPlane::SetCutPlaneWidget()
{
    if(!this->dataStatus){
        std::cerr << "Please set input data first." << std::endl;
        return;
    }
    //第一次打开widget需要新建widget和plane样例,设置并绑定
    if(!this->widgetStatus)
    {
        this->cutPlaneWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
        this->cutPlaneRep = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();

        this->cutPlaneRep->SetPlaceFactor(1.0);
        this->cutPlaneRep->PlaceWidget(this->ug->GetBounds());
        //this->cutPlaneRep->SetHandleSize(100);
        //this->cutPlaneRep->SetHandleColor(1.0,0.0,0.0);显示vtk8.2没有这个成员函数好像
        this->cutPlaneRep->SetEdgeColor(1.0,0.0,0.0);
        this->cutPlaneWidget->SetInteractor(this->qvtkInteractor);
        this->cutPlaneWidget->SetRepresentation(this->cutPlaneRep);
        //this->cutPlaneWidget->SetRepresentationToSurface();
        //this->cutPlaneWidget->SetResolution(1000);
        //this->cutPlaneWidget->SetRepresentationToOutline();
        //激活widget
        this->cutPlaneWidget->On();

        this->widgetStatus = true;
    }
    this->cutPlaneWidget->SetEnabled(1);
    //需要考虑每次打开widget是否得归位
}
void vtkCutPlane::CloseCutPlaneWidget()
{
    this->cutPlaneWidget->SetEnabled(0);
}
void vtkCutPlane::RemoveCutPlaneWidget()
{
    this->cutPlaneWidget->Off();
    this->widgetStatus = false;
}
void vtkCutPlane::AddCutPlane(std::string& name,std::map<std::string,vtkActor*>& actorsList,std::map<std::string,bool>& actorsStatus)
{
    //如果为输入命名，则默认命名方法
    if(name==""){
        cutActorNum++;
        name="Slice"+std::to_string(cutActorNum);
    }
    vtkSmartPointer<vtkPlane> cutPlane = vtkSmartPointer<vtkPlane>::New();
    vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
    vtkSmartPointer<vtkPolyDataMapper> cutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> cutActor = vtkSmartPointer<vtkActor>::New();
    this->cutPlaneRep->GetPlane(cutPlane);
    cutter->SetCutFunction(cutPlane);
    cutter->SetInputData(this->ug);
    cutter->Update();
    cutMapper->SetInputConnection(cutter->GetOutputPort());
    cutActor->SetMapper(cutMapper);
    this->renderer->AddActor(cutActor);

    //在将新的actor添加进actorlist前，默认关闭其他所有actor的显示
    for(auto& pair: actorsList)
    {
        vtkActor* actor = pair.second;
        actor->VisibilityOff();
        actorsStatus[pair.first] = false;
    }
    actorsList[name] = cutActor;
    actorsStatus[name] = true;
}
/*************************************************************************************************
 *************************************************************************************************
 *************************************************************************************************
 *********************class vtkColorMap的实现 ******************************************
 *************************************************************************************************
 *************************************************************************************************
 **************************************************************************************************/
bool vtkColorMap::SetColorMapObject(std::string name, std::map<std::string,vtkActor*>& actorsList)
{
    /*for(auto pair:actorsList)
    {
        if(pair.first==name)
        {
            this->objName = name;
            this->selectedActor = pair.second;
            this->selectedStatus = true;
            return true;
        }
    }
    return false;
    */
    if(actorsList.count(name)==0)
    {
        return false;
    }
    this->objName = name;
    this->selectedActor = actorsList[name];
    this->selectedStatus = true;
    return true;

}
bool vtkColorMap::SetSolidColor(std::map<std::string,vtkScalarBarActor*>& barsList, QColor RGBA)
{
    if(!selectedStatus) return false;
    //颜色转换
    int r=RGBA.red();
    int g=RGBA.green();
    int b=RGBA.blue();
    double normalizedR = r / 255.0;
    double normalizedG = g / 255.0;
    double normalizedB = b / 255.0;
    if(barsList.count(this->objName)!=0)
    {//如果之前执行过颜色映射，需要关闭颜色映射、设置bar不可见
        this->selectedActor->GetMapper()->ScalarVisibilityOff();
        barsList[this->objName]->VisibilityOff();
    }
    vtkProperty* property=this->selectedActor->GetProperty();
    property->SetColor(normalizedR,normalizedG,normalizedB);
    return true;
}
bool vtkColorMap::SetColorMapVariable(std::string selectedVar, std::map<std::string,vtkSmartPointer<vtkLookupTable> >& lutsList, std::map<std::string,vtkScalarBarActor*>& barsList,vtkRenderer* renderer)
{
    if(!selectedStatus) return false;
    //如果对于这个actor是第一次颜色映射，需要new个新的baractor和lookuptatle,并添加进renderer
    if(lutsList.count(this->objName)==0){
        vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
        auto barActor = vtkSmartPointer<vtkScalarBarActor>::New();
        lutsList[this->objName] = lut;
        barsList[this->objName] = barActor;
        renderer->AddActor2D(barActor);

    }
    auto lut = lutsList[this->objName];
    auto barActor = barsList[this->objName];
    vtkMapper* mapper= this->selectedActor->GetMapper();
    vtkDataSet* dataSet = vtkDataSet::SafeDownCast(mapper->GetInput());
    // 设置 Active Scalars
    int selectedId = dataSet->GetPointData()->SetActiveScalars(selectedVar.c_str());
    vtkDataArray* scalar = dataSet->GetPointData()->GetArray(selectedId);
    lut->SetTableRange(scalar->GetRange());
    lut->SetNumberOfColors(256);
    lut->Build();
    barActor->SetLookupTable(lut);
    mapper->SetScalarRange(scalar->GetRange());
    mapper->SetLookupTable(lut);

    mapper->ScalarVisibilityOn();
    barActor->VisibilityOn();
    return true;
}

/*******************************************************************************************
 * *********************************************************************************************
 * ***************************vtkContour实现************************************************
 * ******************************************************************************************
 * *****************************************************************************************/
void  vtkContour::Initialize(std::string contourName,std::string derivedName,std::map<std::string,vtkActor*>& actorsList,vtkRenderer* renderer)
{//初始化，新建contourfiler、polydatamapper、actor、renderer，但没设置actorsStatus
    this->contour = vtkSmartPointer<vtkContourFilter>::New(); //新建一个filter

    vtkActor* derivedActor = actorsList[derivedName];
    this->data = vtkDataSet::SafeDownCast(derivedActor->GetMapper()->GetInput());//filter的输入
    this->contour->SetInputData(this->data);
    //filter连接mapper、actor
    vtkSmartPointer<vtkPolyDataMapper> contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    contourMapper->SetInputConnection(this->contour->GetOutputPort());
    vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();
    contourActor->SetMapper(contourMapper);
    actorsList[contourName] = contourActor;
    renderer->AddActor(contourActor);
}
double* vtkContour::SetActiveProperty(std::string propertyName)
{//设置进行activescalar
    int scalarId = this->data->GetPointData()->SetActiveScalars(propertyName.c_str());
    return this->data->GetPointData()->GetArray(scalarId)->GetRange();
}
int vtkContour::AddEntry(double value)
{
    this->contour->SetValue(this->valueNum,value);
    this->valueNum++;//指向下一个空的entry位置
    return this->valueNum-1;//id从0开始
}
bool vtkContour::EditEntry(int entryId,double value)
{
    if(entryId < 0 || entryId >= this->valueNum){
        return false;
    }
    this->contour->SetValue(entryId,value);
    return true;
}
bool vtkContour::RemoveEntry(int entryId)
{
    if(entryId < 0 || entryId >= this->valueNum){
        return false;
    }
    std::vector<double> contourValues;
    for (int i = 0; i < this->valueNum; ++i)
    {
        if (i != entryId) // 跳过要删除的等值
        {
            contourValues.push_back(this->contour->GetValue(i));
        }
    }
    this->valueNum--;
    this->contour->SetNumberOfContours(this->valueNum);
    for (size_t i = 0; i < this->valueNum; ++i)
    {
        this->contour->SetValue(static_cast<int>(i), contourValues[i]);
    }
    contourValues.clear();
    return true;
}
