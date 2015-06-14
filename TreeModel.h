#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include "TreeItem.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel();
    ~TreeModel();

    void release();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // 构建模型数据
    void setupModelData(TreeItem *parent);

    // 更新模型数据
    void updateData();

private:
    TreeItem *_rootItem;
};

#endif // TREEMODEL_H
