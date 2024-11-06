#include "GeometryWidget.h"
vtkStandardNewMacro(HighlightInteractorStyle);
GeometryWidget::GeometryWidget(QWidget *parent)
    :QVTKOpenGLNativeWidget(parent)
{

    this->geoReader = vtkSmartPointer<vtkOCCTReader>::New();
    this->lut = vtkSmartPointer<vtkLookupTable>::New();
    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->renderWindow->AddRenderer(renderer);
    this->SetRenderWindow(renderWindow);

    this->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    this->actor = vtkSmartPointer<vtkActor>::New();
    this->actor->SetMapper(mapper);
    this->highlightStyle = vtkSmartPointer<HighlightInteractorStyle>::New();
    this->defaultStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    this->cellPicker=vtkSmartPointer<vtkCellPicker>::New();
    //获取交互器
    this->qvtkInteractor = this->GetInteractor();
    this->qvtkInteractor->SetInteractorStyle(this->defaultStyle);
    this->qvtkInteractor->SetPicker(this->cellPicker);
    QColor lightBlue(82, 87, 110);
    renderer->SetBackground(lightBlue.redF(), lightBlue.greenF(), lightBlue.blueF());
}
void GeometryWidget::SetInputData(QString fileName)
{
    //clock_t start = clock();
    this->geoReader->SetFileName(fileName.toStdString().c_str());
    if(fileName.endsWith(".step")||fileName.endsWith(".stp"))
    {
        this->geoReader->SetFileFormat(vtkOCCTReader::Format::STEP);// 执行读取操作
    }else if(fileName.endsWith(".iges")||fileName.endsWith(".igs"))
    {
        this->geoReader->SetFileFormat(vtkOCCTReader::Format::IGES);
    }else
    {
        cout<<"报错"<<endl;
    }
    this->hasData=true;
    this->geoReader->Update();
    vtkMultiBlockDataSet* multiBlock = vtkMultiBlockDataSet::SafeDownCast(this->geoReader->GetOutput());
    //clock_t end = clock();
    // 计算运行时间（以秒为单位）
    //double elapsed_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    //std::cout<< "runtime:" << elapsed_time << " seconds" << std::endl;
    this->polyData = vtkPolyData::SafeDownCast(multiBlock->GetBlock(0));//取出第一块
    this->mapper->SetInputData(this->polyData);
    this->mapper->ScalarVisibilityOff();

    this->renderer->AddActor(this->actor);
    this->renderer->ResetCamera();
    this->renderWindow->Render();
}
void GeometryWidget::SetDeflection(double linearDeflection, double angularDeflection)
{
    if(!this->hasData) return;
    this->geoReader->SetLinearDeflection(linearDeflection);
    this->geoReader->SetAngularDeflection(angularDeflection);
    this->geoReader->Update();
    this->polyData = vtkPolyData::SafeDownCast(this->geoReader->GetOutput()->GetBlock(0));
    this->mapper->SetInputData(this->polyData);
    this->renderer->ResetCamera();
    this->renderWindow->Render();
}
void GeometryWidget::SetColorMapVisibility(bool flag, QString propName)
{
    if(flag)
    {
        this->mapper->ScalarVisibilityOn();  //标量可见
        this->mapper->SetScalarModeToUseCellData();
        int propId = this->polyData->GetCellData()->SetActiveScalars(propName.toStdString().c_str()); // 设置标量
        vtkDataArray* propData = this->polyData->GetCellData()->GetArray(propId);

        lut->SetNumberOfColors(256);
        this->lut->Build();
        // 设置查找表的颜色范围
        this->mapper->SetScalarRange(propData->GetRange());
        this->mapper->SetLookupTable(lut);
    }else
    {
        this->mapper->ScalarVisibilityOff();
    }

    this->renderer->ResetCamera();
    this->renderWindow->Render();
}
void GeometryWidget::EnableHighlightMode()
{
    this->highlightStyle->SetInput(this->polyData,this);
    this->qvtkInteractor->SetInteractorStyle(this->highlightStyle);
    this->qvtkInteractor->Initialize();
}
HighlightInteractorStyle::HighlightInteractorStyle()
{
    this->defaultStyle = vtkSmartPointer< vtkInteractorStyleTrackballCamera>::New();
    this->threshold = vtkSmartPointer<vtkThreshold>::New();
    this->displayMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    this->displayMapper->ScalarVisibilityOff();  // 禁用标量可见性
    this->displayActor = vtkSmartPointer<vtkActor>::New();
    this->displayActor->SetMapper(displayMapper);
    this->displayActor->GetProperty()->SetColor(1.0,0, 1);  // Highlighted color
}
void HighlightInteractorStyle::OnLeftButtonDown()
{
    //鼠标点击位置[x, y]   this->Picker = vtkCellPicker::SafeDownCast(this->GetInteractor()->GetPicker());
    int* clickPos = this->Interactor->GetEventPosition();

    // 进行point拾取
    auto picker = this->Interactor->GetPicker();
    picker->Pick(clickPos[0], clickPos[1], 0, this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

    // 获取拾取到的cell ID
    auto cellPicker = vtkCellPicker::SafeDownCast(picker);
    auto cellId = cellPicker->GetCellId();
    if (cellId == -1) {
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        return;
    }
    auto surfIdArray = vtkIntArray::SafeDownCast(PolyData->GetCellData()->GetArray("SurfID"));
    if (!surfIdArray)
    {
        cout<<"SurfID array not found!"<<endl;
        return;
    }
    int pickedSurfId = surfIdArray->GetValue(cellId);
    //std::cout << "SurfID: " << pickedSurfId << std::endl;
    emit this->geoWidget->SurfIdPicked(pickedSurfId);
    //下面开始提取相同surfid的cell
    threshold->ThresholdBetween(pickedSurfId, pickedSurfId);
    threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, "SurfID");
    threshold->Update();
    displayMapper->SetInputConnection(threshold->GetOutputPort());
    this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(displayActor);
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();  //事件转发
}
void HighlightInteractorStyle::OnRightButtonDown()
{

    //检查鼠标位置，如果位置在cell外就不弹出菜单
    //鼠标点击位置[x, y]   this->Picker = vtkCellPicker::SafeDownCast(this->GetInteractor()->GetPicker());
    int* clickPos = this->Interactor->GetEventPosition();

    // 进行point拾取
    auto picker = this->Interactor->GetPicker();
    picker->Pick(clickPos[0], clickPos[1], 0, this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

    // 获取拾取到的cell ID及其surfid
    auto cellPicker = vtkCellPicker::SafeDownCast(picker);
    auto cellId = cellPicker->GetCellId();
    if (cellId == -1)
    {
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
        return;
    }
    auto surfIdArray = vtkIntArray::SafeDownCast(PolyData->GetCellData()->GetArray("SurfID"));
    if (!surfIdArray)
    {
        cout<<"SurfID array not found!"<<endl;
        return;
    }
    int pickedSurfId = surfIdArray->GetValue(cellId);
    // 弹出菜单
    QMenu menu;
    QAction* action1 = menu.addAction("1");
    QAction* action2 = menu.addAction("2");
    QAction* action3 = menu.addAction("3");

    QAction* selectedAction = menu.exec(QCursor::pos());
    if (selectedAction == nullptr) return;// 用户没有选择菜单项，菜单会自动关闭
    // 选择菜单后，根据菜单选项设置属性
    int propertyValue = 0;
    if (selectedAction == action1) propertyValue = 1;
    else if (selectedAction == action2) propertyValue = 2;
    else if (selectedAction == action3) propertyValue = 3;

    // 获取或创建 "property" 数组
    vtkSmartPointer<vtkIntArray> propertyArray = vtkIntArray::SafeDownCast(PolyData->GetCellData()->GetArray("property"));
    if (!propertyArray) {
        propertyArray = vtkSmartPointer<vtkIntArray>::New();
        propertyArray->SetName("property");
        propertyArray->SetNumberOfComponents(1);
        propertyArray->SetNumberOfTuples(PolyData->GetNumberOfCells());
        propertyArray->Fill(0);
        PolyData->GetCellData()->AddArray(propertyArray);
    }

    // 遍历所有 cells，检查 SurfID，并设置 property
    for (vtkIdType i = 0; i < PolyData->GetNumberOfCells(); ++i) {
        int currentSurfId = surfIdArray->GetValue(i);
        if (currentSurfId == pickedSurfId) { // 比较目标 SurfID
            propertyArray->SetValue(i, propertyValue); // 设置对应的 property 值
            //qInfo()<<propertyArray->GetValue(i);
        }
    }
    qInfo()<<"cell个数"<<PolyData->GetNumberOfCells();
    for (vtkIdType i = 0; i < PolyData->GetNumberOfCells(); ++i) {
        std::cout<<propertyArray->GetValue(i)<<" ";
    }
    std::cout<<endl;
    // 刷新显示
    //displayMapper->SetInputConnection(threshold->GetOutputPort());
    //displayMapper->Update();
    //this->Interactor->Render();


}
void HighlightInteractorStyle::OnKeyPress()
{
    // 获取按下的键
    std::string key = this->Interactor->GetKeySym();
    // 如果按下的是 "Escape" 键
    if (key == "Escape")
    {
        // 调用 InteractionManager 退出高亮模式
        if (this->Interactor)
        {
            this->Interactor->SetInteractorStyle(defaultStyle);
        }
    }
    // 调用父类方法，处理其他按键
    vtkInteractorStyleTrackballCamera::OnKeyPress();
}
void HighlightInteractorStyle::SetInput(vtkSmartPointer<vtkPolyData> data,GeometryWidget* widget)
{
    this->PolyData = data;
    threshold->SetInputData(PolyData);
    geoWidget = widget;
}
