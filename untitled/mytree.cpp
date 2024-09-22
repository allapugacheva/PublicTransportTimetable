#pragma once
#include "mytree.h"

template<class T>
typename MyTree<T>::MyIterator MyTree<T>::MyIterator::operator++()
{
    try
    {
        MyIterator temp(*this);
        if (this->cur->right == nullptr)          // Проверка, что правое поддерево не пусто.
        {
            while (this->cur->prev != nullptr && this->cur->data >= this->cur->prev->data)
                this->cur = this->cur->prev;      // Возврат к предыдущему элементу.
            this->cur = this->cur->prev;
        }
        else
        {
            this->cur = this->cur->right;         // Переход к правому поддереву.
            while (this->cur->left != nullptr)    // Переход к самому левому элементу.
                this->cur = this->cur->left;
        }
        return temp;
    }
    catch (ContainerException& ex)
    {
        ex.mesg();
    }
}
template<class T>
int MyTree<T>::size()             // Количество элементов дерева.
{
    return size(this->root);      // Вызов рекурсвиной функции подсчёта.
}
template<class T>
int MyTree<T>::size(TreeNode<T> *n)
{
    if(n==nullptr)
        return 0;
    int left = size(n->left);     // Вызов для левого поддерева.
    int right = size(n->right);   // Вызов для правого поддерева.
    return 1 + left + right;
}
template<class T>                 // Переход к следующему элементу.
typename MyTree<T>::MyIterator MyTree<T>::MyIterator::operator++(int)
{
    try
    {
        if (this->cur->right == nullptr)
        {                         // Переход к предыдущему элементу.
            while (this->cur->prev != nullptr && this->cur->data >= this->cur->prev->data)
                this->cur = this->cur->prev;
            this->cur = this->cur->prev;
        }
        else
        {                         // Переход на правое поддерево.
            this->cur = this->cur->right;
            while (this->cur->left != nullptr)
                this->cur = this->cur->left;
        }                         // Переход к самому левому элементу.
        return *this;
    }
    catch (ContainerException& ex)
    {
        ex.mesg();
    }
}
template<class T>                 // Переход к предыдущему элементу.
typename MyTree<T>::MyIterator MyTree<T>::MyIterator::operator--()
{
    try
    {
        MyIterator temp(*this);
        if (this->cur->left == nullptr)
        {                        // Переход к предыдущему элементу.
            while (this->cur->prev != nullptr && this->cur->data <= this->cur->prev->data)
                this->cur = this->cur->prev;
            this->cur = this->cur->prev;
        }
        else
        {                        // Переход на левое поддерево.
            this->cur = this->cur->left;
            while (this->cur->right != nullptr)
                this->cur = this->cur->right;
        }                        // Переход к самому правому поддереву.
        return temp;
    }
    catch (ContainerException& ex)
    {
        ex.mesg();
    }
}
template<class T>                // Переход к предыдущему элементу.
typename MyTree<T>::MyIterator MyTree<T>::MyIterator::operator--(int)
{
    try
    {
        if (this->cur->left == nullptr)
        {
            while (this->cur->prev != nullptr && this->cur->data <= this->cur->prev->data)
                this->cur = this->cur->prev;
            this->cur = this->cur->prev;
        }
        else
        {
            this->cur = this->cur->left;
            while (this->cur->right != nullptr)
                this->cur = this->cur->right;
        }
        return *this;
    }
    catch (ContainerException& ex)
    {
        ex.mesg();
    }
}
template <class T>
bool MyTree<T>::MyIterator::isnull()
{
    return this->cur == nullptr;  // Проверка, что указатель не пуст.
}
template<class T>
typename MyTree<T>::MyIterator MyTree<T>::begin() // Переход к первому элементу дерева.
{
    if (this->getRoot() == nullptr)
        return MyIterator(nullptr);
    TreeNode<T>* temp = this->getRoot();
    while (temp->left != nullptr)
        temp = temp->left;                        // Переход к самому левому элементу.
    return MyIterator(temp);
}
template<class T>
typename MyTree<T>::MyIterator MyTree<T>::back()  // Переход к последнему элементу дерева.
{
    if (this->getRoot() == nullptr)
        return MyIterator(nullptr);
    TreeNode<T>* temp = this->getRoot();
    while (temp->right != nullptr)
        temp = temp->right;                       // Переход к самому правому элементу.
    return MyIterator(temp);
}
template<class T>
typename MyTree<T>::MyIterator MyTree<T>::end()   // Установка итератора в конечное состояние.
{
    return MyIterator(nullptr);                   // Задание ему nullptr.
}
template<class T>
MyTree<T>::MyIterator::MyIterator()               // Конструкторы.
{
    this->cur = nullptr;
}
template<class T>
MyTree<T>::MyIterator::MyIterator(TreeNode<T>* cur)
{
    this->cur = cur;
}
template<class T>
MyTree<T>::MyIterator::MyIterator(const MyIterator& it)
{
    this->cur = it.cur;
}
template<class T>                                 // Оператор присваивания.
typename MyTree<T>::MyIterator MyTree<T>::MyIterator::operator=(const MyIterator& it)
{
    this->cur = it.cur;                           // Присваивание значения.
    return *this;
}
template<class T>
T& MyTree<T>::MyIterator::operator*()             // Оператор разыменовывания.
{
    return this->cur->data;
}
template<class T>                                 // Оператор сравнения.
bool MyTree<T>::MyIterator::operator==(const MyIterator& it)
{
    return this->cur == it.cur;
}
template<class T>
bool MyTree<T>::MyIterator::operator!=(const MyIterator& it)
{
    return this->cur != it.cur;
}
template<class T>
TreeNode<T>*& MyTree<T>::getRoot()                // Метод получения корня дерева.
{
    return this->root;                            // Возврат указателя на корень.
}

