#pragma once
#include "transportparks.h"
#include "transportroutes.h"
#include "fileexception.h"

class City : virtual public TransportParks, public TransportRoutes
{
protected:
    QString cityName;                  // Название города.
    QString mapPath;                   // Путь к карте города.
public:
    City();                            // Конструкторы.
    explicit City(const QString&);
    explicit City(City&);
    virtual ~City();                   // Деструктор.

    void setName(const QString&);      // Сеттер и геттер для названия города.
    QString& getName();
    void setPath(const QString&);      // Сеттер и геттер для пути к карте.
    QString& getPath();

    void putInFileC();                 // Запись города в файл.
    void readFromFileC();              // Чтение города из файла.
};
