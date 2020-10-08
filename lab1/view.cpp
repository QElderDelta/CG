#include "view.h"

View::View(QWidget *parent) : QGraphicsView(parent) {}

View::View(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent) {}

void View::wheelEvent(QWheelEvent *event) {
    if(event->delta() > 0) {
        emit changedScale(true);
    } else {
        emit changedScale(false);
    }
    event->accept();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->localPos().x() - lastPos.x();
    int dy = event->localPos().y() - lastPos.y();
    if(event->buttons() == Qt::LeftButton) {
        emit rotatePlot(dx);
    } else if(event->buttons() == Qt::RightButton) {
        emit movePlot(dx, dy);
    }
    lastPos = event->localPos();
}

