#include "stdafx.h"
#include "JumpPointSearch.h"
#include <algorithm>

using namespace std;

inline Node *Searcher::getNode(const Position& pos)
{
	return &nodegrid.insert(std::make_pair(pos, Node(pos))).first->second;
}

Position Searcher::jumpP(const Position &p, const Position& src)
{
	int dx = int(p.x - src.x);
	int dy = int(p.y - src.y);

	if (dx && dy)
		return jumpD(p, dx, dy);
	else if (dx)
		return jumpX(p, dx);
	else if (dy)
		return jumpY(p, dy);

	return npos;
}

Position Searcher::jumpD(Position p, int dx, int dy)
{
	const Position& endpos = endNode->pos;
	int steps = 0;

	while (true)
	{
		if (p == endpos)
			break;

		++steps;
		const int x = p.x;
		const int y = p.y;

		if ((grid(x - dx, y + dy) && !grid(x - dx, y)) || (grid(x + dx, y - dy) && !grid(x, y - dy)))
			break;

		const bool gdx = grid(x + dx, y);
		const bool gdy = grid(x, y + dy);

		if (gdx && jumpX(Pos(x + dx, y), dx).isValid())
			break;

		if (gdy && jumpY(Pos(x, y + dy), dy).isValid())
			break;

		if ((gdx || gdy) && grid(x + dx, y + dy))
		{
			p.x += dx;
			p.y += dy;
		}
		else
		{
			p = npos;
			break;
		}
	}
	stepsDone += (int)steps;
	stepsRemain -= steps;
	return p;
}

inline Position Searcher::jumpX(Position p, int dx)
{
	const int y = p.y;
	const Position& endpos = endNode->pos;
	const int skip = this->skip;
	int steps = 0;

	int a = ~((!!grid(p.x, y + skip)) | ((!!grid(p.x, y - skip)) << 1));

	while (true)
	{
		const int xx = p.x + dx;
		const int b = (!!grid(xx, y + skip)) | ((!!grid(xx, y - skip)) << 1);

		if ((b & a) || p == endpos)
			break;
		if (!grid(xx, y))
		{
			p = npos;
			break;
		}

		p.x += dx;
		a = ~b;
		++steps;
	}

	stepsDone += (int)steps;
	stepsRemain -= steps;
	return p;
}

inline Position Searcher::jumpY(Position p, int dy)
{
	const int x = p.x;
	const Position& endpos = endNode->pos;
	const int skip = this->skip;
	int steps = 0;

	int a = ~((!!grid(x + skip, p.y)) | ((!!grid(x - skip, p.y)) << 1));

	while (true)
	{
		const int yy = p.y + dy;
		const int b = (!!grid(x + skip, yy)) | ((!!grid(x - skip, yy)) << 1);

		if ((a & b) || p == endpos)
			break;
		if (!grid(x, yy))
		{
			p = npos;
			break;
		}

		p.y += dy;
		a = ~b;
	}

	stepsDone += (int)steps;
	stepsRemain -= steps;
	return p;
}

int Searcher::findNeighbors(const Node *n, Position *wptr)
{
	Position *w = wptr;
	const int x = n->pos.x;
	const int y = n->pos.y;

	if (!n->parent)
	{
		// straight moves
		CheckAddPos(w, -skip, 0, x, y);
		CheckAddPos(w, 0, -skip, x, y);
		CheckAddPos(w, 0, skip, x, y);
		CheckAddPos(w, skip, 0, x, y);

		// diagonal moves + prevent tunneling
		AddPosNoTunnel(w, -skip, -skip, x, y);
		AddPosNoTunnel(w, -skip, skip, x, y);
		AddPosNoTunnel(w, skip, -skip, x, y);
		AddPosNoTunnel(w, skip, skip, x, y);

		return int(w - wptr);
	}

	// jump directions (both -1, 0, or 1)
	int dx = int(x - n->parent->pos.x);
	dx /= Max(my_abs(dx), 1);
	dx *= skip;
	int dy = int(y - n->parent->pos.y);
	dy /= Max(my_abs(dy), 1);
	dy *= skip;

	if (dx && dy)
	{
		// diagonal
		// natural neighbors
		bool walkX = false;
		bool walkY = false;
		if ((walkX = grid(x + dx, y)))
			*w++ = Pos(x + dx, y);
		if ((walkY = grid(x, y + dy)))
			*w++ = Pos(x, y + dy);

		if (walkX || walkY)
			CheckAddPos(w, dx, dy, x, y);

		// forced neighbors
		if (walkY && !CheckGrid(-dx, 0, x, y))
			CheckAddPos(w, -dx, dy, x, y);

		if (walkX && !CheckGrid(0, -dy, x, y))
			CheckAddPos(w, dx, -dy, x, y);
	}
	else if (dx)
	{
		// along X axis
		if (CheckGrid(dx, 0, x, y))
		{
			AddPos(w, dx, 0, x, y);

			// Forced neighbors (+ prevent tunneling)
			if (!CheckGrid(0, skip, x, y))
				CheckAddPos(w, dx, skip, x, y);
			if (!CheckGrid(0, -skip, x, y))
				CheckAddPos(w, dx, -skip, x, y);
		}


	}
	else if (dy)
	{
		// along Y axis
		if (CheckGrid(0, dy, x, y))
		{
			AddPos(w, 0, dy, x, y);

			// Forced neighbors (+ prevent tunneling)
			if (!CheckGrid(skip, 0, x, y))
				CheckAddPos(w, skip, dy, x, y);
			if (!CheckGrid(-skip, 0, x, y))
				CheckAddPos(w, -skip, dy, x, y);
		}
	}
	return int(w - wptr);
}

