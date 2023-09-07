#include "Logger.h"

void Logger::writeLog(const string& message) {
    // Захватываем эксклюзивную блокировку для записи в файл
    unique_lock<shared_mutex> lock(mutex);

    if (!file.is_open()) {
        // Выводим сообщение об ошибке в стандартный поток ошибок, 
        // если не удалось открыть файл логов.
        cerr << "Failed to open log file." << "\n";
        return;
    }
    
    // Записываем сообщение в файл
    file << message << "\n"; 

    file.close();
}
//---------------------------------------------------------------------------------------------

string Logger::readLog() const {
    // Захватываем разделяемую блокировку для чтения из файла
    shared_lock<shared_mutex> lock(mutex);

    // Создаем объект ifstream для чтения из файла "log.txt"
    ifstream logFile("log.txt");

    if (logFile.fail()) {
        cerr << "Failed to open log file." << "\n";
        return "";
    }

    if (logFile.is_open()) {
        string line;
        string lastLine;

        while (getline(logFile, line)) {
            lastLine = line;
        }

        logFile.close();

        return lastLine;
    }
    else {
        return "";
    }
}