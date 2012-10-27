/********************************************************************************
** Form generated from reading UI file 'guimainwindow.ui'
**
** Created: Sat 27. Oct 01:43:35 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIMAINWINDOW_H
#define UI_GUIMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GUIMainWindow
{
public:
    QAction *actionReadLearningSet;
    QAction *actionReadTestData;
    QWidget *centralWidget;
    QGraphicsView *scaleView;
    QGraphicsView *resultView;
    QComboBox *dimensionChoiceBox;
    QMenuBar *menuBar;
    QMenu *menuPlik;
    QMenu *menuMetody;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GUIMainWindow)
    {
        if (GUIMainWindow->objectName().isEmpty())
            GUIMainWindow->setObjectName(QString::fromUtf8("GUIMainWindow"));
        GUIMainWindow->resize(800, 454);
        actionReadLearningSet = new QAction(GUIMainWindow);
        actionReadLearningSet->setObjectName(QString::fromUtf8("actionReadLearningSet"));
        actionReadTestData = new QAction(GUIMainWindow);
        actionReadTestData->setObjectName(QString::fromUtf8("actionReadTestData"));
        centralWidget = new QWidget(GUIMainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        scaleView = new QGraphicsView(centralWidget);
        scaleView->setObjectName(QString::fromUtf8("scaleView"));
        scaleView->setGeometry(QRect(5, 0, 81, 391));
        resultView = new QGraphicsView(centralWidget);
        resultView->setObjectName(QString::fromUtf8("resultView"));
        resultView->setGeometry(QRect(80, 0, 711, 411));
        dimensionChoiceBox = new QComboBox(centralWidget);
        dimensionChoiceBox->setObjectName(QString::fromUtf8("dimensionChoiceBox"));
        dimensionChoiceBox->setGeometry(QRect(5, 390, 76, 22));
        GUIMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GUIMainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuPlik = new QMenu(menuBar);
        menuPlik->setObjectName(QString::fromUtf8("menuPlik"));
        menuMetody = new QMenu(menuBar);
        menuMetody->setObjectName(QString::fromUtf8("menuMetody"));
        GUIMainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(GUIMainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        GUIMainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuPlik->menuAction());
        menuBar->addAction(menuMetody->menuAction());
        menuPlik->addAction(actionReadLearningSet);
        menuPlik->addAction(actionReadTestData);

        retranslateUi(GUIMainWindow);
        QObject::connect(actionReadLearningSet, SIGNAL(triggered()), GUIMainWindow, SLOT(readLearnFile()));
        QObject::connect(actionReadTestData, SIGNAL(triggered()), GUIMainWindow, SLOT(readTestFile()));
        QObject::connect(dimensionChoiceBox, SIGNAL(currentIndexChanged(int)), GUIMainWindow, SLOT(chooseDimension(int)));

        QMetaObject::connectSlotsByName(GUIMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *GUIMainWindow)
    {
        GUIMainWindow->setWindowTitle(QApplication::translate("GUIMainWindow", "Wykrywanie Anomalii - Metoda losowa", 0, QApplication::UnicodeUTF8));
        actionReadLearningSet->setText(QApplication::translate("GUIMainWindow", "Wczytaj ci\304\205g ucz\304\205cy", 0, QApplication::UnicodeUTF8));
        actionReadTestData->setText(QApplication::translate("GUIMainWindow", "Wczytaj ci\304\205g testowy", 0, QApplication::UnicodeUTF8));
        menuPlik->setTitle(QApplication::translate("GUIMainWindow", "Plik", 0, QApplication::UnicodeUTF8));
        menuMetody->setTitle(QApplication::translate("GUIMainWindow", "Metody", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GUIMainWindow: public Ui_GUIMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIMAINWINDOW_H
