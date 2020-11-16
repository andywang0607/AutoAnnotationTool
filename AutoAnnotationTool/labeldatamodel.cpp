#include "labeldatamodel.h"
#include "labelcollector.h"

LabelDataModel::LabelDataModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_item(nullptr)
{
}

int LabelDataModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return m_item->dataVec().size();
}

QVariant LabelDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case LabelClass:
        qDebug()<< "item.labelClass"<<m_item->dataVec().at(index.row())->labelClass;
        return QVariant(m_item->dataVec().at(index.row())->labelClass);
    case IsSelect:
        qDebug()<< "item.isSelect"<<m_item->dataVec().at(index.row())->isSelect;
        return QVariant(m_item->dataVec().at(index.row())->isSelect);
    default:
        break;
    }
    return QVariant();
}

bool LabelDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!m_item)
        return false;

    switch (role) {
    case LabelClass:
        m_item->dataVec().at(index.row())->labelClass = value.toString();
        break;
    case IsSelect:
        m_item->dataVec().at(index.row())->isSelect = value.toBool();
        break;
    default:
        break;
    }

    if (m_item->setItemAt(index.row(), m_item->dataVec().at(index.row()))) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags LabelDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> LabelDataModel::roleNames() const
{
  QHash<int, QByteArray> names;
  names[LabelClass] = "labelClass";
  names[IsSelect] = "isSelect";
  return names;
}

LabelCollector *LabelDataModel::item() const
{
    return m_item;
}

void LabelDataModel::setItem(LabelCollector *item)
{
    beginResetModel();

    if(m_item)
        m_item->disconnect(this);

    m_item = item;
    if (m_item) {
        connect(m_item, &LabelCollector::preItemAppended, this, [=]() {
            const int index = m_item->dataVec().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(m_item, &LabelCollector::postItemAppended, this, [=]() {
            endInsertRows();
        });
        connect(m_item, &LabelCollector::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(m_item, &LabelCollector::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
        connect(m_item, &LabelCollector::onModelChanged, this, [=]() {
            emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0), QVector<int>() << IsSelect);
        });
    }
    endResetModel();
}
