#include "BookManager.h"

static const char* BOOK_FILE_NAME = "reversi.book";

#include <fstream>
#include <cstdlib>

BookManager::BookManager() : Root(NULL)
{
	Root = new Node();
	Root->point = Point("f5");

	std::ifstream ifs(BOOK_FILE_NAME);
	if(ifs.fail()) return;
	
	std::string line;
	while(ifs >> line)
	{
		std::vector<Point> book;
		for(int i=0; i<line.length(); i+=2)
		{
			Point p;
			try
			{
				p = Point(line.substr(i));
			}
			catch(std::invalid_argument& e)
			{
				break;
			}

			book.push_back(p);
		}

		add(book);

	}

}

std::vector<Point> BookManager::find(const Board& board) const
{
	Node* node = Root;
	std::vector<Point> history = board.getHistory();
	
	if(history.empty()) return board.getMovablePos();
	
	Point first = history[0];
	CoordinatesTransformer transformer(first);

	// 座標を変換してf5から始まるようにする
	std::vector<Point> normalized;
	for(int i=0; i<history.size(); i++)
	{
		Point p = history[i];
		p = transformer.normalize(p);

		normalized.push_back(p);
	}
	

	// 現在までの棋譜リストと定石の対応を取る
	for(int i=1; i<normalized.size(); i++)
	{
		Point p = normalized[i];

		node = node->child;
		while(node != NULL)
		{
			if(node->point == p) break;

			node = node->sibling;
		}
		if(node == NULL)
		{
			// 定石を外れている
			return board.getMovablePos();
		}
	}

	// 履歴と定石の終わりが一致していた場合
	if(node->child == NULL) return board.getMovablePos();

	Point next_move = getNextMove(node);

	// 座標を元の形に変換する
	next_move = transformer.denormalize(next_move);
	
	std::vector<Point> v;
	v.push_back(next_move);
	
	return v;

}

Point BookManager::getNextMove(const Node* node) const
{
	std::vector<Point> candidates;
	
	for(Node* p = node->child; p != NULL; p = p->sibling)
	{
		candidates.push_back(p->point);
	}
	
	unsigned index = std::rand() % candidates.size();
	return candidates[index];
}

void BookManager::add(const std::vector<Point>& book)
{
	Node *node = Root;

	for(unsigned i=1; i<book.size(); i++)
	{
		Point p = book[i];

		if(node->child == NULL)
		{
			// 新しい定石手
			node->child = new Node();
			node = node->child;
			node->point.x = p.x;
			node->point.y = p.y;
		}
		else
		{
			// 兄弟ノードの探索に移る
			node = node->child;

			while(true)
			{
				// 既にこの手はデータベース中にあり、その枝を見つけた
				if(node->point == p) break;

				// 定石木の新しい枝
				if(node->sibling == NULL)
				{
					node->sibling = new Node();

					node = node->sibling;
					node->point.x = p.x;
					node->point.y = p.y;
					break;
				}
				
				node = node->sibling;
			}
		}
	}
}


BookManager::~BookManager()
{
	erase(Root);
}

void BookManager::erase(Node* node) const
{
	if(node == NULL) return;

	Node* next;
	for(Node* p = node; p != NULL; p = next)
	{
		next = p->sibling;
		erase(p->child);
		p->child = NULL;
		p->sibling = NULL;

		delete p;
	}

}
