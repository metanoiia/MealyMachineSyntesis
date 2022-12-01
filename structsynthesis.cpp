#include "structsynthesis.h"

StructSynthesis::StructSynthesis( std::ifstream & f_table, std::ifstream & g_table ) :
    m_f( Table( f_table ) ),
    m_g( Table( g_table ) )
{
    m_numQ = static_cast <int> ( std::ceil( log2( m_f.getColCount() ) ) );//number of registers Q requiered for states storing
    m_Q = std::string( m_numQ, '0' );

    for( size_t i = 0; i < m_numQ; i++ )
    {
        m_D.push_back( DNF( getStateStringDNF( i ) ) );//push string DNF
        m_D.back().minimize(); //minimize it
    }

    std::unordered_set < TableItem, TableItem::HashFunction > outputs = m_g.getUniqueItems();

    for( const TableItem & item : outputs )
        m_y.insert( std::make_pair < TableItem, DNF > (  TableItem( item.value(), item.isUndefined() ), DNF() ) ); //initilize output items

    setOutputsDNF();

    for( auto & kv : m_y )
        kv.second.minimize();
}
bool equals( std::string first, std::string second )
{
    if( first.length() != second.length() )
        return false;
    else
    {
        std::string::iterator firstIt = first.begin();
        std::string::iterator secondIt = second.begin();

        for( ; firstIt != first.end(); firstIt++, secondIt++ )
        {
            if( ( *firstIt == '-' || *secondIt == '-' ) )
                continue;
            else
            {
                if( *firstIt != *secondIt )
                    return false;
            }
        }
        return true;
    }
}

std::list < std::string > StructSynthesis::put( std::list < int > xList )
{
    std::list < std::string > result;

    std::string outputStr;
    std::string xStr;
    std::string QStr;
    std::string yStr;

    for( int x : xList )
    {
        if( ( x < 0 ) ||  ( x  >  m_f.getRowCount() - 1 ) )
            return std::list < std::string >();
        else
        {
            xStr = std::to_string( x ) + " ";//setting x Q y <- x value
            QStr = std::to_string( std::stoi( m_Q, 0, 2 ) ) + " ";
            std::string newQ = std::string( m_numQ, '0' );

            std::string sectionX( m_f.getRowCount(), '0' );
            sectionX.at( ( sectionX.length() - 1 ) - x  ) = '1';
            std::string inputImpact = sectionX + m_Q;

            for( size_t i = 0; i < m_D.size(); i++ )
            {
                std::list < std::string > mdnf =  m_D.at( i ).print();

                for( const std::string & c : mdnf )
                {
                    if( equals( c, inputImpact ) )
                    {
                        newQ.at( ( m_numQ - 1 ) - i ) = '1';
                        break;
                    }
                }
            }

            for( std::pair < TableItem, DNF > kv : m_y )
            {
                std::list < std::string > mdnf = kv.second.print();

                for( const std::string & c : mdnf )
                {
                    if( equals( c, inputImpact ) )
                    {
                        yStr = ( kv.first.isUndefined() ) ? "-" : std::to_string( kv.first.value() );//setting x Q y <- y value
                        break;
                    }
                }
            }


            m_Q = newQ;
            outputStr = xStr + QStr + yStr;
            result.push_back( outputStr );
        }
    }
    std::string lastStr = "  " + std::to_string( std::stoi( m_Q, 0, 2 ) );

    result.push_back( lastStr );

    return result;
}

std::string StructSynthesis::getStateStringDNF( int idx )
{
    std::string constituent; //'sectX'+'sectQ'

    std::string sectionX( m_f.getRowCount(), '0' );
    std::string sectionQ( m_numQ, '0' );

    std::string res( 1 << ( sectionX.length() + sectionQ.length() ), '0' );
    //'res' is string of values like str ="0 0 1 0 1 - 0 1" with len 2^(len(X) + len(Q))
    //                                  i: 0 1 2 3 4 5 6 7
    //'i' is num of set

    int constNum; // num of set when function takes '1'

    int mask = 1 << idx;
    for( int i = 0; i < m_f.getRowCount(); i++ )
    {
        for( int j = 0; j < m_f.getColCount(); j++ )
        {
            TableItem item = m_f.at( i, j );
            sectionQ.assign( std::string( m_numQ, '0' ) );
            sectionX.assign( std::string( m_f.getRowCount(), '0' ) );

            if( ( mask & item.value() ) == mask || item.isUndefined() )
            {
                sectionX.at( ( sectionX.length() - 1 ) - i  ) = '1';
                sectionQ = decToBinStr( j, sectionQ.length() );

                constituent = sectionX + sectionQ;
                constNum = std::stoi( constituent, 0, 2 ); //converting bool string to int

                res.at( constNum ) = ( !item.isUndefined() ) ? '1' : '-';
            }
        }
    }

    return res;
}

void StructSynthesis::setOutputsDNF()
{
    std::string constituent; //'sectX'+'sectQ'

    std::string sectionX( m_f.getRowCount(), '0' );
    std::string sectionQ( m_numQ, '0' );

    std::unordered_map < TableItem, std::string, TableItem::HashFunction > yPDNF; //outputs and PDNFs
    //second is string of values like str ="0 0 1 0 1 0 0 1" with len 2^(len(X) + len(Q))
    //                                   i: 0 1 2 3 4 5 6 7
    //'i' is num of set

    for( auto & kv : m_y )
        yPDNF.insert( std::make_pair( kv.first, std::string( 1 << ( sectionX.length() + sectionQ.length() ), '0' ) ) );


    int constNum; // num of set when function takes '1'

    for( int i = 0; i < m_g.getRowCount(); i++ )
    {
        for( int j = 0; j < m_g.getColCount(); j++ )
        {
            TableItem item = m_g.at( i, j );
            sectionQ.assign( std::string( m_numQ, '0' ) );
            sectionX.assign( std::string( m_g.getRowCount(), '0' ) );

            sectionX.at( ( sectionX.length() - 1 ) - i  ) = '1';
            sectionQ = decToBinStr( j, sectionQ.length() );

            constituent = sectionX + sectionQ;
            constNum = std::stoi( constituent, 0, 2 ); //converting bool string to int

            yPDNF.find( item )->second.at( constNum ) = '1';
        }
    }

    for( auto & kv : yPDNF )
        m_y.find( kv.first )->second.setData( kv.second ); //setting input strings for each output 'y' DNF

}

void StructSynthesis::print( std::ostream & stream )
{
    stream << "Note: each function is introduced by disjunctions of sets like this:" << std::endl;
    for( int i = m_f.getRowCount() - 1; i >= 0; i-- )
        stream << "x" << i << " ";

    for( int i = m_numQ - 1; i >= 0; i-- )
        stream << "Q" << i << " ";

    stream << std::endl <<"Where xi and Qi take values '0' or '1'" << std::endl << std::endl;

    for( size_t i = 0; i < m_D.size(); i++ )
    {
        stream << "D" << i << ":" << std::endl;
        m_D.at( i ).print( stream );
        stream << std::endl;
    }

    for( auto & kv : m_y )
    {
        if( !kv.first.isUndefined() )
            stream << "y"<< kv.first.value() << ":" << std::endl;
        else
            stream << "-"<< ":" << std::endl;

        kv.second.print( stream );
        stream << std::endl;
    }
}

