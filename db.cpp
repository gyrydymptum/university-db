#include "db.hpp"
#include <iostream>
#include <string>
#include "student.hpp"

Db::Db(const Student& person) {
    db_.push_back(person);
}

Db::Db(std::vector<Student>& db) {
    for (auto person : db) {
        db_.push_back(person);
    }
}

bool Db::checkPesel(size_t pesel) {
    int multiplication[11] = {1, 3, 7, 9, 1, 3, 7, 9, 1, 3, 1};
    int sum = 0;
    int tab_pesel[11]{};
    for (int i = 10; i >= 0; i--) {
        tab_pesel[i] = pesel % 10;
        pesel = pesel / 10;
    }
    for (int i = 0; i < 11; i++) {
        sum = tab_pesel[i] * multiplication[i] + sum;
    }
    return ((sum > 0) && (sum % 10 == 0));
}

// Student::Student(std::string name, std::string surname, std::string address, unsigned int index, size_t pesel, std::string gender)

bool Db::addStudent() {
    Student tmp_stud;
    std::string tmp_str;  //tymczasowy string
    size_t tmp_int;       //tymczasowy numer
    std::cout << "\n Name: ";
    std::cin >> tmp_str;
    tmp_stud.setName(tmp_str);

    return true;
}

void Db::searchSurname(const std::string& s_name) {
    std::vector<Student> Vecsurname;

    for (auto person1 : Db::db_) {
        if (person1.getSurname() == s_name) {
            Vecsurname.push_back(person1);
        }
    }
    if (Vecsurname.empty()) {
        std::cout << "there is no student in the base " << '\n';
    } else if (Vecsurname.size() == 1) {
        std::cout << "There are 1 student in the database: "<<'\n';
        std::cout << Vecsurname[0].getSurname() << ' ';
        std::cout << Vecsurname[0].getName() << std::endl;

    } else {
        std::cout << "There are " << Vecsurname.size() << " students at the base" << '\n';
        for (auto person2 : Vecsurname) {
            std::cout << person2.getSurname() << ' ';
            std::cout << person2.getName() << std::endl;
        }
    }
}

// Student Db::search(size_t pesel) {
// }
// void Db::sort(const std::string& surname) {
// }
// void Db::sort(size_t pesel) {
// }
// void Db::del(unsigned int index) {
// }
