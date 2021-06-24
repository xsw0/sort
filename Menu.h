// Menu.h
#ifndef SORT_MENU_H
#define SORT_MENU_H

#include <utility>
#include <string>
#include <map>
#include <tuple>
#include <iostream>
#include <initializer_list>
#include <iomanip>

#include "console.h"


class Menu
{
public:
    std::map<int, std::pair<std::string, std::function<void()>>> entries;

    Menu(std::initializer_list<std::tuple<int, std::string, std::function<void()>>> entries = {})
    {
        for (auto& tup : entries)
        {
            this->entries[std::get<int>(tup)] = {
                std::get<std::string>(tup),
                std::get<std::function<void()>>(tup)
            };
        }
    }

    void operator()()
    {
        for (auto& entry:entries)
        {
            std::cout << std::right << std::setw(8) << entry.first << "\t";
            std::cout << entry.second.first << "\n";
        }

        int key = console<int>([&](int key) {
            return entries.find(key) != entries.cend();
        });

        std::string newline;
        std::getline(std::cin, newline);
        if (entries[key].second != nullptr)
        {
            (entries[key].second)();
            operator()();
        }
    };
};


#endif //SORT_MENU_H
