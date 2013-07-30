#include <QCoreApplication>
#include <QHostAddress>
#include <sstream>
/* using std */
std::string IntIpToString(const unsigned int ip)
    {
    std::stringstream stream;
    stream << ((ip >> 24) & 0xFF) << '.'
           << ((ip >> 16) & 0xFF) << '.'
           << ((ip >>  8) & 0xFF) << '.'
           << ( ip & 0xFF);
    return stream.str();
    }
/* using Qt library */
QString QIntIpToString(const quint32 ip)
    {
    return QHostAddress(ip).toString ();
    }

int main(int argc, char *argv[])
    {
    QCoreApplication a(argc, argv);
    const quint32 ip = 0xFE000FFF;
    qDebug() << QString::fromStdString (IntIpToString(ip));
    qDebug() << QIntIpToString(ip);
    return a.exec();
    }
