#include "Header.h"
#include <cmath>
#include <memory>

double searchShortestPath(std::vector<int*> curvePoints)
{
	long double pathLength;
	std::vector<Point> path; //the resulting points forming the shortest path
	std::vector<int> curveXPos;
	std::vector<std::shared_ptr<Point>> convexPoints;

	for (auto i : curvePoints)
		curveXPos.push_back(*i);

	CaveBorder ceiling(curvePoints, true); //make the ceiling and floor object
	CaveBorder floor(curvePoints, false);

	for(auto i : curveXPos)
	{ ///////////// THIS PORTION MIXES ALL THE CONVEX POINTS OF THE CEILING AND FLOOR //////////////
		if (ceiling.findConvexPoint(i) != nullptr) //if the convex point belongs to 
		{
			Point temp(*ceiling.findConvexPoint(i));
			auto onCeiling = std::make_shared<Point>(temp);
			convexPoints.push_back(onCeiling);
		}
		else if (floor.findConvexPoint(i) != nullptr)
		{
			Point temp(*floor.findConvexPoint(i));
			auto onFloor = std::make_shared<Point>(temp);
			convexPoints.push_back(onFloor);
		}
	}

	pathLength = calculatePathLength(convexPoints, &ceiling);

	convexPoints.clear();
	return pathLength;
}
double calculatePathLength(std::vector<std::shared_ptr<Point>> convexPoints, const CaveBorder* ceiling)
{
	long double path;
	std::shared_ptr<Point> beginPoint;
	std::shared_ptr<Point> endPoint;

	////////// THIS PORTION IS THE ALGORITHM SETTING THE STARTING POINT AND ENDING POINT //////////////////
	if (ceiling->findConvexPoint(convexPoints[0]->x) == nullptr) // the first convex point is not on the ceiling
	{
		if (convexPoints[1]->y > convexPoints[0]->y) //if the next convex point is higher than the first,
			beginPoint = std::make_shared<Point>(convexPoints[0]->x, convexPoints[1]->y); //than the begin point is not the first convex point
		else
			beginPoint = convexPoints[0];
	}
	else
	{
		if (convexPoints[1]->y < convexPoints[0]->y)
			beginPoint = std::make_shared<Point>(convexPoints[0]->x, convexPoints[1]->y);
		else
			beginPoint = convexPoints[0];
	} //until here it is making the begin Point

	if (ceiling->findConvexPoint(convexPoints.back()->x) == nullptr) //the last convex point is not on the ceiling
	{
		if (convexPoints[convexPoints.size() - 2]->y > convexPoints.back()->y)
			endPoint = std::make_shared<Point>(convexPoints.back()->x, convexPoints[convexPoints.size() - 2]->y);
		else
			endPoint = convexPoints.back();
	}
	else
	{
		if (convexPoints[convexPoints.size() - 2]->y < convexPoints.back()->y)
			endPoint = std::make_shared<Point>(convexPoints.back()->x, convexPoints[convexPoints.size() - 2]->y);
		else
			endPoint = convexPoints.back();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	path = sqrt(pow(beginPoint->x - convexPoints[1]->x, 2.0) + pow(beginPoint->y - convexPoints[1]->y, 2.0));
	path += sqrt(pow(endPoint->x - convexPoints[convexPoints.size() - 2]->x, 2.0) + pow(endPoint->y - convexPoints[convexPoints.size() - 2]->y, 2.0));

	for(int i = 1; i<convexPoints.size()-2; i++)
	{
		path += sqrt(pow(convexPoints[i]->x - convexPoints[i + 1]->x, 2.0) + pow(convexPoints[i]->y - convexPoints[i + 1]->y, 2.0));
	}
	return path;
}
CaveBorder::CaveBorder(std::vector<int*> curvePoints, bool falseForFloorTrueForCeiling)
	: m_curvePoints(curvePoints), m_falseForFloorTrueForCeiling(falseForFloorTrueForCeiling)
{//the code below is for saving convex down/up points on each border
	initializer();
}
void CaveBorder::initializer() //Evaluating convex points
{
	for (int i = 1; i < m_curvePoints.size()-1; i++)
	{
		std::unique_ptr<Point> point_a;
		std::unique_ptr<Point> point_b;
		std::unique_ptr<Point> point_c;
			if (m_falseForFloorTrueForCeiling)
			{
				point_a = std::make_unique<Point>(*(m_curvePoints.at(i - 1)), *(m_curvePoints.at(i - 1) + 2));
				point_b = std::make_unique<Point>(*(m_curvePoints.at(i + 1)), *(m_curvePoints.at(i + 1) + 2));
				point_c = std::make_unique<Point>(*(m_curvePoints.at(i)), *(m_curvePoints.at(i) + 2));
			}
			else
			{
				point_a = std::make_unique<Point>(*(m_curvePoints.at(i - 1)), *(m_curvePoints.at(i - 1) + 1));
				point_b = std::make_unique<Point>(*(m_curvePoints.at(i + 1)), *(m_curvePoints.at(i + 1) + 1));
				point_c = std::make_unique<Point>(*(m_curvePoints.at(i)), *(m_curvePoints.at(i) + 1));
			}


		if (i == m_curvePoints.size() - 2)
		{
			if (TrueisConvexFalseIsConcave(point_a.get(), point_c.get(), point_b.get(), m_falseForFloorTrueForCeiling))
			{
				m_convexPoints.push_back(Point(*point_c));
				break;
			}
			else
				m_convexPoints.push_back(Point(*point_b)); //if the previous point of end() is concave, then end is convex
		}
		else if (i == 1)
		{
			if (TrueisConvexFalseIsConcave(point_a.get() , point_c.get() , point_b.get() , m_falseForFloorTrueForCeiling))
				m_convexPoints.push_back(Point(*point_c));
			else
				m_convexPoints.push_back(Point(*point_a));
		}
		else
		{
			if (TrueisConvexFalseIsConcave(point_a.get() , point_c.get() , point_b.get() , m_falseForFloorTrueForCeiling))
				m_convexPoints.push_back(Point(*point_c));
		}
	}
}

Point::Point(int input_x, int input_y): x(input_x), y(input_y) {}
bool CaveBorder::TrueisConvexFalseIsConcave (Point* a, Point* c, Point* b, bool trueIsCeilingFalseInFloor) const
{
	int delta_x, delta_y;
	double slope;
	double height;
	delta_x = b->x - a->x;
	delta_y = b->y - a->y;
	slope = (double) delta_y / delta_x;
	height = a->y - slope*a->x; // getting the slope and height of the graph that point a, point b form
							 // if point_c is higher or lower that the f(x_of_point_c) then it is convex/concave

	if (trueIsCeilingFalseInFloor)
	{
		if (c->y < (slope * c->y + height))
			return true;
		else
			return false;
	}
	else
	{
		if (c->y > (slope*c->y + height))
			return true;
		else
			return false;
	}
}
CaveBorder::~CaveBorder() {}
Point* CaveBorder::findConvexPoint(int x) const
{
	for(auto i : m_convexPoints)
	{
		if (i.x == x)
			return &i;
	}
	return nullptr;
}