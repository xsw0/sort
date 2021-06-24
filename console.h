#ifndef SORT_CONSOLE_H
#define SORT_CONSOLE_H

#include <functional>
#include <iostream>

template<typename T>
T console(std::function<bool(const T&)> condition = [](const T&) {
    return true;
})
{
    T value;
    std::cin >> value;
    while (std::cin.fail() || !condition(value))
    {
        std::cin.clear();
        std::cin.ignore();
        std::cerr << "输入错误，请重新输入" << std::endl;
        std::cin >> value;
    }
    return value;
}

#endif //SORT_CONSOLE_H
