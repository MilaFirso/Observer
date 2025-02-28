#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

// Базовый класс наблюдателя
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

// Наблюдаемый класс Logger
class Logger {
public:
    // Добавление наблюдателя
    void addObserver(Observer* observer) {
        observers_.push_back(observer);
    }

    // Удаление наблюдателя
    void removeObserver(Observer* observer) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }

    // Оповещение наблюдателей о предупреждении
    void warning(const std::string& message) const {
        for (auto observer : observers_) {
            if (observer) {
                observer->onWarning(message);
            }
        }
    }

    // Оповещение наблюдателей об ошибке
    void error(const std::string& message) const {
        for (auto observer : observers_) {
            if (observer) {
                observer->onError(message);
            }
        }
    }

    // Оповещение наблюдателей о фатальной ошибке
    void fatalError(const std::string& message) const {
        for (auto observer : observers_) {
            if (observer) {
                observer->onFatalError(message);
            }
        }
    }

private:
    std::vector<Observer*> observers_;
};

// Наблюдатель для предупреждений (вывод в консоль)
class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
};

// Наблюдатель для ошибок (вывод в файл)
class ErrorObserver : public Observer {
public:
    explicit ErrorObserver(const std::string& filePath) : filePath_(filePath) {}

    void onError(const std::string& message) override {
        std::ofstream file(filePath_, std::ios::app);
        if (file.is_open()) {
            file << "Error: " << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Failed to open file: " << filePath_ << std::endl;
        }
    }

private:
    std::string filePath_;
};

// Наблюдатель для фатальных ошибок (вывод в консоль и в файл)
class FatalErrorObserver : public Observer {
public:
    explicit FatalErrorObserver(const std::string& filePath) : filePath_(filePath) {}

    void onFatalError(const std::string& message) override {
        // Вывод в консоль
        std::cerr << "Fatal Error: " << message << std::endl;

        // Вывод в файл
        std::ofstream file(filePath_, std::ios::app);
        if (file.is_open()) {
            file << "Fatal Error: " << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Failed to open file: " << filePath_ << std::endl;
        }
    }

private:
    std::string filePath_;
};

int main() {
    // Создаем наблюдаемый объект
    Logger logger;

    // Создаем наблюдателей
    WarningObserver warningObserver;
    ErrorObserver errorObserver("error_log.txt");
    FatalErrorObserver fatalErrorObserver("fatal_error_log.txt");

    // Добавляем наблюдателей к логгеру
    logger.addObserver(&warningObserver);
    logger.addObserver(&errorObserver);
    logger.addObserver(&fatalErrorObserver);

    // Генерируем события
    logger.warning("This is a warning message.");
    logger.error("This is an error message.");
    logger.fatalError("This is a fatal error message.");

    // Удаляем наблюдателей
    logger.removeObserver(&warningObserver);
    logger.removeObserver(&errorObserver);
    logger.removeObserver(&fatalErrorObserver);

    return 0;
}