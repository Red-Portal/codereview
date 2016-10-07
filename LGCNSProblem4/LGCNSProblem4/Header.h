#pragma once
#include <vector>
#include <memory>
#ifndef __HEADER__
#define __HEADER__
double searchShortestPath(std::vector<int*>);
class Point
{
public:
	int x;
	int y;
	Point(int, int);
};
class CaveBorder
{
private:
	std::vector<int*> m_curvePoints;
	std::vector<Point> m_convexPoints;
	const bool m_falseForFloorTrueForCeiling;
public:
	Point* findConvexPoint(int) const;
	CaveBorder(std::vector<int*>, bool);
	bool TrueisConvexFalseIsConcave(Point*, Point*, Point*, bool) const;
	~CaveBorder();
	void initializer(void);
};
double calculatePathLength(std::vector<std::shared_ptr<Point>>, const CaveBorder*);
#endif
