/********************************************************************************
** Form generated from reading UI file 'simulator.ui'
**
** Created: Fri Nov 16 22:30:40 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMULATOR_H
#define UI_SIMULATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SimpleUI
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphics_view;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *frame_spinbox;
    QSlider *frame_slider;
    QPushButton *play_PB;
    QPushButton *stop_PB;
    QPushButton *reload_PB;

    void setupUi(QWidget *SimpleUI)
    {
        if (SimpleUI->objectName().isEmpty())
            SimpleUI->setObjectName(QString::fromUtf8("SimpleUI"));
        SimpleUI->resize(905, 717);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SimpleUI->sizePolicy().hasHeightForWidth());
        SimpleUI->setSizePolicy(sizePolicy);
        SimpleUI->setStyleSheet(QString::fromUtf8("QWidget {background-image: url(:/images/images/HexaBus-Steckdose.jpg)}"));
        verticalLayoutWidget = new QWidget(SimpleUI);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 881, 691));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        graphics_view = new QGraphicsView(verticalLayoutWidget);
        graphics_view->setObjectName(QString::fromUtf8("graphics_view"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphics_view->sizePolicy().hasHeightForWidth());
        graphics_view->setSizePolicy(sizePolicy1);
        graphics_view->setInteractive(false);
        graphics_view->setSceneRect(QRectF(0, 0, 800, 600));

        verticalLayout->addWidget(graphics_view);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        frame_spinbox = new QSpinBox(verticalLayoutWidget);
        frame_spinbox->setObjectName(QString::fromUtf8("frame_spinbox"));

        horizontalLayout->addWidget(frame_spinbox);

        frame_slider = new QSlider(verticalLayoutWidget);
        frame_slider->setObjectName(QString::fromUtf8("frame_slider"));
        frame_slider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(frame_slider);

        play_PB = new QPushButton(verticalLayoutWidget);
        play_PB->setObjectName(QString::fromUtf8("play_PB"));
        QFont font;
        font.setPointSize(14);
        font.setBold(false);
        font.setWeight(50);
        play_PB->setFont(font);

        horizontalLayout->addWidget(play_PB);

        stop_PB = new QPushButton(verticalLayoutWidget);
        stop_PB->setObjectName(QString::fromUtf8("stop_PB"));
        stop_PB->setFont(font);

        horizontalLayout->addWidget(stop_PB);

        reload_PB = new QPushButton(verticalLayoutWidget);
        reload_PB->setObjectName(QString::fromUtf8("reload_PB"));
        reload_PB->setFont(font);

        horizontalLayout->addWidget(reload_PB);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SimpleUI);

        QMetaObject::connectSlotsByName(SimpleUI);
    } // setupUi

    void retranslateUi(QWidget *SimpleUI)
    {
        SimpleUI->setWindowTitle(QApplication::translate("SimpleUI", "FullCircle Simulator", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SimpleUI", "Frame", 0, QApplication::UnicodeUTF8));
        play_PB->setText(QApplication::translate("SimpleUI", "&Play", 0, QApplication::UnicodeUTF8));
        stop_PB->setText(QApplication::translate("SimpleUI", "&Stop", 0, QApplication::UnicodeUTF8));
        reload_PB->setText(QApplication::translate("SimpleUI", "&Reload", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SimpleUI: public Ui_SimpleUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMULATOR_H
