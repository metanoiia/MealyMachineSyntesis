#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "structsynthesis.h"

int main()
{
    std::ifstream f("f_table1.txt");
    std::ifstream g("g_table1.txt");

    StructSynthesis ss( f, g );

    std:: list < std::string > res;
    std::list < int > x = { 0, 1, 2, 3, 4, 5, 6 }; //input impacts

    res = ss.put( x );

    std::ofstream fout( "output.txt", std::ios_base::trunc );

    if( !fout.is_open() )
        return -1;

    ss.print( fout );
    fout.close();

    fout.open( "check.txt", std::ios_base::trunc );

    for( const std::string & s : res )
        fout << s << std::endl;

    fout.close();

    return 0;
}
