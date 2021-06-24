#include <random>
#include <filesystem>
#include <sstream>
#include <fstream>

#include "console.h"
#include "Menu.h"
#include "Test.h"

std::vector<int> generateTestCases(size_t size)
{
    static std::default_random_engine re(std::random_device{}());

    static std::uniform_int_distribution<int> rand_int(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    );

    std::vector<int> result;
    for (size_t i = 0; i < size; ++i)
    {
        result.emplace_back(rand_int(re));
    }
    return result;
}

auto auto_test()
{
    std::cout << "请输入测试次数和最大长度" << std::endl;
    size_t test_num, max_length;
    std::cin >> test_num >> max_length;
    while (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore();
        std::cerr << "输入错误，请重新输入" << std::endl;
        std::cin >> test_num >> max_length;
    }
    static std::default_random_engine re(std::random_device{}());
    std::uniform_int_distribution<size_t> rand_size(0, max_length);
    std::vector<std::vector<int>> cases;
    cases.reserve(test_num);
    for (size_t i = 0; i < test_num; ++i)
    {
        cases.emplace_back(generateTestCases(rand_size(re)));
    }
    return cases;
}

auto test_form_file()
{
    std::cout << "请输入文件名" << std::endl;
    std::string path = console<std::string>([&](const std::string& s) {
        return std::__fs::filesystem::exists(path);
    });
    std::__fs::filesystem::directory_entry entry{ path };
    std::cout << absolute(entry.path()) << std::endl;
    std::ifstream if_strm{ path };
    std::vector<std::vector<int>> cases;
    std::string line;
    while (std::getline(if_strm, line))
    {
        cases.emplace_back();
        std::stringstream s_stream(line);
        int n;
        while (s_stream >> n)
        {
            cases.back().emplace_back(n);
        }
    }
    return cases;
}

auto test_form_input()
{
    std::vector<std::vector<int>> cases;
    std::cout << "请输入测试组数" << std::endl;
    auto test_num = console<size_t>();
    cases.resize(test_num);
    for (size_t i = 0; i < test_num; ++i)
    {
        std::cout << "请输入第" << i + 1 << "组元素个数" << std::endl;
        auto ele_num = console<size_t>();
        cases[i].reserve(ele_num);
        std::cout << "请输入" << ele_num << "个元素" << std::endl;
        for (size_t j = 0; j < ele_num; ++j)
        {
            cases[i].emplace_back(console<int>());
        }
    }
    return cases;
}

int main(int argc, char* argv[])
{
    if (argc != 1)
    {
        std::cerr << "本程序不接受参数" << std::endl;
        return -1;
    }

    std::vector<std::pair<std::string, std::function<void(int*, int*)>>> sorts{
        { "直接插入排序", Sort::straight_insertion_sort },
        { "折半插入排序", Sort::binary_insertion_sort },
        { "二路插入排序", Sort::two_way_insertion_sort },
        { "希尔排序",   Sort::shell_sort },
        { "起泡排序",   Sort::bubble_sort },
        { "快速排序",   Sort::quicksort },
        { "简单选择排序", Sort::selection_sort },
        { "堆排序",    Sort::heapsort },
        { "归并排序",   Sort::merge_sort },
        { "基数排序",   Sort::radix_sort },
    };

    auto test = [&](const std::function<std::vector<std::vector<int>>()>& get_cases) {
        return [&]() {
            Test{ sorts, get_cases() };
        };
    };

    Menu{
        { 0, "退出程序",     nullptr },
        { 1, "自动测试",     test(auto_test) },
        { 2, "从文件读入测试集", test(test_form_file) },
        { 3, "手动输入测试用例", test(test_form_input) },
    }();

    return 0;
}
