// Copyright 2021 Spertis

#include "Student.hpp"
#include <iomanip>
#include <utility>
using std::any,
    std::any_cast;

Student::Student(std::string _name, std::any _group,
                 std::any _avg, std::any _debt) {
  name = std::move(_name);
  group = std::move(_group);
  avg = std::move(_avg);
  debt = std::move(_debt);
}
bool anyCompare(std::any org, std::any cItem)
{
  if (org.type() != cItem.type())
    return false;
  if (org.type() == typeid(std::string))
    return any_cast<std::string>(org) == any_cast<std::string>(cItem);

  if (org.type() == typeid(nullptr))
    return true;
  if (org.type() == typeid(double))
    return std::any_cast<double>(org) == std::any_cast<double>(cItem);
  if (org.type() == typeid(size_t))
   return std::any_cast<size_t>(org) == std::any_cast<size_t>(cItem);
  if (org.type() == typeid(std::vector<std::string>))
    return
        std::any_cast<std::vector<std::string>>(org)
           == std::any_cast<std::vector<std::string>>(cItem);
  return false;
}
bool Student::operator==(const Student& student) const
{
      bool n =  name == student.name;
      bool g = anyCompare(group, student.group);
      bool a = anyCompare(avg, student.avg);
      bool d = anyCompare(debt, student.debt);
      return n && g && a && d;
}

Student::Student() = default;

auto get_name(const json& item) -> std::string {
  return item.get<std::string>();
}
auto get_debt(const json& item) -> std::any {
  if (item.is_null())
    return nullptr;
  else if (item.is_string())
    return item.get<std::string>();
  else
    return item.get<std::vector<std::string>>();
}

auto get_avg(const json& item) -> std::any {
  if (item.is_null())
    return nullptr;
  else if (item.is_string())
    return item.get<std::string>();
  else if (item.is_number_float())
    return item.get<double>();
  else
    return item.get<std::size_t>();
}

auto get_group(const json& item) -> std::any {
  if (item.is_string())
    return item.get<std::string>();
  else
    return item.get<std::size_t>();
}

void from_json(const json& item, Student& student1) {
  student1.name = get_name(item.at("name"));
  student1.group = get_group(item.at("group"));
  student1.avg = get_avg(item.at("avg"));
  student1.debt = get_debt(item.at("debt"));
}
