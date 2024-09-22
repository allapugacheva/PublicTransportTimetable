#pragma once
#include <QFile>
#include <QTextStream>
#include "mytree.h"
#include "mytree.cpp"
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include "transportparks.h"
#include "transportstops.h"
#include "fileexception.h"

class TransportRoutes : public TransportStops, virtual public TransportParks
{
protected:
    list<Route> routes;                                   // Список маршрутов.
public:
    TransportRoutes();                                    // Конструкторы.
    explicit TransportRoutes(TransportRoutes&);
    explicit TransportRoutes(const QString&);
    virtual ~TransportRoutes();                           // Деструктор.
    list<Route>& getRoutes();                             // Геттер и сеттер.
    void setRoutes(list <Route>&);
    void putInFileTR(const QString&);                     // Запись и чтение из файла и вспомогательные
    void readFromFileTR(const QString&);                  // алгоритмы.
    void readStops(bool, QTextStream&, Route&, const QString&);
    void readTime(int, QTextStream&, Route&, const QString&, bool);
    void putTime(int, QTextStream&, Route&);              // Поиск маршрута между остановками.
    void findRoute(Node*, Node*, Node*, QString, int, int, int&, QStringList&);
};
