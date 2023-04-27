#include "interface.h"



using namespace std;


void logging(queue* qu, int count, std::chrono::_V2::system_clock::time_point start) {
    auto l = qu->queue;

    auto nst = chrono::duration_cast<chrono::microseconds>((l[0]->start_generate - start));

    cout << "STARTING TIME\n\n";
    cout << "|N|Генерация|Умножение|Запись|\n";
    for (int i = 0; i < count; i++) {
        auto dur1 = chrono::duration_cast<chrono::microseconds>((l[i]->start_generate - start));
        auto dur2 = chrono::duration_cast<chrono::microseconds>((l[i]->start_mul - start));
        auto dur3 = chrono::duration_cast<chrono::microseconds>((l[i]->start_write - start));
        cout << "|" << i << "|" << dur1.count() - nst.count() << "|" << dur2.count() - nst.count() << "|" << dur3.count() - nst.count() << "|\n";
    }

    cout << "FINISHING TIME\n\n";
    cout << "|N|Генерация|Умножение|Запись|\n";
    for (int i = 0; i < count; i++) {
        auto dur1 = chrono::duration_cast<chrono::microseconds>((l[i]->stop_generate - start));
        auto dur2 = chrono::duration_cast<chrono::microseconds>((l[i]->stop_mul - start));
        auto dur3 = chrono::duration_cast<chrono::microseconds>((l[i]->stop_write - start));
        cout << "|" << i << "|" << dur1.count() - nst.count() << "|" << dur2.count() - nst.count() << "|" << dur3.count() - nst.count() << "|\n";
    }


    cout << "Время простоя\n";
    auto gent = chrono::duration_cast<chrono::microseconds>((l[1]->start_generate - l[0]->start_generate));
    auto kent = chrono::duration_cast<chrono::microseconds>((l[1]->start_mul - l[0]->start_mul));
    auto went = chrono::duration_cast<chrono::microseconds>((l[1]->start_write - l[0]->start_write));
    for (int i = 1; i < int(l.size() - 1); i++) {

        gent += chrono::duration_cast<chrono::microseconds>((l[i+1]->start_generate - l[i]->start_generate));
        kent += chrono::duration_cast<chrono::microseconds>((l[i+1]->start_mul - l[i]->start_mul));
        went += chrono::duration_cast<chrono::microseconds>((l[i+1]->start_write - l[i]->start_write));
    }
    
    cout << "|" << gent.count() << "|" << kent.count() << "|" << went.count() << "|\n";
}