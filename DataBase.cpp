#include "DataBase.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <memory>
#include <random>
#include <string>

DataBase::DataBase(size_t size = 0) {
    db_.reserve(size);
}

void DataBase::changeToSmall(std::string& word) {
    std::transform(begin(word), end(word), begin(word), [](unsigned char c) {
        return std::tolower(c);
    });
}

size_t DataBase::stringToDouble(const std::string& str) {
    size_t tmp = 1;
    if (str.size() != 11) {
        tmp = 2;
    } else {
        try {
            tmp = static_cast<size_t>(std::stod(str));
        } catch (...) {
            tmp = 0;
        }
    }
    return tmp;
}

bool DataBase::testPesel(const std::string& tmp_str) {
    size_t tmp_size = 0;
    if (searchPesel(tmp_str)) {
        tmp_size = stringToDouble(tmp_str);
        if (checkPesel(tmp_size)) {
            std::cout << "Person added.\n";
            return true;
        } else {
            std::cout << "Wrong PESEL !\n";
            return false;
        }
    } else {
        std::cout << "Person with this PESEL already exists!\n";
        return false;
    }
}

bool DataBase::checkPesel(size_t pesel) {
    size_t multiplication[11] = {1, 3, 7, 9, 1, 3, 7, 9, 1, 3, 1};
    size_t sum = 0;
    size_t tab_pesel[11]{};
    for (int i = 10; i >= 0; i--) {
        tab_pesel[i] = pesel % 10;
        pesel = pesel / 10;
    }
    for (int i = 0; i < 11; i++) {
        sum = tab_pesel[i] * multiplication[i] + sum;
    }
    return ((sum > 0) && (sum % 10 == 0));
}

bool DataBase::searchPesel(const std::string& pesel) {
    auto index_position = find_if(begin(db_), end(db_), [pesel](const std::unique_ptr<Person>& ptr) {
        return pesel == ptr->getPesel();
    });
    return index_position == end(db_);
}

bool DataBase::addEmployee(const Employee& person) {
    std::string tmp_str_t = person.getPesel();
    if (testPesel(tmp_str_t)) {
        std::unique_ptr<Employee> ptr = std::make_unique<Employee>(person);

        db_.emplace_back(std::move(ptr));
        return true;
    }
    return false;
}
bool DataBase::addStudent(const Student& person) {
    std::string tmp_str_t = person.getPesel();
    if (testPesel(tmp_str_t)) {
        std::unique_ptr<Student> ptr = std::make_unique<Student>(person);

        db_.emplace_back(std::move(ptr));
        return true;
    }
    return false;
}

void DataBase::sortByEarnings() {
    std::sort(db_.begin(), db_.end(), [](std::unique_ptr<Person>& first, std::unique_ptr<Person>& second) {
        size_t first_person = first->getEarnings();
        size_t second_person = second->getEarnings();

        return first_person < second_person;
    });
}

void DataBase::sortByPesel() {
    std::sort(db_.begin(), db_.end(), [](const std::unique_ptr<Person>& first, const std::unique_ptr<Person>& second) {
        std::string first_pesel = first->getPesel();
        std::string second_pesel = second->getPesel();
        if ((first_pesel[0] == '0') || (first_pesel[0] == '1')) {
            first_pesel += 1;
        }
        if ((second_pesel[0] == '0') || (second_pesel[0] == '1')) {
            second_pesel += 1;
        }

        if (first_pesel.size() == second_pesel.size()) {
            return first_pesel < second_pesel;
        } else {
            return first_pesel.size() < second_pesel.size();
        }

        return first_pesel > second_pesel;
    });
}

void DataBase::sortBySurname() {
    std::sort(db_.begin(), db_.end(), [&](const std::unique_ptr<Person>& first, const std::unique_ptr<Person>& second) {
        std::string first_surname = first->getSurname();
        std::string second_surname = second->getSurname();
        changeToSmall(first_surname);
        changeToSmall(second_surname);
        return first_surname < second_surname;
    });
}

