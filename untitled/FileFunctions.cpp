#include "fileexception.h"
#include <QFile>

void createFile(const QString& name) // Создание текстового файла.
{
    QFile file(name);                // Создание и открытие файла.
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        file.close();                // Закрытие файла, если успешно создан.
    else                             // Ошибка.
        throw FileException("Ошибка при создании файла", name);
}
void removeFile(const QString& name) // Удаление текстового файла.
{
    QFile file(name);                // Создание файлового объекта.
    if(!file.remove())               // Удаление.
        throw FileException("Ошибка при удалении файла", name);
}                                    // Ошибка в случае провала.
