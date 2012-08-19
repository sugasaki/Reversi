#ifndef REVERSI_H_INCLUDED
#define REVERSI_H_INCLUDED


#include <string>
#include <sstream>
#include <stdexcept>


const int BOARD_SIZE =  8;
const int MAX_TURNS =  60;


typedef int Color;

const Color EMPTY = 0;
const Color WHITE = -1;
const Color BLACK = 1;
const Color WALL  = 2;



//石の位置座標 構造体
struct Point
{
	int x;
	int y;
	
	//コンストラクタ
	Point()
	{
		Point(0, 0);
	}
	
	//コンストラクタ（X,Y指定）
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	
	//コンストラクタ（A1フォーマット指定）
	Point(std::string coordstr) throw(std::invalid_argument)
	{
		if(coordstr.length() < 2)
			throw std::invalid_argument("The argument must be Reversi style coordinates!");
		
		x = coordstr[0] - 'a'+1;
		y = coordstr[1] - '1'+1;
	}


	//文字列変換 toStringと同等 ex. (x=1,y=1) => A1
	operator std::string() const
	{
		std::ostringstream oss;
		oss << static_cast<char>('a'+x-1) << static_cast<char>('1'+y-1);
		return oss.str();
	}



};


//石 構造体
struct Disc : public Point
{
	Color color;

	Disc() : Point(0, 0)
	{
		color = EMPTY;
	}

	Disc(int x, int y, Color color) : Point(x, y)
	{
		this->color = color;
	}

};


//それぞれの石の色数を格納する
template<typename T> class ColorStorage
{

private:
	T data[3];

public:

	T& operator[](Color color)
	{
		return data[color+1];
	}

	const T& operator[](Color color) const
	{
		return data[color+1];
	}


};



#endif