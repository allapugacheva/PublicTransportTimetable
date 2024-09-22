#include "dialogparkwindow.h"
#include "ui_dialogparkwindow.h"


dialogParkWindow::dialogParkWindow(City* city, TransportPark* park, QDialog *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    city(city), ui(new Ui::dialogParkWindow), editMode(false), park(park)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                     // Установка внешнего вида.
    setModal(true);
    setFocus();
    QRegularExpression regex("[а-яА-Я0-9- ]*$");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex);
    ui->lineEdit->setValidator(validator);
    if(this->park==nullptr)
        this->park = new TransportPark();
    if(this->park->parkName!="")
    {
        this->editMode = true;             // Запись исходных данных.
        ui->lineEdit->setText(park->parkName);
    }
    else
    {
        ui->pushButton->setToolTip("Введите название парка");
        ui->pushButton->setEnabled(false); // Выключение кнопки.
    }
    QStandardItemModel* qm = new QStandardItemModel();
    ui->listView->setModel(qm);
    ui->listView->setStyleSheet("QListView::item { height: 30px; }");
    if(this->park->parkName!="")
    {
        MyTree<Bus>::MyIterator b = this->park->buses.begin();
        while(!b.isnull())
        {
            addBus((*b).number);           // Запись автобусов.
            b++;
        }
    }
    ui->pushButton_4->setEnabled(false);   // Обработчики нажатия кнопок.
    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, this, &dialogParkWindow::onTextChanged);
    QObject::connect(ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
    {                                      // Активация, если список не пуст.
        ui->pushButton_4->setEnabled(ui->listView->model()->rowCount() > 0);
    });
    connect(ui->pushButton, &QPushButton::clicked, this, &dialogParkWindow::acceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &dialogParkWindow::reject);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &dialogParkWindow::addBusClicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &dialogParkWindow::removeBusClicked);
    if(this->editMode)
        connect(ui->listView, &QListView::doubleClicked, this, &dialogParkWindow::onItemDoubleClicked);
}
dialogParkWindow::~dialogParkWindow() // Деструктор.
{
    delete ui;
    stack.clear();
}
void dialogParkWindow::onTextChanged(const QString &text)    // Изменение текста.
{
    if(!text.isEmpty())
    {
        ui->pushButton->setEnabled(true);             // Активация кнопки при непустом вводе.
        ui->pushButton->setToolTip("");
    }
    else
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton->setToolTip("Введите название парка");
    }
    TransportPark* temp = new TransportPark(*(this->park));
    temp->parkName = ui->lineEdit->text();                   // Сохранение данных.
    stack.push(temp);
}
void dialogParkWindow::onItemDoubleClicked(const QModelIndex &index) // Двойное нажатие на элемент списка.
{
    TransportPark* temp = new TransportPark(*(this->park));
    temp->parkName = ui->lineEdit->text();
    QStandardItemModel* qm= qobject_cast<QStandardItemModel*>(ui->listView->model());
    QStandardItem* item = qm->itemFromIndex(index);                  // Получение выбранного элемента.
    Bus* bus = TreeAlgorithms<Bus>::findOne(Bus(item->text()), this->park->buses); // Поиск маршрута.
    dialogBusWindow* w = new dialogBusWindow(this->park, this->city, bus);
    w->show();                                                       // Диалоговое окно изменения автобуса.
    if(w->exec() == QDialog::Accepted)                               // Изменен?
    {
        stack.push(temp);
        item->setText(bus->number);                                  // Изменение данных.
    }
}
void dialogParkWindow::acceptClicked()                               // Нажата кнопка Ок.
{
    if(!editMode)
    {
        foreach(TransportPark p, this->city->getParks())
            if(p.parkName == ui->lineEdit->text())                   // Проверка на добавление существующего.
            {
                QMessageBox::warning(this, "Добавление парка", "Парк уже существует");
                return;
            }
    }
    this->park->parkName = ui->lineEdit->text();                     // Добавление данных.
    if(!editMode)
        city->getParks().push_back(*park);
    accept();
}
void dialogParkWindow::addBusClicked()                               // Кнопка добавление автобуса.
{
    TransportPark* temp = new TransportPark(*(this->park));
    temp->parkName = ui->lineEdit->text();                           // Получение данных.
    dialogBusWindow* w = new dialogBusWindow(this->park, this->city);
    w->show();                                                       // Диалогое окно создания автобуса.
    if(w->exec() == QDialog::Accepted)
    {
        stack.push(temp);
        addBus((*(park->buses.back())).number);                      // Добавление данных.
    }
}
void dialogParkWindow::addBus(const QString& busnum)                 // Добавление автобуса.
{
    QStandardItemModel* qm = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QStandardItem* item = new QStandardItem(busnum);                 // Создание объекта данных.
    item->setEditable(false);
    item->setTextAlignment(Qt::AlignVCenter);                        // Установка внешнего вида.
    QFont font = item->font();
    font.setPointSize(11);
    item->setFont(font);
    qm->appendRow(item);                                             // Добавление данных.
}
void dialogParkWindow::removeBusClicked()                            // Удаление автобуса.
{
    TransportPark* temp = new TransportPark(*(this->park));
    temp->parkName = ui->lineEdit->text();                           // Получение данных.
    stack.push(temp);
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->listView->model());
    QModelIndex cur = ui->listView->currentIndex();
    MyTree<Bus>::MyIterator it = park->buses.begin();
    for(int i = 0; i<cur.row(); i++)
        it++;
    if(checkRouteBuses(this->city, (*it).number))
        return;                          // Проверка, что автобус не задействован на маршрутах.
    model->removeRow(cur.row());
    park->buses.Remove(*it);                                         // Удаление данных.
    if(model->rowCount() == 0)
        ui->pushButton_4->setEnabled(false);
}
void dialogParkWindow::keyPressEvent(QKeyEvent *event)               // Обработчик нажатия клавиш.
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z)
    {
        if(!stack.isEmpty())                                         // Стек не пуст?
        {
            this->park = new TransportPark(*(stack.pop()));
            ui->lineEdit->setText(this->park->parkName);
            QStandardItemModel* qm = new QStandardItemModel();
            QStandardItem* item;                                     // Получение данных.
            MyTree<Bus>::MyIterator b = this->park->buses.begin();
            while(!b.isnull())
            {
                item = new QStandardItem((*b).number);               // Добавление данных.
                item->setEditable(false);
                item->setTextAlignment(Qt::AlignVCenter);
                QFont font = item->font();
                font.setPointSize(11);
                item->setFont(font);
                qm->appendRow(item);
                b++;
            }
            ui->listView->setModel(qm);                              // Отображение данных.
        }
    }
}
