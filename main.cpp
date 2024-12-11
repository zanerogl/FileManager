#include <QApplication>
#include "include/filemanager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    FileManager *fM;
    fM = new FileManager;
    fM->setViewPath("/mnt");
    fM->show();

    return QApplication::exec();
}
