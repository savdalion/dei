#pragma once

#include "configure.h"
#include <ostream>


namespace dei {

class Option;

}




namespace std {

ostream& operator<<( std::ostream& out, const dei::Option& );

} // std





namespace dei {


/**
* @todo ��������� �� ����� configure.json.
*/
class Option {

protected:
    /**
    * ��������� ������ �� ����� dei::CONFIGURE.
    * �������������� ������.
    */
    Option();


public:
    static inline Option* singleton() {
        if ( instance ) {
            return instance;
        }
        instance = new Option();
        return instance;
    }



    virtual inline ~Option() {
    }



protected:
    /**
    * ������ JSON, �������������� ����� ����� �������� �� ��������� �
    * �������� ��� �������.
    */



    /**
    * ���������.
    */
    static Option* instance;

};



} // dei
