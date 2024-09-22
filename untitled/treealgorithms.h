#pragma once
#include "mytree.h"
#include "mytree.cpp"

template <class T>
class TreeAlgorithms
{
public:                                    // Поиск нескольких элементов.
    static MyTree<T> findEl(const T&, typename MyTree<T>::MyIterator, typename MyTree<T>::MyIterator);
    static T* findOne(T, MyTree<T>&);      // Поиск одного элемента.
};

#include "treealgorithms.cpp"
