#include "dialogfindroutewindow.h"
#include "ui_dialogfindroutewindow.h"

dialogFindRouteWindow::dialogFindRouteWindow(City* city, QDialog *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    ui(new Ui::dialogFindRouteWindow), city(city)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);          // Установка внешнего вида окна.
    setModal(true);
    setFocus();
    MyTree<Stop>::MyIterator s = this->city->getStops().begin();
    while(!s.isnull())          // Заполнение комбо боксов остановками.
    {
        ui->comboBox->addItem((*s).stop->stopName);
        ui->comboBox_2->addItem((*s).stop->stopName);
        s++;
    }                           // Обработчики нажатия кнопок.
    connect(ui->pushButton, &QPushButton::clicked, this, &dialogFindRouteWindow::acceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &dialogFindRouteWindow::reject);
}
dialogFindRouteWindow::~dialogFindRouteWindow()  // Деструктор.
{
    delete ui;
}
void dialogFindRouteWindow::acceptClicked()      // Нажата кнопка Ок.
{
    QStringList l;
    Node* n1, *n2;
    n1 = TreeAlgorithms<Stop>::findOne(Stop(ui->comboBox->currentText()), this->city->getStops())->stop;
    n2 = TreeAlgorithms<Stop>::findOne(Stop(ui->comboBox_2->currentText()), this->city->getStops())->stop;
    int min = 0;                                 // Поиск выбранных остановок.
    this->city->findRoute(n1, nullptr, n2, "", 0, 0, min, l);
    min = 1;                                     // Метод поиска пути между ними.
    QString result;
    foreach(QString str, l)                      // Формирование данных.
    {
        result += "Вариант " + QString::number(min) + ":\n";
        result += str;
        if(str!=l.back())
            result += "\n\n";
        min++;
    }
    if (result.isEmpty())
        result = "Не найден маршрут между указанными остановками";
    QMessageBox::information(this, "Поиск маршрута", result); // Вывод на экран.
    accept();
}
