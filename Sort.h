#ifndef SORT_SORT_H
#define SORT_SORT_H

#include <utility>
#include <string>
#include <functional>


class Sort
{
public:
    static size_t assign_count;
    static size_t compare_count;

    static bool compare(int lhs, int rhs)
    {
        ++compare_count;
        return lhs < rhs;
    }

    static int assign(int rhs)
    {
        ++assign_count;
        return rhs;
    }

    static void swap(int& lhs, int& rhs)
    {
        assign_count += 3;
        std::swap(lhs, rhs);
    }

    static void c_qsort(int* first, int* last);
    static void straight_insertion_sort(int* first, int* last);
    static void binary_insertion_sort(int* first, int* last);
    static void two_way_insertion_sort(int* first, int* last);
    static void shell_sort(int* first, int* last);
    static void bubble_sort(int* first, int* last);
    static void quicksort(int* first, int* last);
    static void selection_sort(int* first, int* last);
    static void heapsort(int* first, int* last);
    static void merge_sort(int* first, int* last);
    static void radix_sort(int* first, int* last);

    static void reset()
    {
        compare_count = 0;
        assign_count = 0;
    }
};


#endif //SORT_SORT_H
