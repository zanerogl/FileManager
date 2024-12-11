//
// Created by ian on 24-12-11.
//

#ifndef FILEMANAGER_FILEMANAGER_H
#define FILEMANAGER_FILEMANAGER_H

#include <QWidget>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class FileManager; }
QT_END_NAMESPACE

class FileManager : public QWidget {
Q_OBJECT

public:
    explicit FileManager(QWidget *parent = nullptr);

    ~FileManager() override;

    void setViewPath(const QString &path);

private:
    void init();
    void onItemSelected();
    void createDir();
    void deleteObj();
    void ok();
    void cancel();

private:
    Ui::FileManager *ui;
    QFileSystemModel *m_model = nullptr;
    QString m_viewPath;
    QString m_selectedPath;
};

#endif //FILEMANAGER_FILEMANAGER_H
