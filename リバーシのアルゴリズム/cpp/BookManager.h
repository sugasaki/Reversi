#ifndef BOOK_MANAGER_H_INCLUDED
#define BOOK_MANAGER_H_INCLUDED

#include "Board.h"

class CoordinatesTransformer
{
public:

	CoordinatesTransformer(const Point& first)
	{
		Rotate = 0;
		Mirror = false;

		if(first == Point("d3"))
		{
			Rotate = 1;
			Mirror = true;
		}
		else if(first == Point("c4"))
		{
			Rotate = 2;
		}
		else if(first == Point("e6"))
		{
			Rotate = -1;
			Mirror = true;
		}
	}

	Point normalize(const Point& p) const
	{
		Point newp = rotatePoint(p, Rotate);
		if(Mirror) newp = mirrorPoint(newp);
		
		return newp;
	}
	
	Point denormalize(const Point& p) const
	{
		Point newp(p.x, p.y);
		if(Mirror) newp = mirrorPoint(newp);
		
		newp = rotatePoint(newp, -Rotate);
		
		return newp;
	}

private:

	int Rotate;
	bool Mirror;

	Point rotatePoint(const Point& old_point, int rotate) const
	{
		rotate %= 4;
		if(rotate < 0) rotate += 4;

		Point new_point;
		
		switch(rotate)
		{
		case 1:
			new_point.x = old_point.y;
			new_point.y = BOARD_SIZE - old_point.x+1;
			break;
		case 2:
			new_point.x = BOARD_SIZE - old_point.x+1;
			new_point.y = BOARD_SIZE - old_point.y+1;
			break;
		case 3:
			new_point.x = BOARD_SIZE - old_point.y+1;
			new_point.y = old_point.x;
			break;
		default: // 0
			new_point.x = old_point.x;
			new_point.y = old_point.y;
			break;
		}
		
		return new_point;
	}

	Point mirrorPoint(const Point& point) const
	{
		Point new_point;
		new_point.x = BOARD_SIZE - point.x + 1;
		new_point.y = point.y;

		return new_point;
	}

};


class BookManager
{
public:

	BookManager();
	virtual ~BookManager();
	std::vector<Point> find(const Board& board) const;

private:

	struct Node
	{
		Node() : child(NULL), sibling(NULL){}

		Node* child;
		Node* sibling;
		Point point;
	};

	Node* Root;
	
	void add(const std::vector<Point>& book);
	Point getNextMove(const Node* node) const;
	void erase(Node* node) const;

};


#endif // BOOK_MANAGER_H_INCLUDED
