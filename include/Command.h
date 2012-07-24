#pragma once

#include "Delivery.h"
#include <typelib/typelib.h>


namespace dei {
    namespace command {

struct Command;
typedef std::shared_ptr< Command >  CommandPtr;
typedef std::unique_ptr< Command >  CommandUPtr;
typedef std::vector< CommandPtr >  listCommandPtr_t;

    }
}



namespace dei {

    
/**
* ������� ��� ������ � ����������.
*/

namespace command {


    
struct Command {
    virtual void run() = 0;
    virtual std::string name() const = 0;
    virtual std::string hotkey() const = 0;
    virtual std::string confirmation() const = 0;
    virtual std::string success() const = 0;
    virtual std::string failure() const = 0;


protected:

    // ��� �������� �������� � ������� CURL
    static inline int writer( char *data, size_t size, size_t nmemb, std::string* dest ) {
        int written = 0;
        if ( dest ) {
            written = size * nmemb;
            dest->append( data, written );
        }
        return written;
    }


    // ��� ��������� ������� � ������� CURL
    static inline size_t reader( void* ptr, size_t size, size_t nmemb, std::string* stream ) {
        int actual  = static_cast< int >( stream->size() );
        int written = size * nmemb;
        if (written > actual) {
            written = actual;
        }
        memcpy( ptr, stream->c_str(), written );
        stream->erase( 0, written );
        return written;
    }

};





struct DeliveryCommand : public Command {
    /**
    * �������� ��� (������ � ��������).
    */
    listDeliveryPtr_t ld;

    
    inline DeliveryCommand() {
    }


    inline DeliveryCommand( const listDeliveryPtr_t& ld ) : ld( ld ) {
    }

};






/**
* ������� ��� ������ � ����� ������.
*/
namespace novaposhta {


/**
* �������� ����� � ������� ��� ������� ����������� � ���.
*/
struct CreateTTN : public DeliveryCommand {

    inline CreateTTN() {
    }

    inline CreateTTN( const listDeliveryPtr_t& ld ) : DeliveryCommand( ld ) {
    }


    virtual void run();


    virtual inline std::string name() const {
        return "������� ���� ��� ������� ���.";
    }


    virtual inline std::string hotkey() const {
        return "1";
    }


    virtual inline std::string confirmation() const {
        //@todo return "�������� ��� �� ����� \"����� �����\"? ������ ����� ����� �� �����."; - ���������� �������.
        std::ostringstream ss;
        ss << "����������� ���� � ����������� ��� ������� �� ����� \"����� �����\"."
            << " ������ ����� ����� �� ����� \"1.csv\""
            << " � �������� � ���� \"b.xml\". �������� ��������� ����, ����� ������ �������������"
            << " ������ � ������ �������� \"����� �����\".";
        return ss.str();
    }


    virtual inline std::string success() const {
        /*
        std::ostringstream ss;
        ss << ld.size() << " ��� ����������."
           << " \"����� �����\" ��������� ��������� ������:" << std::endl;
        typelib::print( ss, mResult );
        return ss.str();
        */
        //return "������ ��� ������� ��� �������� � ����� \"data/b.xml\". �������� ��� � ���������� � ����� �� ����� \"����� �����\".";
        std::ostringstream ss;
        ss << "�������� ������ - " << ld.size() << "."
           << " ������ ��� ������� ��� �������� � ����� \"data/b.xml\"."
           << " �������� ��� � ���������� � ����� �� ����� \"����� �����\".";
        return ss.str();
    }

    
    virtual inline std::string failure() const {
        return "���� ��� ������� ��� �� ������.";
    }

    
    /**
    * ��������� ���������� ���.
    */
    struct Result {
        DeliveryPtr d;
        // ����� ��������� � ���� ����� �����.
        std::string uid;
    };
    typedef std::vector< Result >  result_t;
    
    inline result_t const& result() const {
        return mResult;
    }



private:
    result_t mResult;

};






/**
* ������� ����� � ����������� � ������� � ����� � �������� ������
* �� "����� �����".
*/
struct FusionTTN : public DeliveryCommand {

    inline FusionTTN() {
    }


    virtual void run();


    virtual inline std::string name() const {
        return "������� ���� ��� ������� ����� ������ \"����� �����\" � �������� �������.";
    }


    virtual inline std::string hotkey() const {
        return "2";
    }


    virtual inline std::string confirmation() const {
        std::ostringstream ss;
        ss << "�� ��������������� �� ����� \"����� �����\" ����� \"b.xml\""
            << " � ����� \"1.csv\" �������� \"2.csv\" ��� ������� � �������."
            << " ������������ \"2.csv\" � �������, ����� ������ ������� ������ ������"
            << " \"����� �����\" � �������� � �������.";
        return ss.str();
    }


    virtual inline std::string success() const {
        return "��� ������ ���������.";
    }

    
    virtual inline std::string failure() const {
        return "���� ������������� ������.";
    }



    friend Delivery;

};



} // novaposhta





} // command

} // dei






namespace std {


inline std::ostream& operator<<( std::ostream& out, const dei::command::Command& c ) {
    out << c.hotkey() << " " << c.name();

    return out;
}




/**
* ��������� ������ ��� �������� �� ������.
*/
std::ostream& operator<<( std::ostream& out, const dei::command::novaposhta::CreateTTN& );




inline std::ostream& operator<<( std::ostream& out, const dei::command::novaposhta::CreateTTN::Result& r ) {
    out << r.d->number << " -> " << r.uid << std::endl;

    return out;
}


} // std