#ifndef DIALOGBUSY_H
#define DIALOGBUSY_H

#include <QDialog>

namespace Ui {
class DialogBusy;
}

class DialogBusy : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogBusy(QWidget *parent = 0);
    ~DialogBusy();
    void updateProgressBar();
    void setLabelText(QString text);
    
private:
    Ui::DialogBusy *ui;
};

#endif // DIALOGBUSY_H
