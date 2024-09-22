#pragma once
#include "myexception.h"

class FileException : public MyException
{
protected:
    QString fileName;                     // Название файла.
public:
    FileException() : MyException() {}    // Конструкторы.
    FileException(const QString& extext, const QString& fname) : MyException(extext), fileName(fname) {}
    virtual ~FileException() { }          // Деструктор.
    QString getFName();                   // Геттер и сеттер для имени файла.
    void setFName(const QString&);
    virtual void mesg();                  // Вывод информации на экран.
};
