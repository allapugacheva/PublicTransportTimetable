#include "myexception.h"

void MyException::mesg()   // Вывод сообщения.
{
    QMessageBox::warning(nullptr, "Ошибка", this->exceptionText);
}
QString MyException::getExText()  // Геттер.
{
    return this->exceptionText;
}
void MyException::setExText(const QString& text)
{                                 // Сеттер.
    this->exceptionText = text;
}
