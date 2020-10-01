#include "inputbox.h"

InputBox::InputBox(QString text, int numberOfBoxes, bool floatValue, int boxDefaultValue, int range,
                   bool negativeRange, QWidget* parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    QLabel* label = new QLabel(text, this);
    layout->addWidget(label);
    if(floatValue) {
        for(int i = 0; i < numberOfBoxes; ++i) {
            QDoubleSpinBox* box = new QDoubleSpinBox(this);
            if(negativeRange) {
                box->setRange(-range, range);
            } else {
               box->setRange(0, range);
            }
            box->setValue(boxDefaultValue);
            layout->addWidget(box);
        }
    } else {
        for(int i = 0; i < numberOfBoxes; ++i) {
            QSpinBox* box = new QSpinBox(this);
            if(negativeRange) {
                box->setRange(-range, range);
            } else {
               box->setRange(0, range);
            }
            box->setValue(boxDefaultValue);
            layout->addWidget(box);
        }
    }
    this->setLayout(layout);
}
