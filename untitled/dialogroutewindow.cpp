#include "dialogroutewindow.h"
#include "ui_dialogroutewindow.h"

dialogRouteWindow::dialogRouteWindow(City* city, QDialog *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    city(city), ui(new Ui::dialogRouteWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                         // Внешний вид окна.
    setModal(true);
    setFocus();
    QRegularExpression regex("[а-яА-Я0-9]*$");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex);
    ui->lineEdit->setValidator(validator);
    this->route = new Route();
    ui->pushButton->setEnabled(false);         // Выключение кнопок.
    ui->pushButton->setToolTip("Введите номер маршрута и выберите дни работы");
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_4->setToolTip("Введите номер маршрута, добавьте остановки, выберите дни работы");
    ui->pushButton_6->setEnabled(false);
    QStandardItemModel* qm = new QStandardItemModel();
    ui->listView->setModel(qm);                // Создание модели данных списка.
    ui->listView->setStyleSheet("QListView::item { height: 30px; }");
    ui->comboBox->addItem("Автобус");          // Заполнение комбо бокса.
    ui->comboBox->addItem("Троллейбус");
    ui->comboBox->addItem("Трамвай");
    ui->comboBox->setStyleSheet("QListView::item { height: 30px; }");
    QObject::connect(ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {                                          // Активация, если список не пуст.
        ui->pushButton_6->setEnabled(ui->listView->model()->rowCount() > 0);
    });
    QObject::connect(ui->listView->model(), &QAbstractItemModel::rowsInserted, [&]()
    {
        if(ui->listView->model()->rowCount() > 0 && !ui->lineEdit->text().isEmpty() && (ui->checkBox->isChecked() || ui->checkBox_2->isChecked() || ui->checkBox_3->isChecked() || ui->checkBox_4->isChecked()
                                                                                         || ui->checkBox_5->isChecked() || ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked()))
        {
            ui->pushButton_4->setEnabled(true);
            ui->pushButton_4->setToolTip("");
        }
        else
        {
            QString res;
            if(ui->listView->model()->rowCount() == 0)
                res += "Добавьте остановки. ";
            if(ui->lineEdit->text().isEmpty())
                res += "Введите номер маршрута. ";
            if(!(ui->checkBox->isChecked() || ui->checkBox_2->isChecked() || ui->checkBox_3->isChecked() || ui->checkBox_4->isChecked()
                  || ui->checkBox_5->isChecked() || ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked()))
                res += "Выберите дни работы.";
            ui->pushButton_4->setEnabled(false);
            ui->pushButton_4->setToolTip(res);
        }
    });
    QObject::connect(ui->listView->model(), &QAbstractItemModel::rowsRemoved, [&]()
    {
        if(ui->listView->model()->rowCount() > 0 && !ui->lineEdit->text().isEmpty() && (ui->checkBox->isChecked() || ui->checkBox_2->isChecked() || ui->checkBox_3->isChecked() || ui->checkBox_4->isChecked()
                                                                                         || ui->checkBox_5->isChecked() || ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked()))
        {
            ui->pushButton_4->setEnabled(true);
            ui->pushButton_4->setToolTip("");
        }
        else
        {
            QString res;
            if(ui->listView->model()->rowCount() == 0)
                res += "Добавьте остановки. ";
            if(ui->lineEdit->text().isEmpty())
                res += "Введите номер маршрута. ";
            if(!(ui->checkBox->isChecked() || ui->checkBox_2->isChecked() || ui->checkBox_3->isChecked() || ui->checkBox_4->isChecked()
                  || ui->checkBox_5->isChecked() || ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked()))
                res += "Выберите дни работы.";
            ui->pushButton_4->setEnabled(false);
            ui->pushButton_4->setToolTip(res);
        }
    });
    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, [=]()
    {
        this->onCheckBoxClicked();
    });                                        // Обработчики нажатия кнопок.
    connect(ui->pushButton_4, &QPushButton::clicked, this, &dialogRouteWindow::acceptClicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &dialogRouteWindow::reject);
    connect(ui->pushButton, &QPushButton::clicked, this, &dialogRouteWindow::addStopClicked);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &dialogRouteWindow::deleteStopClicked);
    connect(ui->checkBox, &QCheckBox::clicked, this, &dialogRouteWindow::onCheckBoxClicked);
    connect(ui->checkBox_2, &QCheckBox::clicked, this, &dialogRouteWindow::onCheckBoxClicked);
    connect(ui->checkBox_3, &QCheckBox::clicked, this, &dialogRouteWindow::onCheckBoxClicked);
    connect(ui->checkBox_4, &QCheckBox::clicked, this, &dialogRouteWindow::onCheckBoxClicked);
    connect(ui->checkBox_5, &QCheckBox::clicked, this, &dialogRouteWindow::onCheckBoxClicked);
    connect(ui->checkBox_6, &QCheckBox::clicked, this, &dialogRouteWindow::onCheckBoxClicked);
    connect(ui->checkBox_7, &QCheckBox::clicked, this, &dialogRouteWindow::onCheckBoxClicked);

}
dialogRouteWindow::~dialogRouteWindow() // Деструктор.
{
    delete ui;
    stack1.clear();
    stack2.clear();
}
void dialogRouteWindow::onCheckBoxClicked()
{
    if(!ui->lineEdit->text().isEmpty() && (ui->checkBox->isChecked() || ui->checkBox_2->isChecked() || ui->checkBox_3->isChecked() || ui->checkBox_4->isChecked()
                                            || ui->checkBox_5->isChecked() || ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked()))
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton->setToolTip("");
    }
    else
    {
        QString res;
        if(ui->lineEdit->text().isEmpty())
            res += "Введите номер маршрута. ";
        if(!(ui->checkBox->isChecked() || ui->checkBox_2->isChecked() || ui->checkBox_3->isChecked() || ui->checkBox_4->isChecked()
              || ui->checkBox_5->isChecked() || ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked()))
            res += "Выберите дни работы";
        ui->pushButton->setEnabled(false);
        ui->pushButton->setToolTip(res);
    }
    if(ui->listView->model()->rowCount() > 0 && !ui->lineEdit->text().isEmpty())
    {
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_4->setToolTip("");
    }
    else
    {
        QString res;
        if(ui->listView->model()->rowCount() == 0)
            res += "Добавьте остановки. ";
        if(ui->lineEdit->text().isEmpty())
            res += "Введите номер маршрута.";
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_4->setToolTip(res);
    }
}
void dialogRouteWindow::moveItemUp(const QModelIndex& index) // Элемент вверх.
{
    if(index.row()>0)
    {
        Route* r = new Route(*(this->route));
        City* c = new City(*(this->city));
        stack1.push(r);
        stack2.push(c);                                      // Сохранение данных.
        QStandardItemModel* itemmodel = qobject_cast<QStandardItemModel*>(ui->listView->model());
        moveUp(itemmodel, index.row(), this->route);         // Перемещение.
    }
}
void dialogRouteWindow::moveItemDown(const QModelIndex& index) // Элемент вниз.
{
    QStandardItemModel* itemmodel = qobject_cast<QStandardItemModel*>(ui->listView->model());
    if(index.row() < itemmodel->rowCount() - 1)
    {
        Route* r = new Route(*(this->route));
        City* c = new City(*(this->city));
        stack1.push(r);
        stack2.push(c);                                       // Сохранение данных.
        moveDown(itemmodel, index.row(), this->route);        // Элемент вниз.
    }
}
void dialogRouteWindow::keyPressEvent(QKeyEvent *event)       // Обработчик нажатия клавиш.
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W)
    {
        QItemSelectionModel* selectionModel = ui->listView->selectionModel();
        QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
        if (!selectedIndexes.isEmpty())
            moveItemUp(ui->listView->currentIndex());         // Перемещение элемента вверх.
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
    {
        QItemSelectionModel* selectionModel = ui->listView->selectionModel();
        QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
        if (!selectedIndexes.isEmpty())
            moveItemDown(ui->listView->currentIndex());       // Перемещение элемента вниз.
    }
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z)
    {
        if(!stack1.empty())
        {
            this->city = new City(*(stack2.pop()));
            this->route = new Route(*(stack1.pop()));         // Достать данные из стека.
            if(this->route->first!=nullptr)
            {
                if(this->route->first->routeNumber.left(7) == "Автобус")
                    ui->comboBox->setCurrentIndex(0);
                else if(this->route->first->routeNumber.left(7) == "Трамвай")
                    ui->comboBox->setCurrentIndex(2);
                else
                    ui->comboBox->setCurrentIndex(1);
            }                                                 // Установка данных.
            ui->checkBox->setChecked(this->route->daysOfWork[0]);
            ui->checkBox_2->setChecked(this->route->daysOfWork[1]);
            ui->checkBox_2->setChecked(this->route->daysOfWork[2]);
            ui->checkBox_3->setChecked(this->route->daysOfWork[3]);
            ui->checkBox_4->setChecked(this->route->daysOfWork[4]);
            ui->checkBox_6->setChecked(this->route->daysOfWork[5]);
            ui->checkBox_7->setChecked(this->route->daysOfWork[6]);
            QStandardItemModel* qm = new QStandardItemModel();
            ui->listView->setModel(qm);
            Node* n = this->route->first;
            while(n!=nullptr)
            {
                addStop(n->stopName);                         // Добавление данных в список.
                n = n->stopNext;
            }
        }
    }
}
void dialogRouteWindow::acceptClicked()                       // Нажата кнопка Ок.
{
    foreach(Route r, this->city->getRoutes())                 // Проверка на дублирование.
        if(r.first->routeNumber == this->route->first->routeNumber)
        {
            QMessageBox::warning(this, "Добавление маршрута", "Маршрут уже существует");
            return;
        }
    this->route->daysOfWork[0] = ui->checkBox->isChecked();   // Сохранение данных.
    this->route->daysOfWork[1] = ui->checkBox_2->isChecked();
    this->route->daysOfWork[2] = ui->checkBox_3->isChecked();
    this->route->daysOfWork[3] = ui->checkBox_4->isChecked();
    this->route->daysOfWork[4] = ui->checkBox_5->isChecked();
    this->route->daysOfWork[5] = ui->checkBox_6->isChecked();
    this->route->daysOfWork[6] = ui->checkBox_7->isChecked();
    Node* n = this->route->first;
    while(n!=nullptr)
    {
        Stop* it = TreeAlgorithms<Stop>::findOne(Stop(n->stopName), this->city->getStops());
        if(it == nullptr)
        {
            QMessageBox::warning(this, "Изменение маршрута", "Остановка не найдена");
            return;
        }
        if(it->stop->routeNumber == "")                       // Добавление данных.
            it->stop = n;
        else
        {
            Node* tn = it->stop;
            while(tn->routeNext!=nullptr)
                tn = tn->routeNext;
            tn->routeNext = n;
            n->routePrev = tn;
        }
        n = n->stopNext;
    }
    this->city->getRoutes().push_back(*(this->route));
    this->city->getRoutes().sort();                          // Добавление в обслуживаемые маршруты.
    if(!this->city->getRoutes().back().first->timetableNextWork.empty())
    {
        foreach(TimeBus t, this->city->getRoutes().back().first->timetableNextWork)
            addBusContain(t.bus->number);
    }
    if(!this->city->getRoutes().back().first->timetablePrevWork.empty())
    {
        foreach(TimeBus t, this->city->getRoutes().back().first->timetablePrevWork)
            addBusContain(t.bus->number);
    }
    if(!this->city->getRoutes().back().first->timetableNextWeek.empty())
    {
        foreach(TimeBus t, this->city->getRoutes().back().first->timetableNextWeek)
            addBusContain(t.bus->number);
    }
    if(!this->city->getRoutes().back().first->timetablePrevWeek.empty())
    {
        foreach(TimeBus t, this->city->getRoutes().back().first->timetablePrevWeek)
            addBusContain(t.bus->number);
    }
    accept();
}
void dialogRouteWindow::addBusContain(const QString& num)  // Добавление в обслуживаемые маршруты.
{
    auto p = this->city->getParks().begin();
    while(p!=this->city->getParks().end())                 // Цикл по паркам.
    {
        Bus* b = TreeAlgorithms<Bus>::findOne(Bus(num), p->buses);
        if(b!=nullptr)                                     // Если автобус на маршруте, то добаивть.
        {
            if(!p->servedRoutes.contains(this->city->getRoutes().back().first->routeNumber))
                p->servedRoutes.push_back(this->city->getRoutes().back().first->routeNumber);
            break;
        }
        p++;
    }
}
void dialogRouteWindow::addStopClicked()                   // Добавление остановки.
{
    Route* r = new Route(*(this->route));
    City* c = new City(*(this->city));
    bool work, week;
    work = ui->checkBox->isChecked() || ui->checkBox_2->isChecked() || ui->checkBox_3->isChecked() || ui->checkBox_4->isChecked() || ui->checkBox_5->isChecked();
    week = ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked();
    dialogStopWindow* w = new dialogStopWindow(this->city, work, week, this->route);
    w->show();                                             // Диалоговое окно добавления остановки.
    if(w->exec() == QDialog::Accepted)                     // Добавлена?
    {
        stack1.push(r);
        stack2.push(c);
        if(route->first->routeNumber=="")                  // Добавление данных.
            route->first->routeNumber = ui->comboBox->currentText() + " " + ui->lineEdit->text();
        addStop(this->route->last->stopName);
    }
}
void dialogRouteWindow::addStop(const QString& stopname)   // Добавление остановки в список.
{
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QStandardItem* item = new QStandardItem(stopname);     // Создание объекта модели данных.
    item->setEditable(false);
    item->setTextAlignment(Qt::AlignVCenter);              // Установка внешнего вида.
    QFont font = item->font();
    font.setPointSize(11);
    item->setFont(font);
    qm->appendRow(item);                                   // Добавление в список.
}
void dialogRouteWindow::deleteStopClicked()                // Удаление остановки.
{
    Route* r = new Route(*(this->route));
    City* c = new City(*(this->city));
    stack1.push(r);
    stack2.push(c);
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QModelIndex cur = ui->listView->currentIndex();
    QStandardItem *item = model->itemFromIndex(cur);       // Получение данных.
    model->removeRow(ui->listView->currentIndex().row());
    if(model->rowCount()==0)
        ui->pushButton_6->setEnabled(false);
    Node* temp = this->route->first;
    while(temp->stopPrev!=nullptr)                         // Удаление из маршрута.
        temp = temp->stopPrev;
    while(temp->stopNext!= nullptr && temp->stopName!=item->text())
        temp = temp->stopNext;
    if(temp == this->route->first)
        this->route->first = this->route->first->stopNext;
    else if(temp == this->route->last)                     // Перемещение указателей
        this->route->last = this->route->last->stopPrev;   // первой или последней
    if(temp->stopNext!=nullptr)                            // остановки.
        temp->stopNext->stopPrev = temp->stopPrev;
    if(temp->stopPrev!=nullptr)
        temp->stopPrev->stopNext = temp->stopNext;
}
