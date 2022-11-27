#ifndef TABLEITEM_H
#define TABLEITEM_H
#include <functional>

class TableItem
{
private:
     int m_value;
     bool m_undefined;
public:
    TableItem( int value, bool undefined );
    ~TableItem();

    int value() const;
    bool isUndefined() const;

    struct HashFunction
    {
        size_t operator()( const TableItem & item ) const
        {
            size_t valueHash = std::hash < int >()( item.m_value );
            size_t undefHash = std::hash < bool >()( item.m_undefined ) << 1;
            return valueHash ^ undefHash;
        }
    };

    friend bool operator == ( const TableItem & left, const TableItem & right );
    friend bool operator != ( const TableItem & left, const TableItem & right );
};

#endif // TABLEITEM_H
