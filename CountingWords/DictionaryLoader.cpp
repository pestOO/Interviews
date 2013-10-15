#include "DictionaryLoader.h"

const QRegExp DictionaryLoader::wordRegExp = QRegExp(QFile::decodeName("[^a-zà-ÿ]+"), Qt::CaseInsensitive);

DictionaryLoader::DictionaryLoader(const QString &waytoFile) :
    QObject(NULL), stopped(0), file(waytoFile), stream(&file), fileSize(QFileInfo(file).size())
    {
    Q_ASSERT(file.exists());
    Q_ASSERT(wordRegExp.isValid());
    }
void DictionaryLoader::load()
    {
    Q_ASSERT(dictionary.empty());
    file.open(QIODevice::ReadOnly);
    while(!stream.atEnd())
        {
        QMutexLocker locker(&mutex);
        QString streamWord;
        stream >> streamWord;
        const QStringList wordList = streamWord.split(wordRegExp, QString::SkipEmptyParts);
        foreach (const QString& word, wordList)
            {
            //O(log(N))
            dictionary[word]++;
            qApp->processEvents();
            }
        if(stopped)
            break;
        }
    file.close();
    emit finished();
    }
qint64 DictionaryLoader::getFileSize() const
    {
    return fileSize;
    }
qint64 DictionaryLoader::getCurrentFilePos() const
    {
    return file.pos();
    }
quint32 DictionaryLoader::countWords() const
    {
    return dictionary.size();
    }
QMultiMap<qint32, QString> DictionaryLoader::getMaxWords(const int countWords) const
    {
    //NOTO: mutex need to correct(actual) result returning by this methode
    QMutexLocker locker(&mutex);
    QMultiMap<qint32, QString> outMap;
    //use stl set (not QSet) for fast
    const std::set<quint32> countSet = getCountSet();
    if(!countSet.empty())
        {
        std::set<quint32>::const_iterator it = countSet.end();
        do {
            --it;
            //O(N)
            const QList<QString> words = dictionary.keys(*it);
            foreach (const QString& w, words)
                if(outMap.size() < countWords)
                    outMap.insertMulti(-*it, w);
                else
                    {
                    it = countSet.begin();
                    break;
                    }
            }
        while(it != countSet.begin());
        }
    return outMap;
    }

void DictionaryLoader::saveToFile(const QString &waytoSaveFile) const
    {
    //use stl set (not QSet) for fast
    std::set<quint32> countSet = getCountSet();
    if(!countSet.empty())
        {
        QFile statisticsFile(waytoSaveFile);
        statisticsFile.open(QIODevice::WriteOnly);
        QTextStream streamWriter(&statisticsFile);
        std::set<quint32>::const_iterator it = countSet.end();
        do {
            --it;
            //O(N)
            const QList<QString> words = dictionary.keys(*it);
            foreach (const QString& w, words)
                streamWriter << w << ':' << *it << endl;
            qApp->processEvents();
            }
        while(it != countSet.begin());
        }
    }
void DictionaryLoader::stop()
    {
    stopped = 1;
    }
bool DictionaryLoader::isStopped() const
    {
    return stopped == 1;
    }
std::set<quint32> DictionaryLoader::getCountSet() const
    {
    std::set<quint32> countSet;
    QMap<QString, quint32>::const_iterator it = dictionary.constBegin();
    //O(N)
    while (it != dictionary.end()) {
        countSet.insert(it.value());
        ++it;
        }
    return countSet;
    }
