#include "inputexception.h"

int InputException::getCode()  // Геттер.
{
    return this->errorType;
}
void InputException::setCode(int type) // Сеттер.
{
    this->errorType = type;
}
void InputException::mesg()    // Вывод сообщения об ошибке.
{
    QMessageBox::warning(nullptr, "Ошибка ввода", this->exceptionText);
}
