#pragma once

#include <vector>


// TODO: This really doesn't need to be a templated class, originally had just done it for the fun of it
// TODO: This is specifically made for the weather map

template<typename T, int _cols, int _rows>
// TODO: Name this as the container it actually is
class Array2D // TODO: Named array but really using a vector, this class is poorly named in general. 
{
private:

public:
	std::vector<T> data;

	int getIndex(int col, int row) const;
public:

	int rows = _rows;
	int cols = _cols;

	Array2D();
	T& operator ()(int col, int row);

	// TODO: Don't like having to have this function like this, is there any other ways to do what I want here
	const T& getConst(int col, int row) const;

	void Resize(int _cols, int _rows);


	int getSize() const;

	typename std::vector<T>::iterator Begin();
	typename std::vector<T>::iterator End();
};


template<typename T, int _cols, int _rows>
inline int Array2D<T, _cols, _rows>::getIndex(int col, int row) const
{
	return row * cols + col;
}

template<typename T, int _cols, int _rows>
inline Array2D<T, _cols, _rows>::Array2D()
{
	data.resize(getSize());
}

template<typename T, int _cols, int _rows>
inline T& Array2D<T, _cols, _rows>::operator()(int col, int row)
{
	col = (col % cols + cols) % cols;
	row = (row % rows + rows) % rows;

	return data[getIndex(col, row)];
}

template<typename T, int _cols, int _rows>
inline const T& Array2D<T, _cols, _rows>::getConst(int col, int row) const
{
	col = (col % cols + cols) % cols;
	row = (row % rows + rows) % rows;

	return data.at(getIndex(col, row));
}

template<typename T, int _cols, int _rows>
inline void Array2D<T, _cols, _rows>::Resize(int _cols, int _rows)
{
	cols = _cols;
	rows = _rows;
	data.resize(getSize());
}

template<typename T, int _cols, int _rows>
inline int Array2D<T, _cols, _rows>::getSize() const
{
	return rows * cols;
}

template<typename T, int _cols, int _rows>
inline typename std::vector<T>::iterator Array2D<T, _cols, _rows>::Begin()
{
	return data.begin();
}

template<typename T, int _cols, int _rows>
inline typename std::vector<T>::iterator Array2D<T, _cols, _rows>::End()
{
	return data.end();
}
