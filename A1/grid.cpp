#include <algorithm>
#include <iostream>

#include "grid.hpp"

Grid::Grid( size_t d )
	: m_dim( d )
{
	m_height = new int[ d * d ];
	m_col = new int[ d * d ];

	reset();
}

void Grid::reset()
{
	size_t sz = m_dim*m_dim;
	std::fill( m_height, m_height + sz, 0 );
	std::fill( m_col, m_col + sz, 0 );
}

Grid::~Grid()
{
	delete [] m_height;
	delete [] m_col;
}

size_t Grid::getDim() const
{
	return m_dim;
}

int Grid::getHeight( int cell ) const
{
	return m_height[ cell ];
}

int Grid::getColour( int cell ) const
{
	return m_col[ cell ];
}

void Grid::setHeight( int cell, int height )
{
	m_height[ cell ] = height;
}

void Grid::setColour( int cell, int colour )
{
	m_col[ cell ] = colour;
}
