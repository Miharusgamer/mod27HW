#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>

using namespace std;

class Logger {
private:
    // Поток для записи в файл
    ofstream file; 
    // Общий мьютекс для синхронизации доступа к файлу
    mutable shared_mutex mutex; 

public:
    Logger(const string& filename) : file(filename, ios_base::app), mutex()
    {
        if (!file.is_open()) {
            // Если файл не существует, создаем его
            file.open(filename);
        }
    }

    ~Logger() {
        if (file.is_open()) {
            // Закрываем файл при уничтожении объекта логгера
            file.close();
        }
    }

    void writeLog(const string& message);

    string readLog() const;
};