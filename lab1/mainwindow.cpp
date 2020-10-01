#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::resizeEvent(QResizeEvent* event)
{
    if(parameterValue_) {
        view->fitInView(-parameterValue_, -parameterValue_, parameterValue_ * 2, parameterValue_ * 2,
                        Qt::KeepAspectRatio);
    }
    QMainWindow::resizeEvent(event);
}

void MainWindow::addGridText(QGraphicsScene* scene, int cord, bool isXAxis, QFont font) {
    int numberOfDigits = getNumberOfDigits(qAbs(cord)) - 1;
    int xOffset = -5, yOffset = -5;
    auto item = new QGraphicsTextItem;
    item->setFont(font);
    if(isXAxis && cord >= 0) {
        item->setPos(cord + xOffset - numberOfDigits * 0.25, yOffset);
    } else if(isXAxis && cord < 0) {
        item->setPos(cord + xOffset - numberOfDigits * 0.5, yOffset);
    } else {
        if(cord >= 0) {
            item->setPos(xOffset - numberOfDigits * 0.5, cord + yOffset);
        } else {
            item->setPos(xOffset - numberOfDigits * 0.25, cord + yOffset);
        }
    }
    if(isXAxis) {
        item->setPlainText(QString::number(cord));
    } else {
        item->setPlainText(QString::number(-cord));
    }
    scene->addItem(item);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    QPen gridPen(Qt::black, 0.005, Qt::SolidLine, Qt::FlatCap);
    QPen axisPen(Qt::black, 0.05, Qt::SolidLine, Qt::FlatCap);
    QFont q("Times", 1);
    q.setPixelSize(1);
    scene->addLine(0, 100, 0, -100, axisPen);
    scene->addLine(100, 0, -100, 0, axisPen);
    for(int i = -100; i <= 100; i += 2) {
        if(!i) {
            addGridText(scene, i, true, q);
        } else {
            scene->addLine(i, 100, i, -100, gridPen);
            scene->addLine(100, i, -100, i, gridPen);
            addGridText(scene, i, true, q);
            addGridText(scene, i, false, q);
        }
    }
    view = new QGraphicsView(scene);
    view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    view->fitInView(-10, -10, 20, 20, Qt::KeepAspectRatio);
    setCentralWidget(view);
    QDockWidget* dockWidget = new QDockWidget(this);
    QWidget* inputArea = new QWidget(dockWidget);
    QVBoxLayout* inputLayout = new QVBoxLayout(inputArea);
    InputBox* parameterInput = new InputBox("Значение параметра", 1, false, 0,
                                            std::numeric_limits<int>::max(), true, dockWidget);
    InputBox* approx = new InputBox("Апроксимация", 1, false, 1000, 1000, false, dockWidget);
    InputBox* scale = new InputBox("Масштаб по X, Y", 2, false, 0, 100, true, dockWidget);
    InputBox* shift = new InputBox("Смещение по X, Y", 2, true, 0, 100, true, dockWidget);
    InputBox* angle = new InputBox("Угол поворота", 1, true, 0, 359, true, dockWidget);
    inputLayout->addWidget(parameterInput);
    inputLayout->addWidget(approx);
    inputLayout->addWidget(scale);
    inputLayout->addWidget(shift);
    inputLayout->addWidget(angle);
    inputArea->setLayout(inputLayout);
    dockWidget->setWidget(inputArea);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    connect(parameterInput->findChild<QSpinBox*>(), SIGNAL(valueChanged(int)), this, SLOT(parameterChanged(int)));
    connect(approx->findChild<QSpinBox*>(), SIGNAL(valueChanged(int)), this, SLOT(approxChanged(int)));
    auto scaleValue = scale->findChildren<QSpinBox*>();
    connect(scaleValue.front(), SIGNAL(valueChanged(int)), this, SLOT(xScaleChanged(int)));
    connect(scaleValue.back(), SIGNAL(valueChanged(int)), this, SLOT(yScaleChanged(int)));
    connect(angle->findChild<QDoubleSpinBox*>(), SIGNAL(valueChanged(double)), this, SLOT(rotationAngleChanged(double)));
}

int MainWindow::getNumberOfDigits(int number) const {
    int count = 0;
    while(number) {
        count++;
        number /= 10;
    }
    return count;
}

void MainWindow::approxChanged(int numberOfPoints) {
    if(numberOfPoints) {
        numberOfPoints_ = numberOfPoints;
        drawPlot();
    }
}
void MainWindow::parameterChanged(int paramter) {
    if(paramter) {
        parameterValue_ = paramter;
        drawPlot();
    }
}

void MainWindow::rotationAngleChanged(double angle) {
    view->rotate(-rotationAngle_);
    rotationAngle_ = angle;
    view->rotate(rotationAngle_);
}

void MainWindow::xScaleChanged(int xScale) {
    view->scale(1. / currentXScale_, 1. / currentYScale_);
    currentXScale_ = 50 + xScale;
    resizePlot();
}

void MainWindow::yScaleChanged(int yScale) {
    view->scale(1. / currentXScale_, 1. / currentYScale_);
    currentYScale_ = 50 + yScale;
    resizePlot();
}

void MainWindow::resizePlot() {
    view->scale(currentXScale_, currentYScale_);
}

void MainWindow::drawPlot() {
    for(auto& item : points) {
        scene->removeItem(item);
    }
    points.clear();
    QVector<QPointF> topLineStorage;
    QVector<QPointF> bottomLineStorage;
    QPen plotPen(Qt::red, 0.1, Qt::SolidLine, Qt::FlatCap);
    qreal power = 2. / 3.;
    qreal multiplier = qPow(10, getNumberOfDigits(numberOfPoints_));
    int step = 4 * parameterValue_ * multiplier / numberOfPoints_;
    for(int i = -parameterValue_ * multiplier; i <= parameterValue_ * multiplier; i += step) {
        qreal x = i / multiplier;
        qreal ac = qPow(parameterValue_, power);
        qreal xc = qPow(x,  2.);
        xc = qPow(xc, 1 / 3.);
        qreal y = ac - xc;
        y = qPow(y, 3 / 2.);
        topLineStorage.push_back(QPointF(x, -y));
        bottomLineStorage.push_back(QPointF(x, y));
    }
    for(int i = 0; i < topLineStorage.size() - 1; ++i) {
        points.push_back(scene->addLine(QLineF(topLineStorage[i], topLineStorage[i + 1]),
                         plotPen));
    }
    topLineStorage.clear();
    for(int i = bottomLineStorage.size() - 1; i > 0; --i) {
        points.push_back(scene->addLine(QLineF(bottomLineStorage[i], bottomLineStorage[i - 1]),
                         plotPen));
    }
    bottomLineStorage.clear();
    qDebug() << points.size();
    view->fitInView(-parameterValue_, -parameterValue_, parameterValue_ * 2, parameterValue_ * 2,
                    Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete ui;
}

