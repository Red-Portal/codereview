#include "Header.h"
#include <cmath>
#include <vector>
#include <memory>

double searchShortestPath(std::vector<int*> curvePoints)
{
	long double pathLength;
	std::vector<std::shared_ptr<Point>> convexPoints;
	
	int&& temp = curvePoints.size();
	int *curveXPos = new int[temp];
	for (int i = 0; i < temp; i++)
		curveXPos[i] = (*curvePoints.at(i));
	
	CaveBorder ceiling(curvePoints, true);
	CaveBorder floor(std::move(curvePoints), false);

	for(int i = 0; i<temp; i++)
	{
		if (ceiling.findConvexPoint(curveXPos[i]) != nullptr)
		{
			Point temp(*ceiling.findConvexPoint(curveXPos[i]));
			auto onCeiling = std::make_shared<Point>(temp);
			convexPoints.push_back(onCeiling);
		}
		else if (floor.findConvexPoint(curveXPos[i]) != nullptr)
		{
			Point temp(*floor.findConvexPoint(curveXPos[i]));
			auto onFloor = std::make_shared<Point>(temp);
			convexPoints.push_back(onFloor);
		}
	}

	pathLength = calculatePathLength(std::move(convexPoints), &ceiling);

	delete[] curveXPos;
	convexPoints.clear();
	return pathLength;
}
double calculatePathLength(std::vector<std::shared_ptr<Point>>&& convexPoints, const CaveBorder* ceiling)
{
	long double path;
	std::shared_ptr<Point> beginPoint;
	std::shared_ptr<Point> endPoint;

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

	path = sqrt(pow(beginPoint->x - convexPoints[1]->x, 2.0) + pow(beginPoint->y - convexPoints[1]->y, 2.0));
	path += sqrt(pow(endPoint->x - convexPoints[convexPoints.size() - 2]->x, 2.0) + pow(endPoint->y - convexPoints[convexPoints.size() - 2]->y, 2.0));

	for(int i = 1; i<convexPoints.size()-2; i++)
	{
		path += sqrt(pow(convexPoints[i]->x - convexPoints[i + 1]->x, 2.0) + pow(convexPoints[i]->y - convexPoints[i + 1]->y, 2.0));
	}
	return path;
}
CaveBorder::CaveBorder(std::vector<int*>&& curvePoints, bool falseForFloorTrueForCeiling)
	: m_curvePoints(std::move(curvePoints)), m_falseForFloorTrueForCeiling(falseForFloorTrueForCeiling) 
{//the code below is for saving convex down/up points on each border
	initializer();
}
CaveBorder::CaveBorder(std::vector<int*>& curvePoints, bool falseForFloorTrueForCeiling)
	: m_curvePoints(std::move(curvePoints)), m_falseForFloorTrueForCeiling(falseForFloorTrueForCeiling)
{	initializer(); }
void CaveBorder::initializer()
{
	auto point_a = std::make_unique<Point>(0, 0);
	auto point_b = std::make_unique<Point>(0, 0);
	auto point_c = std::make_unique<Point>(0, 0);
	for (int i = 1; i < m_curvePoints.size()-1; i++)
	{
		if (m_falseForFloorTrueForCeiling)
		{
			point_a->x = *(m_curvePoints.at(i - 1));
			point_a->y = *(m_curvePoints.at(i - 1) + 2);
			point_b->x = *(m_curvePoints.at(i + 1));
			point_b->y = *(m_curvePoints.at(i + 1) + 2);
			point_c->x = *(m_curvePoints.at(i));
			point_c->y = *(m_curvePoints.at(i) + 2);
		}
		else
		{
			point_a->x = *(m_curvePoints.at(i - 1));
			point_a->y = *(m_curvePoints.at(i - 1) + 1);
			point_b->x = *(m_curvePoints.at(i + 1));
			point_b->y = *(m_curvePoints.at(i + 1) + 1);
			point_c->x = *(m_curvePoints.at(i));
			point_c->y = *(m_curvePoints.at(i) + 1);
		}


		if (i == m_curvePoints.size() - 2)
		{
			if (TrueisConvexFalseIsConcave(point_a.get(), point_c.get(), point_b.get(), m_falseForFloorTrueForCeiling))
			{
				m_convexPoints.push_back(std::move(Point(*point_c)));
				break;
			}
			else
				m_convexPoints.push_back(std::move(Point(*point_b))); //if the previous point of end() is concave, then end is convex
		}
		else if (i == 1)
		{
			if (TrueisConvexFalseIsConcave(point_a.get(), point_c.get(), point_b.get(), m_falseForFloorTrueForCeiling))
				m_convexPoints.push_back(std::move(Point(*point_c)));
			else
				m_convexPoints.push_back(std::move(Point(*point_a)));
		}
		else
		{
			if (TrueisConvexFalseIsConcave(point_a.get() , point_c.get() , point_b.get() , m_falseForFloorTrueForCeiling))
				m_convexPoints.push_back(std::move(Point(*point_c)));
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
	height = a->y - slope*a->x;

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