template<class T>                                 // Сеттер для корня.
void MyTree<T>::setRoot(TreeNode<T>*& root)
{
    this->root = root;
}
template <class T>
void MyTree<T>::Add(T& el)                         // Вызов метода добавления элемента.
{
    this->Add(el, this->root);                     // Вызов метода, указав корень.
}

template <class T>
void MyTree<T>::Add(T& el, TreeNode<T>*& node)     // Добавление элемента в поддерево.
{
    if (node == nullptr)                           // Проверка, что дерево пусто.
        node = new TreeNode<T>(el);                // Создание нового элемента.
    else if (el < node->data)                      // Проверка, что элемент меньше данных в узле.
    {
        if (node->left == nullptr)                 // Проверка, что левый указатель пуст.
        {
            node->left = new TreeNode<T>(el);      // Создание нового узла.
            node->left->prev = node;               // Установка указателя на предыдущий узел.
        }
        else
            this->Add(el, node->left);             // Переход к левому узлу.
    }
    else
    {
        if (node->right == nullptr)                // Проверка, что правый указатель пуст.
        {
            node->right = new TreeNode<T>(el);     // Создание нового узла.
            node->right->prev = node;              // Установка указателя на предыдущий узел.
        }
        else
            this->Add(el, node->right);            // Переход к правому узлу.
    }
    this->balance(node);                           // Балансировка узла дерева.
}

template <class T>
bool MyTree<T>::empty()
{
    return this->root == nullptr;                  // Проверка, что корень дерева пуст.
}

template <class T>
TreeNode<T>*& MyTree<T>::parentChildNode(TreeNode<T>*& node) // Метод получения указателя на элемент из родительского элемента.
{
    if (node != nullptr)                                     // Проверка, что элемента не пуст.
    {
        if (node->prev->left == node)                        // Проверка, что элемент является левым элементом у родительского.
            return node->prev->left;                         // Возврат указателя на левый элемент родительского дерева.
        else
            return node->prev->right;                        // Возврат указателя на правый элемент родительского дерева.
    }
    return node;
}

