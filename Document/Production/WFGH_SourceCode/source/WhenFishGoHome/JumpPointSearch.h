#ifndef __JUMPPOINTSEARCH_H_
#define __JUMPPOINTSEARCH_H_

// Public domain Jump Point Search implementation by False.Genesis
// This file originates from: https://github.com/fgenesis/jps
#include "Map.h"
#include <queue>
#include <vector>
#include <map>
#include <math.h>

inline int my_abs(int num)
{
	return num >= 0 ? num : -num;
}

// ctor function to keep Position a real POD struct.
inline static Position Pos(int x, int y)//processor
{
	Position p;
	p.x = x;
	p.y = y;
	return p;
}

static const Position npos = Pos(-1, -1);//global

class Node
{
public:
	Node(const Position& p) : f(0), g(0), pos(p), parent(0), flags(0) {}
	int f, g;
	const Position pos;
	const Node *parent;

	inline void setOpen() { flags |= 1; }
	inline void setClosed() { flags |= 2; }
	inline int unsigned isOpen() const { return flags & 1; }
	inline int unsigned isClosed() const { return flags & 2; }
	inline void clearState() { f = 0; g = 0, parent = 0; flags = 0; }

private:
	int unsigned flags;

	bool operator==(const Node& o); // not implemented, nodes should not be compared
};

inline int Manhattan(const Node *a, const Node *b) //processor
{
	return my_abs(int(a->pos.x - b->pos.x)) + my_abs(int(a->pos.y - b->pos.y));
}

inline int Euclidean(const Node *a, const Node *b) //processor
{
	float fx = float(int(a->pos.x - b->pos.x));
	float fy = float(int(a->pos.y - b->pos.y));
	return int(int(sqrt(fx*fx + fy*fy)));
}

class OpenList
{
public:
	inline void push(Node *node)
	{
		nodes.push_back(node);
		std::push_heap(nodes.begin(), nodes.end(), _compare);
	}
	inline Node *pop()
	{
		std::pop_heap(nodes.begin(), nodes.end(), _compare);
		Node *node = nodes.back();
		nodes.pop_back();
		return node;
	}
	inline bool empty() const
	{
		return nodes.empty();
	}
	inline void clear()
	{
		nodes.clear();
	}
	inline void fixup(const Node *item)
	{
		std::make_heap(nodes.begin(), nodes.end(), _compare);
	}

protected:
	static inline bool _compare(const Node *a, const Node *b)
	{
		return a->f > b->f;
	}
	std::vector<Node*> nodes;
};

class Searcher
{
public:
	Searcher(const Map& g): grid(g), endNode(NULL), skip(1), stepsRemain(0), stepsDone(0)
	{}

	// single-call
	bool findPath(std::vector<Position*>*& path, Position start, Position end, int step = 0);

	// incremental pathfinding
	Result findPathInit(Position start, Position end);
	Result findPathStep(int limit);
	bool findPathFinish(std::vector<Position*>*& path, int step = 0);

	// misc
	void freeMemory();
	inline void setSkip(int s) { skip = Max(1, s); }
	inline size_t getStepsDone() const { return stepsDone; }
	inline size_t getNodesExpanded() const { return nodegrid.size(); }

private:

	typedef std::map<Position, Node> NodeGrid;

	const Map& grid;
	Node *endNode;
	int skip;
	int stepsRemain;
	size_t stepsDone;
	OpenList open;

	NodeGrid nodegrid;
	inline int Max(int a, int b) const
	{ return ((a > b) ? a : b); }
	inline bool CheckGrid(int dx, int dy, int x, int y)
	{
		return (grid(x + (dx), y + (dy)));
	}
	inline void AddPos(Position *&w, int dx, int dy, int x, int y) 	{
		do
		{
			*w++ = Pos(x + (dx), y + (dy));
		}
		while (0);
	}
	inline void CheckAddPos(Position *&w, int dx, int dy, int x, int y)
	{
		do
		{
			if (CheckGrid(dx, dy, x, y)) 
				AddPos(w, dx, dy, x, y);
		} 
		while (0);
	}
	inline void AddPosNoTunnel(Position *&w, int dx, int dy, int x, int y)
	{
		do
		{
			if (grid(x + (dx), y) || grid(x, y + (dy))) CheckAddPos(w, dx, dy, x, y);
		}
		while (0);
	}

	Node *getNode(const Position& pos);
	void identifySuccessors(const Node *n);
	int findNeighbors(const Node *n, Position *wptr);
	Position jumpP(const Position& p, const Position& src);
	Position jumpD(Position p, int dx, int dy);
	Position jumpX(Position p, int dx);
	Position jumpY(Position p, int dy);
	Position* PadPosition(Position dest, Position start) const;
	bool generatePath(std::vector<Position*>*& path, int step) const;
};

#endif