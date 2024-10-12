/**
 * @file    GeometryWidget.h
 * @brief   stp、step和iges文件的读入和显示
 * @details 主要接口函数即使用都在该文件中
*/

#ifndef GEOMETRYWIDGET_H
#define GEOMETRYWIDGET_H

#include <QWidget>

#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkOCCTReader.h>

#include <vtkSmartPointer.h>
#include <vtkNew.h>
#include <vtkCompositePolyDataMapper.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataArray.h>
#include <vtkCellData.h>

#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkCellPicker.h>
#include <vtkIdList.h>
#include <vtkIdTypeArray.h>
#include <vtkExtractSelection.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkSetGet.h>
#include <vtkDataSetMapper.h>
#include <vtkThreshold.h>

#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

/**
 * @class HighlightInteractorStyle
 * @brief 自定义交互方式
 * @details 能够左键选中面高亮，esc键退出高亮选中交互
 */
class HighlightInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static HighlightInteractorStyle* New();
    vtkTypeMacro(HighlightInteractorStyle, vtkInteractorStyleTrackballCamera);

    HighlightInteractorStyle()
    {
        this->defaultStyle = vtkSmartPointer< vtkInteractorStyleTrackballCamera>::New();
        this->threshold = vtkSmartPointer<vtkThreshold>::New();
        this->displayMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        this->displayMapper->ScalarVisibilityOff();  // 禁用标量可见性
        this->displayActor = vtkSmartPointer<vtkActor>::New();
        this->displayActor->SetMapper(displayMapper);
        this->displayActor->GetProperty()->SetColor(1.0,0, 1);  // Highlighted color
    }
    // 定义回调类型
    std::function<void(int)> SurfIdCallback;
    virtual void OnLeftButtonDown() override
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
        std::cout << "SurfID: " << pickedSurfId << std::endl;
        //下面开始提取相同surfid的cell
        threshold->ThresholdBetween(pickedSurfId, pickedSurfId);
        threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, "SurfID");
        threshold->Update();
        displayMapper->SetInputConnection(threshold->GetOutputPort());
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(displayActor);
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();  //事件转发
    }
    virtual void OnKeyPress() override
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
    void SetInput(vtkSmartPointer<vtkPolyData> data)
    {
        this->PolyData = data;
        threshold->SetInputData(PolyData);
    }
private:
    vtkSmartPointer<vtkPolyData> PolyData;
    vtkSmartPointer<vtkDataSetMapper> displayMapper;
    vtkSmartPointer<vtkActor> displayActor;
    vtkSmartPointer<vtkThreshold> threshold;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> defaultStyle;
};

/**
 * @class GeometryWidget
 * @brief stp、step和iges文件的读入显示
 * @details 能够读取stp、step、iges数据并显示，可以开启或关闭根据所在面ID进行的颜色映射，能够鼠标左键选中面高亮显示
 */
class GeometryWidget:public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit GeometryWidget(QWidget *parent = 0);
    ~GeometryWidget(){}
    /**
     * @brief 读取指定路径的数据并显示
     * @param QString fileName，文件所在路径
     * @param double linearDeflection，默认值是0.01，设置线性偏转
     * @param double angularDeflection，默认值是0.05，设置角度偏差
     */
    void SetInputData(QString fileName,double linearDeflection=0.01, double angularDeflection= 0.05);

    /**
     * @brief   开启高亮交互
     * @details 无默认参数
     * 交互方式为：鼠标左键选中高亮，"esc"键退出该高亮交互方式
     */
    void EnableHighlightMode();
    /**
     * @brief SetColorMapVisibility，是否进行面id颜色映射
     * @param bool flag默认是true，进行颜色映射，当设定为false时，关闭面ID颜色映射
     * @param QString propName，进行颜色映射的属性，默认是SurfID，即每个cell的面ID
     */
    void SetColorMapVisibility(bool flag = true, QString propName = "SurfID");
private:
    vtkSmartPointer<vtkOCCTReader> geoReader;
    vtkSmartPointer<vtkPolyData> polyData;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkLookupTable> lut;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<QVTKInteractor> qvtkInteractor;
    vtkSmartPointer<HighlightInteractorStyle> highlightStyle;
    vtkSmartPointer<vtkCellPicker> cellPicker;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> defaultStyle;
};

#endif // GEOMETRYWIDGET_H