template <class T>
void MyTree<T>::Remove(T& el)                      // Вызов метода удаления элемента.
{
    if (!this->empty())
        this->RemoveMany(el, this->root);          // Вызов метода удаления дерева, указав корень.
}
template <class T>
void MyTree<T>::RemoveMany(T& el, TreeNode<T>*& node)
{
    if (node != nullptr)
    {
        if (el == node->data)                       // Обход дерева для поиска всех удовлетвоярющих элементов.
        {
            this->Remove(el, node);
            this->RemoveMany(el, node);
        }
        else
        {
            this->RemoveMany(el, node->left);
            this->RemoveMany(el, node->right);
        }
    }
}
template <class T>
void MyTree<T>::Remove(T& el, TreeNode<T>*& node)      // Метод удаления элемента.
{
    if (node == nullptr)                               // Проверка, что указатель не пуст.
        return;
    if (node->data == el)
    {
        TreeNode<T>* temp = node;                      // Проверка, что удаляется корень.
        if (node == this->root && (node->right == nullptr || node->left == nullptr))
        {
            if (node->right == nullptr)
            {
                node = node->left;
                if (node != nullptr)
                    node->prev = nullptr;              // Установка корня на левое поддерево.
            }
            else
            {
                node = node->right;                    // Установка корня на правое поддерево.
                if (node != nullptr)
                    node->prev = nullptr;
            }
            return;
        }
        if (node->left != nullptr)                       // Проверка, что левое поддерево не пусто.
        {
            if (node->right == nullptr)                  // Проверка, что правое поддерево пусто.
            {
                node->left->prev = node->prev;           // Перемена предыдущего указателя.
                if (node->prev != nullptr)               // Установка указателя предыдущего элемента на следующий.
                    parentChildNode(node) = node->left;
                delete temp;                             // Очистка удалённого элемента.
            }
            else
            {
                temp = node->left;                       // Установка временного указателя на левое поддерево.
                while (temp->right != nullptr)           // Переход к самому правому элементу.
                    temp = temp->right;
                node->data = temp->data;                 // Обмен данных.
                this->Remove(temp->data, temp);          // Удаление элемента.
            }
        }
        else
        {
            if (node->right != nullptr)                  // Перемена предыдущего указателя.
                node->right->prev = node->prev;
            if (node->prev != nullptr)                   // Установка указателя предыдущего элемента на следующий.
                parentChildNode(node) = node->right;
            else
                node = node->right;
            delete temp;                                 // Удаление элемента.
        }
    }
    else if (node->data > el)                            // Проверка, что элемент меньше элемента узла.
    {
        this->Remove(el, node->left);                    // Рекурсивный переход к левому узлу.
        this->balance(node);                             // Балансировка узла.
    }
    else if (node->data < el)                            // Проверка, что элемент больше элемента узла.
    {
        this->Remove(el, node->right);                   // Рекурсивный переход к правому узлу.
        this->balance(node);                             // Балансировка узла.
    }
}

template <class T>
void MyTree<T>::LeftShow()                               // Вызов метода левостороннего обхода.
{
    this->LeftShow(this->root);                          // Вызов метода с указанием корня.
}

template <class T>
void MyTree<T>::LeftShow(TreeNode<T>* node)              // Левосторонний обход.
{
    if (node != nullptr)                                 // Проверка, что элемент не пуст.
    {
        qDebug() << node->data;                          // Вывод информации на экран.
        this->LeftShow(node->left);                      // Переход к левому дереву.
        this->LeftShow(node->right);                     // Переход к правому дереву.
    }
}

template <class T>
void MyTree<T>::RightShow()                              // Вызов метода правостороннего обхода.
{
    this->RightShow(this->root);                         // Вызов метода с указанием корня.
}

