#pragma once
#include "Header.h"
#include <QDialog>
#include "city.h"
#include "mytree.h"
#include "Header2.h"
#include "mytree.cpp"

namespace Ui { class dialogChangeBusWindow; }

class dialogChangeBusWindow : public QDialog
{
public:                           // Конструктор.
    dialogChangeBusWindow(City*, TimeBus*, QDialog *parent = nullptr);
    ~dialogChangeBusWindow();     // Деструктор.

private:
    Ui::dialogChangeBusWindow *ui;// Внешний вид окна.
    City* city;                   // Город.
    TimeBus* tb;                  // Время рейса.

private slots:
    void acceptClicked();         // Обработчик нажатия Ок.
};
