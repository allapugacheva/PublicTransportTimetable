#pragma once
#include "Header.h"
#include <QFile>
#include <QTextStream>
#include "mytree.h"
#include "mytree.cpp"
#include "fileexception.h"

class TransportStops
{
protected:
    MyTree<Stop> stops;                          // Бинарное дерево остановок.
public:
    TransportStops();                            // Конструкторы.
    explicit TransportStops(TransportStops&);
    explicit TransportStops(const QString&);
    virtual ~TransportStops();                   // Деструктор.
    MyTree<Stop>& getStops();                    // Геттер и сеттер.
    void setStops(MyTree<Stop>&);
    void readFromFileTS(const QString&);         // Чтение и запись в файл
    void putInFileTS(const QString&);            // и вспомогательные алгоритмы.
    void readTime(int, QTextStream&, Node*&);
    void putTime(int, QTextStream&, Node*&);
};
