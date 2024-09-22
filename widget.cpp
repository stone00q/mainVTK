#include "widget.h"
#include "ui_widget.h"

#include <vtkDataSetMapper.h>
#include <vtkSmartPointer.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkCylinderSource.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>

#include <qgridlayout.h>
#include <qpushbutton.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //创建自定义widget实例
    //m_tecplotWidget = new TecplotWidget(this);
    m_geometryWidget = new GeometryWidget(this);
    //在当前widget上布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    //layout->addWidget(m_tecplotWidget);
    layout->addWidget(m_geometryWidget);
    QPushButton* interactorButton = new QPushButton("高亮交互",this);
    layout->addWidget(interactorButton);
    connect(interactorButton,&QPushButton::clicked,this,&Widget::interactorButton_clicked);
    /******测试contour的功能*****
    QPushButton* XButton = new QPushButton("Q==1000切片",this);
    QPushButton* YButton = new QPushButton("T=300,350(Y==0.03)切片",this);
    QPushButton* Xcolor = new QPushButton("x颜色映射",this);
    QPushButton* GlyphButton = new QPushButton("速度映射(在x=0.03切片上)",this);
    QPushButton* QButton = new QPushButton("sudu",this);
    layout->addWidget(XButton);
    layout->addWidget(YButton);
    layout->addWidget(Xcolor);
    layout->addWidget(GlyphButton);
    layout->addWidget(QButton);*/
    //设置vtktecplotwidget
    //m_tecplotWidget->SetFileName(R"(D:\Project\VTK_QT\data\Tur_Merge_Field_[2000].dat)");
    m_geometryWidget->SetFileName(R"(D:\Project\VTK_QT\data\f6_clean_dpwII_v2_whole_cf2 v3.step)");
    /*int a = m_tecplotWidget->GetNumberOfProperty();
    cout <<endl<<a;
    // 连接信号槽
    connect(XButton,&QPushButton::clicked,this,&Widget::XButton_clicked);
    connect(YButton,&QPushButton::clicked,this,&Widget::YButton_clicked);
    connect(Xcolor,&QPushButton::clicked,this,&Widget::Xcolor_clicked);
    connect(GlyphButton,&QPushButton::clicked,this,&Widget::GlyphButton_clicked);
    connect(QButton,&QPushButton::clicked,this,&Widget::QButton_clicked);*/
}

Widget::~Widget()
{
    delete ui;
}
/*
void Widget::BackgroundButton_clicked()
{
    //渲染窗口为浅蓝色
    QColor color=QColor(173, 216, 230);
    m_tecplotWidget->SetBackgroundColor(color);
}
void Widget::SolidColorButton_clicked()
{
    //固体为绿色
    QColor green(0, 255, 0);
    m_tecplotWidget->SetSolidColor(green);
}

void Widget::XButton_clicked()
{
    //x坐标颜色映射
    //tecplotWidget->SetColorMapVariable("p");
    tecplotWidget->SetColorMappingFlag(true);
}
void Widget::CutPlaneButton_clicked()
{
    m_tecplotWidget->SetCutPlaneWidget();
}
void Widget:: ColseCutWidget_clicked()
{
    m_tecplotWidget->SetCutPlaneWidget(false);
}
void Widget::CutButton_clicked()
{
    QString tmpName = m_tecplotWidget->AddCutPlane();
    std::cout << endl <<"cutplane name:" << tmpName.toStdString();
}
void Widget::basicButton1_clicked()
{
    m_tecplotWidget->SetColorMapObject("basicActor");
    QColor green(0, 255, 0);
    m_tecplotWidget->SetSolidColor(green);
}
void Widget::basicButton2_clicked()
{
    m_tecplotWidget->SetColorMapObject("basicActor");
    m_tecplotWidget->SetColorMapVariable("p");
}
void Widget::slice1Button1_clicked()
{
    m_tecplotWidget->SetColorMapObject("Slice1");
    QColor red(255, 0, 0);
    m_tecplotWidget->SetSolidColor(red);
}
void Widget::slice1Button2_clicked()
{
    m_tecplotWidget->SetColorMapObject("Slice1");
    m_tecplotWidget->SetColorMapVariable("X");
}

void Widget::XButton_clicked()
{//需要先点击
    QString name = m_tecplotWidget->AddContour("basicActor");
    m_tecplotWidget->SetContouredBy(name,"QCriterion");
    cout<<name.toStdString()<<endl;
    int entryid = m_tecplotWidget->AddEntry(name,1000);
    m_tecplotWidget->ShowContour(name);

}
void Widget::YButton_clicked()
{
    QString name = m_tecplotWidget->AddContour("Contour1");
    m_tecplotWidget->SetContouredBy(name,"T");
    //cout<<name.toStdString()<<endl;
    m_tecplotWidget->AddEntry(name,350);
    m_tecplotWidget->AddEntry(name,300);
    m_tecplotWidget->ShowContour(name);
}
void Widget::Xcolor_clicked()
{
    m_tecplotWidget->SetColorMapObject("Contour1");
    m_tecplotWidget->SetColorMapVariable("p");
}
void Widget::GlyphButton_clicked()
{
    QString name = m_tecplotWidget->AddGlyph("Contour1");
    m_tecplotWidget->SetGlyphVector(name,"velocity");
    m_tecplotWidget->SetGlyphPointsNumber(name,1000);
    m_tecplotWidget->ShowGlyph(name);
}
void Widget::QButton_clicked()
{
    m_tecplotWidget->CalculateQCriterion("basicActor");
}*/
void Widget::interactorButton_clicked()
{
    m_geometryWidget->EnableHighlightMode();
}
