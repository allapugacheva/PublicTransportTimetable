#pragma once
#include "Header.h"
#include "city.h"
#include <QTimer>
#include <QStack>
#include <QKeyEvent>
#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QToolTip>
#include "dialogchangebuswindow.h"
#include "routestopswindow.h"
#include "dialogtimewindow.h"
#include "routeswindow.h"
#include "mytree.h"
#include "mytree.cpp"
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include "Header2.h"

namespace Ui { class routeStopTimetableWindow; }

class routeStopTimetableWindow : public QWidget
{
public:                                            // Конструктор.
    routeStopTimetableWindow(const QString&, const QString&, City*&, bool, bool);
    ~routeStopTimetableWindow();                   // Деструктор.
    void deleteButton(const QString&);             // Удаление кнопки.
    QStandardItem* makeItem(const QString&);       // Создание объекта списка.

protected:
    void keyPressEvent(QKeyEvent *event) override; // Обработчик нажатия клавиш.
    void closeEvent(QCloseEvent *event) override;  // Обработчик закрытия окна.

private:
    Ui::routeStopTimetableWindow *ui;              // Внешний вид окна.
    City* city;                                    // Город.
    bool editMode;                                 // Режим редактирования.
    bool direct;                                   // Направление движения.
    QTimer timer;                                  // Таймер для часов.
    QTimer timer2;                                 // Таймер для расписания.
    QStack<City*> stack;                           // Стек для отмены действий.

private slots:
    void handleComboBoxChange(const QString&);     // Обработчик изменения комбо бокса.
    void onTableViewClicked(const QModelIndex &index); // Обработчик нажатия на список.
    void onAddButtonClicked();                     // Обработчики нажатия кнопок.
    void onDeleteButtonClicked();
    void handleTimerEvent();                       // Обработчик срабатывания таймера.
    void onItemDoubleClicked(const QModelIndex &index);// Обработчик двойного нажатия на список.
};
