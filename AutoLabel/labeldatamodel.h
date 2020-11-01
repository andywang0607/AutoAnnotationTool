#ifndef LABELDATAMODEL_H
#define LABELDATAMODEL_H

#include <QAbstractListModel>

class LabelCollector;

class LabelDataModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(LabelCollector *item READ item WRITE setItem)
public:
    explicit LabelDataModel(QObject *parent = nullptr);

    enum {
      LabelClass = Qt::UserRole,
      IsSelect,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    LabelCollector * item() const;

public slots:
    void setItem(LabelCollector * item);

private:
    LabelCollector * m_item;
};

#endif // LABELDATAMODEL_H
