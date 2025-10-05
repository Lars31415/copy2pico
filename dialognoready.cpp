#include "dialognoready.h"
#include "file_utils.hpp"
#include "ui_dialognoready.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QTimer>

DialogNoReady::DialogNoReady(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogNoReady)
{
    ui->setupUi(this);
    QTimer::singleShot(100, this, &DialogNoReady::timer_1);
    connect(ui->buttonBox->buttons().at(0), &QPushButton::clicked, this, &DialogNoReady::click);
}

DialogNoReady::~DialogNoReady()
{
    delete ui;
}

void DialogNoReady::timer_1()
{
    if(fileExists("B://."))
        QDialog::accept();
    QTimer::singleShot(250, this, &DialogNoReady::timer_1);
}

void DialogNoReady::click(bool)
{
    QDialog::reject();
}
