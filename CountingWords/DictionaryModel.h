#pragma once
#ifndef DictionaryModel_H
#define DictionaryModel_H
#include <QAbstractTableModel>
#include <QMutex>

class DictionaryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DictionaryModel(QObject *parent = 0);
    //QAbstractTableModel implemetation
    int rowCount(const QModelIndex &parent = QModelIndex() ) const;
    int columnCount(const QModelIndex &parent = QModelIndex() ) const;
    QVariant data(const QModelIndex &index, int role) const;
public slots:
    //sets dictioary with pairs [-count, word], negative for growing order
    void setTopDictionary(const QMultiMap<qint32, QString> &dict);
private:
    QMultiMap<qint32, QString> dictionary;
};

#endif // DictionaryModel_H
