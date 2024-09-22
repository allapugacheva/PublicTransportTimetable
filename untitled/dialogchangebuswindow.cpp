#include "dialogchangebuswindow.h"
#include "ui_dialogchangebuswindow.h"

dialogChangeBusWindow::dialogChangeBusWindow(City* city, TimeBus* tb, QDialog *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    city(city), tb(tb), ui(new Ui::dialogChangeBusWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                       // Установка внешнего вида окна.
    setModal(true);
    setFocus();
    ui->comboBox->setStyleSheet("QListView::item { height: 30px; }");
    int i = 0, j;                            // Установка внешнего вида комбо бокса.
    foreach(TransportPark p, this->city->getParks())
    {
        MyTree<Bus>::MyIterator b = p.buses.begin();
        while(!b.isnull())
        {
            ui->comboBox->addItem((*b).number); // Запись данных в комбо бокс.
            if((*b).number == tb->bus->number)
                j = i;
            i++;
            b++;
        }
    }
    ui->comboBox->setCurrentIndex(j);        // Выбор текущего автобуса.
    connect(ui->pushButton, &QPushButton::clicked, this, &dialogChangeBusWindow::acceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &dialogChangeBusWindow::reject);
}                                            // Обработчики нажатий кнопок.
dialogChangeBusWindow::~dialogChangeBusWindow()
{
    delete ui;
}
void dialogChangeBusWindow::acceptClicked()
{
    bool fl = false;
    foreach(TransportPark p, this->city->getParks())
    {                            // Поиск автобуса.
        MyTree<Bus>::MyIterator b = p.buses.begin();
        while(!b.isnull())
        {                        // Копирование данных.
            if((*b).number == ui->comboBox->currentText())
            {
                tb->bus->hasApparel = (*b).hasApparel;
                tb->bus->hasConditioner = (*b).hasConditioner;
                tb->bus->isLarge = (*b).isLarge;
                tb->bus->model = (*b).model;
                tb->bus->number = (*b).number;
                fl = true;
                break;
            }
            b++;
        }
        if(fl)
            break;
    }
    accept();
}
