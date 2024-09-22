#pragma once
#include "Header.h"
#include "city.h"
#include <QKeyEvent>
#include <QDialog>
#include <QStack>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QComboBox>
#include <QListView>
#include <QRegularExpressionValidator>
#include "mytree.h"
#include "mytree.cpp"
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include "Header2.h"
#include "inputexception.h"

namespace Ui { class dialogStopWindow; }

class dialogStopWindow : public QDialog
{
public:                                             // Конструктор.
    dialogStopWindow(City*, bool, bool, Route* route = nullptr, Stop* stop = nullptr, QDialog *parent = nullptr);
    ~dialogStopWindow();                            // Деструктор.
    QPushButton* makePushButton(const QString&);    // Создание кнопки.
    QListView* makeListView();                      // Создание списка.
    QLabel* makeLabel(const QString&);              // Создание текстовой строки.
    QComboBox* makeComboBox();                      // Создание комбо бокса.
    QLineEdit* makeLineEdit();                      // Создание строки ввода.
    void activateMinusButton(int row, int lj);      // Активация кнопки удаления.
    void activatePlusButton(int row);               // Активация кнопки добавления.
    void addTime(int day, int row);                 // Добавление времени.
    void deleteTime(int row);                       // Удаление времению
    void activateOkButton();                        // Активация кнопки Ок.
    QStandardItem* makeItem(const QString&);        // Создание элемент списка.

protected:
    void keyPressEvent(QKeyEvent *event) override;  // Обработчик нажатия клавиш.

private:
    Ui::dialogStopWindow *ui;                       // Внешний вид окна.
    City* city;                                     // Город.
    Route* route;                                   // Маршрут.
    Stop* stop;                                     // Остановка.
    QStack<QString> stack1;                         // Стек для остановки.
    QStack<int> stack2;                             // Стек для направления движения.

private slots:
    void acceptClicked();                           // Обработчик нажатия Ок.
};