template <class T>
void MyTree<T>::RightShow(TreeNode<T>* node)             // Правосторонний обход.
{
    if (node != nullptr)                                 // Проверка, что элемент не пуст.
    {
        this->RightShow(node->right);                    // Переход к правому поддереву.
        this->RightShow(node->left);                     // Переход к левому поддереву.
        qDebug() << node->data;                          // Вывод информации на экран.
    }
}

template <class T>
void MyTree<T>::SimmetricalShow()                        // Вызов метода симметричного обхода.
{
    this->SimmetricalShow(this->root);                   // Вызов метода с указанием корня.
}

template <class T>
void MyTree<T>::SimmetricalShow(TreeNode<T>* node)       // Симметричный обход.
{
    if (node != nullptr)                                 // Проверка, что элемент не пуст.
    {
        this->SimmetricalShow(node->left);               // Переход к левому поддереву.
        qDebug() << node->data;                          // Вывод информации на экран.
        this->SimmetricalShow(node->right);              // Переход к правому поддереву.
    }
}

template <class T>
void MyTree<T>::rightRotate(TreeNode<T>*& node)                          // Метод правого поворота поддерева.
{
    TreeNode<T>* temp = node->left;                                      // Сохранение левого поддерева.
    node->left = temp->right;                                            // Замена левого узла поддерева правым узлом левого узла.
    if (node->left != nullptr)                                           // Установка указателя на предыдущий.
        node->left->prev = node;
    temp->prev = node->prev;                                             // Замена указателей на предыдущий элемент.
    temp->right = node;                                                  // Замена правого узла левого узла поддерева на само поддерево.
    if (temp->right != nullptr)                                          // Установка указателя на предыдущий.
        temp->right->prev = temp;
    node = temp;                                                         // Замена узлов.
}

template <class T>
void MyTree<T>::leftRotate(TreeNode<T>*& node)                           // Метод левого поворота поддерева.
{
    TreeNode<T>* temp = node->right;                                     // Сохранение правого поддерева.
    node->right = temp->left;                                            // Замена правого узла поддерева левым узлом парвого узла.
    if (node->right != nullptr)                                          // Установка указателя на предыдущий.
        node->right->prev = node;
    temp->prev = node->prev;                                             // Замена указателей на предыдущий элемент.
    temp->left = node;                                                   // Замена левого указателя правого узла на само поддерево.
    if (temp->left != nullptr)                                           // Установка указателя на предыдущий.
        temp->left->prev = temp;
    node = temp;                                                         // Замена узлов.
}

template <class T>
int MyTree<T>::treeHeight(TreeNode<T>*& node)                            // Высота дерева.
{
    if (node == nullptr)                                                 // Проверка, что указатель пуст.
        return 0;
    return 1 + max(treeHeight(node->left), treeHeight(node->right));     // Возврат максимальной из высот поддеревьев + 1.
}

template <class T>
void MyTree<T>::balance(TreeNode<T>*& node)                                          // Балансировка дерева.
{
    if (node == nullptr)                                                             // Проверка, что дерево пусто.
        return;
    if (treeHeight(node->right) - treeHeight(node->left) >= 2)                       // Проверка, что правое поддерево значительно больше левого поддерева.
    {
        if (treeHeight(node->right->right) - treeHeight(node->right->left) < 0)      // Проверка, что правое поддерево правого поддерева значительно больше левого поддерева.
            rightRotate(node->right);                                                // Правый поворот правого поддерева.
        leftRotate(node);                                                            // Левый поворот дерева.
    }
    if (treeHeight(node->right) - treeHeight(node->left) <= -2)                      // Проверка, что левое поддерево значительно больше правого поддерева.
    {
        if (treeHeight(node->left->right) - treeHeight(node->left->left) > 0)        // Проверка, что правое поддерево левого поддерева значительно больше левого поддерева.
            leftRotate(node->left);                                                  // Левый поворот левого поддерева.
        rightRotate(node);                                                           // Правый поворот дерева.
    }
}
