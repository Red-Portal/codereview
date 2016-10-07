#pragma once
#include <vector>
#include <memory>
#ifndef __HEADER__
#define __HEADER__
double searchShortestPath(std::vector<int*>);
class Point
{
public:
	int x; //I was too lazy to implement the 'get-set' stuff
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
	Point* findConvexPoint(int) const; //iterate through the convex points
	CaveBorder(std::vector<int*>, bool);
	bool TrueisConvexFalseIsConcave(Point*, Point*, Point*, bool) const;  //determine the convexity of a point c relative to point a and b
	~CaveBorder();
	void initializer(void); //using 'TrueisConvexFalseIsConcave', the initializer initialize the convex points
};
double calculatePathLength(std::vector<std::shared_ptr<Point>>, const CaveBorder*); //calculate the resulting path length
#endif
