#include "copy2pico.h"

#include <QApplication>

#include <QDir>
#include <QLockFile>
#include <QMessageBox>
#include <QString>

int main(int argc, char *argv[]) {
    QString tmpDir = QDir::tempPath();
    QLockFile lockFile(tmpDir + "/copy2pico.lock");
    if (!lockFile.tryLock(100)) {
        return 1;
    }
    QApplication a(argc, argv);
    copy2pico w;
    w.show();
    return a.exec();
}
