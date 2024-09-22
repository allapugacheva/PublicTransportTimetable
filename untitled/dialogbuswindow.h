#pragma once
#include "Header.h"
#include "city.h"
#include <QDialog>
#include <QStack>
#include <QKeyEvent>
#include <QMessageBox>
#include "mytree.h"
#include "mytree.cpp"
#include "Header2.h"
#include <QRegularExpressionValidator>

namespace Ui { class dialogBusWindow; }

class dialogBusWindow : public QDialog
{
public:                                              // Конструктор.
    dialogBusWindow(TransportPark*, City*, Bus* b = nullptr, QDialog* parent = nullptr);
    ~dialogBusWindow();                              // Деструктор.
    void pushBusInStack();                           // Запись автобуса в стек.

protected:
    void keyPressEvent(QKeyEvent *event) override;   // Обработчик нажатия клавиш.

private:
    Ui::dialogBusWindow *ui;                         // Внешний вид окна.
    TransportPark* park;                             // Транспортный парк.
    City* city;                                      // Город.
    Bus* b;                                          // Автобус.
    QStack<Bus*> stack;                              // Стек для отмены действий.

private slots:
    void acceptClicked();                            // Обработчик нажатия клавиши Ок.
    void onCheckBoxStateChanged(int state);          // Обработчик изменения чек боксов.
    void onTextChanged(const QString &text);         // Обработчик изменения текста.
};
