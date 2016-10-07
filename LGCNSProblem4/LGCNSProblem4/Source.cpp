#include <cstdio>
#include <vector>
#include "Header.h"



/* THE "INPUT.TXT" FILE IS THE INPUT
  C-A THE CONTENT AND C-V ON THE PROGRAM CMD WINDOW */
int main(int argc, char ** args)
{
	int numTestCase; 
	int numCurve;
	int *inputs;
	int temp[3];
	std::vector<int*> curvePoints; //The 'input points'
	std::vector<long double> results;

	scanf("%d", &numTestCase);

	for(int i=0; i<numTestCase; i++)
	{
		scanf("%d", &numCurve);

		for(int j=0; j<numCurve; j++)
		{
			scanf("%d %d %d", &temp[0], &temp[1], &temp[2]);
			int* input = new int[3]{ temp[0], temp[1], temp[2] };
			curvePoints.push_back(input);
		}

		results.push_back(searchShortestPath(curvePoints)); //searchShortestPath is the main function
	}
	for (auto i : results)
		printf("%.2f\n", i);

	for (auto i : curvePoints)
		delete i;
	curvePoints.clear();
	return 0;
}
