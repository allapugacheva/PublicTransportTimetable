#include "routeswindow.h"
#include "ui_routeswindow.h"

routesWindow::routesWindow(bool editMode, const QString& cityName, City* city) : editMode(editMode),
    ui(new Ui::routesWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                   // Установка внешнего вида окна.
    setFocus();
    if(city == nullptr)
        this->city = new City(cityName);
    else
        this->city = city;               // Получение данных о городе.
    QPixmap pixmap(this->city->getPath()); // Установка карты.
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);
    ui->comboBox->addItem("Автобус");    // Заполнение комбо бокса.
    ui->comboBox->addItem("Троллейбус");
    ui->comboBox->addItem("Трамвай");
    ui->comboBox->setStyleSheet("QListView::item { height: 30px; }");
    ui->pushButton->setToolTip("Поиск маршрута между остановками");
    ui->lineEdit->setToolTip("Поиск маршрута по номеру или остановке");
    QRegularExpression regex("[а-яА-Я0-9-\" ]*$");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex);
    ui->lineEdit->setValidator(validator);
    QStandardItemModel* qm = new QStandardItemModel;
    QStandardItem* item;
    ui->listView->setStyleSheet("QListView::item { height: 30px; }");
    for(auto it = this->city->getRoutes().begin(); it != this->city->getRoutes().end(); it++)
    {
        int i = 0, n = 0;
        while(it->first->routeNumber[i]!=' ')
        {
            i++;
            n++;
        }
        QString temp = it->first->routeNumber.mid(0, n);
        if(temp != ui->comboBox->currentText())
            continue;
        i++;
        temp = it->first->routeNumber.mid(i, it->first->routeNumber.length() - i);
        item = makeItem(temp + " " + it->first->stopName + " - " + it->last->stopName);
        qm->appendRow(item);             // Добавление в список маршрутов.
    }
    ui->lineEdit->setToolTip("Введите номер искомого маршрута\nИли название остановки маршрута");
    ui->lineEdit->installEventFilter(this);
    ui->listView->setModel(qm);          // Обработчики взаимодействия с элементами.
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &routesWindow::handleComboBoxChange);
    connect(ui->listView, &QListView::doubleClicked, this, &routesWindow::onItemDoubleClicked);
    connect(ui->listView, &QListView::clicked, this, &routesWindow::onListViewClicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &routesWindow::onTextChanged);
    connect(ui->pushButton, &QPushButton::clicked, this, &routesWindow::onFindRouteClicked);
    if(this->editMode)
    {
        QPushButton* button = new QPushButton("Добавить");   // Добавление кнопки.
        button->setObjectName("button1");
        button->setFixedHeight(30);                          // Установка внешнего вида.
        button->setEnabled(true);
        QFont font = button->font();
        font.setBold(true);
        font.setPixelSize(14);
        button->setFont(font);                               // Обработчик нажатия.
        connect(button, &QPushButton::clicked, this, &routesWindow::onAddButtonClicked);
        ui->horizontalLayout->addWidget(button);             // Добавление на экран.
        button = new QPushButton("Удалить");
        button->setObjectName("button3");
        button->setFixedHeight(30);
        button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
        font = button->font();
        font.setBold(true);
        font.setPixelSize(14);
        button->setFont(font);
        connect(button, &QPushButton::clicked, this, &routesWindow::onDeleteButtonClicked);
        ui->horizontalLayout->addWidget(button);
    }
}
routesWindow::~routesWindow()      // Дреструктор.
{
    delete ui;
    stack.clear();
}
QStandardItem* routesWindow::makeItem(const QString& text)  // Создание объекта списка.
{
    QStandardItem* item = new QStandardItem(text);
    item->setEditable(false);           // Установка параметров внешнего вида.
    QFont font = item->font();
    font.setPointSize(11);
    item->setFont(font);
    item->setToolTip("Нажмите дважды, чтобы перейти к остановкам");
    return item;                        // Возврат объекта.
}
void routesWindow::deleteButton(const QString& objname)     // Удалить кнопку.
{
    QPushButton *button = findChild<QPushButton*>(objname); // Поиск объекта.
    delete button;                                          // Удаление.
}
void routesWindow::onFindRouteClicked()   // Нажато на поиск маршрута.
{
    dialogFindRouteWindow* w = new dialogFindRouteWindow(this->city);
    w->show();                            // Вызов окна поиска маршрута.
}
void routesWindow::onTextChanged(const QString &text)  // Изменен текст в окне ввода.
{
    QStandardItemModel* qm = new QStandardItemModel();
    QStandardItem* item;
    if(text!="")
    {
        foreach(Route r, this->city->getRoutes())
        {                                              // Цикл по всем маршрутам.
            if(r.first->routeNumber.left(7) == ui->comboBox->currentText() || r.first->routeNumber.left(10) == ui->comboBox->currentText())
            {
                if(r.first->routeNumber.contains(text))
                {                                      // Проверка, что введен номер маршрута.
                    int i = r.first->routeNumber.length() - 1;
                    while(r.first->routeNumber[i]!=' ')
                        i--;
                    item = makeItem(r.first->routeNumber.right(r.first->routeNumber.length()-1-i) + " " + r.first->stopName + " - " + r.last->stopName);
                    qm->appendRow(item);               // Добавление маршрута в результат.
                    continue;
                }
                Node* n = r.first;
                while(n!=nullptr)
                {
                    if(n->stopName.toLower().contains(text.toLower()))
                    {                                  // Проверка, что маршрут содержит остановку.
                        int i = r.first->routeNumber.length() - 1;
                        while(r.first->routeNumber[i]!=' ')
                            i--;
                        item = makeItem(r.first->routeNumber.right(r.first->routeNumber.length()-1-i) + " " + r.first->stopName + " - " + r.last->stopName);
                        qm->appendRow(item);           // Добавление маршрута в результат.
                        break;
                    }
                    n = n->stopNext;
                }
            }
        }
    }
    else
    {
        foreach(Route r, this->city->getRoutes())
        {
            if(r.first->routeNumber.left(7) == ui->comboBox->currentText() || r.first->routeNumber.left(10) == ui->comboBox->currentText())
            {
                int i = r.first->routeNumber.length() - 1;
                while(r.first->routeNumber[i]!=' ')
                    i--;
                item = makeItem(r.first->routeNumber.right(r.first->routeNumber.length()-1-i) + " " + r.first->stopName + " - " + r.last->stopName);
                qm->appendRow(item);                  // Добавление всех маршрутов, если пусто.
            }
        }
    }
    ui->listView->setModel(qm);
}
bool routesWindow::eventFilter(QObject *obj, QEvent *event) // Фильтр событий.
{
    if (obj->isWidgetType() && event->type() == QEvent::MouseButtonPress)
    {                                                       // Проверка, что было нажато где-то.
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(obj);
        if (lineEdit)
            lineEdit->setText("");                          // Очистка поля ввода, если на неё.
    }
    return false;
}
void routesWindow::handleComboBoxChange(const QString& currentText) // Изменение комбо бокса.
{
    QStandardItemModel* qm = new QStandardItemModel;
    for(auto it = this->city->getRoutes().begin(); it!= this->city->getRoutes().end(); it++)
    {                                                         // Цикл по всем маршрутам.
        int i = 0, n = 0;
        while(it->first->routeNumber[i]!=' ')
        {
            i++;
            n++;
        }
        QString temp = it->first->routeNumber.mid(0, n);
        if(temp != currentText)                               // Проверка, что нужный тип транспорта.
            continue;
        i++;
        temp = it->first->routeNumber.mid(i, it->first->routeNumber.length() - i);
        qm->appendRow(makeItem(temp + " " + it->first->stopName + " - " + it->last->stopName));
    }                                                         // Добавление маршрута.
    ui->listView->setModel(qm);
}
void routesWindow::onItemDoubleClicked(const QModelIndex& index) // Двойное нажатие на объект списка.
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QStandardItem *item = model->itemFromIndex(index);           // Получение данных.
    if (item)
    {
        QString route = ui->comboBox->currentText() + " ";
        int i = 0;
        while(item->text()[i]!=' ')
            i++;
        route += item->text().mid(0, i);
        close();                                                 // Переход к окну с остановками маршрута.
        routeStopsWindow* w = new routeStopsWindow(route, this->city, this->editMode);
        w->show();
    }
}
void routesWindow::keyPressEvent(QKeyEvent *event)               // Обработчик нажатия клавиш.
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_E)
    {
        this->editMode = this->editMode == true ? false : true;  // Включение или выключение режима редактирования.
        if(this->editMode)
        {
            QPushButton* button = new QPushButton("Добавить");   // Создание кнопки.
            button->setObjectName("button1");
            button->setFixedHeight(30);                          // Установка параметров внешнего вида.
            button->setEnabled(true);
            QFont font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);
            connect(button, &QPushButton::clicked, this, &routesWindow::onAddButtonClicked);
            ui->horizontalLayout->addWidget(button);             // Добавление кнопки на экран.
            button = new QPushButton("Удалить");
            button->setObjectName("button3");
            button->setFixedHeight(30);
            button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
            font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);
            connect(button, &QPushButton::clicked, this, &routesWindow::onDeleteButtonClicked);
            ui->horizontalLayout->addWidget(button);
        }
        else
        {
            deleteButton("button1");                       // Удаление кнопки.
            deleteButton("button3");
        }
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q)
    {
        close();
        cityWindow *w = new cityWindow(this->editMode);    // Переход к окну с городами.
        w->show();
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z && editMode)
    {
        if(!stack.empty())                                 // Проверка, что стек не пуст.
        {
            this->city = new City(*(stack.pop()));         // Обновление данных.
            this->handleComboBoxChange(ui->comboBox->currentText());
        }
    }
}
void routesWindow::closeEvent(QCloseEvent *event)          // Событие закрытия окна.
{
    this->city->putInFileC();                              // Запись в файл.
}
void routesWindow::onListViewClicked(const QModelIndex &index) // Нажатие на список.
{
    QPushButton *button3 = findChild<QPushButton*>("button3"); // Поиск кноки.
    if (index.isValid())
    {
        if(button3!=nullptr)                                   // Активация.
            button3->setEnabled(true);
    }
    else
    {
        if(button3!=nullptr)
            button3->setEnabled(false);
    }
}
void routesWindow::onAddButtonClicked()                        // Добавление маршрута.
{
    City* c = new City(*(this->city));                         // Вызов окна добавления маршрута.
    dialogRouteWindow* w = new dialogRouteWindow(this->city);
    w->show();
    if(w->exec() == QDialog::Accepted)
    {
        stack.push(c);                                         // Обвноление данных.
        this->handleComboBoxChange(ui->comboBox->currentText());
    }
}
void routesWindow::onDeleteButtonClicked()                     // Удаление маршрута.
{
    City* c = new City(*(this->city));
    stack.push(c);
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QModelIndex cur = ui->listView->currentIndex();            // Получение данных.
    QStandardItem* item = qm->itemFromIndex(cur);
    int i = 0;
    while(item->text()[i]!=' ')
        i++;
    QString route = ui->comboBox->currentText() + " " + item->text().left(i);
    auto it = this->city->getRoutes().begin();                 // Поиск маршрута.
    while(it!=this->city->getRoutes().end() && it->first->routeNumber != route)
        it++;
    if(it == this->city->getRoutes().end())
        exit(0);
    Node* t = it->first;
    while(t!=nullptr)
    {
        if(t->routeNext!=nullptr)                              // Изменение связей в данных.
            t->routeNext->routePrev = t->routePrev;
        if(t->routePrev!=nullptr)
            t->routePrev->routeNext = t->routeNext;
        t = t->stopNext;
    }
    MyTree<Stop>::MyIterator jt = this->city->getStops().begin();
    while(!jt.isnull())                                        // Обход всех остановок.
    {
        if((*jt).stop->routeNumber == route)
        {
            if((*jt).stop->routeNext!=nullptr)                 // Изменение связей узлов.
                (*jt).stop->routeNext->routePrev = (*jt).stop->routePrev;
            if((*jt).stop->routePrev!=nullptr)
                (*jt).stop->routePrev->routeNext = (*jt).stop->routeNext;

            if((*jt).stop->routeNext != nullptr)
                (*jt).stop = (*jt).stop->routeNext;
            else if((*jt).stop->routePrev!=nullptr)
                (*jt).stop = (*jt).stop->routePrev;
            else
            {
                (*jt).stop->routeNumber = "";
                (*jt).stop->stopNext = nullptr;
                (*jt).stop->stopPrev = nullptr;
            }
        }
        jt++;
    }
    this->city->getRoutes().erase(it);                         // Удаление элемента.
    for(auto lt = this->city->getParks().begin(); lt!=this->city->getParks().end(); lt++)
    {
        if(lt->servedRoutes.contains(route))
            lt->servedRoutes.removeAll(route);
    }
    qm->removeRow(cur.row());
}
