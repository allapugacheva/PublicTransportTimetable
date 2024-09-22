#include "city.h"

City::City() : TransportParks(), TransportRoutes()// Конструкторы.
{
    this->cityName = "";
    this->mapPath = "";
}
City::City(const QString& str) : TransportParks(str), TransportRoutes(str)
{
    this->cityName = str;
    this->readFromFileC();
}
City::City(City& c) : TransportParks(c), TransportRoutes(c)
{
    this->cityName = c.cityName;
    this->mapPath = c.mapPath;
}
City::~City()                                   // Деструктор.
{
    this->putInFileC();
}
void City::setName(const QString& str)          // Сеттеры и геттеры.
{
    this->cityName = str;
}
QString& City::getName()
{
    return this->cityName;
}
void City::setPath(const QString& path)
{
    this->mapPath = path;
}
QString& City::getPath()
{
    return this->mapPath;
}
void City::putInFileC()                        // Сохранение в файл города.
{
    try
    {
        QFile file(this->cityName + "Карта.txt");  // Открытие файла с данными о карте.
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);             // Файловый поток.
            stream << this->mapPath;               // Запись данных.
            file.close();                          // Закрытие файла.
        }
        else                                       // Ошибка.
            throw FileException("Ошибка во время открытия файла", this->cityName + "Карта.txt");
        this->putInFileTR(this->cityName);         // Запись маршрутов.
        this->putInFileTP(this->cityName);         // Запись парков.
        this->putInFileTS(this->cityName);         // Запись остановок.
    }
    catch(FileException& ex)
    {
        ex.mesg();                                 // Информация об ошибках.
    }
}

void City::readFromFileC()                         // Чтение из файла города.
{
    try
    {
        QFile file(this->cityName + "Карта.txt");  // Открытие файла.
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&file);             // Файловый поток.
            this->mapPath = stream.readLine();     // Чтение данных.
            file.close();                          // Закрытие файла.
        }
        else                                       // Ошибка.
            throw FileException("Ошибка во время открытия файла", this->cityName + "Карта.txt");
    }
    catch(FileException& ex)
    {
        ex.mesg();                                 // Информация об ошибке.
    }
}
