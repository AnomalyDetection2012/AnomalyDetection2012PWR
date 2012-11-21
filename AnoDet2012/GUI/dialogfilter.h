#ifndef DIALOGFILTER_H
#define DIALOGFILTER_H

#include <QDialog>
#include <QListWidgetItem>


namespace Ui {
class DialogFilter;
}

class DialogFilter : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogFilter(QWidget *parent = 0);
    ~DialogFilter();

    void addCheckboxesToList(std::vector<QListWidgetItem *> &items);
    
private:
    Ui::DialogFilter *ui;
//    QStandardItemModel *model;
};

#endif // DIALOGFILTER_H
