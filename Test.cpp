#include "Test.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>

#include "Sort.h"

auto Test::time_cost(const std::function<void()>& function)
{
    auto start = std::chrono::steady_clock::now();
    function();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

auto Test::once_time_cost(const std::function<void(int*, int*)>& sort, std::vector<int>& n)
{
    return time_cost([&]() {
        sort(n.begin().base(), n.end().base());
    });
}

void Test::test()
{
    for (size_t i = 0; i < cases.size(); ++i)
    {
        tot_length += cases[i].size();

        std::cout << "第" << i + 1 << "次测试：";
        std::cout << "长度：" << cases[i].size() << std::endl;
        for (auto n : cases[i]) std::cout << n << " ";
        std::cout << std::endl << std::endl;

        auto std_sort_result = cases[i];
        {
            Sort::reset();

            auto run_time = time_cost([&]() {
                std::sort(std_sort_result.begin(), std_sort_result.end(), Sort::compare);
            });

            std_sort_tot_run_time += run_time;
            std_sort_tot_compare_time += Sort::compare_count;

            std::cout << std::left << std::setw(32) << "标准库排序：";

            std::cout << std::left << std::setw(16) << "\t运行时间："
                      << std::right << std::setw(16) << run_time << " 微秒"
                      << std::setw(16) << "";
            std::cout << std::left << std::setw(16) << "\t比较次数："
                      << std::right << std::setw(16) << Sort::compare_count
                      << std::setw(16) << "";
            std::cout << std::endl << std::endl;

            std::cout << "排序结果：" << std::endl;
            for (auto n : std_sort_result) std::cout << n << " ";
            std::cout << std::endl;
            std::cout << std::endl;
        }

        {
            Sort::reset();
            auto result = cases[i];
            auto run_time = once_time_cost(Sort::c_qsort, result);
            c_qsort_tot_run_time += run_time;

            std::cout << std::left << std::setw(32) << "c_qsort：";

            std::cout << std::left << std::setw(16) << "\t运行时间："
                      << std::right << std::setw(16) << run_time << " 微秒"
                      << std::setw(16) << "";
            std::cout << std::left << std::setw(16 * 6) << "";

            if (result != std_sort_result)
            {
                std::cerr << "\t结果错误" << std::endl;
                exit(-1);
            }

            std::cout << "\t结果正确" << std::endl;
        }

        for (auto j = 0; j < sorts.size(); ++j)
        {
            Sort::reset();
            auto result = cases[i];
            auto run_time = once_time_cost(sorts[j].second, result);
            my_sort_tot_run_time[j] += run_time;
            my_sort_tot_compare_count[j] += Sort::compare_count;
            my_sort_tot_assign_count[j] += Sort::assign_count;

            std::cout << std::left << std::setw(32) << sorts[j].first + "：";

            std::cout << std::left << std::setw(16) << "\t运行时间："
                      << std::right << std::setw(16) << run_time << " 微秒"
                      << std::setw(16) << "";
            std::cout << std::left << std::setw(16) << "\t比较次数："
                      << std::right << std::setw(16) << Sort::compare_count
                      << std::setw(16) << "";
            std::cout << std::left << std::setw(16) << "\t移动次数："
                      << std::right << std::setw(16) << Sort::assign_count
                      << std::setw(16) << "";

            if (result != std_sort_result)
            {
                std::cerr << "\t结果错误" << std::endl;
                exit(-1);
            }

            std::cout << "\t结果正确" << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "测试平均长度：" <<
              static_cast<double>(tot_length) /
              static_cast<double>(cases.size())
              << std::endl;

    std::cout << std::left << std::setw(32) << "标准库排序："
              << std::left << std::setw(16) << "\t平均运行时间："
              << std::right << std::setw(16)
              << static_cast<double>(std_sort_tot_run_time) /
                 static_cast<double>(cases.size())
              << std::left << std::setw(16) << " 微秒"
              << std::left << std::setw(16) << "\t平均比较次数："
              << std::right << std::setw(16)
              << static_cast<double>(std_sort_tot_compare_time) /
                 static_cast<double>(cases.size())
              << std::endl;

    std::cout << std::left << std::setw(24) << "c_qsort:"
              << std::left << std::setw(16) << "\t平均运行时间："
              << std::right << std::setw(16)
              << static_cast<double>(c_qsort_tot_run_time) /
                 static_cast<double>(cases.size())
              << std::left << std::setw(16) << " 微秒"
              << std::endl;

    for (auto i = 0; i < sorts.size(); ++i)
    {
        std::cout << std::left << std::setw(32) << sorts[i].first + "："
                  << std::left << std::setw(16) << "\t平均运行时间："
                  << std::right << std::setw(16)
                  << static_cast<double>(my_sort_tot_run_time[i]) /
                     static_cast<double>(cases.size())
                  << std::left << std::setw(16) << " 微秒"
                  << std::left << std::setw(16) << "\t平均比较次数："
                  << std::right << std::setw(16)
                  << static_cast<double>(my_sort_tot_compare_count[i]) /
                     static_cast<double>(cases.size())
                  << std::left << std::setw(16) << "\t平均移动次数："
                  << std::right << std::setw(16)
                  << static_cast<double>(my_sort_tot_assign_count[i]) /
                     static_cast<double>(cases.size())
                  << std::endl;
    }
}
