#pragma once
#include "Header.h"
#include "city.h"
#include <QDialog>
#include <QKeyEvent>
#include <QStack>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include "dialogstopwindow.h"
#include "mytree.h"
#include "mytree.cpp"
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include "Header2.h"
#include <QRegularExpressionValidator>

namespace Ui { class dialogRouteWindow; }

class dialogRouteWindow : public QDialog
{
public:
    dialogRouteWindow(City* city, QDialog *parent = nullptr); // Конструктор.
    ~dialogRouteWindow();                                     // Деструктор.
    void addBusContain(const QString&);                       // Добавление автобуса.
    void addStop(const QString&);                             // Добавление остановки.

protected:
    void keyPressEvent(QKeyEvent *event) override;            // Обработчик нажатия клавиш.

private:
    Ui::dialogRouteWindow *ui;                                // Внешний вид окна.
    City* city;                                               // Город.
    Route* route;                                             // Маршрут.
    QStack<Route*> stack1;                                    // Стек для маршрута.
    QStack<City*> stack2;                                     // Стек для города.
    void moveItemUp(const QModelIndex& index);                // Перемещение остановки вверх
    void moveItemDown(const QModelIndex& index);              // или вниз списка.

private slots:
    void acceptClicked();                                     // Обработчики нажатия кнопок.
    void addStopClicked();
    void deleteStopClicked();
    void addTimeClicked();
    void deleteTimeClicked();
    void addBusClicked();
    void deleteBusClicked();
    void onCheckBoxClicked();
};
