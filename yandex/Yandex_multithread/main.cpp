#include <QCoreApplication>
#include <vector>
#include <mutex>
typedef std::mutex Mutex;
typedef std::lock_guard<std::mutex> Locker;
/*
При описанной реализации класса Bar возможна взаимная блокировка (deadlock) в следующей ситуации :
1.Вызов в потоке №1 метода Add. Вызов блокировки мьютекса m_doubles_.
2.Вызов в потоке №2 метода Find(int). Вызов блокировки мьютекса m_integers_.
3.Попытка блокировки мьютекса m_integers_ в потоке №1.
4. Попытка блокировки мьютекса m_doubles_ в потоке №2.
5.Бесконечный цикл ожидания во всех потоках вызывающих методы  Add, Find
Решение : выполнить блокировку мьютексов во всех методах в одном и том же порядке.

Так же данная реализация не рассчитана на выполнения несколькими потоками 1ой и той же функции (При вызове из 2х потоков метода Add один из потоков будет ждать полного завершения метода Add в другом потоке).
Решение : выполнить отдельную блокировку для каждого из контейнеров, разбить метод Add на 2 метода.
*/

class Bar {
public:
    void Add(int i, double d) {
        AddInt(i);
        AddDouble(d);
        }
    bool Find(int i) {
        return FindInt(i) && Find(double(i));
        }
    //save old interface
    bool Find(double d) {
        return FindDouble(d);
        }
    bool FindDouble(double d) {
        Locker auto_lock(m_doubles_);
        return std::find(doubles_.begin(), doubles_.end(), d) != doubles_.end();
        }
    bool FindInt(int i) {
        Locker auto_lock(m_integers_);
        return std::find(integers_.begin(), integers_.end(), i) != integers_.end();
        }
    void AddInt(int i){
        Locker auto_lock(m_integers_);
        integers_.push_back(i);
        }
    void AddDouble(double d) {
        Locker auto_lock(m_doubles_);
        doubles_.push_back(d);
        }
private:
    std::vector<int> integers_;
    std::vector<double> doubles_;
    Mutex m_integers_;
    Mutex m_doubles_;
};


int main(int argc, char *argv[])
    {
    QCoreApplication a(argc, argv);

    return a.exec();
    }
