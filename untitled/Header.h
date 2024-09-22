#pragma once
#include <iostream>
#include <QString>
#include "mytree.h"
#include "mytree.cpp"
#include <QTime>
#include <list>
#include <QStringList>
using namespace std;

struct Bus                              // Структура для данных автобуса.
{
    QString model;                      // Модель.
    QString number;                     // Номер.
    bool hasApparel;                    // Наличие аппарели.
    bool hasConditioner;                // Наличие кондиционера.
    bool isLarge;                       // Особо большой вместимости.
    Bus() {}                            // Конструкторы
    Bus(const QString& str)
    {
        this->number = str;
    }
    bool operator==(const Bus& b)       // Перегрузка оператора сравнения.
    {
        return this->number == b.number;
    }
    bool operator<(const Bus& b)        // Перегрузка операторов больше, меньше
    {
        return this->number < b.number;
    }
    bool operator>(const Bus& b)
    {
        return this->number > b.number;
    }
    bool operator<=(const Bus& b)      // Перегрузка операторов больше или равно,
    {                                  // меньше или равно.
        return this->number <= b.number;
    }
    bool operator>=(const Bus& b)
    {
        return this->number >= b.number;
    }
};

struct TransportPark              // Структура для данных о транспортном парке.
{
    MyTree<Bus> buses;            // Бинарное дерево транспортных средств.
    QString parkName;             // Название парка.
    QStringList servedRoutes;     // Список обслуживаемых маршрутов.
};

struct TimeBus                    // Структура для хранения информации о рейсе.
{
    QString time;                 // Время прибытия.
    Bus* bus;                     // Автобус на рейсе.
    TimeBus(const QString& t)     // Конструкторы.
    {
        time = t;
        bus = nullptr;
    }
    TimeBus(const QString& t, Bus* b)
    {
        time = t;
        bus = b;
    }
    bool operator<(const TimeBus& a) const // Перегрузка операторов меньше и больше.
    {
        return this->time < a.time;
    }
    bool operator>(const TimeBus& a) const
    {
        return this->time > a.time;
    }
};

struct Node                         // Узел данных связывающий между собой маршруты и остановки.
{
    Node* stopNext;                 // Следующая остановка маршрута.
    Node* stopPrev;                 // Предыдущая остановка маршрута.
    Node* routeNext;                // Следующий маршрут остановки.
    Node* routePrev;                // Предыдущий маршрут остановки.
    QString stopName;               // Название остановки.
    QString routeNumber;            // Номер маршрута.
    list<TimeBus> timetableNextWork;// Расписание по будням в прямом направлении.
    list<TimeBus> timetablePrevWork;// Расписание по будням в обратном направлении.
    list<TimeBus> timetableNextWeek;// Расписание по выходным в прямом направлении.
    list<TimeBus> timetablePrevWeek;// Расписание по выходных в обратном направлении.
    Node()                          // Конструкторы.
    {
        stopNext = nullptr;
        stopPrev = nullptr;
        routeNext = nullptr;
        routePrev = nullptr;
        stopName = "";
        routeNumber = "";
    }
    Node(Node*& el)
    {
        stopNext = el->stopNext;
        stopPrev = el->stopPrev;
        routeNext = el->routeNext;
        routePrev = el->routePrev;
        stopName = el->stopName;
        routeNumber = el->routeNumber;
        timetableNextWork = el->timetableNextWork;
        timetablePrevWork = el->timetablePrevWork;
        timetableNextWeek = el->timetableNextWeek;
        timetablePrevWeek = el->timetablePrevWeek;
    }
};

struct Route                                    // Структура для информации о маршруте.
{
    Node* first;                                // Указатель на первую остановку.
    Node* last;                                 // Указатель на последнюю остановку.
    bool daysOfWork[7];                         // Дни работы.

    bool operator<(const Route& a) const        // Перегрузка оператора меньше.
    {
        int i = this->first->routeNumber.length()-1, n1, n2;
        while(this->first->routeNumber[i]!=' ')
            i--;                                // Получение номера первого маршрута.
        n1 = this->first->routeNumber.right(this->first->routeNumber.length()-1-i).toInt();
        i = a.first->routeNumber.length() - 1;
        while(a.first->routeNumber[i]!=' ')
            i--;                                // Получение номера второго маршрута.
        n2 = a.first->routeNumber.right(a.first->routeNumber.length()-1-i).toInt();
        return n1 < n2;
    }
    bool operator>(const Route& a) const        // Перегрузка оператор больше.
    {
        int i = this->first->routeNumber.length()-1, n1, n2;
        while(this->first->routeNumber[i]!=' ')
            i--;                                // Получение номера первого маршрута.
        n1 = this->first->routeNumber.right(this->first->routeNumber.length()-1-i).toInt();
        i = a.first->routeNumber.length() - 1;
        while(a.first->routeNumber[i]!=' ')
            i--;                                // Получение номера второго маршрута.
        n2 = a.first->routeNumber.right(a.first->routeNumber.length()-1-i).toInt();
        return n1 > n2;
    }
    Route(const Route& r)                       // Конструкторы.
    {
        for(int i = 0; i<7; i++)
            this->daysOfWork[i] = r.daysOfWork[i];
        this->first = r.first;
        this->last = r.last;
    }
    Route()
    {
        for(int i = 0; i<7;i++)
            this->daysOfWork[i] = false;
        this->first = nullptr;
        this->last = nullptr;
    }
};

struct Stop                                      // Структура с данными об остановке
{
    int stopNum;                                 // Номер остановки.
    Node* stop;                                  // Указатель на узел остановки.
    Stop()                                       // Конструкторы.
    {
        stop = nullptr;
    }
    Stop(const QString& str)
    {
        stop = new Node();
        stop->stopName = str;
        stop->routeNext = nullptr;
        stop->routePrev = nullptr;
        stop->stopNext = nullptr;
        stop->stopPrev = nullptr;
    }
    bool operator==(const Stop& stop)           // Перегрузка оператора сравнения.
    {
        if(this->stop!=nullptr && stop.stop!=nullptr)
            return this->stop->stopName == stop.stop->stopName;
        return false;
    }
    bool operator<(const Stop& stop)            // Перегрузка операторов больше и меньше.
    {
        if(this->stop!=nullptr && stop.stop!=nullptr)
            return this->stop->stopName < stop.stop->stopName;
        return false;
    }
    bool operator>(const Stop& stop)
    {
        if(this->stop!=nullptr && stop.stop!=nullptr)
            return this->stop->stopName > stop.stop->stopName;
        return false;
    }
    bool operator<=(const Stop& stop)           // Перегрузка операторов больше или равно
    {                                           // и меньше или равно.
        if(this->stop!=nullptr && stop.stop!=nullptr)
            return this->stop->stopName <= stop.stop->stopName;
        return false;
    }
    bool operator>=(const Stop& stop)
    {
        if(this->stop!=nullptr && stop.stop!=nullptr)
            return this->stop->stopName >= stop.stop->stopName;
        return false;
    }
};
