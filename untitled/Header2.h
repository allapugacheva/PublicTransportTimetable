#pragma once
#include "city.h"
#include "Header.h"
#include <QStandardItemModel>

void createFile(const QString&);                  // Создание файла.
void removeFile(const QString&);                  // Удаление файла.

void moveDown(QStandardItemModel*, int, Route*);  // Перемещение элемента вниз.
void moveUp(QStandardItemModel*, int, Route*);    // Перемещение элемента вверх.

bool checkRouteBuses(City*, const QString&);      // Проверка работает ли автобус на маршруте.
Route* findRoute2(City*, const QString&);         // Поиск маршрута.
