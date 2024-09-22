#pragma once
#include "myexception.h"

class InputException : public MyException
{
protected:
    int errorType;                          // Тип ошибки.
public:
    InputException() : MyException() {}     // Конструкторы.
    InputException(const QString& str, int et) : MyException(str), errorType(et) {}
    virtual ~InputException() {}            // Деструктор.
    int getCode();                          // Геттер и сеттер для кода ошибки.
    void setCode(int);
    virtual void mesg();                    // Вывод сообщения.
};
