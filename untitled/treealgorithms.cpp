#pragma once
#include "treealgorithms.h"

template <class T>                              // Поиск нескольких элементов в дереве.
MyTree<T> TreeAlgorithms<T>::findEl(const T& el, typename MyTree<T>::MyIterator begin, typename MyTree<T>::MyIterator end)
{
    MyTree<T> result;                           // Результат.
    if (!begin.isnull())
    {
        while(!begin.isnull())                  // Обход дерева.
        {
            if ((*begin).data == el)            // Проверка на совпадение.
                result.Add((*begin).data);      // Добавление в результат.
            begin++;
        }
    }
    return result;                              // Возврат результата.
}

template <class T>                              // Поиск одного элемента.
T* TreeAlgorithms<T>::findOne(T el, MyTree<T>& tree)
{
    if(!tree.empty())
    {
        TreeNode<T>* temp = tree.getRoot();
        while(temp!=nullptr)                    // Бинарный поиск используя сравнение.
        {
            if(temp->data == el)                // Возврат данных если совпало.
                return &(temp->data);
            else if(temp->data > el)            // Переход влево или вправо в зависимости от поля.
                temp = temp->left;
            else
                temp = temp->right;
        }
    }
    return nullptr;
}
