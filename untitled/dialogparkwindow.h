#pragma once
#include "Header.h"
#include "city.h"
#include <QDialog>
#include <QStack>
#include <QKeyEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include "dialogbuswindow.h"
#include <QMessageBox>
#include "mytree.h"
#include "mytree.cpp"
#include "treealgorithms.h"
#include "treealgorithms.cpp"
#include "Header2.h"
#include <QRegularExpressionValidator>

namespace Ui { class dialogParkWindow; }

class dialogParkWindow : public QDialog
{
public:                                                 // Конструктор.
    dialogParkWindow(City*, TransportPark* park = nullptr, QDialog* parent = nullptr);
    ~dialogParkWindow();                                // Деструктор.
    void addBus(const QString&);                        // Добавление автобуса в список.

protected:
    void keyPressEvent(QKeyEvent *event) override;      // Обработчик нажатия клавиш.

private:
    Ui::dialogParkWindow *ui;                           // Внешний вид окна.
    City* city;                                         // Город.
    TransportPark* park;                                // Транспортный парк.
    bool editMode;                                      // Режим редактирования.
    QStack<TransportPark*> stack;                       // Стек для отмены действий.

private slots:
    void acceptClicked();                               // Обработчики нажатия кнопок.
    void addBusClicked();
    void removeBusClicked();
    void onItemDoubleClicked(const QModelIndex &index); // Обработчик двойного нажатия на эл-т списка.
    void onTextChanged(const QString &text);            // Обработчик изменения текста.
};
