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

	// ���W��ϊ�����f5����n�܂�悤�ɂ���
	std::vector<Point> normalized;
	for(int i=0; i<history.size(); i++)
	{
		Point p = history[i];
		p = transformer.normalize(p);

		normalized.push_back(p);
	}
	

	// ���݂܂ł̊������X�g�ƒ�΂̑Ή������
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
			// ��΂��O��Ă���
			return board.getMovablePos();
		}
	}

	// �����ƒ�΂̏I��肪��v���Ă����ꍇ
	if(node->child == NULL) return board.getMovablePos();

	Point next_move = getNextMove(node);

	// ���W�����̌`�ɕϊ�����
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
			// �V������Ύ�
			node->child = new Node();
			node = node->child;
			node->point.x = p.x;
			node->point.y = p.y;
		}
		else
		{
			// �Z��m�[�h�̒T���Ɉڂ�
			node = node->child;

			while(true)
			{
				// ���ɂ��̎�̓f�[�^�x�[�X���ɂ���A���̎}��������
				if(node->point == p) break;

				// ��Ζ؂̐V�����}
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
