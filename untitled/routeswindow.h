#pragma once
#include "Header.h"
#include <QWidget>
#include "city.h"
#include <QStack>
#include <QKeyEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include "dialogfindroutewindow.h"
#include "citywindow.h"
#include "dialogroutewindow.h"
#include "routestopswindow.h"
#include "mytree.h"
#include "mytree.cpp"
#include "Header2.h"

namespace Ui { class routesWindow; }

class routesWindow : public QWidget
{
public:                                                      // Конструктор.
    routesWindow(bool, const QString& cityName = "", City* city = nullptr);
    ~routesWindow();                                         // Деструктор.
    void deleteButton(const QString&);                       // Удаление кнопки.
    QStandardItem* makeItem(const QString&);                 // Создание элемента списка.

protected:
    void keyPressEvent(QKeyEvent *event) override;           // Обработчик нажатия клавиш.
    void closeEvent(QCloseEvent *event) override;            // Обработчик закрытия окна.
    bool eventFilter(QObject *obj, QEvent *event) override;  // Фильт событий.

private:
    City* city;                                              // Город.
    bool editMode;                                           // Режим редактирования.
    Ui::routesWindow *ui;                                    // Внешний вид окна.
    QStack<City*> stack;                                     // Стек для отмены действий.

private slots:
    void handleComboBoxChange(const QString&);               // Обработчик изменений комбо бокса.
    void onItemDoubleClicked(const QModelIndex &index);      // Обработчик двойного нажатия списка.
    void onListViewClicked(const QModelIndex &index);        // Обработчик нажатия на список.
    void onAddButtonClicked();                               // Обработчики нажатия на кнопки.
    void onDeleteButtonClicked();
    void onTextChanged(const QString &text);                 // Обработчик изменения текста.
    void onFindRouteClicked();
};
