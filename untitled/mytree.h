#pragma once
#include <iostream>
#include "containerexception.h"
#include <cmath>
using namespace std;

template <class T>
struct TreeNode                         // Узел дерева.
{
    T data;                             // Данные.
    TreeNode* prev;                     // Указатель на предыдущий элемент.
    TreeNode* left;                     // Указатель на левый элемент.
    TreeNode* right;                    // Указатель на правый элемент.
    TreeNode(T data)                    // Конструктор.
    {
        this->data = data;
        this->prev = nullptr;
        this->left = nullptr;
        this->right = nullptr;
    }
};

template <class T>
class MyTree
{
    TreeNode<T>* root;                              // Корень дерева.
    TreeNode<T>*& parentChildNode(TreeNode<T>*&);   // Метод получения указателя предыдущего элемента на следующий элемент.
public:
    class MyIterator                                // Класс итераторю
    {
        TreeNode<T>* cur;                           // Указатель на текущий узел дерева.
    public:
        MyIterator();                               // Конструкторы.
        MyIterator(TreeNode<T>*);
        MyIterator(const MyIterator&);
        MyIterator operator=(const MyIterator&);    // Перегрузка операторов.
        MyIterator operator++();
        MyIterator operator++(int);
        MyIterator operator--();
        MyIterator operator--(int);
        T& operator*();
        bool operator==(const MyIterator&);
        bool operator!=(const MyIterator&);
        bool isnull();                              // Проверка пуст ли итератор.
    };
    MyIterator begin();                             // Получение итератора на первый элемент.
    MyIterator back();                              // Получение итератора на последний элемент.
    MyIterator end();                               // Получение итератора на конец.

    MyTree() { this->root = nullptr; }                                // Конструкторы.
    MyTree(const MyTree<T>& tree) { this->root = tree.root; }
    ~MyTree() { while (root != nullptr) Remove(this->root->data); }   // Деструктор.
    bool empty();                                                     // Пусто ли дерево.
    int size();                                                       // Количество элементов дерева.
    int size(TreeNode<T>* n);
    TreeNode<T>*& getRoot();                                          // Получение корня дерева.
    void setRoot(TreeNode<T>*& root);
    int treeHeight(TreeNode<T>*&);                                    // Глубина дерева.

    void Add(T&);                                                     // Вставка элемента.
    void Add(T&, TreeNode<T>*&);
    void Remove(T&);                                                  // Удаление элемента.
    void RemoveMany(T&, TreeNode<T>*&);
    void Remove(T&, TreeNode<T>*&);

    void RightShow();                                                 // Правосторонний обход.
    void RightShow(TreeNode<T>*);
    void LeftShow();                                                  // Левосторонний обход.
    void LeftShow(TreeNode<T>*);
    void SimmetricalShow();                                           // Симметричный обход.
    void SimmetricalShow(TreeNode<T>*);

    void rightRotate(TreeNode<T>*&);                                  // Правый поворот поддерева.
    void leftRotate(TreeNode<T>*&);                                   // Левый поворот поддерева.
    void balance(TreeNode<T>*&);                                      // Балансировка.
};

#include "mytree.cpp"
