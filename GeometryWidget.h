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
        this->displayActor = vtkSmartPointer<vtkActor>::New();
        this->displayActor->SetMapper(displayMapper);
        this->displayActor->GetProperty()->SetColor(1, 0, 0);  // Highlighted color
    }
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
        std::cout << "CellId: " << cellPicker->GetCellId() << std::endl;
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

class GeometryWidget:public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit GeometryWidget(QWidget *parent = 0);
    ~GeometryWidget(){}
    void SetFileName(QString fileName);
    void EnableHighlightMode();
    void SetColorMapVisibility(bool flag = true, QString propName = "SurfID");
private:
    vtkSmartPointer<vtkOCCTReader> geoReader;
    vtkSmartPointer<vtkPolyData> polyData;
    // vtkSmartPointer<vtkCompositePolyDataMapper> mapper;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkLookupTable> lut;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<QVTKInteractor> qvtkInteractor;
    vtkSmartPointer<HighlightInteractorStyle> highlightStyle;
    vtkSmartPointer<vtkCellPicker> cellPicker;
};

#endif // GEOMETRYWIDGET_H
