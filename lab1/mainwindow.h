#ifndef MAINWINDOWz_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDockWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent* event);
private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    QVector<QGraphicsItem*> points;
    int currentScale = 5;
    int parameterValue = 0;
    void addGridText(QGraphicsScene* scene, int cord, bool isXAxis, QFont font);
    int getNumberOfDigits(int number) const;
private slots:
    void resizeGraph(int scale);
    void drawPlot();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
