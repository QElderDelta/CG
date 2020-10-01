#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::resizeEvent(QResizeEvent* event)
{
    if(parameterValue) {
        view->fitInView(-parameterValue, -parameterValue, parameterValue * 2, parameterValue * 2, Qt::KeepAspectRatio);
    }
    resizeGraph(5);
    this->findChild<QSlider*>()->setSliderPosition(5);
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
    scene = new QGraphicsScene;
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
    QDockWidget* dockWidget = new QDockWidget;
    QWidget* inputBox = new QWidget;
    QVBoxLayout* inputLayout = new QVBoxLayout;
    QLineEdit* line = new QLineEdit;
    QPushButton* button = new QPushButton("Draw");
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(1);
    slider->setMinimum(1);
    slider->setMaximum(10);
    slider->setSliderPosition(5);
    QIntValidator* v = new QIntValidator;
    line->setValidator(v);
    line->setPlaceholderText("Enter value of parameter a");
    inputLayout->addWidget(line);
    inputLayout->addWidget(button);
    inputLayout->addWidget(slider);
    inputBox->setLayout(inputLayout);
    dockWidget->setWidget(inputBox);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(resizeGraph(int)));
    QObject::connect(button, SIGNAL(pressed()), this, SLOT(drawPlot()));
}

int MainWindow::getNumberOfDigits(int number) const {
    int count = 0;
    while(number) {
        count++;
        number /= 10;
    }
    return count;
}

void MainWindow::resizeGraph(int scale) {
    view->scale(1. / (10 * currentScale), 1. / (10 * currentScale));
    view->scale(10 * scale, 10 * scale);
    currentScale = scale;
}

void MainWindow::drawPlot() {
    for(auto& item : points) {
        scene->removeItem(item);
    }
    points.clear();
    QDockWidget* child = this->findChild<QDockWidget*>();
    int a = child->findChild<QLineEdit*>()->text().toInt();
    parameterValue = a;
    QVector<QPointF> topLineStorage;
    QVector<QPointF> bottomLineStorage;
    QPen plotPen(Qt::red, 0.1, Qt::SolidLine, Qt::FlatCap);
    qreal power = 2. / 3.;
    int step = qPow(10, getNumberOfDigits(a) - 1);
    for(int i = -a * 100; i < a * 100; i += step) {
        qreal x = i / 100.;
        qreal ac = qPow(a, power);
        qreal xc = qPow(x,  2.);
        xc = qPow(xc, 1 / 3.);
        qreal y = ac - xc;
        y = qPow(y, 3 / 2.);
        topLineStorage.push_back(QPointF(x, -y));
        bottomLineStorage.push_back(QPointF(x, y));
    }
    for(int i = 0; i < topLineStorage.size() - 1; ++i) {
        points.push_back(scene->addLine(QLineF(topLineStorage[i], topLineStorage[i + 1]), plotPen));
    }
    topLineStorage.clear();
    for(int i = bottomLineStorage.size() - 1; i > 0; --i) {
        points.push_back(scene->addLine(QLineF(bottomLineStorage[i], bottomLineStorage[i - 1]), plotPen));
    }
    bottomLineStorage.clear();
    resizeGraph(5);
    this->findChild<QSlider*>()->setSliderPosition(5);
    view->fitInView(-parameterValue, -parameterValue, parameterValue * 2, parameterValue * 2, Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete ui;
}

