#pragma once
#include <string>
#include <vector>
#include "student.hpp"

// Wyświetlanie całej bazy danych zrobie w interfejsie

class Db {
public:
    std::vector<Student> db_;

public:
    Db(const Student&);
    bool checkPesel(size_t);
    bool addStudent();  //powinna korzystać z funkcji do walidacji pesel
    Student search(const std::string&);
    Student search(size_t);
    void sort(const std::string&);
    void sort(size_t);
    void del(unsigned int);
};
