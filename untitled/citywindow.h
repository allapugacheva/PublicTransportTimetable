#pragma once
#include "city.h"
#include <QWidget>
#include <QStack>
#include <QKeyEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include "routeswindow.h"
#include "dialogcitywindow.h"
#include <QPushButton>
#include "fileexception.h"
#include <functional>
#include "Header2.h"

namespace Ui { class cityWindow; }

class cityWindow : public QWidget
{
public:
    explicit cityWindow(bool editMode = false);        // Конструктор.
    ~cityWindow();                                     // Деструктор.
    void readFromFileCities();                         // Чтение из файла городов.
    void deleteButton(const QString&);                 // Удаление кнопки.
    void addCity(const QString&);                      // Добавление города.
    void removeCity(const QString&, int);              // Удаление города.

protected:
    void keyPressEvent(QKeyEvent *event) override;     // Обработчик нажатия кнопки.
    void closeEvent(QCloseEvent *event) override;      // Обработчик события закрытия окна.

private:
    QStringList cities;                                // Список городов.
    bool editMode;                                     // Режим редактирования.
    Ui::cityWindow *ui;                                // Внешний вид окна.
    QStack<City*> stack;                               // Стек для хранения действий.

private slots:
    void onItemDoubleClicked(const QModelIndex &index);// Обработчик двойного нажатия.
    void onListViewClicked(const QModelIndex &index);  // Обработчик выбора элемента списка.
    void onAddButtonClicked();                         // Обработчики нажатия на кнопки.
    void onEditButtonClicked();
    void onDeleteButtonClicked();
};
