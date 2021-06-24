#ifndef SORT_TEST_H
#define SORT_TEST_H

#include <vector>
#include "Sort.h"

class Test
{
    std::vector<std::vector<int>> cases;
    std::vector<std::pair<std::string, std::function<void(int*, int*)>>> sorts;

    size_t tot_length = 0;

    size_t std_sort_tot_run_time = 0;
    size_t std_sort_tot_compare_time = 0;
    size_t c_qsort_tot_run_time = 0;

    std::vector<size_t> my_sort_tot_run_time;
    std::vector<size_t> my_sort_tot_compare_count;
    std::vector<size_t> my_sort_tot_assign_count;

    static auto time_cost(const std::function<void()>& function);
    static auto once_time_cost(const std::function<void(int*, int*)>& sort, std::vector<int>& n);
    void test();
public:
    Test(
        const std::vector<std::pair<std::string, std::function<void(int*, int*)>>>& sorts,
        std::vector<std::vector<int>> cases
    ) : cases(std::move(cases)),
        sorts{ sorts },
        my_sort_tot_run_time(sorts.size(), 0),
        my_sort_tot_compare_count(sorts.size(), 0),
        my_sort_tot_assign_count(sorts.size(), 0)
    {
        test();
    };
};


#endif //SORT_TEST_H
