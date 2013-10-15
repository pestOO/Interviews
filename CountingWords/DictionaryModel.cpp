#include "DictionaryModel.h"

DictionaryModel::DictionaryModel(QObject *parent) :
    QAbstractTableModel(parent)
    { }
int DictionaryModel::rowCount(const QModelIndex &parent) const
    {
    return dictionary.size();
    }
int DictionaryModel::columnCount(const QModelIndex &parent) const
    {
    return (dictionary.empty() ? 0 : 2);
    }
QVariant DictionaryModel::data(const QModelIndex &index, int role) const
    {
    if(index.isValid())
        {
        if(role == Qt::TextAlignmentRole)
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        //on edit and disaplay call @getData
        if( role==Qt::DisplayRole)
            {
            QMultiMap<qint32, QString>::ConstIterator it = dictionary.begin();
            it+=index.row();
            if(0 == index.column())
                return it.value();
            return -it.key();
            }
        }
    return QVariant();
    }
void DictionaryModel::setTopDictionary(const QMultiMap<qint32, QString> &dict)
    {
    emit layoutAboutToBeChanged();
    dictionary = dict;
    emit layoutChanged();
    }
