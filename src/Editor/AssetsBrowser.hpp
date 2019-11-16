#ifndef ASSETSEXPLORER_HPP
#define ASSETSEXPLORER_HPP

#include <QWidget>
#include <memory>

class QMenu;
class QListView;
class QTreeView;
class QFileInfo;
class QHBoxLayout;
class QFileSystemModel;
class AssetsBrowser : public QWidget {

  Q_OBJECT

public:
  explicit AssetsBrowser(QWidget *parent = nullptr);
  ~AssetsBrowser();

  void SetStyle();

  const QListView* ListView() const;
  const QTreeView* TreeView() const;
  const QFileInfo ListFileItem(const QModelIndex& idx) const;

protected:
  void initialize();

signals:

public slots:

private slots:
  void on_tree_view_clicked(const QModelIndex& idx);
  void on_list_view_double_clicked(const QModelIndex& idx);

private:
  std::unique_ptr<QFileSystemModel> dirModel;
  std::unique_ptr<QFileSystemModel> fileModel;
  std::unique_ptr<QListView> listView;
  std::unique_ptr<QTreeView> treeView;
  std::unique_ptr<QHBoxLayout> layout;
  std::unique_ptr<QMenu> contextMenu;
};

#endif // ASSETSEXPLORER_HPP
