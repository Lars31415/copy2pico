#ifndef DIALOGNOREADY_H
#define DIALOGNOREADY_H

#include <QDialog>

namespace Ui {
class DialogNoReady;
}

class DialogNoReady : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNoReady(QWidget *parent = nullptr);
    ~DialogNoReady();

private:
    Ui::DialogNoReady *ui;

    void timer_1();
    void click(bool);
};

#endif // DIALOGNOREADY_H
