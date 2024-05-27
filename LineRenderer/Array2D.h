#pragma once

#include <vector>


// TODO: This is specifically made for the weather map

template<typename T, int _cols, int _rows>
class Array2D // TODO: Named array but really using a vector
{
private:
	int rows = _rows;
	int cols = _cols;

	std::vector<T> data;

	int getIndex(int col, int row) const;
public:

	Array2D();
	T& operator ()(int col, int row);


	int getRowCount() const;
	int getColCount() const;
	int getSize() const;

	typename std::vector<T>::iterator Begin();
	typename std::vector<T>::iterator End();
};

template<typename T, int _rows, int _cols>
inline int Array2D<T, _rows, _cols>::getIndex(int row, int col) const
{
	return row * cols + col;
}

template<typename T, int _rows, int _cols>
inline Array2D<T, _rows, _cols>::Array2D()
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
inline int Array2D<T, _cols, _rows>::getRowCount() const
{
	return rows;
}

template<typename T, int _cols, int _rows>
inline int Array2D<T, _cols, _rows>::getColCount() const
{
	return cols;
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
