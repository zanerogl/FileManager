#include "../include/filemanager.h"
#include "../ui/ui_filemanager.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

FileManager::FileManager(QWidget *parent) : QWidget(parent), ui(new Ui::FileManager) {
    ui->setupUi(this);
    init();
}

FileManager::~FileManager() {
    delete ui;
}

void FileManager::setViewPath(const QString &path) {
    m_viewPath = path;
    m_selectedPath = m_viewPath;
    QModelIndex rootIndex = m_model->setRootPath(m_viewPath);
    ui->treeView->setRootIndex(rootIndex);  // 设置树形视图的根索引
}

void FileManager::init() {
    m_model = new QFileSystemModel;
    m_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    ui->treeView->setModel(m_model);
    ui->treeView->allColumnsShowFocus();
    ui->treeView->setColumnHidden(2, true);     // 隐藏 "Type" 列
    ui->treeView->setColumnWidth(0, 300);       // 设置 "name" 列宽
    ui->treeView->setColumnWidth(1, 50);        // 设置 "size" 列宽
    ui->treeView->setColumnWidth(3, 80);        // 设置 "date" 列宽

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &FileManager::onItemSelected);

    connect(ui->newDirBtn, &QPushButton::clicked, this, &FileManager::createDir);
    connect(ui->deleteObjBtn, &QPushButton::clicked, this, &FileManager::deleteObj);
    connect(ui->okBtn, &QPushButton::clicked, this, &FileManager::ok);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &FileManager::cancel);
}

void FileManager::onItemSelected() {
    QModelIndex currentIndex = ui->treeView->currentIndex();

    if (currentIndex.isValid()) {
        QString path = m_model->filePath(currentIndex);
        if (!m_model->isDir(currentIndex)) {
            path = QFileInfo(path).absolutePath();  // 如果不是目录，获取父目录的路径
        }
        m_selectedPath = path;  // 记录目录路径
        qDebug() << "Directory Path:" << path;
    }
}

void FileManager::createDir() {
    QString currentPath = m_selectedPath;   // 获取当前路径

    // 弹出输入对话框，获取新目录名
    bool ok;
    QString newDirName = QInputDialog::getText(this, tr("Create Directory"),
                                               tr("Directory name:"), QLineEdit::Normal,
                                               "", &ok);
    if (ok && !newDirName.isEmpty()) {
        QRegExp invalidChars(R"([/\\:*?"<>|])");
        if (newDirName.contains(invalidChars)) {    // 检查是否有不允许的字符
            QMessageBox::warning(this, tr("Invalid Name"),
                                 tr("Directory name contains invalid characters."));
            return;
        }

        // 创建新目录
        QDir dir(currentPath);
        if (dir.mkdir(newDirName)) {
            qDebug() << "Directory created:" << dir.absoluteFilePath(newDirName);
        } else {
            QMessageBox::warning(this, tr("Create Directory"),
                                 tr("Failed to create directory."));
        }
    }
}

void FileManager::deleteObj() {
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, tr("Delete Directory"), tr("No item selected."));
        return;
    }

    QString path = m_model->filePath(currentIndex);
    if (!m_model->isDir(currentIndex)) {
        QMessageBox::information(this, tr("Delete Directory"), tr("Selected item is not a directory."));
        return;
    }

    QDir dir(path);

    // 确认删除操作
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Delete Directory"),
                                  tr("Are you sure you want to delete this directory?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (dir.removeRecursively()) {
            qDebug() << "Directory deleted:" << path;
        } else {
            QMessageBox::warning(this, tr("Delete Directory"), tr("Failed to delete directory."));
        }
    }
}

void FileManager::ok() {
    QModelIndex currentIndex = ui->treeView->currentIndex();

    if (currentIndex.isValid()) {
        QString path = m_model->filePath(currentIndex);
        bool isDir = m_model->isDir(currentIndex);
        qDebug() << "Path:" << path << "Is Directory:" << isDir;
    } else {
        qDebug() << "No item selected.";
    }
}

void FileManager::cancel() {
    this->close();
}