void Searcher::identifySuccessors(const Node *n)
{
	Position buf[8];
	const int num = findNeighbors(n, &buf[0]);
	for (int i = num - 1; i >= 0; --i)
	{
		// Invariant: A node is only a valid neighbor if the corresponding grid position is walkable (asserted in jumpP)
		Position jp = jumpP(buf[i], n->pos);
		if (!jp.isValid())
			continue;

		// Now that the grid position is definitely a valid jump point, we have to create the actual node.
		Node *jn = getNode(jp);
		if (!jn->isClosed())
		{
			int extraG = Euclidean(jn, n);
			int newG = n->g + extraG;
			if (!jn->isOpen() || newG < jn->g)
			{
				jn->g = newG;
				jn->f = jn->g + Manhattan(jn, endNode);
				jn->parent = n;
				if (!jn->isOpen())
				{
					open.push(jn);
					jn->setOpen();
				}
				else
					open.fixup(jn);
			}
		}
	}
}

Position* Searcher::PadPosition(Position dest, Position start) const
{
	Position temp(Pos(dest.x - start.x, dest.y - start.y));
	if (temp.x != 0 && temp.y != 0)
	{
		if (grid(start.x + temp.x, start.y))
			return(new Position(Pos(start.x + temp.x, start.y)));
		else
			return(new Position(Pos(start.x, start.y + temp.y)));
	}

	return NULL;
}

bool Searcher::generatePath(std::vector<Position*>*& path, int step) const
{
	if (!endNode)
		return false;
	size_t offset = path->size();
	if (step)
	{
		const Node *next = endNode;
		const Node *prev = endNode->parent;
		if (!prev)
			return false;
		do
		{
			const int x = next->pos.x;
			const int y = next->pos.y;
			int dx = int(prev->pos.x - x);
			int dy = int(prev->pos.y - y);
			const int steps = Max(my_abs(dx), my_abs(dy));
			dx /= Max(my_abs(dx), 1);
			dy /= Max(my_abs(dy), 1);
			dx *= int(step);
			dy *= int(step);
			int dxa = 0, dya = 0;
			for (int i = 0; i < steps; i += step)
			{
				Position* nextPath = new Position(Pos(x + dxa, y + dya));
				//check direction + insert path
				if (!path->empty())
				{
					Position* temp = PadPosition((Pos(x + dxa, y + dya)), *path->back());
					if (temp)
					{
						path->push_back(temp);
					}
				}
				path->push_back(nextPath);
				dxa += dx;
				dya += dy;
			}
			next = prev;
			prev = prev->parent;
		} while (prev);

		Position* temp = PadPosition((Pos(next->pos.x, next->pos.y)), *path->back());
		if (temp)
		{
			path->push_back(temp);
		}
	}
	else
	{
		const Node *next = endNode;
		if (!next->parent)
			return false;
		do
		{
			path->push_back(new Position(next->pos));
			next = next->parent;
		} while (next->parent);
	}
	std::reverse(path->begin() + offset, path->end());
	return true;
}

bool Searcher::findPath(std::vector<Position*>*& path, Position start, Position end, int step /* = 0 */)
{
	Result res = findPathInit(start, end);

	// If this is true, the resulting path is empty (findPathFinish() would fail, so this needs to be checked before)
	if (res == FOUND_PATH)
		return true;

	while (true)
	{
		switch (res)
		{
		case NEED_MORE_STEPS:
			res = findPathStep(0);
			break; // the switch

		case FOUND_PATH:
			return findPathFinish(path, step);

		case NO_PATH:
		default:
			return false;
		}
	}
}

Result Searcher::findPathInit(Position start, Position end)
{
	for (NodeGrid::iterator it = nodegrid.begin(); it != nodegrid.end(); ++it)
		it->second.clearState();
	open.clear();
	endNode = NULL;
	stepsDone = 0;

	// If skip is > 1, make sure the points are aligned so that the search will always hit them
	start.x = (start.x / skip) * skip;
	start.y = (start.y / skip) * skip;
	end.x = (end.x / skip) * skip;
	end.y = (end.y / skip) * skip;

	if (start == end)
	{
		// There is only a path if this single position is walkable.
		// But since the starting position is omitted, there is nothing to do here.
		return grid(end.x, end.y) ? FOUND_PATH : NO_PATH;
	}

	// If start or end point are obstructed, don't even start
	if (!grid(start.x, start.y) || !grid(end.x, end.y))
		return NO_PATH;

	open.push(getNode(start));
	endNode = getNode(end);

	return NEED_MORE_STEPS;
}

Result Searcher::findPathStep(int limit)
{
	stepsRemain = limit;
	do
	{
		if (open.empty())
			return NO_PATH;
		Node *n = open.pop();
		n->setClosed();
		if (n == endNode)
			return FOUND_PATH;
		identifySuccessors(n);
	} while (stepsRemain >= 0);
	return NEED_MORE_STEPS;
}

bool Searcher::findPathFinish(std::vector<Position*>*& path, int step /* = 0 */)
{
	return generatePath(path, step);
}

void Searcher::freeMemory()
{
	NodeGrid v;
	nodegrid.swap(v);
	endNode = NULL;
	open.clear();
	// other containers known to be empty.
}
