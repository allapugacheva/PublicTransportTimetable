#include "transportstops.h"

TransportStops::TransportStops() {}                // Конструкторы.
TransportStops::TransportStops(TransportStops& ts)
{
    MyTree<Stop>::MyIterator s = ts.stops.begin(); // Глубокое копирование.
    while(!s.isnull())
    {
        Stop* temp = new Stop();
        temp->stopNum = (*s).stopNum;
        temp->stop = new Node();
        temp->stop->stopName = (*s).stop->stopName;
        temp->stop->routeNext = nullptr;
        temp->stop->routePrev = nullptr;
        temp->stop->stopNext = nullptr;
        temp->stop->stopPrev = nullptr;
        this->stops.Add(*temp);
        s++;
    }
}
TransportStops::TransportStops(const QString& str)
{
    this->readFromFileTS(str);
}
TransportStops::~TransportStops() // Деструктор.
{
    this->stops.~MyTree();
}
void TransportStops::readFromFileTS(const QString& cityName) // Чтение из файла остановок.
{
    try
    {
        this->getStops().~MyTree();
        QFile file(cityName + "Остановки.txt");              // Открытие файла.
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw FileException("Ошибка при открытии файла", cityName + "Остановки.txt");
        QTextStream in(&file);                               // Поток вывода.
        QString temp;
        int n, i;
        QString stopn;
        while(!in.atEnd())                                   // Чтение до конца файла.
        {
            Stop stop;
            stop.stop = new Node();
            temp = in.readLine();
            if(temp == "")
                break;
            i = 0;
            n = 0;
            while(temp[i]!=',')
            {
                n*=10;
                n+= temp[i++].unicode() - 48;
            }
            i+=2;
            stop.stopNum = n;                                // Номер остановки.
            stopn = temp.mid(i, temp.length() - i);          // Название остановки.
            while(!in.atEnd() && ((temp = in.readLine())!=""))
            {
                if(temp == "not work")
                {
                    stop.stop->stopName = stopn;
                    stop.stop->routeNumber = "";
                    temp = in.readLine();
                    break;
                }
                Node* tnode = new Node();
                tnode->routeNumber = temp;                    // Маршрут на остновке.
                tnode->stopName = stopn;
                readTime(1, in, tnode);                       // Чтение рейсов во все дни.
                readTime(2, in, tnode);
                readTime(3, in, tnode);
                readTime(4, in, tnode);
                if(stop.stop->routeNumber == "")              // Добавление остановки в маршрут.
                    stop.stop = new Node(tnode);
                else
                {
                    Node* tn = stop.stop;
                    while(tn->routeNext!=nullptr)
                        tn = tn->routeNext;
                    tnode->routePrev = tn;
                    tn->routeNext = new Node(tnode);
                }
            }
            this->stops.Add(stop);                            // Добавление остановки в дерево.
        }
    }
    catch(FileException& ex)
    {
        ex.mesg();
    }
}                                                             // Чтение рейсов.
void TransportStops::readTime(int day, QTextStream& in, Node*& tnode)
{
    QString temp = in.readLine();
    if(temp != "not work")                                    // Проверка, что работае в этот день.
    {
        int j = 0;
        while(j<temp.length())
        {
            int m = 0;
            while(j<temp.length() && temp[j]!=',')
            {
                j++;
                m++;
            }
            if(day == 1)                                     // Запись времени в нужный день.
                tnode->timetableNextWork.push_back(temp.mid(j-m, m));
            else if(day == 2)
                tnode->timetablePrevWork.push_back(temp.mid(j-m, m));
            else if(day == 3)
                tnode->timetableNextWeek.push_back(temp.mid(j-m, m));
            else
                tnode->timetablePrevWeek.push_back(temp.mid(j-m, m));
            j+=2;
        }
    }
}
void TransportStops::putInFileTS(const QString& cityName)   // Запись данных в файл.
{
    try
    {
        QFile file(cityName + "Остановки.txt");             // Файл.
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);                      // Поток ввода.
            MyTree<Stop>::MyIterator stop = this->getStops().begin();
            while(!stop.isnull())                           // Цикл по остановкам.
            {
                stream << (*stop).stopNum << ", " << (*stop).stop->stopName << "\n";
                Node* n = (*stop).stop;                     // Запись названий и номера.
                if(n->routeNumber=="")
                {
                    stream<< "not work\n";                  // Проверка, что работает.
                    stream << "\n";
                    stop++;
                    continue;
                }
                while(n->routePrev != nullptr)
                    n = n->routePrev;
                while(n != nullptr)
                {
                    stream << n->routeNumber << "\n";       // Запись маршрута.
                    putTime(1, stream, n);                  // Запись его расписания на ней.
                    putTime(2, stream, n);
                    putTime(3, stream, n);
                    putTime(4, stream, n);
                    n = n->routeNext;
                }
                stream<<"\n";
                stop++;
            }
            file.close();
        }
        else                                                 // Обработка ошибок в файле.
            throw FileException("Ошибка при открытии файла", cityName + "Остановки.txt");
    }
    catch(FileException& ex)
    {
        ex.mesg();
    }
}                                                            // Запись времени в файл.
void TransportStops::putTime(int day, QTextStream& stream, Node*& n)
{
    if((day == 1 ? n->timetableNextWork.empty() : (day == 2 ? n->timetablePrevWork.empty() : (day == 3 ? n->timetableNextWeek.empty() : n->timetablePrevWeek.empty()))))
        stream << "not work\n";                              // Проверка, что в этот день работает.
    else
    {
        foreach(TimeBus tb, (day == 1 ? n->timetableNextWork : (day == 2 ? n->timetablePrevWork : (day == 3 ? n->timetableNextWeek : n->timetablePrevWeek))))
        {
            stream << tb.time;                               // Запись времени.
            if(tb.time != (day == 1 ? n->timetableNextWork.back().time : (day == 2 ? n->timetablePrevWork.back().time : (day == 3 ? n->timetableNextWeek.back().time : n->timetablePrevWeek.back().time))))
                stream << ", ";
            else
                stream << "\n";
        }
    }
}
MyTree<Stop>& TransportStops::getStops()             // Геттер.
{
    return this->stops;
}
void TransportStops::setStops(MyTree<Stop>& stops)   // Сеттер.
{
    this->stops.~MyTree();
    MyTree<Stop>::MyIterator it = stops.begin();
    while(!it.isnull())
    {
        this->stops.Add(*it);
        it++;
    }
}
