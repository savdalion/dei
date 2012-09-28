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
    * ���� ������������.
    */
    const std::string configure;


    /**
    * �������� ��� (������ � ��������).
    */
    listDeliveryPtr_t ld;

    
    //inline DeliveryCommand() : configure( "" ) {
    //}


    inline DeliveryCommand(
        const listDeliveryPtr_t& ld,
        const std::string& configure
    ) :
        configure( configure ),
        ld( ld )
    {
    }

};






/**
* ������� ��� ������ � ����� ������.
*/
namespace novaposhta {


/**
* �������� ����� � ������� ��� ������� ����������� � ���.
* ������������ ��������� ������������.
*/
struct CreateTTN : public DeliveryCommand {

    inline CreateTTN(
        const std::string& configure
    ) : DeliveryCommand( listDeliveryPtr_t(), configure ) {
    }

    inline CreateTTN(
        const listDeliveryPtr_t& ld,
        const std::string& configure
    ) : DeliveryCommand( ld, configure ) {
    }


    virtual void run();


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



protected:
    result_t mResult;

};






/**
* �������� ����� � ������� ��� ������� ����������� � ���.
* ������������ ������������ �� "1-configure.json".
*/
struct CreateTTN1 : public CreateTTN {

    inline CreateTTN1(
    ) : CreateTTN( "1-configure.json" ) {
    }

    inline CreateTTN1(
        const listDeliveryPtr_t& ld
    ) : CreateTTN( ld, "1-configure.json" ) {
    }


    virtual inline std::string name() const {
        return "������� ���� ��� ������� ��� (��������, ������ ����������).";
    }


    virtual inline std::string hotkey() const {
        return "1";
    }


    virtual inline std::string confirmation() const {
        //@todo return "�������� ��� �� ����� \"����� �����\"? ������ ����� ����� �� �����."; - ���������� �������.
        std::ostringstream ss;
        ss << "����������� ���� � ����������� ��� ������� �� ����� \"����� �����\""
            << " (��������, ������ ����������)."
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
           << " ������ ��� ������� ��� (��������) �������� � ����� \"data/b.xml\"."
           << " �������� ��� � ���������� � ����� �� ����� \"����� �����\".";
        return ss.str();
    }

    
    virtual inline std::string failure() const {
        return "���� ��� ������� ��� (��������) �� ������.";
    }

};






/**
* �������� ����� � ������� ��� ������� ����������� � ���.
* ������������ ������������ �� "2-configure.json".
*/
struct CreateTTN2 : public CreateTTN {

    inline CreateTTN2(
    ) : CreateTTN( "2-configure.json" ) {
    }

    inline CreateTTN2(
        const listDeliveryPtr_t& ld
    ) : CreateTTN( ld, "2-configure.json" ) {
    }


    virtual inline std::string name() const {
        return "������� ���� ��� ������� ��� (�����������, ������ �����������).";
    }


    virtual inline std::string hotkey() const {
        return "2";
    }


    virtual inline std::string confirmation() const {
        //@todo return "�������� ��� �� ����� \"����� �����\"? ������ ����� ����� �� �����."; - ���������� �������.
        std::ostringstream ss;
        ss << "����������� ���� � ����������� ��� ������� �� ����� \"����� �����\""
            << " (�����������, ������ �����������)."
            << " ������ ����� ����� �� ����� \"1.csv\""
            << " � �������� � ���� \"b.xml\". �������� ��������� ����, ����� ������ �������������"
            << " ������ � ������ �������� \"����� �����\".";
        return ss.str();
    }


    virtual inline std::string success() const {
        std::ostringstream ss;
        ss << "�������� ������ - " << ld.size() << "."
           << " ������ ��� ������� ��� (�����������) �������� � ����� \"data/b.xml\"."
           << " �������� ��� � ���������� � ����� �� ����� \"����� �����\".";
        return ss.str();
    }

    
    virtual inline std::string failure() const {
        return "���� ��� ������� ��� (�����������) �� ������.";
    }

};






/**
* ������� ����� � ����������� � ������� � ����� � �������� ������
* �� "����� �����".
* ������������ ��������� ������������.
*/
struct FusionTTN : public DeliveryCommand {

    inline FusionTTN(
        const std::string& configure
    ) : DeliveryCommand( listDeliveryPtr_t(), configure ) {
    }


    virtual void run();


    friend Delivery;

};




/**
* ������� ����� � ����������� � ������� � ����� � �������� ������
* �� "����� �����".
* ������������ ������������ �� "1-configure.json".
*/
struct FusionTTN1 : public FusionTTN {

    // ������ ������ � ������������ ����� �� ������������
    inline FusionTTN1(
    ) : FusionTTN( "1-configure.json" ) {
    }


    virtual inline std::string name() const {
        return "������� ���� ��� ������� ����� ������ (��������, ������ ����������).";
    }


    virtual inline std::string hotkey() const {
        return "3";
    }


    virtual inline std::string confirmation() const {
        std::ostringstream ss;
        ss << "������������: ��������, ������ ����������."
            << "\n�� ��������������� �� ����� \"����� �����\" ����� \"b.xml\""
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

};




/**
* ������� ����� � ����������� � ������� � ����� � �������� ������
* �� "����� �����".
* ������������ ������������ �� "2-configure.json".
*/
struct FusionTTN2 : public FusionTTN {

    inline FusionTTN2(
    ) : FusionTTN( "2-configure.json" ) {
    }


    virtual inline std::string name() const {
        return "������� ���� ��� ������� ����� ������ (�����������, ������ �����������).";
    }


    virtual inline std::string hotkey() const {
        return "4";
    }


    virtual inline std::string confirmation() const {
        std::ostringstream ss;
        ss << "������������: �����������, ������ �����������."
            << "\n�� ��������������� �� ����� \"����� �����\" ����� \"b.xml\""
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