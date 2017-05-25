#pragma once

template <class T>
class block
{

#define BLOCK_ZAHL 9

public:

	static ushort size;
	T blocks[BLOCK_ZAHL];


	block()
	{
	
	}
	
	block(Point p)
	{
		set_position(p);
	}
	

	~block()
	{
	
	}

	Size get_size()
	{
		return Size(size, size);
	}

	void set_position(Point p)
	{
		int sz = blocks[0].get_size().width; //HACK nur breite

		blocks[0].set_position(p);
		blocks[1].set_position(Point(p.x - sz, p.y));
		blocks[2].set_position(Point(p.x - sz, p.y - sz));
		blocks[3].set_position(Point(p.x, p.y - sz));
		blocks[4].set_position(Point(p.x + sz, p.y - sz));
		blocks[5].set_position(Point(p.x + sz, p.y));
		blocks[6].set_position(Point(p.x + sz, p.y + sz));
		blocks[7].set_position(Point(p.x, p.y + sz));
		blocks[8].set_position(Point(p.x - sz, p.y + sz));

		size = 3 * sz;
	}
};

template <class T>
ushort block<T>::size = 0;
