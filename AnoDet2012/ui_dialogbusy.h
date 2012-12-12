/********************************************************************************
** Form generated from reading UI file 'dialogbusy.ui'
**
** Created: Wed 5. Dec 23:02:31 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGBUSY_H
#define UI_DIALOGBUSY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_DialogBusy
{
public:
    QLabel *label;
    QProgressBar *progressBar;

    void setupUi(QDialog *DialogBusy)
    {
        if (DialogBusy->objectName().isEmpty())
            DialogBusy->setObjectName(QString::fromUtf8("DialogBusy"));
        DialogBusy->resize(239, 102);
        DialogBusy->setModal(true);
        label = new QLabel(DialogBusy);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 171, 16));
        progressBar = new QProgressBar(DialogBusy);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(30, 60, 211, 23));
        progressBar->setMaximum(0);
        progressBar->setValue(-1);

        retranslateUi(DialogBusy);

        QMetaObject::connectSlotsByName(DialogBusy);
    } // setupUi

    void retranslateUi(QDialog *DialogBusy)
    {
        DialogBusy->setWindowTitle(QApplication::translate("DialogBusy", "Prosz\304\231 czeka\304\207", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogBusy", "Trwa przetwarzanie", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogBusy: public Ui_DialogBusy {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGBUSY_H
