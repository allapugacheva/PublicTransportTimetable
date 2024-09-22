#include "dialogstopwindow.h"
#include "ui_dialogstopwindow.h"

dialogStopWindow::dialogStopWindow(City* city, bool work, bool week, Route* route, Stop* stop, QDialog *parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    city(city), stop(stop), route(route), ui(new Ui::dialogStopWindow)
{
    this->setWindowIcon(QIcon("bus.png"));
    ui->setupUi(this);                                             // Установка внешнего вида окна.
    setModal(true);
    setFocus();
    QRegularExpression regex("[а-яА-Я0-9-\" ]*$");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex);
    ui->lineEdit->setValidator(validator);
    if(stop!=nullptr)
        ui->pushButton->setEnabled(false);                         // Деактивация кнопки.
    ui->pushButton->setToolTip("Заполните данные об остановке");
    if(route!=nullptr)
    {
        QComboBox* qc = makeComboBox();
        qc->addItem("");
        MyTree<Stop>::MyIterator it = city->getStops().begin();
        while(!it.isnull())
        {
            qc->addItem((*it).stop->stopName);                     // Заполнение комбо бокса остановками.
            it++;
        }     
        QLabel* l = makeLabel("Расписание:");                      // Создание кнопок, текстовых полей.
        QPushButton* plusButton = makePushButton("+");
        plusButton->setToolTip("Введите время, выберите автобус из списка чтобы добавить");
        QPushButton* minusButton = makePushButton("-");
        QLineEdit* ql = makeLineEdit();
        ql->setToolTip("Рабочий день, прямое направление");
        if(!work)
            ql->setEnabled(false);                                 // Деактивация.
        QListView* lv = makeListView();
        if(!work)
            lv->setEnabled(false);
        QPushButton* plusButton2 = makePushButton("+");
        plusButton2->setToolTip("Введите время, выберите автобус из списка чтобы добавить");
        QPushButton* minusButton2 = makePushButton("+");
        QLineEdit* ql2 = makeLineEdit();
        ql2->setToolTip("Рабочий день, обратное направление");
        if(!work)
            ql2->setEnabled(false);
        QListView* lv2 = makeListView();
        if(!work)
            lv2->setEnabled(false);
        QRegularExpression timeRegex("^(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$"); // Регулярное выражение для ввода времени.
        QRegularExpressionValidator *validator = new QRegularExpressionValidator(timeRegex); // Валидатор для ввода данных.
        ql->setValidator(validator);                               // Установка валидатора.
        ql2->setValidator(validator);
        if(route->first!=nullptr)
        {
            ui->gridLayout->addWidget(qc, 0, 1, 1, 2, Qt::AlignCenter); // Добавление элементов на экран.
            ui->gridLayout->addWidget(l, 1, 1, 1, 2, Qt::AlignCenter);
        }
        else
        {
            ui->gridLayout->addWidget(qc, 0, 1, 1, 3, Qt::AlignCenter);
            ui->gridLayout->addWidget(l, 1, 1, 1, 3, Qt::AlignCenter);
        }
        ui->gridLayout->addWidget(plusButton, 2, 0);
        ui->gridLayout->addWidget(minusButton, 2, 4);
        ui->gridLayout->addWidget(ql, 2, 1);
        ui->gridLayout->addWidget(lv, 2, 2);
        ui->gridLayout->addWidget(plusButton2, 3, 0);
        ui->gridLayout->addWidget(minusButton2, 3, 4);
        ui->gridLayout->addWidget(ql2, 3, 1);
        ui->gridLayout->addWidget(lv2, 3, 2);
        if(route->first==nullptr)
        {
            QListView* lv01 = makeListView();                           // Создание списков.
            lv01->setFixedSize(115, 30);
            if(!work)
                lv01->setEnabled(false);
            QListView* lv02 = makeListView();
            lv02->setFixedSize(115, 30);
            if(!work)
                lv02->setEnabled(false);
            QListView* lv03 = makeListView();
            lv03->setFixedSize(115, 30);
            if(!week)
                lv03->setEnabled(false);
            QListView* lv04 = makeListView();
            lv04->setFixedSize(115, 30);
            if(!week)
                lv04->setEnabled(false);
            QStandardItemModel* qm = new QStandardItemModel();
            QListView* lv3 = new QListView();
            for(auto jt = city->getParks().begin(); jt!=city->getParks().end(); jt++)
            {
                MyTree<Bus>::MyIterator kt = jt->buses.begin();
                while(!kt.isnull())
                {
                    QStandardItem* item = makeItem((*kt).number);      // Добавление автобусов в список.
                    qm->appendRow(item);
                    kt++;
                }
            }
            lv3->setModel(qm);
            lv3->setFixedSize(150, 30);
            lv3->setStyleSheet("QListView::item { height: 30px; }");
            ui->gridLayout->addWidget(lv3, 4, 1, 1, 3,Qt::AlignCenter);// Добавление элементов на экран.
            ui->gridLayout->addWidget(lv01, 2, 3);
            ui->gridLayout->addWidget(lv02, 3, 3);
            ui->gridLayout->addWidget(lv03, 5, 3);
            ui->gridLayout->addWidget(lv04, 6, 3);
            QObject::connect(lv3->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
            {
                activatePlusButton(2);                                 // Активация кнопок.
                activatePlusButton(3);
                activatePlusButton(5);
                activatePlusButton(6);
            });                                                        // Обработчики события нажатий кнопок.
            QObject::connect(lv01->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
            {
                activateMinusButton(2, 3);                             // Активация кнопки.
            });
            QObject::connect(lv02->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
            {
                activateMinusButton(3, 3);
            });
            QObject::connect(lv03->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
            {
                activateMinusButton(5, 3);
            });
            QObject::connect(lv04->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
            {
                activateMinusButton(6, 3);
            });
        }
        QPushButton* plusButton3 = makePushButton("+");                // Создание кнопок.
        plusButton3->setToolTip("Введите время, выберите автобус из списка чтобы добавить");
        QPushButton* minusButton3 = makePushButton("-");
        QLineEdit* ql3 = makeLineEdit();                               // Создание текстовых полей.
        ql3->setToolTip("Выходной день, прямой направление");
        if(!week)
            ql3->setEnabled(false);
        QListView* lv3 = makeListView();
        if(!week)
            lv3->setEnabled(false);
        QPushButton* plusButton4 = makePushButton("+");
        plusButton4->setToolTip("Введите время, выберите автобус из списка чтобы добавить");
        QPushButton* minusButton4 = makePushButton("-");
        QLineEdit* ql4 = makeLineEdit();
        ql4->setToolTip("Выходной день, обратное направление");
        if(!week)
            ql4->setEnabled(false);
        QListView* lv4 = makeListView();
        if(!week)
            lv4->setEnabled(false);
        ql3->setValidator(validator);                                  // Установка валидаторов для полей ввода.
        ql4->setValidator(validator);
        ui->gridLayout->addWidget(plusButton3, 5, 0);                  // Добавление элементов на экран.
        ui->gridLayout->addWidget(minusButton3, 5, 4);
        ui->gridLayout->addWidget(ql3, 5, 1);
        ui->gridLayout->addWidget(lv3, 5, 2);
        ui->gridLayout->addWidget(plusButton4, 6, 0);
        ui->gridLayout->addWidget(minusButton4, 6, 4);
        ui->gridLayout->addWidget(ql4, 6, 1);
        ui->gridLayout->addWidget(lv4, 6, 2);
        QObject::connect(plusButton, &QPushButton::clicked, [&]()
        {
            addTime(1, 2);         // Добавление времени.
        });                        // Обработчики нажатия кнопок.
        QObject::connect(plusButton2, &QPushButton::clicked, [&]()
        {
            addTime(2, 3);
        });
        QObject::connect(plusButton3, &QPushButton::clicked, [&]()
        {
            addTime(3, 5);
        });
        QObject::connect(plusButton4, &QPushButton::clicked, [&]()
        {
            addTime(4, 6);
        });
        QObject::connect(qc, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index)
        {                          // Установка текста из комбо бокса в строку ввода.
            QComboBox* qbox = qobject_cast<QComboBox*>(ui->gridLayout->itemAtPosition(0, 1)->widget());
            ui->lineEdit->setText(qbox->itemText(index));
        });
        QObject::connect(minusButton, &QPushButton::clicked, [&]()
        {
            deleteTime(2);         // Удаление времени.
        });
        QObject::connect(minusButton2, &QPushButton::clicked, [&]()
        {
            deleteTime(3);
        });
        QObject::connect(minusButton3, &QPushButton::clicked, [&]()
        {
            deleteTime(5);
        });
        QObject::connect(minusButton4, &QPushButton::clicked, [&]()
        {
            deleteTime(6);
        });
        QObject::connect(lv->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
        {
            activateMinusButton(2, 2); // Активация кнопки удаления.
        });
        QObject::connect(lv2->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
        {
            activateMinusButton(3, 2);
        });
        QObject::connect(lv3->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
        {
            activateMinusButton(5, 2);
        });
        QObject::connect(lv4->selectionModel(), &QItemSelectionModel::selectionChanged, [&]()
        {
            activateMinusButton(6, 2);
        });
        QObject::connect(ql, &QLineEdit::textChanged, [=]()
        {
            activatePlusButton(2);    // Активация кнопки добавления.
        });
        QObject::connect(ql2, &QLineEdit::textChanged, [=]()
        {
            activatePlusButton(3);
        });

        QObject::connect(ql3, &QLineEdit::textChanged, [=]()
        {
            activatePlusButton(5);
        });

        QObject::connect(ql4, &QLineEdit::textChanged, [=]()
        {
            activatePlusButton(6);
        });
    }                        // Обработчик изменения текста.
    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, [=]()
    {
        if(this->route == nullptr)   // Активация кнопки, если введен текст.
            ui->pushButton->setEnabled(!ui->lineEdit->text().isEmpty());
        else
        {
            if(this->route->first!=nullptr)
            {
                activateOkButton();
            }
            else
                ui->pushButton->setEnabled(!ui->lineEdit->text().isEmpty());
        }
    });
    if(stop!=nullptr)
        ui->lineEdit->setText(stop->stop->stopName);  // Обработчик нажатия кнопок.
    connect(ui->pushButton, &QPushButton::clicked, this, &dialogStopWindow::acceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &dialogStopWindow::reject);
}
dialogStopWindow::~dialogStopWindow()  // Деструктор.
{
    delete ui;
    stack1.clear();
    stack2.clear();
}
QPushButton* dialogStopWindow::makePushButton(const QString& text) // Создание кнопки.
{
    QPushButton* temp = new QPushButton(text);    // Создание кнопки с заданным текстом.
    temp->setFixedSize(30, 30);
    temp->setEnabled(false);                      // Установка параметров текста.
    QFont fnt = temp->font();
    fnt.setBold(true);
    fnt.setPixelSize(14);
    temp->setFont(fnt);
    return temp;                                  // Возврат кнопки.
}
QListView* dialogStopWindow::makeListView()                        // Создание списка.
{
    QStandardItemModel* qm = new QStandardItemModel();
    QListView* temp = new QListView();                             // Создание модели данных.
    temp->setModel(qm);
    temp->setFixedSize(100, 30);                                   // Установка размеров.
    temp->setStyleSheet("QListView::item { height: 30px; }");
    return temp;
}
QLabel* dialogStopWindow::makeLabel(const QString& text)           // Создание текстовой строки.
{
    QLabel* temp = new QLabel(text);                               // Создание строки с заданным текстом.
    temp->setAlignment(Qt::AlignCenter);
    temp->setFixedSize(200, 30);                                   // Установка внешнего вида.
    QFont font = temp->font();
    font.setBold(true);
    font.setPixelSize(14);
    temp->setFont(font);
    return temp;
}
QComboBox* dialogStopWindow::makeComboBox()                        // Создание комбо бокса.
{
    QComboBox* temp = new QComboBox();
    temp->setStyleSheet("QListView::item { height: 30px; }");      // Установка параметров текста.
    temp->setFixedSize(200, 30);
    QFont font = temp->font();
    font.setBold(true);
    font.setPixelSize(14);
    temp->setFont(font);
    return temp;                                                   // Возврат выпадающего списка.
}
QLineEdit* dialogStopWindow::makeLineEdit()                        // Создание строки ввода.
{
    QLineEdit* temp = new QLineEdit();
    temp->setFixedSize(100, 30);                                   // Установка параметров текста.
    QFont font;
    font.setBold(false);
    font.setPixelSize(14);
    temp->setFont(font);
    return temp;                                                   // Возврат строки.
}
void dialogStopWindow::activateMinusButton(int row, int lj)        // Активация кнопки удаления.
{
    QPushButton* mb = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(row, 4)->widget());
    QListView* lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(row, lj)->widget());
    mb->setEnabled(lview->model()->rowCount() > 0);  // Проверка, что заполнен список.
}
void dialogStopWindow::activatePlusButton(int row)   // Активация кнопки добавления.
{
    if(this->route->first == nullptr)
    {
        QPushButton* pb = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(row, 0)->widget());
        QLineEdit* ledit = qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(row, 1)->widget());
        QListView* lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(4, 1)->widget());
        pb->setEnabled(!ledit->text().isEmpty() && !lview->selectionModel()->selectedIndexes().isEmpty());
    }
    else       // Проверка, что списки не пусты.
    {
        QPushButton* pb = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(row, 0)->widget());
        QLineEdit* ledit = qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(row, 1)->widget());
        QListView* lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(row, 2)->widget());
        if(row == 2)
        {
            pb->setEnabled(!ledit->text().isEmpty() && lview->model()->rowCount() < this->route->first->timetableNextWork.size());
            if(lview->model()->rowCount() == this->route->first->timetableNextWork.size())
                pb->setToolTip("Введено максимальное число рейсов");
            else
                pb->setToolTip("Введите время, выберите автобус из списка чтобы добавить");
        }
        else if(row == 3)
        {
            pb->setEnabled(!ledit->text().isEmpty() && lview->model()->rowCount() < this->route->first->timetablePrevWork.size());
            if(lview->model()->rowCount() == this->route->first->timetablePrevWork.size())
                pb->setToolTip("Введено максимальное число рейсов");
            else
                pb->setToolTip("Введите время, выберите автобус из списка чтобы добавить");
        }
        else if(row == 4)
        {
            pb->setEnabled(!ledit->text().isEmpty() && lview->model()->rowCount() < this->route->first->timetableNextWeek.size());
            if(lview->model()->rowCount() == this->route->first->timetableNextWeek.size())
                pb->setToolTip("Введено максимальное число рейсов");
            else
                pb->setToolTip("Введите время, выберите автобус из списка чтобы добавить");
        }
        else
        {
            pb->setEnabled(!ledit->text().isEmpty() && lview->model()->rowCount() < this->route->first->timetablePrevWeek.size());
            if(lview->model()->rowCount() == this->route->first->timetablePrevWeek.size())
                pb->setToolTip("Введено максимальное число рейсов");
            else
                pb->setToolTip("Введите время, выберите автобус из списка чтобы добавить");
        }
    }
}
void dialogStopWindow::addTime(int day, int row)     // Добавление времени.
{
    try
    {
        QListView* lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(row, 2)->widget());
        QLineEdit* ledit = qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(row, 1)->widget());
        QString enteredText = ledit->text();             // Получение данных.
        if(enteredText.length() != 5)
            throw InputException("Введено некорректное значение", 0);
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(lview->model());
        QStandardItem* item = makeItem(enteredText);     // Создание объекта списка.
        int i = 0;
        while(i < model->rowCount() && model->item(i)->text() < item->text())
            i++;                                         // Добавление в порядке возрастания.
        if(model->rowCount()!=i && model->item(i)->text() == item->text())
        {                                                // Проверка на добавление существующего времени.
            QMessageBox::warning(this, "Добавление остановки", "Данное время уже присутствует в рейсе");
            return;
        }
        if(route->first!=nullptr)
        {
            Node*n= route->first;
            while(n!=nullptr)
            {                                            // Проверка всех рейсов на совпадение.
                auto ntt = (day == 1 ? n->timetableNextWork.begin() : (day == 2 ? n->timetablePrevWork.begin() :
                                   (day == 3 ? n->timetableNextWeek.begin() : n->timetablePrevWeek.begin())));
                for(int j = 0; j<i; j++, ntt++);
                if(ntt->time == item->text())
                {
                    QMessageBox::warning(this, "Добавление остановки", "Данное время уже присутствует в рейсе");
                    return;
                }
                n = n->stopNext;
            }
        }
        model->insertRow(i, item);                       // Добавление данных.
        stack1.push(enteredText);
        stack2.push(0);
        if(this->route->first == nullptr)
        {                                                // Добавление данных об автобусе.
            QListView* lview01 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(row, 3)->widget());
            QListView* lview2 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(4, 1)->widget());
            QStandardItemModel* model01 = qobject_cast<QStandardItemModel*>(lview01->model());
            QStandardItemModel* model2 = qobject_cast<QStandardItemModel*>(lview2->model());
            QModelIndex cur = lview2->currentIndex();
            item = makeItem(model2->itemFromIndex(cur)->text());
            model01->insertRow(i, item);
        }
        else
        {
            activateOkButton();                          // Активация кнопки Ок.
        }
        ledit->clear();
    }
    catch(InputException& ex)
    {
        ex.mesg();
    }
}
void dialogStopWindow::activateOkButton()            // Активация кнопки Ок.
{
    QListView* lview1 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(2, 2)->widget());
    QListView* lview2 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(3, 2)->widget());
    QListView* lview3 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(5, 2)->widget());
    QListView* lview4 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(6, 2)->widget());
    ui->pushButton->setEnabled(!ui->lineEdit->text().isEmpty() &&  // Проверка, что достигнуто нужное количество рейсов.
                               ((lview1->model()->rowCount() == this->route->first->timetableNextWork.size() && lview1->model()->rowCount()!=0)||
                                (lview2->model()->rowCount() == this->route->first->timetablePrevWork.size() && lview2->model()->rowCount()!=0)||
                                (lview3->model()->rowCount() == this->route->first->timetableNextWeek.size() && lview3->model()->rowCount()!=0)||
                                (lview4->model()->rowCount() == this->route->first->timetablePrevWeek.size() && lview4->model()->rowCount()!=0)) &&
                               ((lview1->model()->rowCount() == this->route->first->timetableNextWork.size() || lview1->model()->rowCount() == 0) &&
                                (lview2->model()->rowCount() == this->route->first->timetablePrevWork.size() || lview2->model()->rowCount() == 0) &&
                                (lview3->model()->rowCount() == this->route->first->timetableNextWeek.size() || lview3->model()->rowCount() == 0) &&
                                (lview4->model()->rowCount() == this->route->first->timetablePrevWeek.size() || lview4->model()->rowCount() == 0)));
}
QStandardItem* dialogStopWindow::makeItem(const QString& text) // Создание объекта.
{
    QStandardItem* item = new QStandardItem(text);
    item->setEditable(false);                 // Установка параметров текста.
    QFont font = item->font();
    font.setPointSize(14);
    item->setFont(font);
    return item;                              // Возврат объекта.
}
void dialogStopWindow::deleteTime(int row)    // Удаление времени.
{
    if(this->route->first == nullptr)
    {
        QListView* lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(row, 2)->widget());
        QListView* lview01 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(row, 3)->widget());
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(lview->model());
        QStandardItemModel* model01 = qobject_cast<QStandardItemModel*>(lview01->model());
        QModelIndex cur;                      // Получение данных.
        if(!lview->selectionModel()->selectedIndexes().isEmpty())
            cur = lview->currentIndex();      // Проверка, что выбрано время.
        else
            cur = lview01->currentIndex();
        model->removeRow(cur.row());          // Удаление данных.
        model01->removeRow(cur.row());
    }
    else
    {
        QListView* lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(row, 2)->widget());
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(lview->model());
        QModelIndex cur = lview->currentIndex();  // Проверка, что выбрано время.
        stack1.push(model->item(cur.row())->text());
        stack2.push(10);
        model->removeRow(cur.row());              // Удаление данных.
        activateOkButton();
    }
}
void dialogStopWindow::acceptClicked()            // Нажата кнопка Ок.
{
    if(stop==nullptr)
    {
        if(route!=nullptr)
        {
            Node* s = route->first;
            while(s!=nullptr)                     // Проверка на добавление существующей остановки.
            {
                if(s->stopName.toLower() == ui->lineEdit->text().toLower())
                {
                    QMessageBox::warning(this, "Добавление остановки", "На маршруте уже есть такая остановка");
                    return;
                }
                s = s->stopNext;
            }
        }
        Stop stop2;
        stop2.stop = new Node();                  // Сохранение данных.
        stop2.stop->stopName = ui->lineEdit->text();
        stop2.stopNum = city->getStops().size() == 0 ? 0 : (*(city->getStops().back())).stopNum + 1;
        Stop* zt = TreeAlgorithms<Stop>::findOne(Stop(stop2.stop->stopName), this->city->getStops());
        if(zt != nullptr && route == nullptr)     // Проверка на существование остановки.
        {
            QMessageBox::warning(this, "Добавление остановки", "Остановка уже добавлена в город");
            return;
        }
        if(this->route!=nullptr)
        {
            bool fl = this->route->first == nullptr;
            QListView* lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(2, 2)->widget());
            QListView* lview2 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(3, 2)->widget());
            QStandardItemModel* model = qobject_cast<QStandardItemModel*>(lview->model());
            QStandardItemModel* model2 = qobject_cast<QStandardItemModel*>(lview2->model());
            QListView* lview3 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(5, 2)->widget());
            QListView* lview4 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(6, 2)->widget());
            QStandardItemModel* model3 = qobject_cast<QStandardItemModel*>(lview3->model());
            QStandardItemModel* model4 = qobject_cast<QStandardItemModel*>(lview4->model());
            if(!fl)
            {
                stop2.stop->routeNumber = route->first->routeNumber;
                if(lview->isEnabled())     // Сохранение расписания в рейсы.
                {
                    auto ln = this->route->first->timetableNextWork.begin();
                    auto lp = this->route->first->timetablePrevWork.begin();
                    for(int i = 0; i<model->rowCount(); i++)
                    {
                        stop2.stop->timetableNextWork.push_back(TimeBus(model->index(i, 0).data(Qt::DisplayRole).toString(), ln->bus));
                        ln++;
                    }
                    for(int i = 0; i<model2->rowCount(); i++)
                    {
                        stop2.stop->timetablePrevWork.push_back(TimeBus(model2->index(i, 0).data(Qt::DisplayRole).toString(), lp->bus));
                        lp++;
                    }
                }
                if(lview3->isEnabled())
                {
                    auto ln = this->route->first->timetableNextWeek.begin();
                    auto lp = this->route->first->timetablePrevWeek.begin();
                    for(int i = 0; i<model3->rowCount(); i++)
                    {
                        stop2.stop->timetableNextWeek.push_back(TimeBus(model3->index(i, 0).data(Qt::DisplayRole).toString(), ln->bus));
                        ln++;
                    }
                    for(int i = 0; i<model4->rowCount(); i++)
                    {
                        stop2.stop->timetablePrevWeek.push_back(TimeBus(model4->index(i, 0).data(Qt::DisplayRole).toString(), lp->bus));
                        lp++;
                    }
                }
                stop2.stop->timetableNextWeek.sort(); // Сортировка времени по возрастанию.
                stop2.stop->timetableNextWork.sort();
                stop2.stop->timetablePrevWeek.sort();
                stop2.stop->timetablePrevWork.sort();
                this->route->last->stopNext = stop2.stop;
                stop2.stop->stopPrev = this->route->last;  // Добавление остановки в структуру.
                this->route->last = this->route->last->stopNext;
            }
            else
            {
                QListView* lview01 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(2, 3)->widget());
                QListView* lview02 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(3, 3)->widget());
                QStandardItemModel* model01 = qobject_cast<QStandardItemModel*>(lview01->model());
                QStandardItemModel* model02 = qobject_cast<QStandardItemModel*>(lview02->model());
                QListView* lview03 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(5, 3)->widget());
                QListView* lview04 = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(6, 3)->widget());
                QStandardItemModel* model03 = qobject_cast<QStandardItemModel*>(lview03->model());
                QStandardItemModel* model04 = qobject_cast<QStandardItemModel*>(lview04->model());
                if(lview->isEnabled())                     // Считывание данных.
                {
                    for(int i = 0; i<model->rowCount(); i++)
                    {                                      // Поиск автобуса по номеру.
                        for(auto at = city->getParks().begin(); at != city->getParks().end(); at++)
                        {
                            Bus* bt = TreeAlgorithms<Bus>::findOne(Bus(model01->index(i, 0).data(Qt::DisplayRole).toString()), at->buses);
                            if(bt!=nullptr)
                            {                              // Добавление автобуса и времени в рейс.
                                stop2.stop->timetableNextWork.push_back(TimeBus(model->index(i, 0).data(Qt::DisplayRole).toString(), bt));
                                break;
                            }
                        }
                    }
                    for(int i = 0; i<model2->rowCount(); i++)
                    {
                        for(auto at = city->getParks().begin(); at != city->getParks().end(); at++)
                        {
                            Bus* bt = TreeAlgorithms<Bus>::findOne(Bus(model02->index(i, 0).data(Qt::DisplayRole).toString()), at->buses);
                            if(bt!=nullptr)
                            {                             // Добавление автобуса и времени в рейс.
                                stop2.stop->timetablePrevWork.push_back(TimeBus(model2->index(i, 0).data(Qt::DisplayRole).toString(), bt));
                                break;
                            }
                        }
                    }
                }
                if(lview3->isEnabled())
                {
                    for(int i = 0; i<model3->rowCount(); i++)
                    {
                        for(auto at = city->getParks().begin(); at != city->getParks().end(); at++)
                        {
                            Bus* bt = TreeAlgorithms<Bus>::findOne(Bus(model03->index(i, 0).data(Qt::DisplayRole).toString()), at->buses);
                            if(bt!=nullptr)
                            {                              // Добавление автобуса и времени в рейс.
                                stop2.stop->timetableNextWeek.push_back(TimeBus(model3->index(i, 0).data(Qt::DisplayRole).toString(), bt));
                                break;
                            }
                        }
                    }
                    for(int i = 0; i<model4->rowCount(); i++)
                    {
                        for(auto at = city->getParks().begin(); at != city->getParks().end(); at++)
                        {
                            Bus* bt = TreeAlgorithms<Bus>::findOne(Bus(model04->index(i, 0).data(Qt::DisplayRole).toString()), at->buses);
                            if(bt!=nullptr)
                            {                               // Добавление автобуса и времени в рейс.
                                stop2.stop->timetablePrevWeek.push_back(TimeBus(model4->index(i, 0).data(Qt::DisplayRole).toString(), bt));
                                break;
                            }
                        }
                    }
                }
                stop2.stop->timetableNextWeek.sort();       // Сортировка времени по возрастанию.
                stop2.stop->timetableNextWork.sort();
                stop2.stop->timetablePrevWeek.sort();
                stop2.stop->timetablePrevWork.sort();
                this->route->first = stop2.stop;
                this->route->last = stop2.stop;
                for(int i = 0; i<7; i++)
                    this->route->daysOfWork[i] = false;
            }
        }
        if(zt == nullptr)
        {
            Stop stop1;
            stop1.stop = new Node();
            stop1.stop->stopName = stop2.stop->stopName;
            stop1.stopNum = stop2.stopNum;
            city->getStops().Add(stop1);                    // Добавление остановки в список.
        }
    }
    else
        this->stop->stop->stopName = ui->lineEdit->text();
    accept();
}
void dialogStopWindow::keyPressEvent(QKeyEvent* event)      // Обработчик нажатия клавиш.
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z)
    {
        if(!stack1.empty())                                 // Проверка, что стек не пуст.
        {
            int v = stack2.pop();
            QString temp = stack1.pop();
            QListView* lview;
            if(v==0 || v == 10)                             // Получение модели данных списка.
                lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(2, 2)->widget());
            else if(v==1 || v == 11)
                lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(3, 2)->widget());
            else if(v==2 || v == 12)
                lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(5, 2)->widget());
            else if(v==3 || v == 13)
                lview = qobject_cast<QListView*>(ui->gridLayout->itemAtPosition(6, 2)->widget());
            QStandardItemModel* model = qobject_cast<QStandardItemModel*>(lview->model());
            if(v == 0 || v ==1 || v==2||v==3)
            {         // Поиск  элемента.
                for(int i = 0; i<model->rowCount(); i++)
                    if(model->item(i)->text() == temp)
                    { // Удаление элемента.
                        model->removeRow(i);
                        break;
                    }
            }
            else
            {
                int i = 0;
                while(i<model->rowCount() && model->item(i)->text() < temp)
                    i++;
                QStandardItem* item = new QStandardItem(temp);  // Создание нового объекта.
                item->setEditable(false);
                QFont font = item->font();
                font.setPointSize(14);
                item->setFont(font);
                model->insertRow(i, item);                      // Добавление объекта.
            }
        }
    }
}
