#ifndef TABLE_H
#define TABLE_H
#include <vector>
#include "tableitem.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_set>

class Table
{
private:
    std::vector < std::vector < TableItem > > m_matrix;

public:
    Table( std::ifstream & fin );

    TableItem at( int i, int j );

    int getRowCount();
    int getColCount();

    std::vector < TableItem > getRow( int i );
    std::unordered_set < TableItem, TableItem::HashFunction > getUniqueItems();
};

#endif // TABLE_H
