#pragma once
#include "Header.h"
#include "city.h"
#include <QDialog>
#include <QStack>
#include <QKeyEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include "mytree.h"
#include "mytree.cpp"
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include "Header2.h"
#include "inputexception.h"

namespace Ui { class dialogTimeWindow; }

class dialogTimeWindow : public QDialog
{
public:                                           // Конструктор.
    dialogTimeWindow(City*, const QString&, bool, bool, QDialog *parent = nullptr);
    ~dialogTimeWindow();                          // Деструктор.
    QStandardItem* makeItem(const QString&);      // Создание объекта списка.
    void addTime(int);                            // Добавление времени.
    void deleteTime(int);                         // Удаление времени.
    void activateOkButton();                      // Активация кнопки Ок.
    void addBus(int, Node*&, int);                // Добавление автобуса.

protected:
    void keyPressEvent(QKeyEvent *event) override;// Обработчик нажатия клавиш.

private:
    Ui::dialogTimeWindow *ui;                     // Внешний вид окна.
    City* city;                                   // Город.
    QString route;                                // Маршрут.
    QStack<QString> stack1;                       // Стек для времени.
    QStack<int> stack2;                           // Стек для дня работы.

private slots:
    void acceptClicked();                         // Обработчик нажатия Ок.
    void onTextChanged(const QString &text);      // Обработчик изменения текста.
};
