#pragma once
#include "Header.h"
#include <QFile>
#include <QTextStream>
#include "mytree.h"
#include "mytree.cpp"
#include "fileexception.h"

class TransportParks
{
protected:
    list<TransportPark> parks;                // Список транспортных парков.
public:
    TransportParks();                         // Конструкторы.
    explicit TransportParks(const QString&);
    explicit TransportParks(TransportParks&);
    virtual ~TransportParks();                // Деструктор.
    list<TransportPark>& getParks();          // Геттер и сеттер.
    void setParks(list<TransportPark>&);
    void putInFileTP(const QString&);         // Запись и чтение из файла.
    void readFromFileTP(const QString&);
};
