#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <cstring> 
/*
?	Manager：等级是月薪除以1000的整数部分。例如，如果月薪是5000，那么等级就是5。
?	PartTimeTechnician：等级是小时工资除以10的整数部分。例如，如果小时工资是50，那么等级就是5。
?	SalesManager：等级是基本工资除以1000的整数部分。例如，如果基本工资是8000，那么等级就是8。
?	PartTimeSales：等级是佣金比例乘以100的整数部分。例如，如果佣金比例是0.15，那么等级就是15。
*/
enum class EmployeeType { 经理, 兼职技术人员, 销售经理, 兼职推销员 };
enum class Gender { 男, 女 };

template <typename T>
T getValidInput(const std::string& prompt) {
    T input;
    while (!(std::cin >> input)) {
        std::cout << "无效输入。" << prompt << "请重试。\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return input;
}

//员工基类
class Employee {
public:
    int id;
    std::string name;
    EmployeeType type;
    double salary;
    Gender gender; // 添加性别字段

    Employee() : id(0), name(""), type(EmployeeType::经理), salary(0), gender(Gender::男) {}

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
    std::cout << "请输入姓名: ";
    in >> std::ws;
    std::getline(in, name);
    std::cout << "请输入性别 (男/女): ";
    std::string genderStr;
    in >> genderStr;
    if (genderStr == "男") {
        gender = Gender::男;
    }
    else if (genderStr == "女") {
        gender = Gender::女;
    }
    else {
        std::cout << "无效的性别输入，默认为男。\n";
        gender = Gender::男;
    }
}
std::ostream& operator<<(std::ostream& out, const Employee& emp) {
    emp.print(out);
    return out;
}//运算符重载
void Employee::print(std::ostream& out) const {
    out << "ID: " << id << ", 姓名: " << name << ", 性别: " << (gender == Gender::男 ? "男" : "女") << ", 职位: ";
    switch (type) {
    case EmployeeType::经理: out << "经理"; break;
    case EmployeeType::兼职技术人员: out << "兼职技术人员"; break;
    case EmployeeType::销售经理: out << "销售经理"; break;
    case EmployeeType::兼职推销员: out << "兼职推销员"; break;
    }
    out << ", 月薪: " << calculateSalary() << std::endl;
    out << "等级: " << getLevel() << std::endl;
}

//兼职技术人员
class PartTimeTechnician : public Employee {
public:
    int hours;
    double hourlyRate;

    PartTimeTechnician() : hours(0), hourlyRate(0) {}

    PartTimeTechnician(int id, const std::string& name, int hours, double hourlyRate, Gender gender)
        : Employee(id, name, EmployeeType::兼职技术人员, 0, gender), hours(hours), hourlyRate(hourlyRate) {}

    void input(std::istream& in) override;
    double calculateSalary() const override;
    void promote() override;
    int getLevel() const override;
};
int PartTimeTechnician::getLevel() const {
    return static_cast<int>(hourlyRate / 10); // 示例实现，根据小时工资计算等级
}
void PartTimeTechnician::promote() {
    hourlyRate *= 1.1; // 提升 10% 的小时工资
}
void PartTimeTechnician::input(std::istream& in) {
    Employee::input(in);
    std::cout << "请输入工作小时数: ";
    hours = getValidInput<int>("请输入工作小时数:");
    std::cout << "请输入每小时工资: ";
    hourlyRate = getValidInput<double>("请输入每小时工资:");
}
double PartTimeTechnician::calculateSalary() const {
    return hours * hourlyRate;
}

//兼职推销员
class PartTimeSales : public Employee {
public:
    double salesAmount;
    double commissionRate;

    PartTimeSales() : salesAmount(0), commissionRate(0) {}

    PartTimeSales(int id, const std::string& name, double salesAmount, double commissionRate, Gender gender)
        : Employee(id, name, EmployeeType::兼职推销员, 0, gender), salesAmount(salesAmount), commissionRate(commissionRate) {}

    void input(std::istream& in) override;
    double calculateSalary() const override;
    int getLevel() const override;
    void promote() override;
};
int PartTimeSales::getLevel() const {
    return static_cast<int>(commissionRate * 100); // 示例实现，根据佣金比例计算等级
}
void PartTimeSales::promote() {
    commissionRate *= 1.1; // 提升 10% 的佣金比例
}
void PartTimeSales::input(std::istream& in) {
    Employee::input(in);
    std::cout << "请输入销售额: ";
    salesAmount = getValidInput<double>("请输入销售额:");
    std::cout << "请输入佣金比例 (例如，0.1 表示 10%): ";
    commissionRate = getValidInput<double>("请输入佣金比例:");
}
double PartTimeSales::calculateSalary() const {
    return salesAmount * commissionRate;
}

//销售经理
class SalesManager : public Employee {
public:
    double baseSalary;
    double salesAmount;
    double commissionRate;

    SalesManager() : baseSalary(0), salesAmount(0), commissionRate(0) {}

    SalesManager(int id, const std::string& name, double baseSalary, double salesAmount, double commissionRate, Gender gender)
        : Employee(id, name, EmployeeType::销售经理, 0, gender), baseSalary(baseSalary), salesAmount(salesAmount), commissionRate(commissionRate) {}

    void input(std::istream& in) override;
    double calculateSalary() const override;
    int getLevel() const override;
    void promote() override;
};
int SalesManager::getLevel() const {
    return static_cast<int>(baseSalary / 1000); // 示例实现，根据基本工资计算等级
}
void SalesManager::promote() {
    baseSalary *= 1.1; // 提升 10% 的基本工资
    commissionRate *= 1.1; // 提升 10% 的佣金比例
}
void SalesManager::input(std::istream& in) {
    Employee::input(in);
    std::cout << "请输入基本工资: ";
    baseSalary = getValidInput<double>("请输入基本工资:");
    std::cout << "请输入销售额: ";
    salesAmount = getValidInput<double>("请输入销售额:");
    std::cout << "请输入佣金比例 (例如，0.1 表示 10%): ";
    commissionRate = getValidInput<double>("请输入佣金比例:");
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
    file.write(reinterpret_cast<const char*>(&emp->gender), sizeof(emp->gender)); // 添加性别字段

    if (emp->type == EmployeeType::兼职技术人员) {
        auto pt = static_cast<const PartTimeTechnician*>(emp);
        file.write(reinterpret_cast<const char*>(&pt->hours), sizeof(pt->hours));
        file.write(reinterpret_cast<const char*>(&pt->hourlyRate), sizeof(pt->hourlyRate));
    }
    else if (emp->type == EmployeeType::兼职推销员) {
        auto pt = static_cast<const PartTimeSales*>(emp);
        file.write(reinterpret_cast<const char*>(&pt->salesAmount), sizeof(pt->salesAmount));
        file.write(reinterpret_cast<const char*>(&pt->commissionRate), sizeof(pt->commissionRate));
    }
    else if (emp->type == EmployeeType::销售经理) {
        auto sm = static_cast<const SalesManager*>(emp);
        file.write(reinterpret_cast<const char*>(&sm->baseSalary), sizeof(sm->baseSalary));
        file.write(reinterpret_cast<const char*>(&sm->salesAmount), sizeof(sm->salesAmount));
        file.write(reinterpret_cast<const char*>(&sm->commissionRate), sizeof(sm->commissionRate));
    }
}

//经理
class Manager : public Employee {
public:
    Manager() {}

    Manager(int id, const std::string& name, double salary, Gender gender)
        : Employee(id, name, EmployeeType::经理, salary, gender) {}

    void input(std::istream& in) override;
    double calculateSalary() const override;
    int getLevel() const override;
    void promote() override;
};
int Manager::getLevel() const {
    return static_cast<int>(salary / 1000); // 示例实现，根据月薪计算等级
}
void Manager::promote() {
    salary *= 1.1; // 提升 10% 的月薪
}
void Manager::input(std::istream& in) {
    Employee::input(in);
    std::cout << "请输入经理月薪: ";
    salary = getValidInput<double>("请输入经理月薪:");
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
    file.read(reinterpret_cast<char*>(&gender), sizeof(gender)); // 添加性别字段

    Employee* emp;
    if (type == EmployeeType::兼职技术人员) {
        int hours;
        double hourlyRate;
        file.read(reinterpret_cast<char*>(&hours), sizeof(hours));
        file.read(reinterpret_cast<char*>(&hourlyRate), sizeof(hourlyRate));
        emp = new PartTimeTechnician(id, nameBuffer, hours, hourlyRate, gender);
    }
    else if (type == EmployeeType::兼职推销员) {
        double salesAmount, commissionRate;
        file.read(reinterpret_cast<char*>(&salesAmount), sizeof(salesAmount));
        file.read(reinterpret_cast<char*>(&commissionRate), sizeof(commissionRate));
        emp = new PartTimeSales(id, nameBuffer, salesAmount, commissionRate, gender);
    }
    else if (type == EmployeeType::销售经理) {
        double baseSalary, salesAmount, commissionRate;
        file.read(reinterpret_cast<char*>(&baseSalary), sizeof(baseSalary));
        file.read(reinterpret_cast<char*>(&salesAmount), sizeof(salesAmount));
        file.read(reinterpret_cast<char*>(&commissionRate), sizeof(commissionRate));
        emp = new SalesManager(id, nameBuffer, baseSalary, salesAmount, commissionRate, gender);
    }
    else {
        // 经理类型
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
                std::cerr << "读取员工数据失败: " << e.what() << std::endl;
                break;
            }
        }
        inputFile.close();
    }
    else {
        std::cout << "员工数据文件不存在，将创建新文件。\n";
        std::ofstream outputFile(filename, std::ios::binary);
        if (!outputFile) {
            std::cerr << "无法创建新文件！" << std::endl;
            return 1;
        }
        outputFile.close();
    }

    char choice;
    do {
        std::cout << "\n员工管理系统\n";
        std::cout << "1. 添加员工\n";
        std::cout << "2. 删除员工\n";
        std::cout << "3. 搜索员工\n";
        std::cout << "4. 显示所有员工\n";
        std::cout << "5. 计算总工资及各类员工工资占比\n";
        std::cout << "6. 提升员工等级\n";
        std::cout << "0. 退出并保存\n";
        std::cout << "请输入你的选择: ";
        std::cin >> choice;

        switch (choice) {
        case '1': {
            int type;
            std::cout << "请选择员工类型:\n";
            std::cout << "1. 经理\n";
            std::cout << "2. 兼职技术人员\n";
            std::cout << "3. 销售经理\n";
            std::cout << "4. 兼职推销员\n";
            std::cout << "请输入你的选择: ";
            type = getValidInput<int>("");

            Employee* newEmployee;
            switch (type) {
            case 1: newEmployee = new Manager(nextId++, "", 0, Gender::男); break;
            case 2: newEmployee = new PartTimeTechnician(nextId++, "", 0, 0, Gender::男); break;
            case 3: newEmployee = new SalesManager(nextId++, "", 0, 0, 0, Gender::男); break;
            case 4: newEmployee = new PartTimeSales(nextId++, "", 0, 0, Gender::男); break;
            default: std::cout << "无效的类型。请重试。\n"; continue;
            }
            newEmployee->input(std::cin);
            employees.push_back(newEmployee);
            break;
        }
        case '2': {
            int id;
            std::cout << "请输入要删除的员工ID: ";
            id = getValidInput<int>("请输入要删除的员工ID:");
            auto it = std::find_if(employees.begin(), employees.end(), [id](Employee* emp) { return emp->id == id; });
            if (it != employees.end()) {
                delete* it;
                employees.erase(it);
                std::cout << "员工已删除。\n";
            }
            else {
                std::cout << "未找到ID为 " << id << " 的员工。\n";
            }
            break;
        }
        case '3': {
            int searchOption;
            std::cout << "请选择搜索方式:\n";
            std::cout << "1. 按ID搜索\n";
            std::cout << "2. 按姓名搜索\n";
            std::cout << "请输入你的选择: ";
            searchOption = getValidInput<int>("");

            if (searchOption == 1) {
                int id;
                std::cout << "请输入要搜索的员工ID: ";
                id = getValidInput<int>("请输入要搜索的员工ID:");
                auto it = std::find_if(employees.begin(), employees.end(), [id](Employee* emp) { return emp->id == id; });
                if (it != employees.end()) {
                    std::cout << "找到的员工信息:\n";
                    (*it)->print(std::cout);
                }
                else {
                    std::cout << "未找到ID为 " << id << " 的员工。\n";
                }
            }
            else if (searchOption == 2) {
                std::string name;
                std::cout << "请输入要搜索的员工姓名: ";
                std::cin >> std::ws;
                std::getline(std::cin, name);
                auto it = std::find_if(employees.begin(), employees.end(), [&name](Employee* emp) { return emp->name == name; });
                if (it != employees.end()) {
                    std::cout << "找到的员工信息:\n";
                    (*it)->print(std::cout);
                }
                else {
                    std::cout << "未找到姓名为 " << name << " 的员工。\n";
                }
            }
            else {
                std::cout << "无效的选择。请重试。\n";
            }
            break;
        }
        case '4': {
            std::cout << "\n所有员工信息:\n";
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
                case EmployeeType::经理:
                    managerSalary += salary;
                    break;
                case EmployeeType::兼职技术人员:
                    partTimeTechnicianSalary += salary;
                    break;
                case EmployeeType::销售经理:
                    salesManagerSalary += salary;
                    break;
                case EmployeeType::兼职推销员:
                    partTimeSalesSalary += salary;
                    break;
                }
            }

            std::cout << "总工资: " << totalSalary << std::endl;
            std::cout << "经理工资占比: " << (managerSalary / totalSalary) * 100 << "%" << std::endl;
            std::cout << "兼职技术人员工资占比: " << (partTimeTechnicianSalary / totalSalary) * 100 << "%" << std::endl;
            std::cout << "销售经理工资占比: " << (salesManagerSalary / totalSalary) * 100 << "%" << std::endl;
            std::cout << "兼职推销员工资占比: " << (partTimeSalesSalary / totalSalary) * 100 << "%" << std::endl;
            break;
        }
        case '6': {
            int id;
            std::cout << "请输入要提升等级的员工ID: ";
            id = getValidInput<int>("请输入要提升等级的员工ID:");
            auto it = std::find_if(employees.begin(), employees.end(), [id](Employee* emp) { return emp->id == id; });
            if (it != employees.end()) {
                (*it)->promote();
                std::cout << "员工等级已提升。\n";
            }
            else {
                std::cout << "未找到ID为 " << id << " 的员工。\n";
            }
            break;
        }
        case '0':
            std::cout << "退出...\n";
            break;
        default:
            std::cout << "无效的选择。请重试。\n";
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
        std::cerr << "无法打开文件进行写入！" << std::endl;
    }

    for (Employee* emp : employees) {
        delete emp;
    }

    return 0;
}