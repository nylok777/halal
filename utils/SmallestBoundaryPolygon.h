#pragma once
#include <numbers>
#include <vector>
#include <string>

constexpr float PI = std::numbers::pi_v<float>;

class Point
{
public:
	float x;
	float y;
};

class SmallestBoundaryPolygonProblem
{
protected:
	std::vector<Point> points;

	static float distanceFromLine(Point lp1, Point lp2, Point p);
	[[nodiscard]] float outerDistanceToBoundary(const std::vector<Point>& solution) const;
	static float lengthOfBoundary(const std::vector<Point>& solution);

	static float objective(const std::vector<Point>& solution);
	[[nodiscard]] float constraint(const std::vector<Point>& solution) const;
public:
	void loadPointsFromFile(const std::string& fileName);
	static void savePointsToFile(const std::string& fileName, const std::vector<Point>& pointVector);
};