#include "Header2.h"

bool checkRouteBuses(City* city, const QString& busnum)        // Проверка, что автобус находится в рейсе.
{
    foreach(Route r, city->getRoutes())                        // Цикл по всем маршрутам.
    {
        foreach(TimeBus tb, r.first->timetableNextWeek)        // Цикл по всем рейсам.
            if(tb.bus->number == busnum)                       // Проверка автобуса.
            {
                QString str = "Автобус задействован на маршруте ";
                str += r.first->routeNumber + "\nСнимите его с рейса перед удалением";
                QMessageBox::warning(nullptr, "Добавление парка", str); // Вывод сообщения.
                return true;
            }
        foreach(TimeBus tb, r.first->timetableNextWork)
            if(tb.bus->number == busnum)
            {
                QString str = "Автобус задействован на маршруте ";
                str += r.first->routeNumber + "\nСнимите его с рейса перед удалением";
                QMessageBox::warning(nullptr, "Добавление парка", str);
                return true;
            }
        foreach(TimeBus tb, r.first->timetablePrevWeek)
            if(tb.bus->number == busnum)
            {
                QString str = "Автобус задействован на маршруте ";
                str += r.first->routeNumber + "\nСнимите его с рейса перед удалением";
                QMessageBox::warning(nullptr, "Добавление парка", str);
                return true;
            }
        foreach(TimeBus tb, r.first->timetablePrevWork)
            if(tb.bus->number == busnum)
            {
                QString str = "Автобус задействован на маршруте ";
                str += r.first->routeNumber + "\nСнимите его с рейса перед удалением";
                QMessageBox::warning(nullptr, "Добавление парка", str);
                return true;
            }
    }
    return false;
}
Route* findRoute2(City* city, const QString& route)         // Поиск маршрута.
{
    auto it = city->getRoutes().begin();                    // Цикл по всем маршрутам.
    while(it!=city->getRoutes().end() && it->first->routeNumber!=route)
        it++;
    if(it==city->getRoutes().end())                         // Проверка, что маршрут найден.
    {
        QMessageBox::warning(nullptr, "Редактирование", "Маршрут не найден");
        return nullptr;
    }
    return &(*it);                                          // Возврат маршрута.
}
void moveDown(QStandardItemModel* qm, int row, Route* route)// Элемент списка вниз.
{
    QStandardItem* item1 = qm->item(row),
        *item2 = qm->item(row+1);
    QString temp = item1->text();                           // Получение соседних элементов.
    item1->setText(item2->text());                          // Замена текста в списке.
    item2->setText(temp);
    Node* jt = route->first;
    while(jt->stopPrev!=nullptr)
        jt = jt->stopPrev;
    while(jt->stopNext!=nullptr && jt->stopName!=temp)
        jt = jt->stopNext;
    Node* temp1 = jt->stopNext;                             // Замена указателей местами.
    QString ts = jt->stopName;
    jt->stopName = temp1->stopName;
    temp1->stopName = ts;
    ts = jt->routeNumber;
    jt->routeNumber = temp1->routeNumber;
    temp1->routeNumber = ts;
}
void moveUp(QStandardItemModel* qm, int row, Route* route)  // Элемент списка вверх.
{
    QStandardItem* item1 = qm->item(row),
        *item2 = qm->item(row-1);                           // Получение данных.
    QString temp = item1->text();
    item1->setText(item2->text());                          // Замена текста в списках местами.
    item2->setText(temp);
    temp = item1->text();
    Node* jt = route->first;
    while(jt->stopPrev!=nullptr)                            // Замена связей местами.
        jt = jt->stopPrev;
    while(jt->stopNext!=nullptr && jt->stopName!=temp)
        jt = jt->stopNext;
    Node* temp1 = jt->stopNext;
    QString ts = jt->stopName;
    jt->stopName = temp1->stopName;
    temp1->stopName = ts;
    ts = jt->routeNumber;
    jt->routeNumber = temp1->routeNumber;
    temp1->routeNumber = ts;
}
