#pragma once

#include <QDebug>
#include <QGraphicsView>
#include <QWheelEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class View;}
QT_END_NAMESPACE

class View : public QGraphicsView {
    Q_OBJECT
public:
    View(QWidget * parent = nullptr);
    View(QGraphicsScene * scene, QWidget * parent = nullptr);
signals:
    void changedScale(bool zoomIn);
    void movePlot(int dx, int dy);
    void rotatePlot(int angle);
protected:
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
private:
    QPointF lastPos;
    int dx = 0, dy = 0;
};

