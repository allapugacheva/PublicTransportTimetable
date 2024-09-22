#include "citywindow.h"
#include "ui_citywindow.h"

cityWindow::cityWindow(bool editMode) : editMode(editMode), ui(new Ui::cityWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                // Установка внешнего вида.
    readFromFileCities();             // Чтение данных из файла.
    setFocus();
    if(this->editMode)                // Проверка, что включен режим редактирования.
    {
        QPushButton *button = new QPushButton("Добавить");  // Создание кнопок и
        button->setObjectName("button1");                   // придание им внешнего вида.
        button->setEnabled(true);
        button->setFixedHeight(30);
        QFont font = button->font();
        font.setBold(true);
        font.setPixelSize(14);
        button->setFont(font);
        connect(button, &QPushButton::clicked, this, &cityWindow::onAddButtonClicked);
        ui->horizontalLayout->addWidget(button);
        button = new QPushButton("Изменить");               // Текст кнопки.
        button->setObjectName("button2");                   // Имя объекта.
        button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
        button->setFixedHeight(30);                         // Размеры.
        font = button->font();                              // Шрифт.
        font.setBold(true);
        font.setPixelSize(14);                              // Размер шрифта.
        button->setFont(font);                              // Обработчик нажатия.
        connect(button, &QPushButton::clicked, this, &cityWindow::onEditButtonClicked);
        ui->horizontalLayout->addWidget(button);            // Добавление в окно.
        button = new QPushButton("Удалить");
        button->setObjectName("button3");
        button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
        button->setFixedHeight(30);
        font = button->font();
        font.setBold(true);
        font.setPixelSize(14);
        button->setFont(font);
        connect(button, &QPushButton::clicked, this, &cityWindow::onDeleteButtonClicked);
        ui->horizontalLayout->addWidget(button);
    }
    connect(ui->listView, &QListView::doubleClicked, this, &cityWindow::onItemDoubleClicked);
    connect(ui->listView, &QListView::clicked, this, &cityWindow::onListViewClicked);
}
cityWindow::~cityWindow()  // Деструктор.
{
    delete ui;       // Удаление внешнего вида.
    stack.clear();   // Очистка стека.
}
void cityWindow::closeEvent(QCloseEvent *event)  // Событие закрытия окна.
{
    try
    {
        QFile file("Города.txt");           // Запись данных в файл.
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            QTextStream out(&file);         // Файловый поток.
            for(auto it = this->cities.begin(); it!=this->cities.end(); it++)
                out << *it << '\n';         // Запись всех городов.
            file.close();
        }
        else                                // Ошибка.
            throw FileException("Ошибка во время открытия файла", "Города.txt");
    }
    catch(FileException& ex)
    {
        ex.mesg();
    }
}
void cityWindow::onItemDoubleClicked(const QModelIndex &index) // Двойное нажатие на список.
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QStandardItem *item = model->itemFromIndex(index);  // Получение выделенного элементы.
    if (item)
    {
        close();                                        // Вызов нового окна.
        routesWindow* w = new routesWindow(this->editMode, item->text(), nullptr);
        w->show();                                      // Отображение нового окна.
    }
}
void cityWindow::keyPressEvent(QKeyEvent *event)        // Обработчик нажатия клавиш.
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_E)
    {                                                   // Изменение режима редактирования.
        this->editMode = this->editMode == true ? false : true;
        if(this->editMode)
        {
            QPushButton *button = new QPushButton("Добавить"); // Создание кнопок и
            button->setObjectName("button1");                  // изменение внешнего вида.
            button->setEnabled(true);
            button->setFixedHeight(30);
            QFont font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);
            connect(button, &QPushButton::clicked, this, &cityWindow::onAddButtonClicked);
            ui->horizontalLayout->addWidget(button);
            button = new QPushButton("Изменить");
            button->setObjectName("button2");
            button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
            button->setFixedHeight(30);
            font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);
            connect(button, &QPushButton::clicked, this, &cityWindow::onEditButtonClicked);
            ui->horizontalLayout->addWidget(button);
            button = new QPushButton("Удалить");
            button->setObjectName("button3");
            button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
            button->setFixedHeight(30);
            font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);
            connect(button, &QPushButton::clicked, this, &cityWindow::onDeleteButtonClicked);
            ui->horizontalLayout->addWidget(button);
        }
        else
        {
            deleteButton("button1");             // Удаление кнопок.
            deleteButton("button2");
            deleteButton("button3");
            stack.clear();
        }
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z && editMode)
    {
        if(!this->stack.isEmpty())               // Проверка, что стек не пуст.
        {
            City* city = stack.pop();            // Достать город.
            if(city == nullptr)
            {                                    // Удаление добавленного.
                removeCity(this->cities.back(), this->cities.size()-1);
            }
            else
            {                                    // Добавление удалённого.
                addCity(city->getName());
                city->putInFileC();
                this->cities.push_back(city->getName());
            }
        }
    }
}
void cityWindow::onListViewClicked(const QModelIndex &index) // Нажатие на список.
{
    QPushButton *button2 = findChild<QPushButton*>("button2"), // Получение кнопок.
        *button3 = findChild<QPushButton*>("button3");
    if (index.isValid())
    {
        if(button2!=nullptr)
            button2->setEnabled(true);                       // Активация если
        if(button3!=nullptr)                                 // выбран элемент списка.
            button3->setEnabled(true);
    }
    else
    {
        if(button2!=nullptr)
            button2->setEnabled(false);
        if(button3!=nullptr)
            button3->setEnabled(true);
    }
}
void cityWindow::onAddButtonClicked()                        // Добавление города.
{
    dialogCityWindow* w = new dialogCityWindow(&cities);     // Вызов диалового окна
    w->show();                                               // добавления.
    if(w->exec() == QDialog::Accepted)                       // Проверка на успешность.
    {
        addCity(this->cities.last());                        // Добавление города.
        stack.push(nullptr);
    }
}
void cityWindow::onEditButtonClicked()                       // Редактирование города.
{
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QModelIndex cur = ui->listView->currentIndex();          // Получение выбранного города.
    QStandardItem* item = qm->itemFromIndex(cur);
    City* city = new City(item->text());                     // Сохранение старых данных.
    dialogCityWindow* w = new dialogCityWindow(&(this->cities), city);
    w->show();                                               // Вызов диалогового окна изменения.
    if(w->exec() == QDialog::Accepted)
        item->setText(city->getName());                      // Изменение данных.
}
void cityWindow::onDeleteButtonClicked()                     // Удаление города.
{
    City* city = new City(ui->listView->currentIndex().data().toString());
    this->stack.push(city);                                  // Удаление из списка.
    removeCity(ui->listView->currentIndex().data().toString(), ui->listView->currentIndex().row());
}
void cityWindow::readFromFileCities()
{
    try
    {
        QFile file("Города.txt");            // Открытие файла.
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw FileException("Ошибка во время открытия файла", "Города.txt");
        QTextStream in(&file);               // Файловый поток.
        QStandardItemModel* qm = new QStandardItemModel;
        QStandardItem* item;
        QFont font;
        QString temp;
        while(!in.atEnd())                   // Чтение до конца файла.
        {
            temp = in.readLine();            // Чтение строки.
            this->cities.push_back(temp);    // Добавление в список.
            item = new QStandardItem(temp);
            item->setEditable(false);
            font = item->font();
            font.setBold(true);
            font.setPointSize(14);
            item->setFont(font);
            qm->appendRow(item);             // Добавление в модель данных.
        }
        ui->listView->setModel(qm);          // Установка модели данных.
    }
    catch(FileException& ex)
    {
        ex.mesg();                           // Сообщение об ошибке.
    }
}
void cityWindow::deleteButton(const QString& objname)           // Удаление кнопки.
{
    QPushButton *button = findChild<QPushButton*>(objname);     // Поиск кнопки.
    delete button;                                              // Удаление.
}
void cityWindow::addCity(const QString& name)                   // Добавление города.
{
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QStandardItem* item = new QStandardItem(name);              // Создание объекта для списка.
    item->setEditable(false);
    QFont font = item->font();                                  // Редактирование внешнего вида.
    font.setBold(true);
    font.setPointSize(14);
    item->setFont(font);
    qm->appendRow(item);                                        // Добавление в модель данных.
}
void cityWindow::removeCity(const QString& city, int row)       // Удаление города.
{
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    removeFile(city + "Маршруты.txt");                          // Удаление файлов.
    removeFile(city + "Остановки.txt");
    removeFile(city + "Парки.txt");
    removeFile(city + "Карта.txt");
    this->cities.removeOne(city);                               // Удаление из списка.
    qm->removeRow(row);
}
