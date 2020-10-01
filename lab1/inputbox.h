#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <QDoubleSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class InputBox : public QWidget {
    Q_OBJECT
public:
    InputBox(QString text, int numberOfBoxes = 1, bool floatValue = true, int boxDefaultValue = 0,
             int range = 100, bool negativeRange = false, QWidget* parent = nullptr);
};

#endif // INPUTBOX_H
