#pragma once
#ifndef DictionaryLoader_H
#define DictionaryLoader_H
#include <QtCore>
#include <set>

class DictionaryLoader : public QObject
{
    Q_OBJECT
public:
    explicit DictionaryLoader(const QString& waytoFile);
    qint64 getFileSize() const;
    qint64 getCurrentFilePos() const;
    quint32 countWords() const;
public slots:
    // map with pair [-count, word], negative count for growing order
    QMultiMap<qint32, QString> getMaxWords(const int countWords) const;
    void load();
    void stop();
    bool isStopped() const;
    void saveToFile(const QString& waytoSaveFile) const;
signals:
    void finished();
protected:
    static const QRegExp wordRegExp;
    std::set<quint32> getCountSet() const;
private:
    QAtomicInt stopped;
    QFile file;
    QTextStream stream;
    qint64 fileSize;
    QMap<QString, quint32> dictionary;
    mutable QMutex mutex;
};

#endif // DictionaryLoader_H
