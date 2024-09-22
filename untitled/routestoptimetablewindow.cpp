#include "routestoptimetablewindow.h"
#include "ui_routestoptimetablewindow.h"

routeStopTimetableWindow::routeStopTimetableWindow(const QString& stop, const QString& route, City*& city, bool editMode, bool dir) :
    city(city), editMode(editMode), direct(dir), ui(new Ui::routeStopTimetableWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                                                   // Установка внешнего вида окна.
    setFocus();
    QFont font;
    QStandardItemModel* qm;
    QPixmap pixmap(this->city->getPath());                               // Установка изображения.
    ui->label_3->setPixmap(pixmap);
    ui->label_3->setScaledContents(true);
    font.setPointSize(11);
    QToolTip::setFont(font);
    ui->listView->setStyleSheet("QListView::item { height: 30px; }");    // Установка параметров списка и комбо бокса.
    ui->tableView->verticalHeader()->setDefaultSectionSize(30);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(30);
    QObject::connect(&timer, &QTimer::timeout, [&]() {                   // Обработчик срабатывания таймера.
        QTime currentTime = QTime::currentTime();
        QString text = currentTime.toString("hh:mm:ss");                 // Перевод времени в строку и вывод на экран.
        ui->label_5->setText(text);
    });
    timer.start(1000);                                                   // Запуск таймера.
    QObject::connect(&timer2, &QTimer::timeout, this, &routeStopTimetableWindow::handleTimerEvent);
    timer2.start(60000);                                                 // Обратчик таймера для автобусов.
    Stop* it = TreeAlgorithms<Stop>::findOne(Stop(stop), this->city->getStops());
    if(it == nullptr)
    {
        QMessageBox::warning(this, "Расписание маршрута", "Остановка не найдена");
        return;                                                          // Поиск остановки.
    }
    ui->label->setText(it->stop->stopName);
    ui->label_2->setText(route);                                         // Установка текста в надписях.
    int a = 0;
    Node* kt = it->stop;
    while(kt->routePrev!=nullptr)
        kt = kt->routePrev;
    while(kt->routeNext!=nullptr && kt->routeNumber!=route)              // Поиск остановки маршрута.
    {
        kt = kt->routeNext;
        a++;
    }
    if(kt->routeNumber!=route)
    {
        QMessageBox::warning(this, "Расписание маршрута", "Остановка не найдена");
        return;
    }
    qm = new QStandardItemModel;
    Route* r = findRoute2(this->city, route);
    ui->comboBox->addItem("Пн");                                         // Заполнение комбо бокса.
    ui->comboBox->addItem("Вт");
    ui->comboBox->addItem("Ср");
    ui->comboBox->addItem("Чт");
    ui->comboBox->addItem("Пт");
    ui->comboBox->addItem("Сб");
    ui->comboBox->addItem("Вс");
    ui->comboBox->setStyleSheet("QListView::item { height: 30px; }");
    for(int i = 0; i<7; i++)
        if(r->daysOfWork[i] == false)
            ui->comboBox->setItemData(i, QVariant(0), Qt::ItemDataRole::UserRole - 1);
    ui->comboBox->setCurrentIndex(QDate::currentDate().dayOfWeek()-1);  // Выбор текущего дня недели.
    this->handleTimerEvent();
    if((r->daysOfWork[QDate::currentDate().dayOfWeek()-1]))
        this->handleComboBoxChange(ui->comboBox->currentText());        // Установка данных.
    connect(ui->tableView, &QTableView::clicked, this, &routeStopTimetableWindow::onTableViewClicked);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &routeStopTimetableWindow::handleComboBoxChange);
    if(this->editMode)                                                  // Обработчики событий нажатия на кнопки.
    {
        QPushButton* button = new QPushButton("Добавить");              // Создание кнопок.
        button->setObjectName("button1");
        button->setFixedHeight(30);                                     // Установка параметров текста.
        button->setEnabled(true);
        QFont font = button->font();
        font.setBold(true);
        font.setPixelSize(14);
        button->setFont(font);                                          // Обработчик нажатия.
        connect(button, &QPushButton::clicked, this, &routeStopTimetableWindow::onAddButtonClicked);
        ui->horizontalLayout->addWidget(button);                        // Добавление на экран.
        button = new QPushButton("Удалить");
        button->setObjectName("button3");
        button->setFixedHeight(30);
        button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
        font = button->font();
        font.setBold(true);
        font.setPixelSize(14);
        button->setFont(font);
        connect(button, &QPushButton::clicked, this, &routeStopTimetableWindow::onDeleteButtonClicked);
        ui->horizontalLayout->addWidget(button);
        ui->listView->setToolTip("Дважды нажмите на время, чтобы изменить автобус на рейсе");
    }
    connect(ui->tableView, &QTableView::doubleClicked, this, &routeStopTimetableWindow::onItemDoubleClicked);
    QTimer::singleShot(0, this, [&]()
    {
        Route* r1 = findRoute2(this->city, ui->label_2->text());   // Проверка, что маршрут работает.
        if(!(r1->daysOfWork[QDate::currentDate().dayOfWeek()-1]))  // Вывод сообщения на экран.
            QMessageBox::information(this, "Расписание городского транспорта", "Маршрут сегодня не работает\nВыберите другой день");
    });
}
routeStopTimetableWindow::~routeStopTimetableWindow()  // Деструктор.
{
    delete ui;
    stack.clear();
}
void routeStopTimetableWindow::onItemDoubleClicked(const QModelIndex &index) // Двойное нажатие на объект.
{
    if(index.column()==0 || !this->editMode)
        return;
    Node*n = findRoute2(this->city, ui->label_2->text())->first;    // Поиск маршрута.
    while(n!=nullptr && n->stopName!=ui->label->text())             // Поиск остановки.
        n = n->stopNext;
    if(n==nullptr)
    {
        QMessageBox::warning(this, "Изменение автобуса", "Остановка не найдена");
        return;
    }
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    QStandardItem* item = qm->itemFromIndex(index);                 // Получение данных.
    QStandardItem* item1 = qm->item(index.row(), 0);
    QString tm = item1->text() + item->text();
    bool week = ui->comboBox->currentText() == "Сб" || ui->comboBox->currentText() == "Вс";
    int i = 0;
    auto jt = (direct ? (week ?  n->timetableNextWeek.begin() : n->timetableNextWork.begin()) : (week ? n->timetablePrevWeek.begin(): n->timetablePrevWork.begin()));
    while(jt !=(direct ? (week ?  n->timetableNextWeek.end() : n->timetableNextWork.end()) : (week ? n->timetablePrevWeek.end(): n->timetablePrevWork.end()))
           && jt->time!= tm)                                        // Поиск рейса.
    {
        i++;
        jt++;
    }
    if(jt == (direct ? (week ?  n->timetableNextWeek.end() : n->timetableNextWork.end()) : (week ? n->timetablePrevWeek.end(): n->timetablePrevWork.end())))
    {
        QMessageBox::warning(this, "Изменение автобус", "Рейс не найден");
        return;
    }
    dialogChangeBusWindow* w = new dialogChangeBusWindow(this->city, &(*jt));
    w->show();                                    // Вызов окна изменения автобуса.
    if(w->exec() == QDialog::Accepted)
    {
        while(n!=nullptr)
        {                                         // Замена данных.
            auto kt = (direct ? (week ?  n->timetableNextWeek.begin() : n->timetableNextWork.begin()) : (week ? n->timetablePrevWeek.begin(): n->timetablePrevWork.begin()));
            for(int j = 0; j<i; j++, kt++);
            kt->bus = jt->bus;
            n = n->stopNext;
        }                                         // Обновление данных на экране.
        this->handleComboBoxChange(ui->comboBox->currentText());
    }
}
void routeStopTimetableWindow::handleComboBoxChange(const QString& currentText) // Изменение комбо бокса.
{
    int i = 0,j=1,n=0;
    QStandardItemModel* qm = new QStandardItemModel;
    QStandardItem* item;
    Stop* it = TreeAlgorithms<Stop>::findOne(Stop(ui->label->text()), this->city->getStops());
    if(it == nullptr)                                  // Поиск остановки.
    {
        QMessageBox::warning(this, "Редактирование", "Остановка не найдена");
        return;
    }
    Node* kt = it->stop;                               // Поиск маршрута на остновке.
    while(kt->routePrev!=nullptr)
        kt = kt->routePrev;
    while(kt->routeNext!=nullptr && kt->routeNumber!= ui->label_2->text())
        kt = kt->routeNext;
    if(kt->routeNumber!=ui->label_2->text())
    {
        QMessageBox::warning(this, "Редактирование", "Маршрут не найден");
        return;
    }                                                  // Поиск маршрута.
    Route* r = findRoute2(this->city, ui->label_2->text());
    QString temp = "";
    QFont font;
    int c = 0;
    bool work = currentText == "Пн" || currentText == "Вт" || currentText == "Ср" || currentText == "Чт" || currentText == "Пт";
    for(auto zt = (work ? (direct ? kt->timetableNextWork.begin() : kt->timetablePrevWork.begin()) : (direct ? kt->timetableNextWeek.begin() : kt->timetablePrevWeek.begin()));
        zt != (work ? (direct ? kt->timetableNextWork.end() : kt->timetablePrevWork.end()) : (direct ? kt->timetableNextWeek.end() : kt->timetablePrevWeek.end())); zt++, n++)
    {                                                  // Цикл по всем рейсам.
        QString timetable="\n";
        if(temp == "")
        {
            temp = (*zt).time.mid(0,2);
            QStandardItem* item = makeItem(temp + ":");// Добавление первого времени.
            qm->setItem(i, 0, item);
            qm->item(i, 0)->setFlags(qm->item(i, 0)->flags() & ~Qt::ItemIsEditable);
        }
        else if(temp!=(*zt).time.mid(0,2))
        {
            i++;
            j = 1;
            temp = (*zt).time.mid(0,2);
            QStandardItem* item = makeItem(temp + ":");
            qm->setItem(i, 0, item);
            qm->item(i, 0)->setFlags(qm->item(i, 0)->flags() & ~Qt::ItemIsEditable);
        }
        item = new QStandardItem((*zt).time.mid(3, 2));
        Node*at = direct ? r->first : r->last;
        while(1)                                      // Обход всех остановок рейса.
        {
            auto bt =(work ? (direct ? at->timetableNextWork.begin() : at->timetablePrevWork.begin()) : (direct ? at->timetableNextWeek.begin() : at->timetablePrevWeek.begin()));
            for(int i = 0; i<c;i++, bt++);            // Добавление данных.
            timetable += at->stopName + " " + (*bt).time;
            if(direct ? at->stopNext == nullptr : at->stopPrev == nullptr)
                break;
            else
                timetable += "\n";
            at = direct ? at->stopNext : at->stopPrev;
        }
        at = direct ? r->first : r->last;
        auto bt = (work ? (direct ? at->timetableNextWork.begin() : at->timetablePrevWork.begin()) : (direct ? at->timetableNextWeek.begin() : at->timetablePrevWeek.begin()));
        for(int i = 0; i<c;i++, bt++);               // Переход к нужному рейсу.
        QString labelText = "";
        if(bt->bus->hasApparel)                      // Считывание данных об автобусе.
            labelText += QString("<html><img src='vehicleHasApparel.png'></html>");
        if(bt->bus->hasConditioner)
            labelText += QString("<html><img src='vehicleHasConditioner.png'></html>");
        if(bt->bus->isLarge)
            labelText += QString("<html><img src='vehicleIncreasedCapacity.png'></html>");
        timetable.replace("\n", "<br>");
        labelText += timetable;
        item->setToolTip(labelText);                 // Установка подсказки.
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        font = item->font();
        font.setPointSize(11);
        item->setFont(font);
        qm->setItem(i, j++, item);                   // Установка объекта.
        c++;
    }
    ui->tableView->setModel(qm);
    this->handleTimerEvent();
}
QStandardItem* routeStopTimetableWindow::makeItem(const QString& text) // Создание объекта списка.
{
    QStandardItem* item = new QStandardItem(text);
    item->setEditable(false);                        // Установка внешнего вида.
    QFont font = item->font();
    font.setPointSize(11);
    item->setFont(font);
    return item;                                     // Возврат объекта.
}
void routeStopTimetableWindow::deleteButton(const QString& objname)    // Удаление кнопки.
{
    QPushButton *button = findChild<QPushButton*>(objname);   // Поиск объекта.
    delete button;                                            // Удаление кнопки.
}
void routeStopTimetableWindow::keyPressEvent(QKeyEvent *event)// Обработчик нажатия на клавишу.
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_E)
    {
        this->editMode = this->editMode == true ? false : true;// Включение или выключение режима.
        if(this->editMode)
        {
            QPushButton* button = new QPushButton("Добавить"); // Создание кнопки.
            button->setObjectName("button1");
            button->setFixedHeight(30);                        // Изменение внешнего вида.
            button->setEnabled(true);
            QFont font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);                             // Обработчик нажатия.
            connect(button, &QPushButton::clicked, this, &routeStopTimetableWindow::onAddButtonClicked);
            ui->horizontalLayout->addWidget(button);           // Добавление на экран.
            button = new QPushButton("Удалить");
            button->setObjectName("button3");
            button->setFixedHeight(30);
            button->setEnabled(!ui->listView->selectionModel()->selectedIndexes().empty());
            font = button->font();
            font.setBold(true);
            font.setPixelSize(14);
            button->setFont(font);
            connect(button, &QPushButton::clicked, this, &routeStopTimetableWindow::onDeleteButtonClicked);
            ui->horizontalLayout->addWidget(button);
            ui->listView->setToolTip("Дважды нажмите на время, чтобы изменить автобус на рейсе");
        }
        else
        {
            deleteButton("button1");                            // Удаление кнопки.
            deleteButton("button3");
            stack.clear();
            ui->listView->setToolTip("");
        }
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q)
    {
        close();                                                // Закрытие окна.
        routeStopsWindow* w = new routeStopsWindow(ui->label_2->text(), this->city, this->editMode);
        w->show();                                              // Открыть окно с остановками.
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z && editMode)
    {
        if(!stack.empty())                                      // Проверка, что стек не пуст.
        {
            this->city = new City(*(stack.pop()));              // Возврат данных.
            this->handleComboBoxChange(ui->comboBox->currentText());
        }
    }
}
void routeStopTimetableWindow::closeEvent(QCloseEvent *event)   // Закрытие окна.
{
    this->city->putInFileC();                                   // Сохранение данных.
}
void routeStopTimetableWindow::onTableViewClicked(const QModelIndex &index)
{                                                               // Нажатие на элемент списка.
    if(index.column() == 0)
        ui->tableView->clearSelection();
    else
    {                                                           // Включение кнопки.
        QPushButton *button3 = findChild<QPushButton*>("button3");
        if (index.isValid())
        {
            if(button3!=nullptr)
                button3->setEnabled(true);
        }
        else
        {
            if(button3!=nullptr)
                button3->setEnabled(false);
        }
    }
}
void routeStopTimetableWindow::onAddButtonClicked()             // Добавление времени.
{
    City* c = new City(*(this->city));
    Route* r = findRoute2(this->city, ui->label_2->text());     // Поиск маршрута.
    bool work, week;
    work = r->daysOfWork[0] || r->daysOfWork[1] || r->daysOfWork[2] || r->daysOfWork[3] || r->daysOfWork[4];
    week = r->daysOfWork[5] || r->daysOfWork[6];                // Вызов окна добавления времени.
    dialogTimeWindow* w = new dialogTimeWindow(this->city, ui->label_2->text(), work, week);
    w->show();
    if(w->exec() == QDialog::Accepted)                          // Время добавлено?
    {
        stack.push(c);                                          // Запись данных.
        this->handleComboBoxChange(ui->comboBox->currentText());
    }
}
void routeStopTimetableWindow::onDeleteButtonClicked()          // Удаление времени.
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    if (!selectedIndexes.isEmpty())                             // Получение данных.
    {
        City* c = new City(*(this->city));
        stack.push(c);
        QModelIndex selectedIndex = selectedIndexes.first();
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
        QStandardItem* item = model->itemFromIndex(selectedIndex);
        QModelIndex ind = model->index(selectedIndex.row(), 0);
        QStandardItem* item2 = model->itemFromIndex(ind);
        QString t = item2->text() + item->text();
        int m = 0;
        Route* r = findRoute2(this->city, ui->label_2->text());// Поиск маршрута.
        Node* n = r->first;
        while(n!=nullptr && n->stopName!=ui->label->text())
            n = n->stopNext;
        if(n==nullptr)                                         // Поиск остановки.
        {
            QMessageBox::warning(this, "Редактирование", "Остановка не найдена");
            return;
        }                                                      // Цикл по рейсам до нужного времени.
        bool week = ui->comboBox->currentText() == "Сб" || ui->comboBox->currentText() == "Вс";
        auto jt = (direct ? (week ? n->timetableNextWeek.begin() : n->timetableNextWork.begin()) :
                       (week ? n->timetablePrevWeek.begin() : n->timetablePrevWork.begin()));
        while(jt != (direct ? (week ? n->timetableNextWeek.end() : n->timetableNextWork.end()) :
                          (week ? n->timetablePrevWeek.end() : n->timetablePrevWork.end())) && jt->time != t)
        {
            m++;
            jt++;
        }
        if( jt ==  (direct ? (week ? n->timetableNextWeek.end() : n->timetableNextWork.end()) :
                       (week ? n->timetablePrevWeek.end() : n->timetablePrevWork.end())))
        {                         // Проверка, что рейс найден.
            QMessageBox::warning(this, "Редактирование", "Рейс не найден");
            return;
        }
        n = r->first;
        while(n!=nullptr)
        {                         // Удаление рейса.
            jt = (direct ? (week ? n->timetableNextWeek.begin() : n->timetableNextWork.begin()) :
                      (week ? n->timetablePrevWeek.begin() : n->timetablePrevWork.begin()));
            for(int b = 0; b<m; b++, jt++);
            (direct ? (week ? n->timetableNextWeek : n->timetableNextWork) : (week ? n->timetablePrevWeek : n->timetablePrevWork)).erase(jt);
            n = n->stopNext;
        }
        if(r->first->timetableNextWeek.empty() && r->first->timetableNextWork.empty() && r->first->timetablePrevWeek.empty() &&
            r->first->timetablePrevWork.empty())
        {
            auto nt = this->city->getRoutes().begin();
            while(nt!=this->city->getRoutes().end() && nt->first->routeNumber != ui->label_2->text())
                nt++;
            if(nt == this->city->getRoutes().end())
            {                     // Поиск маршрута.
                QMessageBox::warning(this, "Редактирование", "Маршрут не найден");
                return;
            }
            Node* mt = nt->first;
            while(mt!=nullptr)
            {                    // Удаление маршрута, если был удалён последний рейс.
                if(mt->routeNext!=nullptr)
                    mt->routeNext->routePrev = mt->routePrev;
                if(mt->routePrev!=nullptr)
                    mt->routePrev->routeNext = mt->routeNext;
                mt = mt->stopNext;
            }
            MyTree<Stop>::MyIterator tt = this->city->getStops().begin();
            while(!tt.isnull())
            {                   // Установка новых связей в структуре данных.
                if((*tt).stop->routeNumber == ui->label_2->text())
                {
                    if((*tt).stop->routeNext!=nullptr)
                        (*tt).stop->routeNext->routePrev = (*tt).stop->routePrev;
                    if((*tt).stop->routePrev!=nullptr)
                        (*tt).stop->routePrev->routeNext = (*tt).stop->routeNext;

                    if((*tt).stop->routeNext != nullptr)
                        (*tt).stop = (*tt).stop->routeNext;
                    else if((*tt).stop->routePrev!=nullptr)
                        (*tt).stop = (*tt).stop->routePrev;
                    else
                    {
                        (*tt).stop->routeNumber = "";
                        (*tt).stop->stopNext = nullptr;
                        (*tt).stop->stopPrev = nullptr;
                    }
                }
                tt++;
            }                     // Удаление маршрута.
            this->city->getRoutes().erase(nt);
            foreach(TransportPark park, this->city->getParks())
            {                     // Удаление из обслуживаемых маршрутов.
                auto kt = park.servedRoutes.begin();
                while(kt!=park.servedRoutes.end())
                {
                    if(*kt == ui->label_2->text())
                        park.servedRoutes.erase(kt);
                    else
                        kt++;
                }
            }
            close();              // Вызов окна маршрутов.
            routesWindow* w = new routesWindow(this->editMode, "", this->city);
            w->show();
        }
        else                      // Обновление данных.
            this->handleComboBoxChange(ui->comboBox->currentText());
    }
}
void routeStopTimetableWindow::handleTimerEvent() // Обработчик срабатывания таймера.
{
    QStandardItemModel* qqm = new QStandardItemModel();
    QStandardItem* item1 = new QStandardItem();
    Stop* it = TreeAlgorithms<Stop>::findOne(Stop(ui->label->text()), this->city->getStops());
    if(it == nullptr)                // Поиск остановки.
    {
        QMessageBox::warning(this, "Редактирование", "Остановка не найдена");
        return;
    }
    Node* tempn = it->stop;
    while(tempn->routePrev!=nullptr)
        tempn = tempn->routePrev;
    while(tempn!=nullptr)            // Обход всех маршрутов.
    {
        QString str = "";
        QTextStream help(&str);
        help.setPadChar(' ');  // Задаем символ для заполнения поля (пробел)
        help.setFieldAlignment(QTextStream::AlignCenter);
        help.setFieldWidth(5);
        if(tempn->routeNumber.left(7) == "Автобус")
            help << "А" + tempn->routeNumber.right(tempn->routeNumber.length() - 8);
        else if(tempn->routeNumber.left(7) == "Трамвай")
            help << "Тр" + tempn->routeNumber.right(tempn->routeNumber.length() - 8);
        else if(tempn->routeNumber.left(10) == "Троллейбус")
            help << "Т" + tempn->routeNumber.right(tempn->routeNumber.length() - 11);
        auto it = this->city->getRoutes().begin();
        while(it!=this->city->getRoutes().end() && it->first->routeNumber != tempn->routeNumber)
            it++;              // Запись номера и направления.
        help.setFieldWidth(32);
        help.setFieldAlignment(QTextStream::AlignLeft);
        if(direct)
            help << it->last->stopName;
        else
            help << it->first->stopName;
        if(it->daysOfWork[ui->comboBox->currentIndex()]) // Проверка, что работает в выбранный день.
        {
            QTime cur = QTime::currentTime();
            bool w = ui->comboBox->currentText() == "Пн" || ui->comboBox->currentText() == "Вт" || ui->comboBox->currentText() == "Ср"
                    || ui->comboBox->currentText() == "Чт" || ui->comboBox->currentText() == "Пт";
            for(auto ittt = w ? (direct ? tempn->timetableNextWork.begin() : tempn->timetablePrevWork.begin()) : (direct ? tempn->timetableNextWeek.begin() : tempn->timetablePrevWeek.begin());
                ittt != (w ? (direct ? tempn->timetableNextWork.end() : tempn->timetablePrevWork.end()) : (direct ? tempn->timetableNextWeek.end() : tempn->timetablePrevWeek.end()));
                ittt++)                                 // Обход всех рейсов остановки.
            {
                int h = ittt->time.left(2).toInt(), m = ittt->time.right(2).toInt();
                QTime tim(h, m);
                if(tim > cur)
                {
                    int re = cur.secsTo(tim);
                    if(re/60 <= 80)
                    {                                   // Поиск ближайшего времени прибытия в пределах 80 минут.
                        help.setFieldAlignment(QTextStream::AlignCenter);
                        help.setFieldWidth(15);
                        help << re/60;
                        ittt++;
                        if(ittt != (w ? (direct ? tempn->timetableNextWork.end() : tempn->timetablePrevWork.end()) : (direct ? tempn->timetableNextWeek.end() : tempn->timetablePrevWeek.end())))
                        {
                            QTime tm(ittt->time.left(2).toInt(), ittt->time.right(2).toInt());
                            re = cur.secsTo(tm);
                            if(re/ 60 <= 80)
                            {
                                help.setFieldAlignment(QTextStream::AlignCenter);
                                help.setFieldWidth(7);
                                help << re / 60;        // Запись данных.
                            }
                        }
                    }
                    break;
                }
            }
        }
        item1 = makeItem(str);
        qqm->appendRow(item1);                          // Добавление данных.
        tempn = tempn->routeNext;
    }
    ui->listView->setModel(qqm);
}
