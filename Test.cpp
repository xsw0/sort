#include "Test.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

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
        std::cout << "第" << i + 1 << "次测试" << std::endl;

        std::cout << "长度：" << cases[i].size() << std::endl;
        tot_length += cases[i].size();
        for (auto n : cases[i]) std::cout << " " << n;
        std::cout << std::endl;

        auto std_sort_result = cases[i];
        {
            Sort::reset();
            std::cout << "标准库排序" << std::endl;
            auto run_time = time_cost([&]() {
                std::sort(std_sort_result.begin(), std_sort_result.end(), Sort::compare);
            });
            std_sort_tot_run_time += run_time;
            std::cout << "运行时间\t" << run_time << " 微秒" << std::endl;
            std_sort_tot_compare_time += Sort::compare_count;
            std::cout << "比较次数\t" << Sort::compare_count << std::endl;

            std::cout << "排序结果：" << std::endl;
            for (auto n : std_sort_result) std::cout << " " << n;
            std::cout << std::endl;
            std::cout << std::endl;
        }

        {
            Sort::reset();
            std::cout << "c_qsort" << std::endl;
            auto result = cases[i];
            auto run_time = once_time_cost(Sort::c_qsort, result);
            c_qsort_tot_run_time += run_time;
            std::cout << "运行时间\t" << run_time << " 微秒" << std::endl;
            if (result == std_sort_result)
            {
                std::cout << "结果正确" << std::endl;
            }
            else
            {
                std::cerr << "结果错误" << std::endl;
                exit(-1);
            }
            std::cout << std::endl;
        }

        for (auto j = 0; j < sorts.size(); ++j)
        {
            Sort::reset();
            std::cout << sorts[j].first << std::endl;
            auto result = cases[i];
            auto run_time = once_time_cost(sorts[j].second, result);
            my_sort_tot_run_time[j] += run_time;
            std::cout << "运行时间\t" << run_time << " 微秒" << std::endl;
            my_sort_tot_compare_count[j] += Sort::compare_count;
            std::cout << "比较次数\t" << Sort::compare_count << std::endl;
            my_sort_tot_assign_count[j] += Sort::assign_count;
            std::cout << "移动次数\t" << Sort::assign_count << std::endl;
            if (result == std_sort_result)
            {
                std::cout << "结果正确" << std::endl;
            }
            else
            {
                std::cerr << "结果错误" << std::endl;
                exit(-1);
            }
            std::cout << std::endl;
        }
    }

    std::cout << "测试平均长度：\t" <<
              static_cast<double>(tot_length) /
              static_cast<double>(cases.size())
              << std::endl;
    std::cout << std::endl;

    std::cout << "标准库排序：" << std::endl;
    std::cout << "平均运行时间\t" <<
              static_cast<double>(std_sort_tot_run_time) /
              static_cast<double>(cases.size())
              << " 微秒" << std::endl;
    std::cout << "平均比较次数\t" <<
              static_cast<double>(std_sort_tot_compare_time) /
              static_cast<double>(cases.size())
              << std::endl;
    std::cout << std::endl;

    std::cout << "c_qsort:" << std::endl;
    std::cout << "平均运行时间\t" <<
              static_cast<double>(c_qsort_tot_run_time) /
              static_cast<double>(cases.size())
              << " 微秒" << std::endl;
    std::cout << std::endl;

    for (auto i = 0; i < sorts.size(); ++i)
    {
        std::cout << sorts[i].first << "：" << std::endl;
        std::cout << "平均运行时间\t" <<
                  static_cast<double>(my_sort_tot_run_time[i]) /
                  static_cast<double>(cases.size())
                  << " 微秒" << std::endl;
        std::cout << "平均比较次数\t" <<
                  static_cast<double>(my_sort_tot_compare_count[i]) /
                  static_cast<double>(cases.size())
                  << std::endl;
        std::cout << "平均移动次数\t" <<
                  static_cast<double>(my_sort_tot_assign_count[i]) /
                  static_cast<double>(cases.size())
                  << std::endl;
        std::cout << std::endl;
    }
}
