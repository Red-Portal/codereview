#include <utility>
#include <queue>
#include <vector>
#include <list>
#include <unordered_map>
#include <cstdio>
#include <string>
#include <cmath>
#include <memory>
#include <iostream>

/*
this is an implementation of A* algorithm for the problem  "Pac-Man Astar"
at hackerrank https://s3.amazonaws.com/hr-filepicker/astar.txt

This version is currently a 'barely working' version not refactored at all
Maybe I'll clean this up later
but for now at least is works.
*/

std::pair<int, int> boardSize;
class Node;
class mypair_comp;
inline int index(int, int);
inline int index(std::pair<int, int>);
inline auto inverseIndex(int)->std::pair<int, int>;
auto makeGraph(const std::vector<char>&)->std::unordered_map<int, std::unique_ptr<Node>>;
auto astarAlgorithm(std::unordered_map<int, std::unique_ptr<Node>>&&, std::pair<int, int>, std::pair<int, int>)
->std::unordered_map<int, std::pair<int, int>>;
auto convertMapToList(std::unordered_map<int, std::pair<int, int>>&, std::pair<int, int>, std::pair<int, int>)
->std::list<std::pair<int, int>>;
int heuristic(std::pair<int, int>, std::pair<int, int>);

int main(void)
{
	std::pair<int, int> start;
	std::pair<int, int> end;
	scanf("%d %d", &start.second, &start.first);
	scanf("%d %d", &end.second, &end.first);

	scanf("%d %d", &boardSize.first, &boardSize.second);

	std::vector<char> board((boardSize.first)*(boardSize.second), 0);

	for (int j = 0; j<boardSize.first; ++j)
	{
		std::string input;
		//scanf("%s", &board[index(i, j)]);
		std::cin >> input;

		for (int i = 0; i < boardSize.second; ++i)
			board[index(i, j)] = input.at(i);
	}

	auto&& graph = makeGraph(board);
	auto&& path = astarAlgorithm(std::move(graph), start, end);
	auto&& result = convertMapToList(path, start, end);

	printf("%d\n", result.size() - 1);
	for (auto&& i : result)
		printf("%d %d\n", i.second, i.first);

	return 0;
}
class mypair_comp
{
public:
	bool operator()(std::pair<int, double> const& lhs, std::pair<int, double> const& rhs) const
	{
		return lhs.second > rhs.second;
	}
};
inline int index(int x, int y)
{
	return x + y*boardSize.first;
}
inline int index(std::pair<int, int> pair)
{
	return pair.first + pair.second * boardSize.first;
}
inline auto inverseIndex(int key)
->std::pair<int, int>
{
	int x = key % boardSize.first;
	int y = key / boardSize.first;
	return std::make_pair(x, y);
}
class Node
{
public:
	Node(const std::vector<char>& board, int x, int y)
	{
		neighbors.reserve(4);

		if (board[index(x - 1, y)] != '%')
		{
			neighbors.push_back(std::make_pair(x - 1, y));
		}
		if (board[index(x + 1, y)] != '%')
		{
			neighbors.push_back(std::make_pair(x + 1, y));
		}
		if (board[index(x, y + 1)] != '%')
		{
			neighbors.push_back(std::make_pair(x, y + 1));
		}
		if (board[index(x, y - 1)] != '%')
		{
			neighbors.push_back(std::make_pair(x, y - 1));
		}
	}
	~Node() = default;
	std::vector<std::pair<int, int>> neighbors;
};

auto makeGraph(const std::vector<char>& board)
->std::unordered_map<int, std::unique_ptr<Node>>
{
	std::unordered_map<int, std::unique_ptr<Node>> graph;
	graph.reserve(boardSize.first * boardSize.second);

	for (int i = 0; i<boardSize.first; ++i)
	{
		for (int j = 0; j<boardSize.second; ++j)
		{
			if (board[index(i, j)] != '%')
			{
				graph[index(i, j)] = std::make_unique<Node>(board, i, j);
			}
		}
	}
	return graph;
}

auto astarAlgorithm(std::unordered_map<int, std::unique_ptr<Node>>&& graph, std::pair<int, int> start, std::pair<int, int> end)
-> std::unordered_map<int, std::pair<int, int>>
{
	std::unordered_map<int, double> costAccumulative;
	std::unordered_map<int, bool> discoveredMaps;
	std::unordered_map<int, std::pair<int, int>> path;

	costAccumulative[index(start)] = 0;

	std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, mypair_comp> queue;
	queue.push(std::make_pair<int, double>(index(start), heuristic(start, end)));

	while (!queue.empty())
	{
		int indexCurrent = queue.top().first;
		queue.pop();
		discoveredMaps[indexCurrent] = true;

		if (indexCurrent == index(end))
			break;

		for (auto&& neighbor : graph[indexCurrent]->neighbors)
		{
			if (discoveredMaps[index(neighbor)] == true)
				continue;

			double cost = costAccumulative[indexCurrent] + 1;
			if (costAccumulative.find(index(neighbor)) == costAccumulative.end() || cost < costAccumulative[index(neighbor)])
			{
				costAccumulative[index(neighbor)] = cost;
				queue.push(std::make_pair<int, double>(index(neighbor), cost + heuristic(neighbor, end)));
				path[index(neighbor)] = inverseIndex(indexCurrent);
			}
		}
	}
	return path;
}

auto convertMapToList(std::unordered_map<int, std::pair<int, int>>& map, std::pair<int, int> start, std::pair<int, int> end)
-> std::list<std::pair<int, int>>
{
	std::list<std::pair<int, int>> list;
	int endIndex = index(end);

	list.push_back(end);
	for (int i = index(end); map[i] != start; i = index(map[i]))
	{
		auto currentPoint = map[i];
		if (index(currentPoint) == endIndex)
			break;

		list.emplace_front(currentPoint);
	}
	list.emplace_front(start);
	return list;
}

int heuristic(std::pair<int, int> node, std::pair<int, int> end)
{
	return abs(node.first - end.first) + abs(node.second - end.second);
}
