#pragma once
#include <QMessageBox>

class MyException
{
protected:
    QString exceptionText;                // Текст ошибки.
public:
    MyException() {}                      // Конструкторы.
    explicit MyException(const QString& ex) : exceptionText(ex) {}
    virtual ~MyException() {}             // Деструктор.
    virtual void mesg();                  // Вывод сообщения.
    QString getExText();                  // Геттер и сеттер для сообщения.
    void setExText(const QString&);
};
