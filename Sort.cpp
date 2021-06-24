#include "Sort.h"

#include <random>

size_t Sort::assign_count = 0;
size_t Sort::compare_count = 0;

void Sort::quicksort(int* first, int* last)
{
    static std::default_random_engine re{ std::random_device{}() };

    if (first == last) return;

    size_t size = std::distance(first, last);
    auto pivot = first;
    auto lm1 = std::prev(last);

    swap(first[re() % size], *lm1);

    for (auto it = first; it != last; ++it)
    {
        if (compare(*it, *lm1))
        {
            swap(*pivot, *it);
            ++pivot;
        }
    }

    swap(*pivot, *lm1);

    quicksort(first, pivot);
    quicksort(std::next(pivot), last);
}

void Sort::straight_insertion_sort(int* first, int* last)
{
    if (first == last) return;
    for (auto pivot = std::next(first); pivot != last; ++pivot)
    {
        int value = assign(*pivot);
        auto it = pivot;
        for (; it != first && compare(value, *std::prev(it)); --it)
        {
            *it = assign(*std::prev(it));
        }
        *it = assign(value);
    }
}

void Sort::binary_insertion_sort(int* first, int* last)
{
    if (first == last) return;
    for (auto pivot = std::next(first); pivot != last; ++pivot)
    {
        int value = assign(*pivot);

        auto lower = first;
        auto upper = pivot;

        while (lower != upper)
        {
            auto mid = lower + std::distance(lower, upper) / 2;
            if (compare(value, *mid))
            {
                upper = mid;
            }
            else
            {
                lower = mid + 1;
            }
        }

        for (auto it = pivot; it != lower; --it)
        {
            *it = assign(*std::prev(it));
        }
        *lower = assign(value);
    }
}

void Sort::two_way_insertion_sort(int* first, int* last)
{
    if (first == last) return;

    size_t size = std::distance(first, last);
    int* two_way = new int[size];

    *two_way = assign(*first);

    auto last_of_big = std::next(two_way);
    auto first_of_small = two_way + size;

    for (auto pivot = std::next(first); pivot != last; ++pivot)
    {
        if (compare(*pivot, *two_way))
        {
            auto dest = std::upper_bound(first_of_small, two_way + size, *pivot, compare);
            for (auto it = first_of_small; it != dest; ++it)
            {
                *std::prev(it) = assign(*it);
            }
            *std::prev(dest) = assign(*pivot);
            --first_of_small;
        }
        else
        {
            auto dest = std::upper_bound(std::next(two_way), last_of_big, *pivot, compare);
            for (auto it = last_of_big; it != dest; --it)
            {
                *it = assign(*std::prev(it));
            }
            *dest = assign(*pivot);
            ++last_of_big;
        }
    }

    assert(last_of_big == first_of_small);

    while (first_of_small != two_way + size)
    {
        *first = assign(*first_of_small);
        ++first;
        ++first_of_small;
    }

    first_of_small = two_way;
    while (first_of_small != last_of_big)
    {
        *first = assign(*first_of_small);
        ++first;
        ++first_of_small;
    }

    delete[] two_way;
}

void Sort::shell_sort(int* first, int* last)
{
    size_t gap = std::distance(first, last) / 2;
    while (gap != 0)
    {
        for (auto offset = first; offset != first + gap; ++offset)
        {
            for (auto pivot = offset + gap; pivot < last; pivot += gap)
            {
                int value = assign(*pivot);
                auto it = pivot;
                for (; it != offset && compare(value, *(it - gap)); it -= gap)
                {
                    *it = assign(*(it - gap));
                }
                *it = assign(value);
            }
        }
        gap /= 2;
    }
}

void Sort::bubble_sort(int* first, int* last)
{
    while (first != last)
    {
        --last;
        for (auto it = first; it != last; ++it)
        {
            if (compare(*std::next(it), *it))
            {
                swap(*it, *std::next(it));
            }
        }
    }
}

void Sort::selection_sort(int* first, int* last)
{
    while (first != last)
    {
        auto minimum = first;
        for (auto it = std::next(first); it != last; ++it)
        {
            if (compare(*it, *minimum))
            {
                minimum = it;
            }
        }
        swap(*first, *minimum);
        ++first;
    }
}

static void bubble_down(int* first, size_t size, size_t index)
{
    if (index * 2 + 1 < size && Sort::compare(first[index], first[index * 2 + 1]))
    {
        if (index * 2 + 2 < size && Sort::compare(first[index * 2 + 1], first[index * 2 + 2]))
        {
            Sort::swap(first[index], first[index * 2 + 2]);
            bubble_down(first, size, index * 2 + 2);
        }
        else
        {
            Sort::swap(first[index], first[index * 2 + 1]);
            bubble_down(first, size, index * 2 + 1);
        }
    }
    else if (index * 2 + 2 < size && Sort::compare(first[index], first[index * 2 + 2]))
    {
        Sort::swap(first[index], first[index * 2 + 2]);
        bubble_down(first, size, index * 2 + 2);
    }
}

void Sort::heapsort(int* first, int* last)
{
    if (first == last) return;
    size_t size = std::distance(first, last);
    size_t index = size / 2;
    while (index-- != 0) bubble_down(first, size, index);

    while (first != --last)
    {
        swap(*first, *last);
        --size;
        bubble_down(first, size, 0);
    }
}

void Sort::merge_sort(int* first, int* last)
{
    size_t size = std::distance(first, last);
    if (size < 2) return;
    auto mid = first + size / 2;
    merge_sort(first, mid);
    merge_sort(mid, last);
    int* merge = new int[size];
    auto l = first;
    auto r = mid;
    for (size_t i = 0;; ++i)
    {
        if (compare(*l, *r))
        {
            merge[i] = assign(*l);
            ++l;
            if (l == mid)
            {
                while (r != last)
                {
                    ++i;
                    merge[i] = assign(*r);
                    ++r;
                }
                break;
            }
        }
        else
        {
            merge[i] = assign(*r);
            ++r;
            if (r == last)
            {
                while (l != mid)
                {
                    ++i;
                    merge[i] = assign(*l);
                    ++l;
                }
                break;
            }
        }
    }
    for (size_t i = 0; first != last; ++i, ++first)
    {
        *first = merge[i];
    }
    delete[] merge;
}

static bool radix(int i, size_t index)
{
    if (index < 31)
    {
        return (1 << index) & i;
    }
    else
    {
        return i >= 0;
    }
}

void Sort::radix_sort(int* first, int* last)
{
    size_t size = std::distance(first, last);
    int* bucket = new int[size];
    for (size_t i = 0; i < 32; ++i)
    {
        size_t l = 0;
        size_t r = 0;
        for (auto it = first; it != last; ++it)
        {
            if (!radix(*it, i)) ++r;
        }
        for (auto it = first; it != last; ++it)
        {
            if (radix(*it, i))
            {
                bucket[r++] = assign(*it);
            }
            else
            {
                bucket[l++] = assign(*it);
            }
        }
        for (size_t j = 0; j != size; ++j)
        {
            first[j] = assign(bucket[j]);
        }
    }
    delete[] bucket;
}

void Sort::c_qsort(int* first, int* last)
{
    qsort(first, std::distance(first, last), sizeof(int),
        [](const void* lhs, const void* rhs) -> int {
            auto lhs_value = *static_cast<const int*>(lhs);
            auto rhs_value = *static_cast<const int*>(rhs);
            return (lhs_value > rhs_value) - (lhs_value < rhs_value);
        });
}
