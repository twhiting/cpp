//  Created by Trent Whiting on 4/12/18.

#include <iostream>
#include <vector>

enum JOB_TYPE
{
    Developer = 1,
    Support,
    Janitor
};

/*
    Base employee interface
*/
class IEmployee
{
public:
    
    virtual ~IEmployee()
    {
        
    }
    
    /* Return the salary of the employee */
    double getSalary()
    {
        return _salary;
    }
    
    std::wstring FullName()
    {
        return _fName + L" " + _lName;
    }
    
    virtual std::wstring getInfo();
    
protected:
    
    /*
     @fName - Employees first name
     @lName - Employees last name
     @job - Employees job description
     */
    IEmployee(std::wstring fName, std::wstring lName, double salary) : _fName {fName}, _lName{lName}{}
    
    
private:
    
    std::wstring _fName;
    std::wstring _lName;
    double _salary;
};

class Developer : public IEmployee
{
public:
    Developer(std::wstring fName, std::wstring lName, double salary) : IEmployee(fName, lName, salary){}
    ~Developer() override
    {
        std::wcout << L"Destroying Developer: " << FullName().c_str() << std::endl;
    }
    
    std::wstring getInfo() override
    {
        std::wstring info;
        
        info += L"-- DEVELOPER -- \n";
        info += L"Name: " + FullName() + L"\n";
        info += L"Salary: " + std::to_wstring(getSalary());
        
        return info;
    }
};

class Support : public IEmployee
{
public:
    Support(std::wstring fName, std::wstring lName, double salary) : IEmployee(fName, lName, salary){}
    ~Support() override
    {
        std::wcout << L"Destroying Support: " << FullName().c_str() << std::endl;
    }
    
    std::wstring getInfo() override
    {
        std::wstring info;
        
        info += L"=== Support ===\n";
        info += L"Name: " + FullName() + L"\n";
        info += L"Salary: " + std::to_wstring(getSalary());
        
        return info;
    }
};

class Janitor : public IEmployee
{
public:
    Janitor(std::wstring fName, std::wstring lName, double salary) : IEmployee(fName, lName, salary){}
    ~Janitor() override
    {
        std::wcout << L"Destroying Janitor: " << FullName().c_str() << std::endl;
    }
    
    std::wstring getInfo() override
    {
        std::wstring info;
        
        info += L"<<< Janitor >>>\n";
        info += L"Name: " + FullName() + L"\n";
        info += L"Salary: " + std::to_wstring(getSalary());
        
        return info;
    }
};

/*
    IFactory is an example Factory Pattern implementation
*/
class EmployeeFactory
{
public:
    static IEmployee* Get(JOB_TYPE type, std::wstring firstName, std::wstring lastName)
    {
        switch (type) {
            case Developer:
                return new class Developer(firstName, lastName, 70000);
                break;
            case Support:
                return new class Support(firstName, lastName, 50000);
                break;
            case Janitor:
                return new class Janitor(firstName, lastName, 40000);
                break;
            default:
                throw new std::exception();
                break;
        }
    }
};

int main(int argc, const char * argv[])
{
    std::vector<IEmployee*> employees;
    
    std::wstring index = 0;
    std::wstring name = L"", first, last;
    
    while(true)
    {
        JOB_TYPE job;
        
        std::wcout << L"Enter job index (X to exit):\n1 - Developer\n2 - Support\n3 - Janitor\n\nSelection> ";
        std::wcin >> index;
        
        if(index == L"X")
            break;
        
        std::wcout << std::endl;
        
        std::wcout << L"Enter Full Name: ";
        std::wcin >> name;
        
        if(stoi(index) == 1)
        {
            job = Developer;
        }
        else if (stoi(index) == 2)
        {
            job = Support;
        }
        else
        {
            job = Janitor;
        }
        
        first = name.substr(0, name.find(L' '));
        last = name.substr(name.find(L' '), name.length());
        
        
        employees.push_back(EmployeeFactory::Get(job, first, last));
    }

    
    std::cout << "Printing all employees...";
    
    int eCount = 1;
    for(auto& employee : employees)
    {
        std::wcout << eCount << L".\n---------------------------" << employee->getInfo() << L"\n---------------------------\n" << std::endl;
    }
    
    return 0;
}
