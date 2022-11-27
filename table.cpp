#include "table.h"

Table::Table( std::ifstream & fin )
{

    std::string line;

    while( !fin.eof() )
    {
        std::getline( fin, line );
        if( !line.empty() )
        {
            line.erase( remove_if( line.begin(), line.end(), isspace), line.end() );

            std::vector < TableItem > row;
            std::transform( line.begin(), line.end(), std::back_inserter( row ), //push to row table items
                            []( char sym ){  return TableItem( sym  - '0',  sym == '-' ); } );

            m_matrix.push_back( row ); //push to matrix new row
        }
    }
}

int Table::getRowCount()
{
    return m_matrix.size();
}

int Table::getColCount()
{
    return m_matrix.at(0).size();
}

std::vector < TableItem > Table::getRow( int i )
{
    return m_matrix.at( i );
}

TableItem Table::at( int i, int j )
{
    return  m_matrix.at(i).at(j);
}

std::unordered_set < TableItem, TableItem::HashFunction > Table::getUniqueItems()
{
    std::unordered_set < TableItem, TableItem::HashFunction > set;

    for( std::vector < TableItem > row : m_matrix )
    {
        std::copy( row.begin(), row.end(), std::inserter( set, set.begin() ) );
    }
    return set;
}
