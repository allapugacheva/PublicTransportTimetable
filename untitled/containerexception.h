#pragma once
#include "myexception.h"

class ContainerException : public MyException
{
    QString contType;                             // Тип контейнера.
public:
    ContainerException() : MyException() {}       // Конструкторы.
    ContainerException(const QString& etext, const QString& ctype) : MyException(etext), contType(ctype) {}
    virtual ~ContainerException() {}              // Деструктор.
    QString getCType();                           // Геттер и сеттер для типа.
    void setCType(const QString&);
    virtual void mesg();                          // Вывод информации.
};
