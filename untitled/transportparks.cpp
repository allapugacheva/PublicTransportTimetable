#include "transportparks.h"

TransportParks::TransportParks() {}                       // Конструкторы.

TransportParks::TransportParks(const QString& str)
{
    this->readFromFileTP(str);
}
TransportParks::TransportParks(TransportParks& tp)
{
    foreach(TransportPark p, tp.parks)                // Цикл по всем паркам.
    {
        TransportPark* temp = new TransportPark();    // Глубокое копирование.
        temp->parkName = p.parkName;
        foreach(QString sr, p.servedRoutes)
            temp->servedRoutes.push_back(sr);
        MyTree<Bus>::MyIterator b = p.buses.begin();
        while(!b.isnull())                             // Цикл по всем автобусам.
        {
            Bus* bu = new Bus();
            bu->hasApparel = (*b).hasApparel;
            bu->hasConditioner = (*b).hasConditioner;
            bu->isLarge = (*b).isLarge;
            bu->model = (*b).model;
            bu->number = (*b).number;
            temp->buses.Add(*bu);
            b++;
        }
        this->parks.push_back(*temp);
    }
}
TransportParks::~TransportParks()                      // Деструктор.
{
    this->parks.clear();
}
void TransportParks::readFromFileTP(const QString& cityName) // Чтение из файла.
{
    try
    {
        this->getParks().clear();
        QFile file(cityName + "Парки.txt");                  // Открыть файл.
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw FileException("Ошибка открытия файла", cityName + "Парки.txt");
        }
        QTextStream in(&file);                               // Файловый поток.
        QString temp;
        int i, n;
        while(!in.atEnd())                                   // Чтение до конца файла.
        {
            TransportPark park;
            Bus* bus;                                        // Новые объекты.
            i = 0;
            park.parkName = in.readLine();                   // Чтение названия парка.
            temp = in.readLine();
            if(temp != "no")
            {
                while(i<temp.length())
                {
                    n = 0;
                    while(i<temp.length() && temp[i]!=',')
                    {
                        i++;
                        n++;
                    }
                    park.servedRoutes.push_back(temp.mid(i - n, n)); // Считывание обслуживаемых маршрутов.
                    i+=2;
                }
            }
            while(!in.atEnd() && (temp = in.readLine())!= "") // Считывание данных об автобусах.
            {
                bus = new Bus();
                i = 0;
                n = 0;
                while(temp[i]!='|')
                {
                    i++;
                    n++;
                }
                bus->model = temp.mid(i - n, n);              // Модель автобуса.
                n = 0;
                i++;
                while(temp[i]!='|')
                {
                    i++;
                    n++;
                }
                bus->number = temp.mid(i - n, n);             // Номер автобуса.
                i++;
                if(temp[i] == '1')                            // Данные об опциях.
                    bus->hasApparel = true;
                else
                    bus->hasApparel = false;
                i++;
                if(temp[i] == '1')
                    bus->hasConditioner = true;
                else
                bus->hasConditioner = false;
                i++;
                if(temp[i] == '1')
                    bus->isLarge = true;
                else
                    bus->isLarge = false;
                park.buses.Add(*bus);                         // Добавление в парк.
            }
            this->parks.push_back(park);                      // Добавление парка в город.
        }
    }
    catch(FileException& ex)
    {
        ex.mesg();
    }
}
void TransportParks::putInFileTP(const QString& cityName)     // Запись в файл.
{
    try
    {
        QFile file(cityName + "Парки.txt");                   // Открытие файла.
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);                        // Файловый поток.
            foreach(TransportPark park, this->getParks())
            {
                stream << park.parkName << "\n";              // Название парка.
                if(park.servedRoutes.empty())
                    stream << "no\n";
                else
                {
                    foreach(QString route, park.servedRoutes)
                    {
                        stream << route;                          // Запись обслуживаемых маршрутов.
                        if(route != park.servedRoutes.last())
                            stream << ", ";
                        else
                            stream << "\n";
                    }
                }
                MyTree<Bus>::MyIterator bus = park.buses.begin();
                while(!bus.isnull())                          // Запись автобусов.
                {
                    stream << (*bus).model << "|" << (*bus).number << "|";
                    stream << (*bus).hasApparel << (*bus).hasConditioner << (*bus).isLarge;
                    if(park.parkName != this->getParks().back().parkName ||
                        (*(park.buses.back())).number != (*bus).number)
                        stream << "\n";
                    bus++;
                }
                if(park.parkName != this->getParks().back().parkName)
                    stream << "\n";
            }
            file.close();
        }
        else                          // Обработка ошибок.
            throw FileException("Ошибка открытия файла", cityName + "Парки.txt");
    }
    catch(FileException& ex)
    {
        ex.mesg();
    }
}

list<TransportPark>& TransportParks::getParks() // Геттер.
{
    return this->parks;
}

void TransportParks::setParks(list<TransportPark>& parks) // Сеттер.
{
    this->parks = parks;
}
