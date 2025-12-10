#include "copy2pico.h"
#include "dialognoready.h"
#include "file_utils.hpp"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <iostream>

copy2pico::copy2pico(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_copy2pico), wsize0(0, 0)
{
    // qApp->setStyle("Fusion");
    ui->setupUi(this);
    tbp.reset(new TaskBarProgress(this));
    tbp->startProgressMode();
    tbp->setProgress(0, 100);
    ui->progressBar->setStyleSheet(QPBStyleNormal);

    QTimer::singleShot(1, this, &copy2pico::Timer1Timer);
}

copy2pico::~copy2pico()
{
}

void copy2pico::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QSize wsize1 = ui->centralwidget->size();
    QSize wsize2 = wsize1 - wsize0;
    if (wsize0.width() != 0)
        ui->progressBar->resize(ui->progressBar->width() + wsize2.width(), ui->progressBar->height());
    wsize0 = wsize1;
}

void copy2pico::MyCopy(QString src, QString dest)
{
    const size_t BUF_SIZE = 0x0200;

    ui->progressBar->setValue(0);

    std::ifstream in(src.toStdString().c_str(), std::ios::in | std::ios::binary);

    std::ofstream out(dest.toStdString().c_str(), std::ios::out | std::ios::binary);

    in.seekg(0, std::ios::end);
    uint32_t size = in.tellg();
    in.seekg(0, std::ios::beg);
    size_t togo = size;
    char buf[BUF_SIZE];
    ui->progressBar->setStyleSheet(QPBStyleNormal);
    size_t rs = std::min(togo, BUF_SIZE);
    while ((!in.fail()) && (!in.eof()) && (!out.fail()) && (togo > 0))
    {
        in.read(buf, rs);
        out.write(buf, rs);
        togo -= rs;
        QCoreApplication::processEvents();
        tbp->setProgress(std::ceil((100.0 * (size - togo)) / size), 100, TBPF_NORMAL);
        ui->progressBar->setValue(std::ceil((100.0 * (size - togo)) / size));
        for (int i = 0; i < 2; ++i)
        {
            QCoreApplication::processEvents();
            Sleep(1);
        }
        rs = std::min(togo, BUF_SIZE);
    }
    if(out.fail())
    {
        ui->progressBar->setStyleSheet(QPBStyleError);
        QMessageBox msgBox;
        msgBox.setText("Error while writing.");
        msgBox.exec();
    }
    else
    {
        tbp->setProgress(100, 100, TBPF_NORMAL);
        ui->progressBar->setValue(100);
    }
    for (int i = 0; i < 10; ++i)
    {
        QCoreApplication::processEvents();
        Sleep(50);
    }
}

void copy2pico::Timer1Timer() {
    ui->progressBar->setValue(0);
    bool flag = true;
    if (QApplication::arguments().count() >= 1) {
        QString fn;
        if (QApplication::arguments().size() > 1)
            fn = QApplication::arguments().at(1);
        if ((fileExists(fn)) && (!fn.isEmpty())) {
            QString fn2 = "B://" + extractFileName(fn);
            QString ext = extractFileExt(fn);
            setWindowTitle(extractFileName(fn));
            while (flag) {
                ui->progressBar->setStyleSheet(QPBStyleNormal);
                tbp->setState(TBPF_NORMAL);
                QCoreApplication::processEvents();
                flag = false;
                if (fileExists("B://.")) {
                    if (ext.toLower() == "uf2") {
                        MyCopy(fn, fn2);
                    } else {
                        tbp->setError();
                        ui->progressBar->setStyleSheet(QPBStyleError);
                        ui->progressBar->setValue(0);
                        ui->progressBar->setRange(0, 100);
                        QCoreApplication::processEvents();
                        QMessageBox msgBox;
                        msgBox.setText("File is not a uf2.");
                        msgBox.exec();
                    }
                } else {
                    tbp->setPaused();
                    ui->progressBar->setStyleSheet(QPBStylePause);
                    ui->progressBar->setValue(0);
                    ui->progressBar->setRange(0, 100);
                    QCoreApplication::processEvents();
                    DialogNoReady form;
                    int ret = form.exec();
                    flag = (ret == 1);
                }
            }
        } else {
            std::cout << __FILE__ << " " << __FUNCTION__ << " " << std::dec
                      << __LINE__ << std::endl;
            tbp->setError();
            ui->progressBar->setStyleSheet(QPBStyleError);
            ui->progressBar->setValue(0);
            ui->progressBar->setRange(0, 100);
            QCoreApplication::processEvents();
            QMessageBox msgBox;
            msgBox.setText(fn + ": File not found!");
            msgBox.exec();
        }
    } else {
        tbp->setError();
        ui->progressBar->setStyleSheet(QPBStyleError);
        ui->progressBar->setValue(0);
        ui->progressBar->setRange(0, 100);
        QCoreApplication::processEvents();
        QMessageBox msgBox;
        msgBox.setText("File name is missing");
        msgBox.exec();
    }
    QApplication::quit();
}
