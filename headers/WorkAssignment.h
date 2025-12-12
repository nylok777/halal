#pragma once
#include <string>
#include <vector>

class Person
{
public:
	float salary;
	float quality;
};

class WorkAssignmentProblem
{
protected:
	std::vector<Person> persons;
	int requested_time;

	float sumSalary(const std::vector<int>& solution) const;
	float avgQuality(const std::vector<int>& solution) const;

public:
	virtual ~WorkAssignmentProblem() = default;
	virtual void loadFromFile(const std::string& fileName);
};