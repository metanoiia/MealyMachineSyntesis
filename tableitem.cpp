#include "tableitem.h"

TableItem::TableItem( int value, bool undefined ) :
    m_value( value ),
    m_undefined( undefined )
{

}

bool TableItem::isUndefined() const
{
    return m_undefined;
}

int TableItem::value() const
{
    return m_value;
}

bool operator == ( const TableItem & left, const TableItem & right )
{
    return ( left.m_undefined == right.m_undefined &&
             left.m_value == right.m_value );
}

bool operator != ( const TableItem & left, const TableItem & right )
{
    return !(left == right);
}

TableItem::~TableItem()
{

}
