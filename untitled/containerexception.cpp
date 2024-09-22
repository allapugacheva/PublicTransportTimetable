#include "containerexception.h"

QString ContainerException::getCType()  // Геттер.
{
    return this->contType;
}
void ContainerException::setCType(const QString& ctype) // Сеттер.
{
    this->contType = ctype;
}
void ContainerException::mesg()         // Вывод сообщения на экран.
{
    QMessageBox::warning(nullptr, "Ошибка контейнера " + this->contType, this->exceptionText);
}
