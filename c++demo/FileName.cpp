#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <cstring> 
/*
?	Manager���ȼ�����н����1000���������֡����磬�����н��5000����ô�ȼ�����5��
?	PartTimeTechnician���ȼ���Сʱ���ʳ���10���������֡����磬���Сʱ������50����ô�ȼ�����5��
?	SalesManager���ȼ��ǻ������ʳ���1000���������֡����磬�������������8000����ô�ȼ�����8��
?	PartTimeSales���ȼ���Ӷ���������100���������֡����磬���Ӷ�������0.15����ô�ȼ�����15��
*/
enum class EmployeeType { ����, ��ְ������Ա, ���۾���, ��ְ����Ա };
enum class Gender { ��, Ů };

template <typename T>
T getValidInput(const std::string& prompt) {
    T input;
    while (!(std::cin >> input)) {
        std::cout << "��Ч���롣" << prompt << "�����ԡ�\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return input;
}

//Ա������
class Employee {
public:
    int id;
    std::string name;
    EmployeeType type;
    double salary;
    Gender gender; // ����Ա��ֶ�

    Employee() : id(0), name(""), type(EmployeeType::����), salary(0), gender(Gender::��) {}

    Employee(int id, const std::string& name, EmployeeType type, double salary, Gender gender)
        : id(id), name(name), type(type), salary(salary), gender(gender) {}

    virtual void input(std::istream& in);
    virtual void print(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const Employee& emp);
    virtual double calculateSalary() const = 0;
    virtual void promote() = 0;
    virtual int getLevel() const = 0;
    virtual ~Employee() = default;
};
void Employee::input(std::istream& in) {
    std::cout << "����������: ";
    in >> std::ws;
    std::getline(in, name);
    std::cout << "�������Ա� (��/Ů): ";
    std::string genderStr;
    in >> genderStr;
    if (genderStr == "��") {
        gender = Gender::��;
    }
    else if (genderStr == "Ů") {
        gender = Gender::Ů;
    }
    else {
        std::cout << "��Ч���Ա����룬Ĭ��Ϊ�С�\n";
        gender = Gender::��;
    }
}
std::ostream& operator<<(std::ostream& out, const Employee& emp) {
    emp.print(out);
    return out;
}//���������
void Employee::print(std::ostream& out) const {
    out << "ID: " << id << ", ����: " << name << ", �Ա�: " << (gender == Gender::�� ? "��" : "Ů") << ", ְλ: ";
    switch (type) {
    case EmployeeType::����: out << "����"; break;
    case EmployeeType::��ְ������Ա: out << "��ְ������Ա"; break;
    case EmployeeType::���۾���: out << "���۾���"; break;
    case EmployeeType::��ְ����Ա: out << "��ְ����Ա"; break;
    }
    out << ", ��н: " << calculateSalary() << std::endl;
    out << "�ȼ�: " << getLevel() << std::endl;
}

//��ְ������Ա
class PartTimeTechnician : public Employee {
public:
    int hours;
    double hourlyRate;

    PartTimeTechnician() : hours(0), hourlyRate(0) {}

    PartTimeTechnician(int id, const std::string& name, int hours, double hourlyRate, Gender gender)
        : Employee(id, name, EmployeeType::��ְ������Ա, 0, gender), hours(hours), hourlyRate(hourlyRate) {}

    void input(std::istream& in) override;
    double calculateSalary() const override;
    void promote() override;
    int getLevel() const override;
};
int PartTimeTechnician::getLevel() const {
    return static_cast<int>(hourlyRate / 10); // ʾ��ʵ�֣�����Сʱ���ʼ���ȼ�
}
void PartTimeTechnician::promote() {
    hourlyRate *= 1.1; // ���� 10% ��Сʱ����
}
void PartTimeTechnician::input(std::istream& in) {
    Employee::input(in);
    std::cout << "�����빤��Сʱ��: ";
    hours = getValidInput<int>("�����빤��Сʱ��:");
    std::cout << "������ÿСʱ����: ";
    hourlyRate = getValidInput<double>("������ÿСʱ����:");
}
double PartTimeTechnician::calculateSalary() const {
    return hours * hourlyRate;
}

//��ְ����Ա
class PartTimeSales : public Employee {
public:
    double salesAmount;
    double commissionRate;

    PartTimeSales() : salesAmount(0), commissionRate(0) {}

    PartTimeSales(int id, const std::string& name, double salesAmount, double commissionRate, Gender gender)
        : Employee(id, name, EmployeeType::��ְ����Ա, 0, gender), salesAmount(salesAmount), commissionRate(commissionRate) {}

    void input(std::istream& in) override;
    double calculateSalary() const override;
    int getLevel() const override;
    void promote() override;
};
int PartTimeSales::getLevel() const {
    return static_cast<int>(commissionRate * 100); // ʾ��ʵ�֣�����Ӷ���������ȼ�
}
void PartTimeSales::promote() {
    commissionRate *= 1.1; // ���� 10% ��Ӷ�����
}
void PartTimeSales::input(std::istream& in) {
    Employee::input(in);
    std::cout << "���������۶�: ";
    salesAmount = getValidInput<double>("���������۶�:");
    std::cout << "������Ӷ����� (���磬0.1 ��ʾ 10%): ";
    commissionRate = getValidInput<double>("������Ӷ�����:");
}
double PartTimeSales::calculateSalary() const {
    return salesAmount * commissionRate;
}

//���۾���
class SalesManager : public Employee {
public:
    double baseSalary;
    double salesAmount;
    double commissionRate;

    SalesManager() : baseSalary(0), salesAmount(0), commissionRate(0) {}

    SalesManager(int id, const std::string& name, double baseSalary, double salesAmount, double commissionRate, Gender gender)
        : Employee(id, name, EmployeeType::���۾���, 0, gender), baseSalary(baseSalary), salesAmount(salesAmount), commissionRate(commissionRate) {}

    void input(std::istream& in) override;
    double calculateSalary() const override;
    int getLevel() const override;
    void promote() override;
};
int SalesManager::getLevel() const {
    return static_cast<int>(baseSalary / 1000); // ʾ��ʵ�֣����ݻ������ʼ���ȼ�
}
void SalesManager::promote() {
    baseSalary *= 1.1; // ���� 10% �Ļ�������
    commissionRate *= 1.1; // ���� 10% ��Ӷ�����
}
void SalesManager::input(std::istream& in) {
    Employee::input(in);
    std::cout << "�������������: ";
    baseSalary = getValidInput<double>("�������������:");
    std::cout << "���������۶�: ";
    salesAmount = getValidInput<double>("���������۶�:");
    std::cout << "������Ӷ����� (���磬0.1 ��ʾ 10%): ";
    commissionRate = getValidInput<double>("������Ӷ�����:");
}
double SalesManager::calculateSalary() const {
    return baseSalary + salesAmount * commissionRate;
}


void serializeEmployee(std::ofstream& file, const Employee* emp) {
    file.write(reinterpret_cast<const char*>(&emp->id), sizeof(emp->id));
    int nameLen = emp->name.length();
    file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
    file.write(emp->name.c_str(), nameLen);
    file.write(reinterpret_cast<const char*>(&emp->type), sizeof(emp->type));
    file.write(reinterpret_cast<const char*>(&emp->salary), sizeof(emp->salary));
    file.write(reinterpret_cast<const char*>(&emp->gender), sizeof(emp->gender)); // ����Ա��ֶ�

    if (emp->type == EmployeeType::��ְ������Ա) {
        auto pt = static_cast<const PartTimeTechnician*>(emp);
        file.write(reinterpret_cast<const char*>(&pt->hours), sizeof(pt->hours));
        file.write(reinterpret_cast<const char*>(&pt->hourlyRate), sizeof(pt->hourlyRate));
    }
    else if (emp->type == EmployeeType::��ְ����Ա) {
        auto pt = static_cast<const PartTimeSales*>(emp);
        file.write(reinterpret_cast<const char*>(&pt->salesAmount), sizeof(pt->salesAmount));
        file.write(reinterpret_cast<const char*>(&pt->commissionRate), sizeof(pt->commissionRate));
    }
    else if (emp->type == EmployeeType::���۾���) {
        auto sm = static_cast<const SalesManager*>(emp);
        file.write(reinterpret_cast<const char*>(&sm->baseSalary), sizeof(sm->baseSalary));
        file.write(reinterpret_cast<const char*>(&sm->salesAmount), sizeof(sm->salesAmount));
        file.write(reinterpret_cast<const char*>(&sm->commissionRate), sizeof(sm->commissionRate));
    }
}

//����
class Manager : public Employee {
public:
    Manager() {}

    Manager(int id, const std::string& name, double salary, Gender gender)
        : Employee(id, name, EmployeeType::����, salary, gender) {}

    void input(std::istream& in) override;
    double calculateSalary() const override;
    int getLevel() const override;
    void promote() override;
};
int Manager::getLevel() const {
    return static_cast<int>(salary / 1000); // ʾ��ʵ�֣�������н����ȼ�
}
void Manager::promote() {
    salary *= 1.1; // ���� 10% ����н
}
void Manager::input(std::istream& in) {
    Employee::input(in);
    std::cout << "�����뾭����н: ";
    salary = getValidInput<double>("�����뾭����н:");
}
double Manager::calculateSalary() const {
    return salary;
}


Employee* deserializeEmployee(std::ifstream& file) {
    int id, nameLen;
    EmployeeType type;
    double salary;
    Gender gender;

    file.read(reinterpret_cast<char*>(&id), sizeof(id));
    file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    char* nameBuffer = new char[nameLen + 1];
    file.read(nameBuffer, nameLen);
    nameBuffer[nameLen] = '\0';
    file.read(reinterpret_cast<char*>(&type), sizeof(type));
    file.read(reinterpret_cast<char*>(&salary), sizeof(salary));
    file.read(reinterpret_cast<char*>(&gender), sizeof(gender)); // ����Ա��ֶ�

    Employee* emp;
    if (type == EmployeeType::��ְ������Ա) {
        int hours;
        double hourlyRate;
        file.read(reinterpret_cast<char*>(&hours), sizeof(hours));
        file.read(reinterpret_cast<char*>(&hourlyRate), sizeof(hourlyRate));
        emp = new PartTimeTechnician(id, nameBuffer, hours, hourlyRate, gender);
    }
    else if (type == EmployeeType::��ְ����Ա) {
        double salesAmount, commissionRate;
        file.read(reinterpret_cast<char*>(&salesAmount), sizeof(salesAmount));
        file.read(reinterpret_cast<char*>(&commissionRate), sizeof(commissionRate));
        emp = new PartTimeSales(id, nameBuffer, salesAmount, commissionRate, gender);
    }
    else if (type == EmployeeType::���۾���) {
        double baseSalary, salesAmount, commissionRate;
        file.read(reinterpret_cast<char*>(&baseSalary), sizeof(baseSalary));
        file.read(reinterpret_cast<char*>(&salesAmount), sizeof(salesAmount));
        file.read(reinterpret_cast<char*>(&commissionRate), sizeof(commissionRate));
        emp = new SalesManager(id, nameBuffer, baseSalary, salesAmount, commissionRate, gender);
    }
    else {
        // ��������
        emp = new Manager(id, nameBuffer, salary, gender);
    }

    delete[] nameBuffer;
    return emp;
}


int main() {
    std::vector<Employee*> employees;
    int nextId = 1;
    std::string filename = "employees.dat";

    std::ifstream inputFile(filename, std::ios::binary);
    if (inputFile.is_open()) {
        while (inputFile.peek() != EOF) {
            try {
                Employee* emp = deserializeEmployee(inputFile);
                if (emp) {
                    employees.push_back(emp);
                    nextId = std::max(nextId, emp->id + 1);
                }
                else {
                    break;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "��ȡԱ������ʧ��: " << e.what() << std::endl;
                break;
            }
        }
        inputFile.close();
    }
    else {
        std::cout << "Ա�������ļ������ڣ����������ļ���\n";
        std::ofstream outputFile(filename, std::ios::binary);
        if (!outputFile) {
            std::cerr << "�޷��������ļ���" << std::endl;
            return 1;
        }
        outputFile.close();
    }

    char choice;
    do {
        std::cout << "\nԱ������ϵͳ\n";
        std::cout << "1. ���Ա��\n";
        std::cout << "2. ɾ��Ա��\n";
        std::cout << "3. ����Ա��\n";
        std::cout << "4. ��ʾ����Ա��\n";
        std::cout << "5. �����ܹ��ʼ�����Ա������ռ��\n";
        std::cout << "6. ����Ա���ȼ�\n";
        std::cout << "0. �˳�������\n";
        std::cout << "���������ѡ��: ";
        std::cin >> choice;

        switch (choice) {
        case '1': {
            int type;
            std::cout << "��ѡ��Ա������:\n";
            std::cout << "1. ����\n";
            std::cout << "2. ��ְ������Ա\n";
            std::cout << "3. ���۾���\n";
            std::cout << "4. ��ְ����Ա\n";
            std::cout << "���������ѡ��: ";
            type = getValidInput<int>("");

            Employee* newEmployee;
            switch (type) {
            case 1: newEmployee = new Manager(nextId++, "", 0, Gender::��); break;
            case 2: newEmployee = new PartTimeTechnician(nextId++, "", 0, 0, Gender::��); break;
            case 3: newEmployee = new SalesManager(nextId++, "", 0, 0, 0, Gender::��); break;
            case 4: newEmployee = new PartTimeSales(nextId++, "", 0, 0, Gender::��); break;
            default: std::cout << "��Ч�����͡������ԡ�\n"; continue;
            }
            newEmployee->input(std::cin);
            employees.push_back(newEmployee);
            break;
        }
        case '2': {
            int id;
            std::cout << "������Ҫɾ����Ա��ID: ";
            id = getValidInput<int>("������Ҫɾ����Ա��ID:");
            auto it = std::find_if(employees.begin(), employees.end(), [id](Employee* emp) { return emp->id == id; });
            if (it != employees.end()) {
                delete* it;
                employees.erase(it);
                std::cout << "Ա����ɾ����\n";
            }
            else {
                std::cout << "δ�ҵ�IDΪ " << id << " ��Ա����\n";
            }
            break;
        }
        case '3': {
            int searchOption;
            std::cout << "��ѡ��������ʽ:\n";
            std::cout << "1. ��ID����\n";
            std::cout << "2. ����������\n";
            std::cout << "���������ѡ��: ";
            searchOption = getValidInput<int>("");

            if (searchOption == 1) {
                int id;
                std::cout << "������Ҫ������Ա��ID: ";
                id = getValidInput<int>("������Ҫ������Ա��ID:");
                auto it = std::find_if(employees.begin(), employees.end(), [id](Employee* emp) { return emp->id == id; });
                if (it != employees.end()) {
                    std::cout << "�ҵ���Ա����Ϣ:\n";
                    (*it)->print(std::cout);
                }
                else {
                    std::cout << "δ�ҵ�IDΪ " << id << " ��Ա����\n";
                }
            }
            else if (searchOption == 2) {
                std::string name;
                std::cout << "������Ҫ������Ա������: ";
                std::cin >> std::ws;
                std::getline(std::cin, name);
                auto it = std::find_if(employees.begin(), employees.end(), [&name](Employee* emp) { return emp->name == name; });
                if (it != employees.end()) {
                    std::cout << "�ҵ���Ա����Ϣ:\n";
                    (*it)->print(std::cout);
                }
                else {
                    std::cout << "δ�ҵ�����Ϊ " << name << " ��Ա����\n";
                }
            }
            else {
                std::cout << "��Ч��ѡ�������ԡ�\n";
            }
            break;
        }
        case '4': {
            std::cout << "\n����Ա����Ϣ:\n";
            for (const Employee* emp : employees) {
                emp->print(std::cout);
            }
            break;
        }
        case '5': {
            double totalSalary = 0;
            double managerSalary = 0;
            double partTimeTechnicianSalary = 0;
            double salesManagerSalary = 0;
            double partTimeSalesSalary = 0;

            for (const Employee* emp : employees) {
                double salary = emp->calculateSalary();
                totalSalary += salary;
                switch (emp->type) {
                case EmployeeType::����:
                    managerSalary += salary;
                    break;
                case EmployeeType::��ְ������Ա:
                    partTimeTechnicianSalary += salary;
                    break;
                case EmployeeType::���۾���:
                    salesManagerSalary += salary;
                    break;
                case EmployeeType::��ְ����Ա:
                    partTimeSalesSalary += salary;
                    break;
                }
            }

            std::cout << "�ܹ���: " << totalSalary << std::endl;
            std::cout << "������ռ��: " << (managerSalary / totalSalary) * 100 << "%" << std::endl;
            std::cout << "��ְ������Ա����ռ��: " << (partTimeTechnicianSalary / totalSalary) * 100 << "%" << std::endl;
            std::cout << "���۾�����ռ��: " << (salesManagerSalary / totalSalary) * 100 << "%" << std::endl;
            std::cout << "��ְ����Ա����ռ��: " << (partTimeSalesSalary / totalSalary) * 100 << "%" << std::endl;
            break;
        }
        case '6': {
            int id;
            std::cout << "������Ҫ�����ȼ���Ա��ID: ";
            id = getValidInput<int>("������Ҫ�����ȼ���Ա��ID:");
            auto it = std::find_if(employees.begin(), employees.end(), [id](Employee* emp) { return emp->id == id; });
            if (it != employees.end()) {
                (*it)->promote();
                std::cout << "Ա���ȼ���������\n";
            }
            else {
                std::cout << "δ�ҵ�IDΪ " << id << " ��Ա����\n";
            }
            break;
        }
        case '0':
            std::cout << "�˳�...\n";
            break;
        default:
            std::cout << "��Ч��ѡ�������ԡ�\n";
        }
    } while (choice != '0');

    std::ofstream outputFile(filename, std::ios::binary);
    if (outputFile.is_open()) {
        for (const Employee* emp : employees) {
            serializeEmployee(outputFile, emp);
        }
        outputFile.close();
    }
    else {
        std::cerr << "�޷����ļ�����д�룡" << std::endl;
    }

    for (Employee* emp : employees) {
        delete emp;
    }

    return 0;
}