std::vector<Person*> DataBase::searchSurname(const std::string& surname) {
    std::vector<Person*> vecSurname;
    Person* ptr;
    std::string surnameA = surname;
    std::string surnameB;
    changeToSmall(surnameA);

    for (long unsigned int i = 0; i < db_.size(); i++) {
        surnameB = db_[i]->getSurname();
        changeToSmall(surnameB);
        if (surnameA == surnameB) {
            ptr = db_[i].get();
            vecSurname.push_back(ptr);
        }
    }
    return vecSurname;
}

bool DataBase::modificationOfEarnings(const std::string& pesel, const size_t& new_earnings) {
    auto index = std::find_if(begin(db_), end(db_), [pesel](const std::unique_ptr<Person>& ptr) {
        return pesel == ptr->getPesel();
    });
    if (index != end(db_)) {
        auto const index_position = std::distance(begin(db_), index);
        if (db_[index_position]->getTypeOfPerson() == "employee") {
            db_[index_position]->setEarnings(new_earnings);
            return true;
        }
    }
    return false;
}

bool DataBase::deleteStudent(const std::string& index) {
    auto index_position = find_if(begin(db_), end(db_), [index](const std::unique_ptr<Person>& ptr) {
        return (index == ptr->getIndex());
    });

    if (index_position != std::end(db_)) {
        db_.erase(index_position);
        return true;
    }
    return false;
}

void DataBase::printDb() {
    int nr_person = 1;
    int st = 20;
    if (db_.size() > 0) {
        std::cout << "     ";
        std::cout << std::left;
        std::cout << std::setw(st) << "[      NAME       ]";
        std::cout << std::setw(st) << "[     SURNAME     ]";
        std::cout << std::setw(st) << "[      PESEL      ]";
        std::cout << std::setw(st) << "[      GENDER     ]";
        std::cout << std::setw(st) << "[     ADDRESS     ]";
        std::cout << std::setw(st) << "[      INDEX      ]";
        std::cout << std::setw(st) << "[     EARNINGS    ]";

        for (auto& person1 : db_) {
            std::cout << '\n';
            std::cout << std::right;
            std::cout << std::setw(5) << nr_person << ".   ";
            std::cout << std::left;
            std::cout << std::setw(st) << person1->getName();
            std::cout << std::setw(st) << person1->getSurname();
            std::cout << std::setw(st) << person1->getPesel();
            std::cout << std::setw(st) << static_cast<char>(person1->getGender());
            std::cout << std::setw(st) << person1->getAddress();
            std::cout << std::setw(st) << person1->getIndex();
            if (person1->getTypeOfPerson() == "student") {
                std::cout << std::setw(st) << "no_earnings";
            } else {
                std::cout << std::setw(st) << person1->getEarnings();
            }

            nr_person++;
        }
        std::cout << '\n';
    } else {
        std::cout << "No records in database.\n";
    }
}

void DataBase::saveDataBaseToFile(const std::string& file_name) {
    int st = 20;
    std::ofstream str;
    str.open(file_name);

    if (db_.size() > 0) {
        for (auto& person : db_) {
            str << std::left;
            str << "   ";
            str << std::setw(st) << person->getTypeOfPerson();
            str << std::setw(st) << person->getName();
            str << std::setw(st) << person->getSurname();
            str << std::setw(st) << person->getPesel();
            str << std::setw(st) << static_cast<char>(person->getGender());
            str << std::setw(st) << person->getAddress();
            str << std::setw(st) << person->getIndex();
            if (person->getTypeOfPerson() == "student") {
                str << std::setw(st) << "no_earnings";
            } else {
                str << std::setw(st) << person->getEarnings();
            }

            str << '\n';
        }
    }
    str.close();
}

void DataBase::loadDataBaseFromFile(const std::string& base_txt, DataBase& base) {
    std::ifstream str(base_txt);
    std::string type_of_person;
    std::string name;
    std::string surname;
    std::string address;
    std::string pesel;
    ID gender2;
    char gender = 'O';
    size_t earnings = 0;
    std::string earnings_s;
    std::string index;
    while (str >> type_of_person) {
        if (type_of_person == "student") {
            str >> name >> surname >> pesel >> gender >> address >> index >> earnings_s;
            if (gender == 'M') {
                gender2 = ID::Male;
            } else {
                gender2 = ID::Female;
            }

            Student st(name, surname, address, pesel, gender2, index);
            base.addStudent(st);

        } else {
            str >> name >> surname >> pesel >> gender >> address >> index >> earnings;
            if (gender == 'M') {
                gender2 = ID::Male;
            } else {
                gender2 = ID::Female;
            }
            Employee emp(name, surname, address, pesel, gender2, earnings);
            base.addEmployee(emp);
        }
    }
}

