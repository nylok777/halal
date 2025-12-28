#include "WorkAssignment.h"
#include <fstream>

using std::string;
using std::ifstream;

//
// Basic IO
//
void WorkAssignmentProblem::loadFromFile(const std::string& fileName)
{
	ifstream fpersons(fileName);
	fpersons >> requested_time;
	while (!fpersons.eof()) {
		Person person;
		fpersons >> person.salary;
		fpersons >> person.quality;
		persons.push_back(person);
	};
	fpersons.close();
}

//
// Objective functions
//
auto WorkAssignmentProblem::sumSalary(const std::vector<int>& solution) const -> float
{
	float sum = 0;
	for (unsigned int i = 0; i < solution.size(); i++)
		sum += solution[i] * persons[i].salary;
	return sum;
}

auto WorkAssignmentProblem::avgQuality(const std::vector<int>& solution) const -> float
{
	float sum = 0;
	for (unsigned int i = 0; i < solution.size(); i++)
		sum += solution[i] * persons[i].quality;
	return sum / requested_time;
}