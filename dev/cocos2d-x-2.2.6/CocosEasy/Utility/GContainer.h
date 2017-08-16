#ifndef __CE_GCONTAINER_H__
#define __CE_GCONTAINER_H__

#include "Common/CocosEasyDef.h"

template <class T>
class CVector2D
{
public:
	void reserve(size_t row, size_t col, T zero)
	{
		m_container.reserve(row);
		for (size_t i=0; i<m_container.capacity(); ++i)
		{
			std::vector< T > list;
			m_container.push_back(list);
			std::vector<T>& column = m_container[i];
			column.reserve(col);
			for (size_t j=0; j<column.capacity(); ++j)
			{
				column.push_back(zero);
			}
		}
	}
	inline std::vector< T >& operator[](size_t row)
	{
		assert(row < m_container.size());
		return m_container[row];
	}

private:
	std::vector< std::vector< T > > m_container;
};


#endif