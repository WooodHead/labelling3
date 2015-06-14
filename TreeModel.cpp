#include "TreeModel.h"
#include "Global.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>
#include <QFileInfo>

TreeModel::TreeModel()
{
    _rootItem = 0;

    updateData();
}

TreeModel::~TreeModel()
{
    if(_rootItem)
    {
        delete _rootItem;
        _rootItem=NULL;
    }
}

void TreeModel::release()
{
    if(_rootItem)
    {
        delete _rootItem;
        _rootItem=NULL;
    }
}

void TreeModel::updateData()
{
    if(_rootItem)
    {
        delete _rootItem;
        _rootItem=NULL;
    }

    QList<QVariant> rootData;
    rootData << tr("列表");

    _rootItem = new TreeItem(rootData);
    setupModelData(_rootItem);

    reset();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role==Qt::DecorationRole&&index.column()==0)
        return Global::Awesome->icon(plane);

    if(role == Qt::DisplayRole)
    {
        TreeItem *item=static_cast<TreeItem*>(index.internalPointer());
        return item->data(index.column());
    }

    return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column,const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    TreeItem *parentItem;
    if(!parent.isValid())
    {
        parentItem=_rootItem;
    }else
    {
        parentItem=static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem=parentItem->child(row);
    if(childItem)
        return createIndex(row,column,childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    TreeItem *childItem=static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem=childItem->parent();

    if(parentItem==_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(),0,parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;

    if(!parent.isValid())
        parentItem=_rootItem;
    else
        parentItem=static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent ) const
{
    return _rootItem->columnCount();
}

void TreeModel::setupModelData(TreeItem *parent)
{
    QList<QVariant> departIDs;
    QList<QVariant> planeTypes;
    QList<QVariant> planeIDs;
    QList<QVariant> paths;

    QStringList departIDsCopy;
    QStringList planeTypesCopy;
    QStringList planeIDsCopy;
    QStringList pathsCopy;

    QSqlDatabase db;
    QSqlQuery query;
    if(Global::createConnection(db))
    {
        QString sql = QString("select equipmentinfo.departid, equipmentinfo.planetype, equipmentinfo.planeid, ferrographypicinfo.ferrographypicpath from ferrographypicinfo join ferrographyinfo join oilsampleinfo join equipmentinfo where ferrographypicinfo.ferrographysheetid = ferrographyinfo.ferrographysheetid and ferrographyinfo.oilsampleid = oilsampleinfo.oilsampleid and oilsampleinfo.planeid = equipmentinfo.planeid");
        query.prepare(sql);

        if(query.exec())
        {
            while(query.next())
            {
                departIDs << query.value(0).toString();
                departIDsCopy << query.value(0).toString();

                planeTypes << query.value(1).toString();
                planeTypesCopy << query.value(1).toString();

                planeIDs << query.value(2).toString();
                planeIDsCopy << query.value(2).toString();

                paths << query.value(3).toString();
                pathsCopy << query.value(3).toString();
            }
        }
    }

    departIDsCopy.removeDuplicates();
    planeTypesCopy.removeDuplicates();
    planeIDsCopy.removeDuplicates();
    pathsCopy.removeDuplicates();

    QList<QVariant> data;
    for(int i = 0; i < departIDsCopy.count(); i++)
    {
        data.clear();
        data << departIDsCopy[i];

        TreeItem* departsNode = new TreeItem(data, parent);
        parent->appendChild(departsNode);

        for(int j = 0; j < planeTypes.size();)
        {
            if(departIDs[j] == departIDsCopy[i])
            {
                data.clear();
                QString planeTypeItem = planeTypes[j].toString();
                data << planeTypes[j];

                TreeItem* planeTypeNode = new TreeItem(data, departsNode);
                departsNode->appendChild(planeTypeNode);

                for(int k = 0; k < planeIDs.size();)
                {
                    if(departIDs[k] == departIDsCopy[i] && planeTypes[k] == planeTypeItem)
                    {
                        data.clear();
                        QString planeIDItem = planeIDs[k].toString();
                        data << planeIDs[k];

                        TreeItem* planeIDNode = new TreeItem(data, planeTypeNode);
                        planeTypeNode->appendChild(planeIDNode);

                        for(int m = 0; m < paths.size();)
                        {
                            if(departIDs[m] == departIDsCopy[i] && planeTypes[m] == planeTypeItem && planeIDs[m] == planeIDItem)
                            {
                                data.clear();
                                data << QFileInfo(paths[m].toString()).fileName();

                                planeIDNode->appendChild(new TreeItem(data, planeIDNode));

                                departIDs.removeAt(m);
                                planeTypes.removeAt(m);
                                planeIDs.removeAt(m);
                                paths.removeAt(m);
                            }
                            else
                            {
                                m++;
                            }
                        }
                    }
                    else
                    {
                        k++;
                    }
                }
            }
            else
            {
                j++;
            }
        }
    }
}

