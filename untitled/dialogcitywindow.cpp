#include "dialogcitywindow.h"
#include "ui_dialogcitywindow.h"

dialogCityWindow::dialogCityWindow(QStringList* cities, City* city,QDialog* parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    cities(cities), city(city), ui(new Ui::dialogCityWindow), editMode(false)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                             // Установка внешнего вида окна.
    setModal(true);
    setFocus();
    ui->pushButton_7->setEnabled(false);           // Выключение кнопок.
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    QRegularExpression regex("[а-яА-Я- ]*$");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex);
    ui->lineEdit->setValidator(validator);
    if(city==nullptr)
    {
        ui->pushButton->setEnabled(false);         // Выключение кнопок.
        ui->pushButton->setToolTip("Введите название города");
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_5->setToolTip("Добавьте транспортные парки");
    }
    QStandardItemModel* qm = new QStandardItemModel();
    ui->listView->setModel(qm);                    // Создание модели данных для списка.
    ui->listView->setStyleSheet("QListView::item { height: 30px; }");
    if(city!=nullptr)                              // Установка внешнего вида для списка.
    {
        this->editMode = true;                     // Заполнение списка автобусами.
        for(auto at = this->city->getParks().begin(); at!=this->city->getParks().end(); at++)
            addIn(1, at->parkName);
    }
    qm = new QStandardItemModel();
    ui->listView_2->setModel(qm);
    ui->listView_2->setStyleSheet("QListView::item { height: 30px; }");
    if(city!=nullptr)
    {
        for(MyTree<Stop>::MyIterator at = this->city->getStops().begin(); !at.isnull(); at++)
            addIn(2, (*at).stop->stopName);        // Заполнение списка остановками.
    }
    qm = new QStandardItemModel();
    ui->listView_3->setModel(qm);
        ui->listView_3->setStyleSheet("QListView::item { height: 30px; }");
    if(city!=nullptr)
    {
        for(auto at = this->city->getRoutes().begin(); at!=this->city->getRoutes().end(); at++)
            addIn(3, at->first->routeNumber);      // Заполнение списка маршрутами.
    }
    if(city!=nullptr)                              // Установка имени города.
        ui->lineEdit->setText(this->city->getName());           // Обработчики нажатия кнопок.
    connect(ui->pushButton, &QPushButton::clicked, this, &dialogCityWindow::acceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &dialogCityWindow::reject);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &dialogCityWindow::addStopClicked);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &dialogCityWindow::deleteStopClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &dialogCityWindow::addParkClicked);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &dialogCityWindow::deleteParkClicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &dialogCityWindow::addRouteClicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &dialogCityWindow::deleteRouteClicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &dialogCityWindow::addMapClicked);
    QObject::connect(ui->listView_2->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {                 // Активация кнопки, если есть элементы в списке.
        ui->pushButton_7->setEnabled(ui->listView_2->model()->rowCount() > 0);
    });
    QObject::connect(ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {                 // Активация кнопки, если есть элементы в списке.
        ui->pushButton_6->setEnabled(ui->listView->model()->rowCount() > 0);
    });
    QObject::connect(ui->listView_3->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {                 // Активация кнопки, если есть элементы в списке.
        ui->pushButton_8->setEnabled(ui->listView_3->model()->rowCount() > 0);
    });
    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, [=]()
    {                 // Активация кнопки, если строка не пуста.
        if(!ui->lineEdit->text().isEmpty() && ui->listView->model()->rowCount() > 0)
        {
            ui->pushButton->setEnabled(true);
            ui->pushButton->setToolTip("");
        }
        else
        {
            ui->pushButton->setToolTip("Введите название города");
            ui->pushButton->setEnabled(false);
        }
    });
    connect(ui->listView, &QListView::doubleClicked, this, &dialogCityWindow::onItem1DoubleClicked);
    connect(ui->listView_2, &QListView::doubleClicked, this, &dialogCityWindow::onItem2DoubleClicked);
}
dialogCityWindow::~dialogCityWindow()  // Деструктор.
{
    delete ui;
    stack.clear();
}
void dialogCityWindow::addMapClicked() // Добавление карты.
{
    if(this->city == nullptr)
        this->city = new City();
    QString path = QFileDialog::getOpenFileName(this, "Выберите изображение с картой города", QDir::homePath(),
         "Файлы изображения (*.png)", nullptr, QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly);
    if(path.isEmpty())                                // Диалоговое окно для выбора файла.
        this->city->setPath("defaultCity.png");       // Файл по умолчанию.
    else
        this->city->setPath(path);                    // Выбранный файл.
}
void dialogCityWindow::onItem1DoubleClicked(const QModelIndex &index) // Двойное нажатие на элемент списка.
{
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QStandardItem* item = qm->itemFromIndex(index);                   // Получение выбранного элемента списка.
    auto it = this->city->getParks().begin();
    while(it!=this->city->getParks().end() && it->parkName!=item->text())
        it++;                                                         // Поиска парка.
    if(it == this->city->getParks().end())
    {
        QMessageBox::warning(this, "Изменение города", "Парк не найден");
        return;
    }
    City* city = new City(*(this->city));                             // Создание копии.
    dialogParkWindow* w = new dialogParkWindow(this->city, &(*it));   // Вызов окна редактирования парка.
    w->show();
    if(w->exec() == QDialog::Accepted)                                // Изменение проведено?
    {
        stack.push(city);
        item->setText(it->parkName);                                  // Изменение данных в списке.
    }
}
void dialogCityWindow::onItem2DoubleClicked(const QModelIndex &index) // Двойное нажатие на список.
{
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView_2->model());
    QStandardItem* item = qm->itemFromIndex(index);                   // Получение выбранного элемента списка.
    Stop* it = TreeAlgorithms<Stop>::findOne(Stop(item->text()), this->city->getStops());
    if(it == nullptr)                                                 // Поиск остановки.
    {
        QMessageBox::warning(this, "Изменение города", "Остановка не найдена");
        return;
    }
    City* city = new City(*(this->city));             // Вызов диалогового окна изменния остановки.
    dialogStopWindow* w = new dialogStopWindow(this->city, false,false,nullptr,it);
    w->show();
    if(w->exec()==QDialog::Accepted)                  // Изменение проведено?
    {
        stack.push(city);
        item->setText(it->stop->stopName);            // Ищменение данных в списке.
        Node* n = it->stop;
        while(n->routePrev!=nullptr)
            n = n->routePrev;
        while(n!=nullptr)                             // Изменение данных.
        {
            n->stopName = it->stop->stopName;
            n = n->routeNext;
        }
    }
}
void dialogCityWindow::acceptClicked()                // Нажатие клавиши Ок.
{
    if(!editMode)                                     // Проверка на добавление существующего города.
        if(this->cities->contains(ui->lineEdit->text()))
        {
            QMessageBox::warning(this, "Добавление города", "Город уже существует");
            return;
        }
    if(this->cities->contains(this->city->getName()) && this->city->getName()!=ui->lineEdit->text())
    {                                                // Удаление старых данных.
        this->cities->removeAll(this->city->getName());
        removeFile(this->city->getName() + "Маршруты.txt");
        removeFile(this->city->getName() + "Остановки.txt");
        removeFile(this->city->getName() + "Парки.txt");
        removeFile(this->city->getName() + "Карта.txt");
    }                                                // Добавление новых данных.
    if(!this->cities->contains(ui->lineEdit->text()))
        this->cities->push_back(ui->lineEdit->text());
    this->city->setName(ui->lineEdit->text());
    if(this->city->getPath().isEmpty())
        this->city->setPath("defaultCity.png");
    createFile(ui->lineEdit->text() + "Маршруты.txt");
    createFile(ui->lineEdit->text() + "Остановки.txt");
    createFile(ui->lineEdit->text() + "Парки.txt");
    createFile(ui->lineEdit->text() + "Карта.txt");
    this->city->putInFileC();                       // Запись в файл.
    accept();
}
void dialogCityWindow::addStopClicked()             // Добавление остановки.
{
    if(this->city == nullptr)
        this->city = new City();
    City* city = new City(*(this->city));           // Вызов диалогового окна
    dialogStopWindow* w = new dialogStopWindow(this->city, false, false);
    w->show();                                      // добавления остановки.
    if(w->exec() == QDialog::Accepted)              // Добавлена?
    {
        stack.push(city);                           // Добавление данных.
        addIn(2, (*(this->city->getStops().back())).stop->stopName);
    }
}
void dialogCityWindow::addIn(int i, const QString& text) // Добавление данных.
{
    QStandardItemModel* qm;
    if(i==1)                                        // Получение модели данных.
        qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    else if(i==2)
        qm = qobject_cast<QStandardItemModel*>(ui->listView_2->model());
    else
        qm = qobject_cast<QStandardItemModel*>(ui->listView_3->model());
    QStandardItem* item = new QStandardItem(text);  // Создание объекта.
    item->setEditable(false);                       // Установка параметров.
    item->setTextAlignment(Qt::AlignVCenter);
    QFont font = item->font();
    font.setPointSize(11);
    item->setFont(font);
    qm->appendRow(item);                            // Добавление объекта.
}
void dialogCityWindow::deleteStopClicked()          // Удаление остановки.
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->listView_2->model());
    Stop* it = TreeAlgorithms<Stop>::findOne(Stop(ui->listView_2->currentIndex().data().toString()), this->city->getStops());
    if(it == nullptr)                               // Поиск остановки.
    {
        QMessageBox::warning(this, "Изменение города", "Остановка не найдена");
        return;
    }
    City* city = new City(*(this->city));
    stack.push(city);
    Node*n = it->stop;
    while(n->routePrev!=nullptr)
        n = n->routePrev;
    while(n!=nullptr)                              // Переустановка связей в данных.
    {
        if(n->stopNext!=nullptr)
            n->stopNext->stopPrev = n->stopPrev;
        if(n->stopPrev!=nullptr)
            n->stopPrev->stopNext = n->stopNext;
        n = n->routeNext;
    }
    auto jt = this->city->getRoutes().begin();
    while(jt!=this->city->getRoutes().end())
    {                                              // Удаление из маршрутов.
        if(jt->first->stopName == it->stop->stopName)
            jt->first = jt->first->stopNext;
        if(jt->last->stopName == it->stop->stopName)
            jt->last = jt->last->stopPrev;
        if(jt->first == nullptr || jt->last == nullptr)
            this->city->getRoutes().erase(jt);
        else
            jt++;
    }                                              // Удаление из списков.
    model->removeRow(ui->listView_2->currentIndex().row());
    this->city->getStops().Remove(*it);
    if(model->rowCount() == 0)
        ui->pushButton_7->setEnabled(false);
}
void dialogCityWindow::addParkClicked()            // Добавление парка.
{
    if(this->city == nullptr)
        this->city = new City();
    City* city = new City(*(this->city));          // Диалоговое окно добавления парка.
    dialogParkWindow* w = new dialogParkWindow(this->city);
    w->show();
    if(w->exec() == QDialog::Accepted)             // Парк добавлен?
    {
        stack.push(city);
        addIn(1, this->city->getParks().back().parkName);  // Добавление парка в списки.
        ui->pushButton->setEnabled(!ui->lineEdit->text().isEmpty() && ui->listView->model()->rowCount() > 0);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_5->setToolTip("");
    }
}
void dialogCityWindow::deleteParkClicked()         // Удаление парка.
{
    City* city = new City(*(this->city));
    stack.push(city);                              // Получение выбранного элемента.
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->listView->model());
    auto it = city->getParks().begin();
    for(int i = 0; i<ui->listView->currentIndex().row(); i++)
        it++;
    MyTree<Bus>::MyIterator b = it->buses.begin();
    while(!b.isnull())
    {                                              // Проверка, что автобус не работает на маршруте.
        if(checkRouteBuses(this->city, (*b).number))
            return;
        b++;
    }                                              // Удаление из списков.
    model->removeRow(ui->listView->currentIndex().row());
    this->city->getParks().erase(it);
    if(model->rowCount() == 0)
        ui->pushButton_6->setEnabled(false);       // Установка состояний кнопок.
    ui->pushButton->setEnabled(!ui->lineEdit->text().isEmpty() && ui->listView->model()->rowCount() > 0);
    if(ui->listView->model()->rowCount() > 0)
    {
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_5->setToolTip("");
    }
    else
    {
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_5->setToolTip("Добавьте транспортные парки");
    }
}
void dialogCityWindow::addRouteClicked()           // Добавление маршрута.
{
    if(this->city == nullptr)
        this->city = new City();
    City* city = new City(*(this->city));          // Диалоговое окно добавления маршрута.
    dialogRouteWindow* w = new dialogRouteWindow(this->city);
    w->show();
    if(w->exec() == QDialog::Accepted)             // Маршрут добавлен?
    {
        stack.push(city);                          // Добавление города.
        addIn(3, this->city->getRoutes().back().first->routeNumber);
        QStandardItemModel* qm = new QStandardItemModel();
        ui->listView_2->setModel(qm);
        MyTree<Stop>::MyIterator s = this->city->getStops().begin();
        while(!s.isnull())
        {
            addIn(2, (*s).stop->stopName);         // Добавление в модель данных.
            s++;
        }
    }
}
void dialogCityWindow::keyPressEvent(QKeyEvent *event) // Обработчик нажатия клавиш.
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z)
    {
        if(!stack.isEmpty())                           // Стек не пуст?
        {
            this->city = new City(*(this->stack.pop())); // Достать город.
            QStandardItemModel* qm = new QStandardItemModel();
            ui->listView->setModel(qm);
            foreach(TransportPark p, this->city->getParks())
            {
                addIn(1, p.parkName);                  // Запись парков.
            }
            qm = new QStandardItemModel();
            ui->listView_2->setModel(qm);
            MyTree<Stop>::MyIterator s = this->city->getStops().begin();
            while(!s.isnull())
            {
                addIn(2, (*s).stop->stopName);         // Запись остановок.
                s++;
            }
            qm = new QStandardItemModel();
            ui->listView_3->setModel(qm);
            foreach(Route r, this->city->getRoutes())
            {
                addIn(3, r.first->routeNumber);        // Запись маршрутов.
            }
        }
    }
}
void dialogCityWindow::deleteRouteClicked()            // Удаление маршрута.
{
    City* city = new City(*(this->city));
    stack.push(city);
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->listView_3->model());
    QModelIndex cur = ui->listView_3->currentIndex();  // Получение выбранного элемента.
    QStandardItem* item = model->itemFromIndex(cur);
    auto nt = this->city->getRoutes().begin();
    while(nt!=this->city->getRoutes().end() && nt->first->routeNumber != item->text())
        nt++;
    if(nt == this->city->getRoutes().end())            // Поиск маршрута.
    {
        QMessageBox::warning(this, "Изменение города", "Маршрут не найден");
        return;
    }
    Node* mt = nt->first;
    while(mt!=nullptr)
    {
        if(mt->routeNext!=nullptr)                     // Удаление остановок маршрута.
            mt->routeNext->routePrev = mt->routePrev;
        if(mt->routePrev!=nullptr)
            mt->routePrev->routeNext = mt->routeNext;
        mt = mt->stopNext;
    }
    MyTree<Stop>::MyIterator tt = this->city->getStops().begin();
    while(!tt.isnull())
    {
        if((*tt).stop->routeNumber == item->text())    // Поиск остановки маршрута.
        {
            if((*tt).stop->routeNext!=nullptr)         // Удаление из маршрута.
                (*tt).stop->routeNext->routePrev = (*tt).stop->routePrev;
            if((*tt).stop->routePrev!=nullptr)
                (*tt).stop->routePrev->routeNext = (*tt).stop->routeNext;
            if((*tt).stop->routeNext != nullptr)       // Смещение первой и последней остановки.
                (*tt).stop = (*tt).stop->routeNext;
            else if((*tt).stop->routePrev != nullptr)
                (*tt).stop = (*tt).stop->routePrev;
            else
            {
                (*tt).stop->routePrev = nullptr;       // Удаление данных об остановке, если это был
                (*tt).stop->routeNext = nullptr;       // единственный на ней маршрут.
                (*tt).stop->stopNext = nullptr;
                (*tt).stop->stopPrev = nullptr;
                (*tt).stop->routeNumber = "";
                (*tt).stop->timetableNextWeek.clear();
                (*tt).stop->timetableNextWork.clear();
                (*tt).stop->timetablePrevWeek.clear();
                (*tt).stop->timetablePrevWork.clear();
            }
        }
        tt++;
    }
    foreach(TransportPark park, this->city->getParks()) // Удаление маршрута из парка.
    {
        auto kt = park.servedRoutes.begin();
        while(kt!=park.servedRoutes.end())
        {
            if(*kt == item->text())
                park.servedRoutes.erase(kt);
            else
                kt++;
        }
    }
    this->city->getRoutes().erase(nt);                  // Удаление маршрута.
    model->removeRow(cur.row());
    if(model->rowCount() == 0)
        ui->pushButton_8->setEnabled(false);
}
