#pragma once
#include "Header.h"
#include "city.h"
#include <QKeyEvent>
#include <QStack>
#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include "routeswindow.h"
#include "dialogstopwindow.h"
#include "routestoptimetablewindow.h"
#include "mytree.h"
#include "mytree.cpp"
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include <functional>
#include "Header2.h"

namespace Ui { class routeStopsWindow; }

class routeStopsWindow : public QWidget
{
public:
    routeStopsWindow(const QString&, City*&, bool);     // Конструктор.
    ~routeStopsWindow();                                // Деструктор.
    void deleteButton(const QString&);                  // Удаление кнопки.
    QStandardItem* makeItem(const QString&);            // Создание элемента списка.
    void remakeComboBox(const QString&, const QString&);// Обновление данных в комбо боксе.

protected:
    void keyPressEvent(QKeyEvent *event) override;      // Обработчик нажатия клавиш.
    void closeEvent(QCloseEvent *event) override;       // Обработчик закрытия окна.

private:
    Ui::routeStopsWindow *ui;                           // Внешний вид окна.
    City* city;                                         // Город.
    bool editMode;                                      // Режим редактирования.
    QStack<City*> stack;                                // Стек для отмены действий.
    void moveItemUp(const QModelIndex& index);          // Перемещение элемента вверх
    void moveItemDown(const QModelIndex& index);        // или вниз.

private slots:
    void onItemDoubleClicked(const QModelIndex &index); // Обработчик двойного нажатия.
    void onListViewClicked(const QModelIndex &index);   // Обработчик нажатия на элемент списка.
    void onAddButtonClicked();                          // Обработчики нажатия кнопок.
    void onDeleteButtonClicked();
    void handleComboBoxChange(const QString&);          // Обработчик изменения комбо бокса.
};
