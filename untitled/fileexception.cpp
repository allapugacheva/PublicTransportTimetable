#include "fileexception.h"

QString FileException::getFName() // Геттер.
{
    return this->fileName;
}
void FileException::setFName(const QString& fname) // Сеттер.
{
    this->fileName = fname;
}
void FileException::mesg()        // Вывод сообщения.
{
    QMessageBox::warning(nullptr, "Ошибка файла " + this->fileName, this->exceptionText);
}
