#include "dialogbuswindow.h"
#include "ui_dialogbuswindow.h"

dialogBusWindow::dialogBusWindow(TransportPark* park, City* city, Bus* b, QDialog *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    park(park), city(city), b(b), ui(new Ui::dialogBusWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                                   // Установка внешнего вида.
    setModal(true);
    setFocus();
    QRegularExpression regex("[а-яА-Яa-zA-Z0-9- .]*$");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex);
    ui->lineEdit->setValidator(validator);
    ui->lineEdit_2->setValidator(validator);
    if(b==nullptr)
        ui->pushButton->setEnabled(false);               // Выключение кнопки Ок.
    else
    {
        ui->lineEdit->setText(b->model);                 // Запись имеющихся данных.
        ui->lineEdit_2->setText(b->number);
        ui->checkBox->setChecked(b->hasApparel);
        ui->checkBox_2->setChecked(b->hasConditioner);
        ui->checkBox_3->setChecked(b->isLarge);
    }                                                    // Установка обработчиков события.
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &dialogBusWindow::onTextChanged);
    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, &dialogBusWindow::onTextChanged);
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &dialogBusWindow::onCheckBoxStateChanged);
    connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &dialogBusWindow::onCheckBoxStateChanged);
    connect(ui->checkBox_3, &QCheckBox::stateChanged, this, &dialogBusWindow::onCheckBoxStateChanged);
    connect(ui->pushButton, &QPushButton::clicked, this, &dialogBusWindow::acceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &dialogBusWindow::reject);
}
dialogBusWindow::~dialogBusWindow()
{
    delete ui;        // Удаление внешнего вида.
    stack.clear();    // Очистка стека.
}
void dialogBusWindow::onTextChanged(const QString &text)   // Изменение текста в строке ввода.
{
    ui->pushButton->setEnabled(!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty());
    pushBusInStack();                                      // Включение кнопки если поля заполнены.
}
void dialogBusWindow::pushBusInStack()                     // Добавление автобуса в стек.
{
    Bus* temp = new Bus();                                 // Сохранение данных во временный объект.
    temp->number = ui->lineEdit_2->text();
    temp->model = ui->lineEdit->text();
    temp->hasApparel = ui->checkBox->isChecked();
    temp->hasConditioner = ui->checkBox_2->isChecked();
    temp->isLarge = ui->checkBox_3->isChecked();
    stack.push(temp);                                      // Занесение объекта в стек.
}
void dialogBusWindow::onCheckBoxStateChanged(int state)    // Изменение чек бокса.
{
    pushBusInStack();                                      // Занесение автобуса в стек.
}
void dialogBusWindow::keyPressEvent(QKeyEvent *event)      // Обработчик нажатия клавиш.
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z)
    {
        if(!stack.isEmpty())                               // Проверка, что стек не пуст.
        {
            Bus* temp = stack.pop();                       // Достать из стека.
            ui->checkBox->setEnabled(temp->hasApparel);    // Вернуть данные.
            ui->checkBox_2->setEnabled(temp->hasConditioner);
            ui->checkBox_3->setEnabled(temp->isLarge);
            ui->lineEdit->setText(temp->model);
            ui->lineEdit_2->setText(temp->number);
        }
    }
}
void dialogBusWindow::acceptClicked()                      // Нажатие клавиши Ок.
{
    if(this->b==nullptr)
    {
        foreach(TransportPark p, this->city->getParks())   // Проверка, что автобус уже добавлен.
        {
            MyTree<Bus>::MyIterator b = p.buses.begin();
            while(!b.isnull())
            {
                if((*b).number == ui->lineEdit_2->text())
                {
                    QMessageBox::warning(this, "Добавление автобуса", "Автобус уже существует в это или другом парке");
                    return;
                }
                b++;
            }
        }
        Bus* bus = new Bus();                               // Создание нового объекта.
        bus->model = ui->lineEdit->text();                  // Сохранение данных.
        bus->number = ui->lineEdit_2->text();
        bus->hasApparel = ui->checkBox->isChecked();
        bus->hasConditioner = ui->checkBox_2->isChecked();
        bus->isLarge = ui->checkBox_3->isChecked();
        park->buses.Add(*bus);                              // Добавление в дерево.
    }
    else
    {
        b->model = ui->lineEdit->text();                    // Изменение данных в существующем объекте.
        b->number = ui->lineEdit_2->text();
        b->hasApparel = ui->checkBox->isChecked();
        b->hasConditioner = ui->checkBox_2->isChecked();
        b->isLarge = ui->checkBox_3->isChecked();
    }
    accept();
}
