#include "LED_gui.h"

// QT stuff
#include <QtGlobal>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <iostream>
#include <functional>

using namespace std;

LED_App::LED_App(QWidget *parent)
    : QWidget(parent), 
    led(new LED())
    {


  // LED on //
  QGroupBox *LEDonGroup = new QGroupBox(tr("LED ON"));
  QLabel *LEDonLabel = new QLabel(tr("Current / mA:"));

  QDoubleSpinBox *CurrentSpinBox = new QDoubleSpinBox;
  CurrentSpinBox->setRange(-20., 20.);
  CurrentSpinBox->setSingleStep(0.01);
  CurrentSpinBox->setValue(0.00);
  this->led->SetCurr(0.00);

  QObject::connect(CurrentSpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->led->SetCurr(newValue);
});
//   QObject::connect(CurrentSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), [=](double newValue) {
//   led->SetCurr(newValue);
// });


  QPushButton *Btn_LED_on = new QPushButton("LED ON", this);

  QGridLayout *LEDonLayout = new QGridLayout;
  LEDonLayout->addWidget(LEDonLabel, 0, 0);
  LEDonLayout->addWidget(CurrentSpinBox, 1, 0);
  LEDonLayout->addWidget(Btn_LED_on, 2, 0);
  LEDonGroup->setLayout(LEDonLayout);

  // LED off //
  QGroupBox *LEDoffGroup = new QGroupBox(tr("LED Off"));
  QPushButton *Btn_LED_off = new QPushButton("LED OFF", this);
  QGridLayout *LEDoffLayout = new QGridLayout;
  LEDoffLayout->addWidget(Btn_LED_off, 0, 0);
  LEDoffGroup->setLayout(LEDoffLayout);

  // Quit Application //

  QGroupBox *QuitGroup = new QGroupBox();
  QPushButton *Btn_quit = new QPushButton("Quit", this);
  QGridLayout *QuitLayout = new QGridLayout;
  QuitLayout->addWidget(Btn_quit, 0, 0);
  QuitGroup->setLayout(QuitLayout);
  
  // Put Layout together //        
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(LEDonGroup, 0, 1);
  layout->addWidget(LEDoffGroup, 1, 1);
  layout->addWidget(QuitGroup, 3, 1);
  setLayout(layout);

  setWindowTitle(tr("LED Class"));

  connect(Btn_LED_on, &QPushButton::clicked, [=]() {this->led->LEDon();});
  connect(Btn_LED_off, &QPushButton::clicked, [=]() {this->led->LEDoff();});
  connect(Btn_quit, &QPushButton::clicked, &QApplication::quit);
}