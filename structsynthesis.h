#ifndef STRUCTSYNTHESIS_H
#define STRUCTSYNTHESIS_H
#include <unordered_map>
#include "table.h"
#include "QuineMcCluskey/dnf.h"

class StructSynthesis
{
private:
    Table m_f;
    Table m_g;

    std::vector < DNF > m_D; // excitation functions for registers
    std::unordered_map < TableItem, DNF, TableItem::HashFunction > m_y;// outputs items and functions for outputs

    size_t m_numQ; //num of registers 'Q' to store current state

    std::string m_Q;

    std::string currentOutputString;
    std::string nextOutputString;

    std::string getStateStringDNF( int idx ); // 'idx' is number of bit

    void setOutputsDNF(); //init DNF for m_y

    friend bool equals( std::string first, std::string second );

public:
    StructSynthesis( std::ifstream & f_table, std::ifstream & g_table );

    std::list < std::string > put( std::list < int > xList );

    void print( std::ostream & stream );

};

#endif // STRUCTSYNTHESIS_H
