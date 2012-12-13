/********************************************************************************
** Form generated from reading UI file 'dialogfilter.ui'
**
** Created: Thu 13. Dec 02:56:11 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGFILTER_H
#define UI_DIALOGFILTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogFilter
{
public:
    QDialogButtonBox *buttonBox;
    QListWidget *listWidget;

    void setupUi(QDialog *DialogFilter)
    {
        if (DialogFilter->objectName().isEmpty())
            DialogFilter->setObjectName(QString::fromUtf8("DialogFilter"));
        DialogFilter->resize(400, 300);
        buttonBox = new QDialogButtonBox(DialogFilter);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        listWidget = new QListWidget(DialogFilter);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(50, 20, 291, 211));

        retranslateUi(DialogFilter);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogFilter, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogFilter, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogFilter);
    } // setupUi

    void retranslateUi(QDialog *DialogFilter)
    {
        DialogFilter->setWindowTitle(QApplication::translate("DialogFilter", "Wybierz pomiary do wy\305\233wietlenia", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogFilter: public Ui_DialogFilter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGFILTER_H
