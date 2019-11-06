#include "AssetsBrowser.hpp"
#include <QMenu>
#include <QTreeView>
#include <QListView>
#include <QHBoxLayout>
#include <QFileSystemModel>
#include <QHeaderView>
#include <iostream>

AssetsBrowser::AssetsBrowser(QWidget *parent) : QWidget(parent)
{
  layout = std::make_unique<QHBoxLayout>(this);
  listView = std::make_unique<QListView>();
  treeView = std::make_unique<QTreeView>();
  dirModel = std::make_unique<QFileSystemModel>(this);
  fileModel = std::make_unique<QFileSystemModel>(this);
  contextMenu = std::make_unique<QMenu>();
  initialize();
}

void AssetsBrowser::initialize()
{
  layout->addWidget(treeView.get());
  layout->addWidget(listView.get());

  //QDir cur;
  //cur.cdUp();
  //QString path(ASSETS_PATH);
  QString path("../../PikaEditor/Resources");

  dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
  //dirModel->setRootPath(path);
  treeView->setModel(dirModel.get());
  treeView->setRootIndex(dirModel->setRootPath(path));

  fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
  //fileModel->setRootPath(path);
  listView->setModel(fileModel.get());
  listView->setRootIndex(fileModel->setRootPath(path));

  connect(treeView.get(), &QTreeView::clicked, this, &AssetsBrowser::on_tree_view_clicked);
  connect(listView.get(), &QListView::doubleClicked, this, &AssetsBrowser::on_list_view_double_clicked);
}


AssetsBrowser::~AssetsBrowser()
{
  dirModel.reset(nullptr);
  fileModel.reset(nullptr);
  listView.reset(nullptr);
  treeView.reset(nullptr);
  layout.reset(nullptr);
  contextMenu.reset(nullptr);
}

void AssetsBrowser::on_tree_view_clicked(const QModelIndex& idx)
{
  QString path = dirModel->fileInfo(idx).absoluteFilePath();
  listView->setRootIndex(fileModel->setRootPath(path));
}

void AssetsBrowser::on_list_view_double_clicked(const QModelIndex& idx)
{
  QDir dir = fileModel->fileInfo(idx).absoluteFilePath();

  const QModelIndex i = dirModel->setRootPath(dir.path());
  listView->setRootIndex(fileModel->setRootPath(dir.path()));

  treeView->setExpanded(i.parent(), true);
  treeView->setExpanded(i, true);
}

void AssetsBrowser::SetStyle()
{
  this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  this->setStyleSheet("background: #414345; color: #EEEEEE;");
  this->treeView->setStyleSheet("background: #303133; color: #EEEEEE;");
  this->treeView->header()->setStyleSheet("QHeaderView::section {background-color: #303133; color: #EEEEEE; border: none;}");
  this->listView->setStyleSheet("background: #303133; color: #EEEEEE;");
}

const QListView* AssetsBrowser::ListView() const
{
  return this->listView.get();
}

const QTreeView* AssetsBrowser::TreeView() const
{
  return this->treeView.get();
}

const QFileInfo AssetsBrowser::ListFileItem(const QModelIndex& idx) const
{
  return dirModel->fileInfo(idx);
}
