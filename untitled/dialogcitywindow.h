#pragma once
#include "Header.h"
#include <QDialog>
#include "city.h"
#include <QStack>
#include <QKeyEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFileDialog>
#include "mytree.h"
#include "mytree.cpp"
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include "dialogparkwindow.h"
#include "dialogstopwindow.h"
#include <QMessageBox>
#include "dialogroutewindow.h"
#include <list>
#include "Header2.h"
#include <QRegularExpressionValidator>

namespace Ui { class dialogCityWindow; }

class dialogCityWindow : public QDialog
{
public:                                                   // Конструктор.
    dialogCityWindow(QStringList*, City* city = nullptr, QDialog *parent = nullptr);
    ~dialogCityWindow();                                  // Деструктор.
    void addIn(int, const QString&);                      // Добавление элемента в список.

protected:
    void keyPressEvent(QKeyEvent *event) override;        // Обработчик нажатия клавиш.

private:
    Ui::dialogCityWindow *ui;                             // Внешний вид окна.
    QStringList* cities;                                  // Список городов.
    City* city;                                           // Город.
    bool editMode;                                        // Режим изменения.
    QStack<City*> stack;                                  // Стек для отмены действий.

private slots:
    void acceptClicked();                                 // Обработчики нажатия кнопок.
    void addStopClicked();
    void deleteStopClicked();
    void addParkClicked();
    void deleteParkClicked();
    void addRouteClicked();
    void deleteRouteClicked();
    void onItem1DoubleClicked(const QModelIndex &index);  // Обработчики двойных нажатий на
    void onItem2DoubleClicked(const QModelIndex &index);  // элементы списков.
    void addMapClicked();
};
