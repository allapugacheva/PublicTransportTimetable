#include "routestopswindow.h"
#include "ui_routestopswindow.h"

routeStopsWindow::routeStopsWindow(const QString& route, City*& city, bool editMode) :
    city(city), editMode(editMode), ui(new Ui::routeStopsWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                                      // Установка внешнего вида окна.
    setFocus();
    QPixmap pixmap(this->city->getPath());                  // Установка изображения карты.
    ui->label_2->setPixmap(pixmap);
    ui->label_2->setScaledContents(true);
    ui->label->setText(route);                              // Установка текста.
    Route* r = findRoute2(this->city, route);
    ui->comboBox->setStyleSheet("QListView::item { height: 30px; }");
    if(!r->first->timetableNextWeek.empty() || !r->first->timetableNextWork.empty())
        ui->comboBox->addItem(r->first->stopName + " - " + r->last->stopName);  // Добавление направлений движения.
    if(!r->first->timetablePrevWeek.empty() || !r->first->timetablePrevWork.empty())
        ui->comboBox->addItem(r->last->stopName + " - " + r->first->stopName);
    ui->listView->setStyleSheet("QListView::item { height: 30px; }");
    this->handleComboBoxChange(ui->comboBox->currentText());                    // Вывод остановок.
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &routeStopsWindow::handleComboBoxChange);
    connect(ui->listView, &QListView::doubleClicked, this, &routeStopsWindow::onItemDoubleClicked);
    connect(ui->listView, &QListView::clicked, this, &routeStopsWindow::onListViewClicked);
    if(this->editMode)                                                          // Обработчики взаимодействия с элементами.
    {
        QPushButton* button = new QPushButton("Добавить");      // Создание кнопок.
        button->setObjectName("button1");
        button->setFixedHeight(30);                             // Настройка их внешнего вида.
        button->setEnabled(true);
        QFont font = button->font();
        font.setBold(true);
        font.setPixelSize(14);
        button->setFont(font);
        connect(button, &QPushButton::clicked, this, &routeStopsWindow::onAddButtonClicked);
        ui->horizontalLayout->addWidget(button);                // Добавление на экран.
        button = new QPushButton("Удалить");
        button->setObjectName("button3");
        button->setFixedHeight(30);
        button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
        font = button->font();
        font.setBold(true);
        font.setPixelSize(14);
        button->setFont(font);
        connect(button, &QPushButton::clicked, this, &routeStopsWindow::onDeleteButtonClicked);
        ui->horizontalLayout->addWidget(button);
    }
}
routeStopsWindow::~routeStopsWindow()                           // Деструктор.
{
    delete ui;
    stack.clear();
}
void routeStopsWindow::deleteButton(const QString& objname)     // Удаление кнопки.
{
    QPushButton *button = findChild<QPushButton*>(objname);     // Получение объекта.
    delete button;                                              // Удаление кнопки.
}
void routeStopsWindow::onItemDoubleClicked(const QModelIndex &index) // Двойное нажатие на элемент списка.
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QStandardItem *item = model->itemFromIndex(index);               // Получение данных.
    if (item)
    {
        Route* r = findRoute2(this->city, ui->label->text());        // Поиск маршрута.
        close();                                                     // Вызов окна с расписанием остановки.
        routeStopTimetableWindow* w = new routeStopTimetableWindow(item->text(), ui->label->text(), this->city, this->editMode,
                           ui->comboBox->currentText() == r->first->stopName + " - " + r->last->stopName);
        w->show();
    }
}
void routeStopsWindow::moveItemUp(const QModelIndex& index)      // Движение элемента вверх.
{
    if(index.row()>0)
    {
        City* c = new City((*this->city));
        stack.push(c);                                           // Сохранение данных.
        QStandardItemModel* itemmodel = qobject_cast<QStandardItemModel*>(ui->listView->model());
        moveUp(itemmodel, index.row(), findRoute2(this->city, ui->label->text()));
    }                                                            // Движение элемента вверх.
}
void routeStopsWindow::moveItemDown(const QModelIndex& index)    // Движение элемента вниз.
{
    QStandardItemModel* itemmodel = qobject_cast<QStandardItemModel*>(ui->listView->model());
    if(index.row() < itemmodel->rowCount() - 1)                  // Проверка, что есть куда двигаться.
    {
        City* c = new City((*this->city));
        stack.push(c);                                           // Сохранение данных.
        moveDown(itemmodel, index.row(), findRoute2(this->city, ui->label->text()));
    }                                                            // Движение элемента вниз.
}
void routeStopsWindow::keyPressEvent(QKeyEvent *event)           // Обработка нажатий клавиатуры.
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_E)
    {
        this->editMode = this->editMode == true ? false : true;  // Включение или выключение режима
        if(this->editMode)                                       // редактирования.
        {
            QPushButton* button = new QPushButton("Добавить");   // Создание кнопок.
            button->setObjectName("button1");
            button->setFixedHeight(30);                          // Установка их внешнего вида.
            button->setEnabled(true);
            QFont font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);                               // Установка обработчика нажатия.
            connect(button, &QPushButton::clicked, this, &routeStopsWindow::onAddButtonClicked);
            ui->horizontalLayout->addWidget(button);             // Добавление на экран.
            button = new QPushButton("Удалить");
            button->setObjectName("button3");
            button->setFixedHeight(30);
            button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
            font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);
            connect(button, &QPushButton::clicked, this, &routeStopsWindow::onDeleteButtonClicked);
            ui->horizontalLayout->addWidget(button);
        }
        else
        {
            deleteButton("button1");                             // Удаление кнопок.
            deleteButton("button3");
            stack.clear();
        }
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q)
    {
        close();                                                 // Закрытие окна.
        routesWindow* w = new routesWindow(this->editMode, "", this->city);
        w->show();                                               // Открытие окна с маршрутами.
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W && editMode)
    {                                                            // Получение данных.
        QItemSelectionModel* selectionModel = ui->listView->selectionModel();
        QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
        if (!selectedIndexes.isEmpty())
            moveItemUp(ui->listView->currentIndex());            // Перенос элемента вверх.
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S && editMode)
    {
        QItemSelectionModel* selectionModel = ui->listView->selectionModel();
        QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
        if (!selectedIndexes.isEmpty())
            moveItemDown(ui->listView->currentIndex());          // Перенос элемента вниз.
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z && editMode)
    {
        if(!stack.empty())                                       // Проверка, что стек не пуст.
        {
            this->city = new City(*(stack.pop()));
            Route* r = findRoute2(this->city, ui->label->text());// Поиск маршрута.
            Node* n = r->first;
            QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
            while(n!=nullptr)                                    // Получение модели данных.
            {
                QStandardItem* item = makeItem(n->stopName);
                qm->appendRow(item);                             // Обновление данных.
                n = n->stopNext;
            }
            ui->listView->setModel(qm);
            remakeComboBox(r->first->stopName, r->last->stopName);
        }
    }
}
void routeStopsWindow::closeEvent(QCloseEvent *event)            // Обработчик закрытия окна.
{
    this->city->putInFileC();                                    // Запись города в файл.
}
QStandardItem* routeStopsWindow::makeItem(const QString& text)   // Создание объекта списка.
{
    QStandardItem* item = new QStandardItem(text);
    item->setEditable(false);                                    // Установка параметров.
    QFont font = item->font();
    font.setPointSize(11);
    item->setFont(font);
    item->setToolTip("Нажмите дважды, что открыть расписание");
    return item;                                                 // Возврат объекта.
}
void routeStopsWindow::onListViewClicked(const QModelIndex &index)// Нажатие на элемент списка.
{
    QPushButton *button3 = findChild<QPushButton*>("button3");
    if (index.isValid())                                         // Получение кнопки.
    {
        if(button3!=nullptr)
            button3->setEnabled(true);                           // Включение, если нажат.
    }
    else
    {
        if(button3!=nullptr)
            button3->setEnabled(false);
    }
}
void routeStopsWindow::onAddButtonClicked()                      // Добавление элемента.
{
    City* c = new City((*this->city));
    Route* r = findRoute2(this->city, ui->label->text());        // Получение данных.
    bool work = r->daysOfWork[0] == true || r->daysOfWork[1] == true || r->daysOfWork[2] == true || r->daysOfWork[3] == true || r->daysOfWork[4] == true;
    bool week = r->daysOfWork[5] == true || r->daysOfWork[6] == true;
    dialogStopWindow* w = new dialogStopWindow(this->city, work, week, r);
    w->show();                                                   // Вызов диалогового окна о добавлении элемента.
    if(w->exec() == QDialog::Accepted)
    {
        stack.push(c);
        QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
        QStandardItem* item = makeItem(r->last->stopName);       // Получение модели данных.
        qm->appendRow(item);
        Stop* jt = TreeAlgorithms<Stop>::findOne(Stop(r->last->stopName), this->city->getStops());
        if(jt==nullptr)                                          // Поиск остановки.
        {
            QMessageBox::warning(this, "Добавление остановки", "Остановка не найдена");
            return;
        }
        if(jt->stop->routeNumber=="")                            // Добавление в данные.
            jt->stop = r->last;
        else
        {
            Node* n = jt->stop;
            while(n->routeNext!=nullptr)
                n = n->routeNext;
            n->routeNext = r->last;
            r->last->routePrev = n;
        }
        remakeComboBox(r->first->stopName, r->last->stopName);   // Обновление маршрута.
    }
}
void routeStopsWindow::onDeleteButtonClicked()   // Удаление остановки.
{
    City* c = new City((*this->city));
    stack.push(c);                               // Сохранение данных.
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QModelIndex cur = ui->listView->currentIndex();
    QStandardItem* item = qm->itemFromIndex(cur);
    auto it = this->city->getRoutes().begin();
    while(it!=this->city->getRoutes().end() && it->first->routeNumber!=ui->label->text())
        it++;                                    // Поиск маршрута.
    if(it==this->city->getRoutes().end())
    {
        QMessageBox::warning(this, "Редактирование", "Маршрут не найден");
        return;
    }
    Node* n = it->first;                         // Поиск остановки.
    while(n!= nullptr && n->stopName!=item->text())
        n = n->stopNext;
    if(n==nullptr)
    {
        QMessageBox::warning(this, "Удаление остановки", "Остановка не найдена");
        return;
    }
    if(n->routeNext!=nullptr)                    // Изменение связей в узлах.
        n->routeNext->routePrev = n->routePrev;
    if(n->routePrev!=nullptr)
        n->routePrev->routeNext = n->routeNext;
    if(n == it->first && n == it->last)
    {
        this->city->getRoutes().erase(it);
        foreach(TransportPark park, this->city->getParks())
        {
            auto kt = park.servedRoutes.begin();
            while(kt!=park.servedRoutes.end())
            {
                if(*kt == ui->label->text())
                    park.servedRoutes.erase(kt);
                else
                    kt++;
            }
        }
        close();
        routesWindow* w = new routesWindow(this->editMode, "", this->city);
        w->show();
    }
    else if(n == it->first)                      // Перемещение указателей на конечные остановки.
    {
        it->first = it->first->stopNext;
        it->first->stopPrev = nullptr;
    }
    else if(n == it->last)
    {
        it->last = it->last->stopPrev;
        it->last->stopNext = nullptr;
    }
    else
    {
        if(n->stopNext != nullptr)
            n->stopNext->stopPrev = n->stopPrev;
        if(n->stopPrev != nullptr)
            n->stopPrev->stopNext = n->stopNext;
    }
    Stop* jt = TreeAlgorithms<Stop>::findOne(Stop(item->text()), this->city->getStops());
    if(jt == nullptr)
    {
        QMessageBox::warning(this, "Удаление остановки", "Остановка не найдена");
        return;
    }
    if(jt->stop->routeNumber == ui->label->text())
    {
        if(jt->stop->routeNext == nullptr && jt->stop->routePrev == nullptr)
        {
            jt->stop->routeNumber = "";
            jt->stop->stopNext = nullptr;
            jt->stop->stopPrev = nullptr;
        }
        else
        {
            if(jt->stop->routeNext!=nullptr)
                jt->stop->routeNext->routePrev = jt->stop->routePrev;
            if(jt->stop->routePrev!=nullptr)
                jt->stop->routePrev->routeNext = jt->stop->routeNext;
            if(jt->stop->routeNext!=nullptr)
                jt->stop = jt->stop->routeNext;
            else
                jt->stop = jt->stop->routePrev;
        }
    }
    qm->removeRow(cur.row());                                // Удаление остановки из списков.
    remakeComboBox(it->first->stopName, it->last->stopName);
}
void routeStopsWindow::remakeComboBox(const QString& s1, const QString& s2)
{                                        // Обновлении информации о конечных пунктах.
    ui->comboBox->removeItem(0);         // Удаление старой.
    ui->comboBox->removeItem(0);
    ui->comboBox->addItem(s1 + " - " + s2); // Добавление новой.
    ui->comboBox->addItem(s2 + " - " + s1);
}
void routeStopsWindow::handleComboBoxChange(const QString& currentText) // Изменение выбора в комбо боксе.
{
    QStandardItemModel* qm = new QStandardItemModel;
    Route* r = findRoute2(this->city, ui->label->text());               // Поиск маршрута.
    if(currentText == r->first->stopName + " - " + r->last->stopName)
    {
        Node* n = r->first;
        while(n!=nullptr)
        {
            qm->appendRow(makeItem(n->stopName));
            n = n->stopNext;                                            // Занесение остановок в прямом направлении.
        }
    }
    else
    {
        Node* n = r->last;
        while(n!=nullptr)
        {
            qm->appendRow(makeItem(n->stopName));
            n = n->stopPrev;                                            // Занесение остановок в обратном направлении.
        }
    }
    ui->listView->setModel(qm);
}
