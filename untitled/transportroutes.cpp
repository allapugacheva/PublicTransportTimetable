#include "transportroutes.h"

TransportRoutes::TransportRoutes() : TransportStops(), TransportParks() {}    // Конструкторы.
TransportRoutes::TransportRoutes(TransportRoutes& tr) : TransportStops(tr), TransportParks(tr)
{
    foreach(Route r, tr.routes)                                     // Глубокое копирование.
    {
        Route* temp = new Route();                                  // Новый маршрут.
        for(int i = 0; i<7; i++)
            temp->daysOfWork[i] = r.daysOfWork[i];                  // Дни работы.
        Node* n1 = r.first;
        Node* n2;
        while(n1!=nullptr)
        {
            n2 = new Node();
            n2->routeNumber = n1->routeNumber;                      // Копирование узлов маршрута.
            n2->stopName = n1->stopName;
            foreach(TimeBus tb, n1->timetableNextWeek)
            {
                foreach(TransportPark p, this->parks)
                {
                    Bus* b = TreeAlgorithms<Bus>::findOne(Bus(tb.bus->number), p.buses);
                    if(b!=nullptr)
                    {                                               // Копирование рейсов в выходные.
                        n2->timetableNextWeek.push_back(TimeBus(tb.time, b));
                        break;
                    }
                }
            }
            foreach(TimeBus tb, n1->timetableNextWork)
            {
                foreach(TransportPark p, this->parks)
                {
                    Bus* b = TreeAlgorithms<Bus>::findOne(Bus(tb.bus->number), p.buses);
                    if(b!=nullptr)
                    {                                              // Копирование рейсов в будние.
                        n2->timetableNextWork.push_back(TimeBus(tb.time, b));
                        break;
                    }
                }
            }
            foreach(TimeBus tb, n1->timetablePrevWeek)
            {
                foreach(TransportPark p, this->parks)
                {
                    Bus* b = TreeAlgorithms<Bus>::findOne(Bus(tb.bus->number), p.buses);
                    if(b!=nullptr)
                    {
                        n2->timetablePrevWeek.push_back(TimeBus(tb.time, b));
                        break;
                    }
                }
            }
            foreach(TimeBus tb, n1->timetablePrevWork)
            {
                foreach(TransportPark p, this->parks)
                {
                    Bus* b = TreeAlgorithms<Bus>::findOne(Bus(tb.bus->number), p.buses);
                    if(b!=nullptr)
                    {
                        n2->timetablePrevWork.push_back(TimeBus(tb.time, b));
                        break;
                    }
                }
            }
            Stop* it = TreeAlgorithms<Stop>::findOne(Stop(n2->stopName), this->stops);
            if(it!=nullptr)                           // Поиск маршрута.
            {
                if(it->stop->routeNumber == "")
                    it->stop = n2;
                else
                {
                    Node*n3 = it->stop;               // Добавление остановки.
                    while(n3->routeNext!=nullptr)
                        n3=n3->routeNext;
                    n3->routeNext = n2;
                }
            }
            if(temp->first == nullptr)
                temp->first = n2;
            else
            {
                temp->first->stopNext = n2;
                n2->stopPrev = temp->first;
                temp->first = temp->first->stopNext;
            }
            n1 = n1->stopNext;                        // Переход к следующей остновке.
        }
        temp->last = temp->first;
        while(temp->first->stopPrev!=nullptr)
            temp->first = temp->first->stopPrev;      // Установка указателей на первую и последнюю остановки.
        this->routes.push_back(*temp);                // Добавление маршрута.
    }
}
TransportRoutes::TransportRoutes(const QString& str) : TransportStops(str), TransportParks(str)
{
    this->readFromFileTR(str);
}
TransportRoutes::~TransportRoutes()   // Деструктор.
{
    this->routes.clear();
}
void TransportRoutes::readFromFileTR(const QString & cityName)  // Чтение из файла.
{
    try
    {
        this->getRoutes().clear();
        QFile file(cityName + "Маршруты.txt");                  // Открытие файла.
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw FileException("Ошибка открытия файла", cityName + "Маршруты.txt");
        QTextStream in(&file);                                  // Поток файла.
        QString temp, routen;
        while(!in.atEnd())                                      // Чтение до конца.
        {
            Route route;
            route.first = new Node();
            route.last = new Node();
            routen = in.readLine();                             // Номер маршрута.
            temp = in.readLine();
            for(int i = 0; i<7; i++)
            {
                if(temp[i] == '1')
                  route.daysOfWork[i] = true;                   // Дни работы.
                else
                    route.daysOfWork[i] = false;
            }
            readStops(true, in, route, routen);                 // Чтение остановок в обоих направлениях.
            readStops(false, in, route, routen);
            readTime(1, in, route, routen, true);               // Чтение рейсов во всех напарвлениях.
            readTime(2, in, route, routen, false);
            readTime(3, in, route, routen, true);
            readTime(4, in, route, routen, false);
            this->routes.push_back(route);                      // Добавление маршрута.
        }
    }
    catch(FileException& ex)
    {
        ex.mesg();
    }
}                                                               // Чтение остановок.
void TransportRoutes::readStops(bool direct, QTextStream& in, Route& route, const QString& routen)
{
    QString temp;
    while((temp = in.readLine())!="")                           // Чтение до первой пустой строки.
    {
        Stop* it = TreeAlgorithms<Stop>::findOne(Stop(temp), this->getStops());
        if(it == nullptr)                       // Поиск маршрута.
            break;
        Node* ttn = it->stop;
        while(ttn->routePrev!=nullptr)          // Установка связей.
            ttn = ttn->routePrev;
        while(ttn->routeNumber != routen)
            ttn = ttn->routeNext;
        if(direct)
        {
            if(route.first->stopName == "")      // Установка указателей на начальный элемент.
                route.first = ttn;
            else
            {
                Node* tn = route.first;
                while(tn->stopNext!=nullptr)
                    tn = tn->stopNext;
                tn->stopNext = ttn;
            }
        }
        else
        {
            if(route.last->stopName == "")
                route.last = ttn;                // Установка указателей на конечный элемент.
            else
            {
                Node*tn = route.last;
                while(tn->stopPrev!=nullptr)
                    tn = tn->stopPrev;
                tn->stopPrev = ttn;
            }
        }
    }
}                                                // Чтение рейсов.
void TransportRoutes::readTime(int day, QTextStream& in, Route& route, const QString& routen, bool direct)
{
    QString temp, temp1;
    int i, n;
    while((temp = in.readLine())!="")            // Чтение до первой пустой строки.
    {
        if(temp == "not work")                   // Проверка, что рейс в указанный день есть.
        {
            temp = in.readLine();
            break;
        }
        n = 0;
        i = 0;
        while(temp[i]!='|')
        {
            i++;
            n++;
        }
        temp1 = temp.mid(i - n, n);
        i++;
        auto jt = this->parks.begin();          // Поиск парка.
        while(jt != this->parks.end() && jt->parkName!=temp1)
            jt++;
        if(jt == this->parks.end())
        {
            QMessageBox::warning(nullptr, "Чтение данных", "Парк не найден");
            return;
        }
        n=0;
        while(temp[i]!='|')
        {
            i++;
            n++;
        }
        temp1 = temp.mid(i - n, n);
        i++;                                // Поиск автобуса.
        Bus* kt = TreeAlgorithms<Bus>::findOne(Bus(temp1), jt->buses);
        if(kt==nullptr)
        {
            QMessageBox::warning(nullptr, "Чтение данных", "Автобус не найден");
            return;
        }
        while(i<temp.length())              // Цикл по рейсам.
        {
            n = 0;
            while(i<temp.length() && temp[i]!=',')
            {
                n*=10;
                n+=temp[i].unicode() - 48;
                i++;
            }
            i+=2;
            if(direct)
            {
                Node* tn = route.first;
                if(route.first->stopName!="")
                {
                    while(tn->routePrev!=nullptr)
                        tn = tn->routePrev;
                    while(tn->routeNumber != routen)
                        tn = tn->routeNext;
                }                           // Поиск маршрута.
                while(tn!=nullptr)
                {                           // Переход к нужному рейсу.
                    auto at = (day == 1 ? tn->timetableNextWork.begin() : tn->timetableNextWeek.begin());
                    for(int i = 0; i<n; i++, at++);
                    at->bus = kt;           // Добавление автобуса на рейс.
                    tn = tn->stopNext;
                }
            }
            else
            {
                Node* tn = route.last;
                if(route.last->stopName!="")
                {
                    while(tn->routePrev!=nullptr)
                        tn = tn->routePrev;
                    while(tn->routeNumber != routen)
                        tn = tn->routeNext;
                }
                while(tn!=nullptr)
                {
                    auto at = (day == 2 ? tn->timetablePrevWork.begin() : tn->timetablePrevWeek.begin());
                    for(int i = 0; i<n; i++, at++);
                    at->bus = kt;
                    tn = tn->stopPrev;
                }
            }
        }
    }
}
void TransportRoutes::putInFileTR(const QString& cityName) // Запись в файл.
{
    try
    {
        QFile file(cityName + "Маршруты.txt");      // Файл.
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);              // Поток ввода.
            foreach(Route route, this->getRoutes())
            {
                stream << route.first->routeNumber << "\n"; // Номер маршрута.
                stream << route.daysOfWork[0] << route.daysOfWork[1] << route.daysOfWork[2] << route.daysOfWork[3] << route.daysOfWork[4]
                       << route.daysOfWork[5] << route.daysOfWork[6];
                stream << "\n";                             // Дни работы.
                Node* n = route.first;
                while(n!=nullptr)
                {
                    stream << n->stopName << "\n";          // Остановки в прямом направлении.
                    n = n->stopNext;
                }
                stream << "\n";
                n = route.last;
                while(n!=nullptr)
                {
                    stream << n->stopName << "\n";          // Остановка в обратном направлении.
                    n = n->stopPrev;
                }
                stream << "\n";
                putTime(1, stream, route);                  // Запись рейсов по всем случаям.
                stream <<"\n";
                putTime(2, stream, route);
                stream <<"\n";
                putTime(3, stream, route);
                stream<< "\n";
                putTime(4, stream, route);
                if(route.first->routeNumber!=this->getRoutes().back().first->routeNumber)
                stream << "\n";
            }
            file.close();                                   // Закрыть файл.
        }
        else                                                // Обработка ошибок.
            throw FileException("Ошибка открытия файла", cityName + "Маршруты.txt");
    }
    catch(FileException& ex)
    {
        ex.mesg();
    }
}                                                           // Запись рейса.
void TransportRoutes::putTime(int day, QTextStream& stream, Route& route)
{
    if((day == 1 ? route.first->timetableNextWork.empty() : (day == 2 ? route.first->timetablePrevWork.empty() : (day == 3 ? route.first->timetableNextWeek.empty() : route.first->timetablePrevWeek.empty()))))
        stream << "not work\n";                             // Проверка, что в выбранный день есть рейсы.
    else
    {
        QStringList wrotenBus;
        foreach(TimeBus a, (day == 1 ? route.first->timetableNextWork : (day == 2 ? route.first->timetablePrevWork : (day == 3 ? route.first->timetableNextWeek : route.first->timetablePrevWeek))))
        {                                                    // Цикл по автобусам на рейсах.
            if(wrotenBus.contains(a.bus->number))
                continue;
            else
            {
                wrotenBus.push_back(a.bus->number);
                QString temps = "";
                int c = 0;
                foreach(TimeBus b, (day == 1 ? route.first->timetableNextWork : (day == 2 ? route.first->timetablePrevWork : (day == 3 ? route.first->timetableNextWeek : route.first->timetablePrevWeek))))
                {                                            // Запись данных для автобуса.
                    if(a.bus->number == b.bus->number)
                        temps += QString::number(c) + ", ";
                    c++;
                }
                temps = temps.left(temps.length() - 2);
                temps += "\n";
                foreach(TransportPark park, this->getParks())
                {
                    Bus* b = TreeAlgorithms<Bus>::findOne(Bus(a.bus->number), park.buses);
                    if(b!=nullptr)
                    {                                         // Запись рейсов автобуса в файл.
                        stream << park.parkName << "|" << b->number << "|" << temps;
                        break;
                    }
                }
            }
        }
    }
}
list<Route>& TransportRoutes::getRoutes()  // Геттер.
{
    return this->routes;
}
void TransportRoutes::setRoutes(list <Route>& routes) // Сеттер.
{
    this->routes = routes;
}                                          // Поиск маршрута между остановками.
void TransportRoutes::findRoute(Node* curr, Node* prev, Node* dest, QString str, int nc, int nt, int& min, QStringList& l)
{
    if(min!=0 && nt > min)                 // Если превысил минимальные параметры - выход.
        return;
    if(curr->stopName == dest->stopName)   // Если найден результат.
    {
        if(min == 0 || (nt <= min && (str.count('\n') <= l.back().count('\n'))))
        {
            if(nt != min || str.count('\n') != l.back().count('\n'))
                l.clear();
            min = nt;
            l.push_back(str + curr->stopName); // Запись данных.
            return;
        }
    }
    if(str == "" || str.right(1) == "\n")      // Переход во все стороны узла.
        str += curr->routeNumber + ": " + curr->stopName + " - ";
    if(curr->stopNext!=nullptr && curr->stopNext!=prev/* && curr->routeNumber!=dest->routeNumber*/)
        this->findRoute(curr->stopNext, curr, dest, str, nc+1, nt+1, min, l);
    if(curr->stopPrev!=nullptr && curr->stopPrev!=prev/* && curr->routeNumber!=dest->routeNumber*/)
        this->findRoute(curr->stopPrev, curr, dest, str, nc+1, nt+ 1, min, l);
    if(nc == 0)                                // Удаление дублируемых данных.
    {
        int i = str.length() - 1;
        while(i>0 && str[i]!='\n')
            i--;
        if(i!=0)
            i++;
        str.remove(i, str.length() - i);
    }
    else
        str += curr->stopName + "\n";
    if(curr->routeNext!=nullptr && curr->routeNext!=prev/* && curr->routeNext->routeNumber!=dest->routeNumber*/)
        this->findRoute(curr->routeNext, curr, dest, str, 0, nt, min, l);
    if(curr->routePrev!=nullptr && curr->routePrev!=prev/* && curr->routePrev->routeNumber!=dest->routeNumber*/)
        this->findRoute(curr->routePrev, curr, dest, str, 0, nt, min, l);
}
