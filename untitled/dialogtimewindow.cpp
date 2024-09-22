#include "dialogtimewindow.h"
#include "ui_dialogtimewindow.h"

dialogTimeWindow::dialogTimeWindow(City* city, const QString& route, bool work, bool week, QDialog *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    city(city), route(route), ui(new Ui::dialogTimeWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                                                       // Установка внешнего вида.
    setModal(true);
    setFocus();
    Node* n = findRoute2(this->city, route)->first;                          // Поиск маршрута.
    QStandardItemModel* qm = new QStandardItemModel();
    while(n!=nullptr)
    {
        qm->appendRow(makeItem(n->stopName));                                // Добавление остановок.
        n = n->stopNext;
    }
    ui->listView->setModel(qm);
    ui->listView->setStyleSheet("QListView::item { height: 30px; }");
    qm = new QStandardItemModel();                                           // Создание списков.
    ui->listView_2->setModel(qm);
    ui->listView_2->setStyleSheet("QListView::item { height: 30px; }");      // Установка их внешнего вида.
    qm = new QStandardItemModel();
    ui->listView_3->setModel(qm);
    ui->listView_3->setStyleSheet("QListView::item { height: 30px; }");
    qm = new QStandardItemModel();
    ui->listView_4->setModel(qm);
    ui->listView_4->setStyleSheet("QListView::item { height: 30px; }");
    qm = new QStandardItemModel();
    ui->listView_5->setModel(qm);
    ui->listView_5->setStyleSheet("QListView::item { height: 30px; }");
    for(auto xt = this->city->getParks().begin(); xt!=this->city->getParks().end(); xt++)
    {
        MyTree<Bus>::MyIterator yt = xt->buses.begin();
        while(!yt.isnull())
        {
            ui->comboBox->addItem((*yt).number);                             // Добавление автобусов в комбо боксы.
            ui->comboBox_2->addItem((*yt).number);
            ui->comboBox_3->addItem((*yt).number);
            ui->comboBox_4->addItem((*yt).number);
            yt++;
        }
    }
    ui->pushButton->setEnabled(false);                                       // Выключение кнопок.
    ui->pushButton->setToolTip("Заполните данные, должен быть введён хотя бы один рейс");
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    if(!work)
    {
        ui->listView_2->setEnabled(false);                                   // Выключение списков и строк ввода
        ui->listView_5->setEnabled(false);                                   // в рабочие дни.
        ui->comboBox->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
    }
    else
    {
        ui->listView_2->setToolTip("Рабочий день, прямое направление");
        ui->listView_5->setToolTip("Рабочий день, обратное направление");
        ui->comboBox->setToolTip("Выберите автобус для рейса");
        ui->comboBox_3->setToolTip("Выберите автобус для рейса");
    }
    if(!week)
    {
        ui->listView_3->setEnabled(false);                                   // в выходные дни.
        ui->listView_4->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_4->setEnabled(false);
    }
    else
    {
        ui->listView_3->setToolTip("Выходной день, прямое направление");
        ui->listView_4->setToolTip("Выходной день, обратное направление");
        ui->comboBox->setToolTip("Выберите автобус для рейса");
        ui->comboBox_3->setToolTip("Выберите автобус для рейса");
    }
    QRegularExpression timeRegex("^(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$");     // Регулярное выражение для проверки ввода.
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(timeRegex);
    ui->lineEdit->setValidator(validator);                                   // Установка валидатора для проверки ввода.
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &dialogTimeWindow::onTextChanged);
    connect(ui->listView_2->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {                                   // Включение кнопки при выборе объекта.
        ui->pushButton_4->setEnabled(ui->listView_2->model()->rowCount() > 0);
    });
    connect(ui->listView_3->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {
        ui->pushButton_6->setEnabled(ui->listView_3->model()->rowCount() > 0);
    });
    connect(ui->listView_4->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {
        ui->pushButton_10->setEnabled(ui->listView_4->model()->rowCount() > 0);
    });                                  // Обработчики собтиый кнопок.
    connect(ui->listView_5->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {
        ui->pushButton_8->setEnabled(ui->listView_5->model()->rowCount() > 0);
    });
    connect(ui->pushButton_3, &QPushButton::clicked, [&]()
    {
        addTime(1);                      // Добавление времени.
    });
    QObject::connect(ui->pushButton_5, &QPushButton::clicked, [&]()
    {
        addTime(3);
    });
    QObject::connect(ui->pushButton_7, &QPushButton::clicked, [&]()
    {
        addTime(2);
    });
    QObject::connect(ui->pushButton_9, &QPushButton::clicked, [&]()
    {
        addTime(4);
    });
    QObject::connect(ui->pushButton_4, &QPushButton::clicked, [&]()
    {
        deleteTime(1);                   // Удаление времени.
    });
    QObject::connect(ui->pushButton_6, &QPushButton::clicked, [&]()
    {
        deleteTime(3);
    });
    QObject::connect(ui->pushButton_8, &QPushButton::clicked, [&]()
    {
        deleteTime(2);
    });
    QObject::connect(ui->pushButton_10, &QPushButton::clicked, [&]()
    {
        deleteTime(4);
    });                                 // Обработчики событий кнопок Ок и Отмена.
    connect(ui->pushButton, &QPushButton::clicked, this, &dialogTimeWindow::acceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &dialogTimeWindow::reject);
}
dialogTimeWindow::~dialogTimeWindow()   // Деструктор.
{
    delete ui;
    stack1.clear();
    stack2.clear();
}
void dialogTimeWindow::addTime(int day) // Добавление времени.
{
    try
    {
        if(ui->lineEdit->text().length()!=5)  // Обработка ошибки.
            throw InputException("Введено некорректное значение", 0);
        QStandardItem* item = new QStandardItem(ui->lineEdit->text());  // Получение данных.
        QStandardItemModel* qm = (day == 1 ? qobject_cast<QStandardItemModel*>(ui->listView_2->model()) : (day == 2 ? qobject_cast<QStandardItemModel*>(ui->listView_5->model()) :
                (day == 3 ? qobject_cast<QStandardItemModel*>(ui->listView_3->model()) : qobject_cast<QStandardItemModel*>(ui->listView_4->model()))));
        QFont font = item->font();  // Установка шрифта.
        font.setPointSize(14);
        item->setFont(font);
        int i = 0;
        while(i < qm->rowCount() && qm->item(i)->text() < item->text())
            i++;
        if(qm->rowCount()!=i && qm->item(i)->text() == item->text())
        {                           // Проверка на добавление существующего.
            QMessageBox::warning(this, "Добавление остановки", "Данное время уже присутствует в рейсе");
            return;
        }
        stack1.push(item->text());  // Сохранение данных.
        stack2.push(day - 1);
        qm->insertRow(i, item);     // Запись в список.
        ui->lineEdit->clear();      // Очистка строки ввода.
        activateOkButton();         // Активация кнопки Ок.
    }
    catch(InputException& ex)
    {
        ex.mesg();
    }
}
void dialogTimeWindow::deleteTime(int day)  // Удаление времени.
{
    QStandardItemModel* model = (day == 1 ? qobject_cast<QStandardItemModel*>(ui->listView_2->model()) : (day == 2 ? qobject_cast<QStandardItemModel*>(ui->listView_5->model()) :
            (day == 3 ? qobject_cast<QStandardItemModel*>(ui->listView_3->model()) : qobject_cast<QStandardItemModel*>(ui->listView_4->model()))));
    QModelIndex cur = (day == 1 ? ui->listView_2->currentIndex() : (day == 2 ? ui->listView_5->currentIndex() :
                  (day == 3 ? ui->listView_3->currentIndex() : ui->listView_4->currentIndex())));
    stack1.push(model->item(cur.row())->text());  // Получение данных и их сохранение.
    stack2.push(10 + day - 1);
    model->removeRow(cur.row());                  // Удаление из списка.
    if(model->rowCount() == 0)
        ui->pushButton_4->setEnabled(false);      // Выключение кнопки, если было удалено всё.
    activateOkButton();
}
void dialogTimeWindow::activateOkButton()  // Активация кнопки Ок.
{
    if((ui->listView_2->model()->rowCount() == 0 || ui->listView_2->model()->rowCount() == ui->listView->model()->rowCount()) &&
        (ui->listView_3->model()->rowCount() == 0 || ui->listView_3->model()->rowCount() == ui->listView->model()->rowCount()) &&
        (ui->listView_4->model()->rowCount() == 0 || ui->listView_4->model()->rowCount() == ui->listView->model()->rowCount()) &&
        (ui->listView_5->model()->rowCount() == 0 || ui->listView_5->model()->rowCount() == ui->listView->model()->rowCount()))
        ui->pushButton->setEnabled(true);  // Проверка, что введено нужное количество рейсов.
    else
        ui->pushButton->setEnabled(false);
}
void dialogTimeWindow::onTextChanged(const QString &text) // Обработчик изменение текста в строке ввода.
{
    if(text.isEmpty())
    {
        ui->pushButton_3->setEnabled(false);              // Выключение кнопок, если пусто.
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_9->setEnabled(false);
    }
    else
    {                                                     // Включение, если ещё можно добавить рейс.
        if(ui->listView_2->isEnabled() && ui->listView_2->model()->rowCount() < ui->listView->model()->rowCount())
            ui->pushButton_3->setEnabled(true);
        if(ui->listView_3->isEnabled() && ui->listView_3->model()->rowCount() < ui->listView->model()->rowCount())
            ui->pushButton_5->setEnabled(true);
        if(ui->listView_5->isEnabled() && ui->listView_5->model()->rowCount() < ui->listView->model()->rowCount())
            ui->pushButton_7->setEnabled(true);
        if(ui->listView_4->isEnabled() && ui->listView_4->model()->rowCount() < ui->listView->model()->rowCount())
            ui->pushButton_9->setEnabled(true);
    }
}
QStandardItem* dialogTimeWindow::makeItem(const QString& text)  // Создание объекта.
{
    QStandardItem* item = new QStandardItem(text);
    item->setEditable(false);                                   // Установка внешнего вида.
    QFont font = item->font();
    font.setPointSize(11);
    item->setFont(font);
    return item;                                                // Возврат объекта.
}
void dialogTimeWindow::acceptClicked()                          // Нажата кнопка Ок.
{
    Node* n = findRoute2(this->city, this->route)->first;       // Поиск маршрута.
    for(int a = 0; a<ui->listView->model()->rowCount(); a++)
    {                                                           // Добавление автобуса в список.
        if(ui->listView_2->isEnabled() && ui->listView_2->model()->rowCount() != 0)
            addBus(1, n, a);
        if(ui->listView_3->isEnabled() && ui->listView_3->model()->rowCount() != 0)
            addBus(3, n, a);
        if(ui->listView_4->isEnabled() && ui->listView_4->model()->rowCount() != 0)
            addBus(4, n, a);
        if(ui->listView_5->isEnabled() && ui->listView_5->model()->rowCount() != 0)
            addBus(2, n, a);
        n = n->stopNext;                                        // Переход к следующей остановке.
    }
    accept();
}
void dialogTimeWindow::addBus(int day, Node*& n, int a)         // Добавление автобуса.
{
    int i = 0;
    QStandardItemModel* qm = (day == 1 ? qobject_cast<QStandardItemModel*>(ui->listView_2->model()) : (day == 2 ? qobject_cast<QStandardItemModel*>(ui->listView_5->model()) :
            (day == 3 ? qobject_cast<QStandardItemModel*>(ui->listView_3->model()) : qobject_cast<QStandardItemModel*>(ui->listView_4->model()))));
    QModelIndex cur = qm->index(a, 0);                          // Получение данных.
    QStandardItem* item = qm->itemFromIndex(cur);
    auto jt = (day == 1 ? n->timetableNextWork.begin() : (day == 2 ? n->timetablePrevWork.begin() : (day == 3 ? n->timetableNextWeek.begin() : n->timetablePrevWeek.begin())));
    while( jt!= (day == 1 ? n->timetableNextWork.end() : (day == 2 ? n->timetablePrevWork.end() : (day == 3 ? n->timetableNextWeek.end() : n->timetablePrevWeek.end()))))
    {
        if(jt->time > item->text())                             // Добавление по возрастанию.
            break;
        jt++;
        i++;
    }
    for(auto xt = this->city->getParks().begin(); xt!=this->city->getParks().end(); xt++)
    {
        Bus* yt = TreeAlgorithms<Bus>::findOne(Bus(ui->comboBox_2->currentText()), xt->buses);
        if(yt!=nullptr)                                         // Поиск автобуса для рейса.
        {
            if(day==1)                                          // Добавление в нужное расписание.
                n->timetableNextWork.insert(jt, TimeBus(item->text(), yt));
            else if(day == 2)
                n->timetablePrevWork.insert(jt, TimeBus(item->text(), yt));
            else if(day == 3)
                n->timetableNextWeek.insert(jt, TimeBus(item->text(), yt));
            else
                n->timetablePrevWeek.insert(jt, TimeBus(item->text(), yt));
            break;
        }
    }
}
void dialogTimeWindow::keyPressEvent(QKeyEvent *event)         // Обработчик нажатия клавиш.
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z)
    {
        if(!stack1.empty())                                    // Проверка, что стек не пуст.
        {
            int v = stack2.pop();
            QString temp = stack1.pop();                       // Чтение данных.
            QStandardItemModel* model;
            if(v==0||v==10)                                    // Считывание модели данных.
                model = qobject_cast<QStandardItemModel*>(ui->listView_2->model());
            else if(v==1 || v==11)
                model = qobject_cast<QStandardItemModel*>(ui->listView_3->model());
            else if(v==2 || v==12)
                model = qobject_cast<QStandardItemModel*>(ui->listView_5->model());
            else if(v==3 || v==13)
                model = qobject_cast<QStandardItemModel*>(ui->listView_4->model());
            if(v == 0 || v ==1 || v==2||v==3)
            {
                for(int i = 0; i<model->rowCount(); i++)
                    if(model->item(i)->text() == temp)
                    {
                        model->removeRow(i);                   // Удаление данных.
                        break;
                    }
            }
            else
            {
                int i = 0;
                while(i<model->rowCount() && model->item(i)->text() < temp)
                    i++;
                QStandardItem* item = makeItem(temp);          // Создание элемента.
                model->insertRow(i, item);                     // Добавление данных.
            }
        }
    }
}
