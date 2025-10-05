#ifndef COPY2PICO_HPP
#define COPY2PICO_HPP

#include "ui_copy2pico.h"
#include "taskbarprogress.hpp"

#include <QMainWindow>
#include <QGraphicsAnchorLayout>

class copy2pico : public QMainWindow
{
    Q_OBJECT

public:
    copy2pico(QWidget *parent = nullptr);
    virtual ~copy2pico();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    std::unique_ptr<Ui_copy2pico> ui;
    QMenu *fileMenu;
    std::unique_ptr<TaskBarProgress> tbp;

    QSize wsize0;

    void MyCopy(QString src, QString dest);
    void Timer1Timer();
};

#endif // COPY2PICO_HPP