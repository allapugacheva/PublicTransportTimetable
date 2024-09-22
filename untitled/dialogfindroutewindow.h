#pragma once
#include "Header.h"
#include "city.h"
#include <QDialog>
#include "mytree.h"
#include "mytree.cpp"
#include <QMessageBox>
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include "Header2.h"

namespace Ui { class dialogFindRouteWindow; }

class dialogFindRouteWindow : public QDialog
{
public:                                // Конструктор.
    dialogFindRouteWindow(City*, QDialog *parent = nullptr);
    ~dialogFindRouteWindow();          // Деструктор.

private:
    Ui::dialogFindRouteWindow *ui;     // Внешний вид окна.
    City* city;                        // Город.

private slots:
    void acceptClicked();              // Обработчик нажатия Ок.
};
