/**
 * @file  widget.h
 * @brief 对GeometryWidget一些功能实现的测试
 * @details
*/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include "GeometryWidget.h"
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
/**
 * @class Widget
 * @brief 测试案例
 * @details 主要设置了三个按钮，分别是高亮交互highlightButton、开启颜色映射openColorMapButton和关闭颜色映射closeColorMapButton
 * 其中按钮槽函数的实现都是调用GeometryWidget的成员函数去实现的。
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Widget构造函数
     * @param parent父类
     * @details 需要在构造函数中，调用GeometryWidget类的SetFileName()函数指定读取的文件，该函数参数为文件路径
     */
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    /**
     * @brief 高亮交互
     * @details 调用GeometryWidget类的EnableHighlightMode()函数实现，无参数
     * @memberof Widget
     */
    void highlightButton_clicked();

    /**
     * @brief 开启颜色映射
     * @details 调用GeometryWidget类的SetColorMapVisibility()函数实现，默认参数
     * @memberof Widget
     */
    void openColorMapButton_clicked();

    /**
     * @brief 关闭颜色映射
     * @details 调用GeometryWidget类的SetColorMapVisibility()函数实现，参数是FALSE
     * @memberof Widget
     */
    void closeColorMapButton_clicked();
    void setDeflectionButton_clicked();
    void handleSurfIdPicked(int surId)
    {
        qInfo()<< "emit信号接收：surfid "<<surId;
    }
private:
    Ui::Widget *ui;
    GeometryWidget* m_geometryWidget;
};
#endif // WIDGET_H