size_t DataBase::generatingANumber(size_t range) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distrib(0, range);
    return distrib(gen);
}

std::string DataBase::generatingAMaleName() {
    std::vector<std::string> name{"Bartosz", "Allan", "Artur", "Wiktor", "Sylwester"};
    size_t index_tab = generatingANumber(name.size() - 1);
    return name.at(index_tab);
}
std::string DataBase::generatingAFemaleName() {
    std::vector<std::string> name{"Mariola", "Anna", "Sylwia", "Irena", "Alicja"};
    size_t index_tab = generatingANumber(name.size() - 1);
    return name.at(index_tab);
}
std::string DataBase::generatingAMaleSurname() {
    std::vector<std::string> surname{"Kapustka", "Kowalski", "Lewandowski", "Bednarek", "Kowalczyk"};
    size_t index_tab = generatingANumber(surname.size() - 1);
    return surname.at(index_tab);
}
std::string DataBase::generatingAFemaleSurname() {
    std::vector<std::string> surname{"Lewandowska", "Stanowska", "Nowak", "Kowalewska", "Konieczna"};
    size_t index_tab = generatingANumber(surname.size() - 1);
    return surname.at(index_tab);
}
std::string DataBase::generatingAPeselMale() {
    std::vector<std::string> pesel{"03222576535", "83091644917", "79051074234", "98010233714", "01241249667"};
    size_t index_tab = generatingANumber(pesel.size() - 1);
    return pesel.at(index_tab);
}
std::string DataBase::generatingAPeselFemale() {
    std::vector<std::string> pesel{"03310167447", "91031583965", "03282974346", "02322962488", "47052253642"};
    size_t index_tab = generatingANumber(pesel.size() - 1);
    return pesel.at(index_tab);
}
std::string DataBase::generatingAddress() {
    std::vector<std::string> address{"Opole", "Krakow", "Bydgoszcz", "Warszawa", "Torun", "Budapeszt", "Wilno", "Turyn", "Gdansk", "Gdynia"};
    size_t index_tab = generatingANumber(address.size() - 1);
    return address.at(index_tab);
}
std::string DataBase::generatingIndex() {
    std::string index;
    for (int i = 0; i < 5; i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> distrib(0, 9);
        index += std::to_string(distrib(gen));
    }
    return index;
}
size_t DataBase::generatingEarnings() {
    std::string index;

    for (int i = 1; i <= 4; i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> distrib(0, 9);
        index += std::to_string(distrib(gen));
    }
    return std::stoull(index);
}

Student DataBase::generatingStudentData() {
    std::string name;
    std::string surname;
    std::string address;
    std::string pesel;
    std::string index;
    ID gen;
    address = generatingAddress();
    index = generatingIndex();
    bool gender = generatingANumber(1);
    if (gender) {
        name = generatingAMaleName();
        surname = generatingAMaleSurname();
        pesel = generatingAPeselMale();
        gen = ID::Male;

    } else {
        name = generatingAFemaleName();
        surname = generatingAFemaleSurname();
        pesel = generatingAPeselFemale();
        gen = ID::Female;
    }
    Student stud(name, surname, address, pesel, gen, index);
    return stud;
}

Employee DataBase::GeneratingEmployeeData() {
    std::string name;
    std::string surname;
    std::string address;
    std::string pesel;
    ID gen;
    size_t earnings;
    bool gender = generatingANumber(1);
    address = generatingAddress();
    earnings = generatingEarnings();
    if (gender) {
        name = generatingAMaleName();
        surname = generatingAMaleSurname();
        pesel = generatingAPeselMale();
        gen = ID::Male;
    } else {
        name = generatingAFemaleName();
        surname = generatingAFemaleSurname();
        pesel = generatingAPeselFemale();
        gen = ID::Female;
    }
    Employee emp(name, surname, address, pesel, gen, earnings);
    return emp;
}
