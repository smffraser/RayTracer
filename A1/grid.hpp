#pragma once

class Grid
{
public:
	Grid( size_t dim );
	~Grid();

	void reset();

	size_t getDim() const;

	int getHeight( int cell ) const;
	int getColour( int cell ) const;

	void setHeight( int cell, int height );
	void setColour( int cell, int colour );
	
private:
	size_t m_dim;
	int *m_height;
	int *m_col;
};
