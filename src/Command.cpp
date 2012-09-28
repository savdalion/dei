#include "../include/StdAfx.h"
#include "../include/Command.h"
#include "../external/plustache/include/template.hpp"
#include "../external/plustache/include/plustache_types.hpp"
#include "../external/plustache/include/context.hpp"
#include "../external/markup/Markup.h"
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/math/special_functions/round.hpp>
#include <curl/curl.h>


void dei::command::novaposhta::CreateTTN::run() {

    std::cout << std::endl << confirmation() << std::endl;
    /* @todo? �������������
    std::cout << "�������������? ";
    std::string answer;
    std::cin >> answer;
    boost::to_lower( answer, std::locale( "Russian" ) );
    if (
        (answer != "y") && (answer != "yes")
     && (answer != "�") && (answer != "��")
     && (answer != "s") && (answer != "si")
     && (answer != "+")
     && (answer != "ok")
     && (answer != "�������")
    ) {
        std::cout << "������." << std::endl;
        return;
    }
    */


    std::cout << std::endl;
    std::cout << "��������:" << std::endl;



    // I
    std::cout << std::endl;
    const std::string source = "data/1.csv";
    std::cout << "  > ��������� ������ �� " << source << " ... ";
    ld.clear();
    Delivery::load( ld, source, configure );
    std::cout << "�������" << std::endl;



    // II
    std::cout << std::endl;
    std::cout << "  > �������������� ��� ��� �������� \"����� ������\" ... ";
    std::ostringstream ss;
    ss << *this;
    // �������� ������ �� ��������� �����
    const std::string destination = "last-query-novaposhta.query";
    std::ofstream out( destination.c_str() );

    /* - ����������. ���������� ��������� WINDOWS-1251 � ������� "create-ttn.tpl".
    // ����������� � UTF-8, ����� ��������� ���������� ������ � XML
    // ��������� ����������
    // @source http://mariusbancila.ro/blog/2008/10/20/writing-utf-8-files-in-c
    const std::string utf8ss = typelib::convert::codepage::correctUTF8Windows( ss.str() );
    out << utf8ss;
    */
    out << ss.str();

    if ( !out.is_open() ) {
        const std::string s = "���� \"" + destination + "\" �� ������� ������� ��� ������."
            + " ��������� ������� ���������� ����� �� ����� � ����� ������� � �����.";
        throw Exception( s );
    }
    out.close();
    std::cout << "�������" << std::endl;



    // III
    std::cout << std::endl;
    std::cout << "  > ��������� ������ ��� � ���� \"data/b.xml\" ... ";

    const std::string result = "data/b.xml";
    std::ofstream outResult( result.c_str() );
    outResult << ss.str();
    if ( !outResult.is_open() ) {
        const std::string s = "���� \"" + result + "\" �� ������� ������� ��� ������."
            + " ��������� ������� ���������� ����� �� ����� � ����� ������� � �����.";
        throw Exception( s );
    }
    outResult.close();

    std::cout << "�������" << std::endl;


    // ��� ����� �� ���� - �� ��
    std::cout << std::endl;
    std::cout << success() << std::endl;



#if 0
// - ����� ����� �� ��������� ��������� �� curl. �������� ����� ������ �������
//   �� �� �����: �������� ����� � ���� ���� �������� ����������� �������
//   �� 1.csv � ������ ����� ����� � �������� ������.

#if 1
    // III
    std::cout << std::endl;
    std::cout << "���������� ��� \"����� �����\" ... ";

    static const std::string method = "POST";
    static const std::string url = "http://orders.novaposhta.ua/xml.php";

    std::string buffer;
    curl_global_init( CURL_GLOBAL_DEFAULT );
    CURL* curl = curl_easy_init();
    if ( !curl )
        throw Exception( "Unable to create CURL object." );
    if (curl_easy_setopt( curl, CURLOPT_NOPROGRESS, 1L ) != CURLE_OK)
        throw Exception( "Unable to set NOPROGRESS option." );
    if (curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, writer ) != CURLE_OK)
        throw Exception( "Unable to set writer function." );
    if (curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buffer ) != CURLE_OK)
        throw Exception( "Unable to set write buffer." );
    if (curl_easy_setopt( curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1 ) != CURLE_OK)
        throw Exception( "Unable to set http-version" );
    if (curl_easy_setopt( curl, CURLOPT_NOSIGNAL, 1 ) != CURLE_OK)
        throw Exception( "Unable to set NOSIGNAL option." );
    if (curl_easy_setopt( curl, CURLOPT_FAILONERROR, 0 ) != CURLE_OK)
        throw Exception( "Unable to set FAILONERROR option." );
    if (curl_easy_setopt( curl, CURLOPT_TIMEOUT, 30 ) != CURLE_OK)
        throw Exception( "Unable to set TIMEOUT option." );

    // @test
    curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L );
    curl_easy_setopt( curl, CURLOPT_HEADER, 1L );

    const bool presentData = !ss.str().empty();
    //const std::string preparedData =
    //    curl_easy_escape( curl, ss.str().c_str(), ss.str().length() );
    const std::string preparedData = ss.str();

    /* @test
    const std::string preparedData = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\
<file>\
<auth>363e5b2b2fb02543a9cedc6e4f1470bc</auth>\
<city/>\
</file>";
*/

    const long sizePreparedData = static_cast< long >( preparedData.size() );

    struct curl_slist* chunk = nullptr;
    typedef std::map< std::string, std::string >  HeaderMap;
    HeaderMap headers;
    if ( presentData ) {
        chunk = curl_slist_append( chunk, "Content-Type: text/xml" );
        chunk = curl_slist_append( chunk, "charset: utf-8" );
    }

    if (curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk) != CURLE_OK)
        throw Exception( "Unable to set custom header" );

    if (curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str()) != CURLE_OK)
        throw Exception( "Unable to set HTTP method: " + method );
    if (curl_easy_setopt(curl, CURLOPT_URL, url.c_str()) != CURLE_OK)
        throw Exception( "Unable to set URL: " + url );

    if ( presentData ) {
#ifdef _DEBUG
        std::cout << "Sending data: " << preparedData << std::endl;
#endif

        if (curl_easy_setopt(curl, CURLOPT_READFUNCTION, reader) != CURLE_OK)
            throw Exception( "Unable to set read function" );
        if (curl_easy_setopt(curl, CURLOPT_READDATA, &preparedData) != CURLE_OK)
            throw Exception( "Unable to set data: " + preparedData);
        if (curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L) != CURLE_OK)
            throw Exception( "Unable to set upload request" );
        if (curl_easy_setopt(curl, CURLOPT_INFILESIZE, sizePreparedData) != CURLE_OK)
            throw Exception( "Unable to set content size" );
    }

    const auto errorPerform = curl_easy_perform( curl );
    if (errorPerform != CURLE_OK) {
        CURLINFO info = CURLINFO_NONE;
        const CURLcode codeError = curl_easy_getinfo( curl, info );
        const auto strError = curl_easy_strerror( codeError );
        std::cerr << strError << std::endl;
        throw Exception( "����� \"" + url + "\" ����������." );
    }


    if ( presentData || !headers.empty() ) {
        if (curl_easy_setopt( curl, CURLOPT_UPLOAD, 0L ) != CURLE_OK)
            throw Exception( "Unable to reset upload request" );
        if (curl_easy_setopt( curl, CURLOPT_HTTPHEADER, NULL ) != CURLE_OK)
            throw Exception( "Unable to reset custom headers" );
    }

#ifdef _DEBUG
    long responseCode;
    if (curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &responseCode ) != CURLE_OK)
        throw Exception( "Unable to get response code" );
    std::cout << "Response code: " << responseCode << std::endl;
    std::cout << "Raw buffer: " << buffer;
    //const std::string decodeBuffer = typelib::convert::codepage::cp1251utf8( buffer );
    //std::cout << "Raw buffer: " << decodeBuffer;
#endif

    /* - ������ ����� �������� ������ ����. �� ��������� �����.
    std::cout << "�������" << std::endl;
    */

#endif



    // III
    std::cout << std::endl;
    std::cout << "������� ����� ... ";

    /* @test
    const std::string answer = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<file>\
<order id=\"220610\" np_id=\"���-08978787878787\" />\
<order id=\"220765\" np_id=\"00033333331232\" />\
</file>";
*/
    const std::string& answer = buffer;

    // ��� ���� ���������������� ���������� XML � ��������� ������ �
    // ������������ ���������

    // @source http://firstobject.com/fast-start-to-xml-in-c++.htm
    CMarkup xml( answer );
    xml.FindElem();
    xml.IntoElem();
    bool hasOrder = false;
    while ( xml.FindElem( "order" ) ) {
        hasOrder = true;
        const std::string ourNumber = xml.GetAttrib( "id" );
        const std::string npNumber =  xml.GetAttrib( "np_id" );
        // ���� ����� ����� ������������ ������,
        // ��������� ������ ����������� � ���������� �� ����� ����� ������
        for (auto itr = ld.begin(); itr != ld.end(); ++itr) {
            dei::DeliveryPtr d = *itr;
            if (d->number == ourNumber) {
                d->novaposhtaNumber = npNumber;
            }
        }

    } // while ( xml.FindElem( "order" ) )

    if ( !hasOrder ) {
        throw Exception( "���� ��� ��������/��������� ������. ������ �������: \"" + answer + "\"" );
    }

    std::cout << "�������" << std::endl;



    // IV
    // @todo fine ������ ����������! ��������� ������ ���-����� � ��������� �����.
    std::cout << std::endl;
    std::cout << "��������� ���� \"data/1.csv\" ������� ������ � ���������������� ... ";

    // @see ���������� Delivery::load()
    std::ifstream in( source.c_str() );
    if ( !in.is_open() ) {
        throw Exception( "���� \"" + source + "\" �� ������." );
    }

    typedef boost::tokenizer< boost::escaped_list_separator< char > >  Tokenizer;

    const boost::escaped_list_separator< char > sep( '\\', ';', '\"' );

    std::string allLine;
    std::string tempBuffer;

    // ����� ���. ������
    size_t i = 0;
    while ( std::getline( in, tempBuffer ) ) {
        ++i;
        // ������ ������ - �������� "." (������): �������� ����� ��������
        if (i == 1) {
            tempBuffer += ";.\n";
        }

        // ��������� � ������ ������ ����� ����������� � ���������������� ������
        for (auto itr = ld.cbegin(); itr != ld.cend(); ++itr) {
            const dei::Delivery d = **itr;
            // @todo fine (!) ��������, ��� ������� � ������� � ����� - �� ������ � �� ���������.
            //       �� ������. �� ������.
            std::ostringstream variant1;
            variant1 << ";" << d.number << ";";
            std::ostringstream variant2;
            variant2 << ";\"" << d.number << "\";";
            if ( boost::contains( tempBuffer, variant1.str() ) || boost::contains( tempBuffer, variant2.str() ) ) {
                std::ostringstream ss;
                ss << tempBuffer << ";" << d.novaposhtaNumber << std::endl;
                tempBuffer = ss.str();
                // ����� ����� ����������� ������ 1 ���
                break;
            }
        }

        allLine.append( tempBuffer );

    } // while ( std::getline( in, tempBuffer ) )

    in.close();

    std::cout << "�������" << std::endl;



    // V
    std::cout << std::endl;
    std::cout << "��������� ��������� � ���� \"data/2.csv\" ... ";

    const std::string result2 = "data/2.csv";
    std::ofstream outResult2( result2.c_str() );
    outResult2 << allLine;
    if ( !outResult2.is_open() ) {
        const std::string s = "���� \"" + result2 + "\" �� ������� ������� ��� ������."
            + " ��������� ������� ���������� ����� �� ����� � ����� ������� � �����.";
        throw Exception( s );
    }
    outResult2.close();

    std::cout << "�������" << std::endl;



    // VI
    std::cout << std::endl;
    std::cout << "���������, ��� ��� ��� ��������� �� \"����� �����\": " << std::endl;

    bool ok = true;
    for (auto itr = ld.begin(); itr != ld.end(); ++itr) {
        dei::DeliveryPtr d = *itr;
        std::cout << "\t������ " << d->number << " ��������� ��� ������� \""
            << d->novaposhtaNumber << "\"" << std::endl;
        if ( d->novaposhtaNumber.empty() ) {
            ok = false;
        }
    }

    //std::cout << ( ok ? "��� ������ ���������." : "(!) ���� ������������� ������." ) << std::endl;
    if ( !ok ) {
        throw Exception( "���� ������������� ������." );
    }

    std::cout << "��� ������ ���������." << std::endl;

#endif

}







void dei::command::novaposhta::FusionTTN::run() {

    std::cout << std::endl << confirmation() << std::endl;


    std::cout << std::endl;
    std::cout << "��������:" << std::endl;


    // I
    std::cout << std::endl;
    const std::string source = "data/1.csv";
    std::cout << "  > ��������� ������ �� " << source << " ... ";
    ld.clear();
    Delivery::load( ld, source, configure );
    std::cout << "�������" << std::endl;



    // II
    // ������������ ������ �� ���������� ������ *�������* ����������
    // �� ����� �� ����� ����� � �������� ������
    // @todo ���������� / �������� CURL'��. ������ ����� ����� ��� �� ���������.
    std::cout << std::endl;
    const std::string sourceAnswer = "data/b.xml";
    std::cout << "  > ������� ����� \"����� �����\" (������ ���� ���������� � ���� \"" << sourceAnswer << "\" �� �������� ���������� ��������) ... ";

    std::ifstream inAnswer( sourceAnswer.c_str() );
    if ( !inAnswer.is_open() ) {
        throw dei::Exception( "���� \"" + sourceAnswer + "\" �� ������." );
    }
    std::string tempStr;
    std::string answer = "";
    while ( getline( inAnswer, tempStr ) ) {
        answer += tempStr + "\n";
    }
    inAnswer.close();

    // �������������� ���������� XML � �������� ������ �
    // ������������ ���������

    // @source http://firstobject.com/fast-start-to-xml-in-c++.htm
    CMarkup xml( answer );
    xml.FindElem();
    xml.IntoElem();
    bool hasAnswerOrder = true;
    while ( xml.FindElem( "order" ) ) {
        const std::string ourNumber = xml.GetAttrib( "id" );
        const std::string npNumber =  xml.GetAttrib( "np_id" );
        if ( npNumber.empty() ) {
            // ���������� ����� �� �������� ������� �� ����� �����
            // (����� ������ ���� ���������� � ����� ����� �����)
            hasAnswerOrder = false;
            break;
        }
        // ���� ����� ����� ������������ ������,
        // ��������� ������ ����������� � ���������� �� ����� ����� ������
        for (auto itr = ld.begin(); itr != ld.end(); ++itr) {
            dei::DeliveryPtr d = *itr;
            if (d->number == ourNumber) {
                d->novaposhtaNumber = npNumber;
            }
        }

    } // while ( xml.FindElem( "order" ) )

    if ( !hasAnswerOrder ) {
        std::ostringstream ss;
        ss << "���� \"b.xml\" �� �������� ���������� �� ������������ ���������."
           << " ������ ������ ���� ����������� �� ������ \"����� �����\" �� �������� ���������� ������.";
        throw Exception( ss.str() );
    }

    std::cout << "�������" << std::endl;



    // II
    // @todo fine ������ ����������! ��������� ������ ���-����� � ��������� �����.
    std::cout << std::endl;
    const std::string source1CSV = "data/1.csv";
    std::cout << "  > ��������� ���� \"" << source1CSV << "\" �������� ������ �� ����������������."
        << " ��������� ��������� � \"2.csv\" ... ";

    // @see ���������� Delivery::load()
    std::ifstream in( source1CSV.c_str() );
    if ( !in.is_open() ) {
        throw Exception( "���� \"" + source1CSV + "\" �� ������." );
    }

    typedef boost::tokenizer< boost::escaped_list_separator< char > >  Tokenizer;

    const boost::escaped_list_separator< char > sep( '\\', ';', '\"' );

    std::string allLine;
    std::string tempBuffer;

    // ����� ���. ������
    size_t i = 0;
    while ( std::getline( in, tempBuffer ) ) {
        ++i;
        // ������ ������ - �������� ����� �������� - ��������� ����������� (��. ������� ����)
        if (i == 1) {
            tempBuffer += ";...\n";
        }

        // ��������� � ������ ������:
        //   - ����� ����������� � ���������������� ������
        //   - ��� ��������
        //   - ����� ��������
        for (auto itr = ld.cbegin(); itr != ld.cend(); ++itr) {
            const dei::Delivery d = **itr;
            // @todo fine (!) ��������, ��� ������� � ������� ������ - �� ������ � �� ���������.
            //       �� ������. �� ������.
            std::ostringstream variant1;
            variant1 << ";" << d.number << ";";
            std::ostringstream variant2;
            variant2 << ";\"" << d.number << "\";";
            if ( boost::contains( tempBuffer, variant1.str() ) || boost::contains( tempBuffer, variant2.str() ) ) {
                std::ostringstream ss;

                ss << tempBuffer << ";" << d.novaposhtaNumber;
                
                std::string nameTD = "";
                std::string costTD = "";
                const std::string td = d.typeDelivery();
                if (td == "stock-door") {
                    nameTD = d.stockDoorDelivery[0];
                    costTD = d.stockDoorDelivery[1];
                } else if (td == "stock-stock") {
                    nameTD = d.stockStockDelivery[0];
                    costTD = d.stockStockDelivery[1];
                } else {
                    throw Exception( "��� �������� ��� *������ ������* ����� ���� ������ \"�����-�����\" ��� \"�����-�����\"." );
                }

                // ����� ������� � ����� ���������� ������ ����� ��������
                // ���� / ��������� ������� � ����� "*-configure.json"
                if ( !nameTD.empty() ) {
                    ss << ";" << nameTD;
                }
                if ( !costTD.empty() ) {
                    ss << ";" << costTD;
                }

                ss << std::endl;

                tempBuffer = ss.str();
                // ����� ����� ����������� ������ 1 ���
                break;
            }
        }

        allLine.append( tempBuffer );

    } // while ( std::getline( in, tempBuffer ) )

    in.close();


    // ��������� ��������� � ���� "data/2.csv"
    const std::string result = "data/2.csv";
    std::ofstream outResult( result.c_str() );
    outResult << allLine;
    if ( !outResult.is_open() ) {
        const std::string s = "���� \"" + result + "\" �� ������� ������� ��� ������."
            + " ��������� ������� ���������� ����� �� ����� � ����� ������� � �����.";
        throw Exception( s );
    }
    outResult.close();

    std::cout << "�������" << std::endl;



    // III
    std::cout << std::endl;
    std::cout << "  > ���������, ��� ��� ��� ��������� �� \"����� �����\": " << std::endl;

    bool ok = true;
    for (auto itr = ld.cbegin(); itr != ld.cend(); ++itr) {
        dei::DeliveryPtr d = *itr;
        std::cout << "\t������ " << d->number << " ��������� ��� ������� \""
            << d->novaposhtaNumber << "\"" << std::endl;
        if ( d->novaposhtaNumber.empty() ) {
            ok = false;
        }
    }


    std::cout << std::endl;
    //std::cout << ( ok ? "��� ������ ���������." : "(!) ���� ������������� ������." ) << std::endl;
    if ( !ok ) {
        throw Exception( failure() );
    }
    std::cout << success() << std::endl;
}











std::ostream& std::operator<<( std::ostream& out, const dei::command::novaposhta::CreateTTN& c ) {

    const std::string file = "t-query/novaposhta/create-ttn.tpl";
    std::ifstream in( file.c_str() );
    if ( !in.is_open() ) {
        throw dei::Exception( "���� \"" + file + "\" �� ������." );
    }
    std::string tempStr;
    std::string str = "";
    while ( getline( in, tempStr ) ) {
        str += tempStr + "\n";
    }
    in.close();

    
    // �������������� ��������
    Context ctx;
    PlustacheTypes::CollectionType l;
    PlustacheTypes::ObjectType order;

    for (auto itr = c.ld.cbegin(); itr != c.ld.cend(); ++itr) {
        const dei::Delivery d = **itr;

        // @todo fine ������� � ��������� �����.
        if (itr == c.ld.cbegin()) {
            // ��� �������� ������� ���������� ��������������, ������ ����������
            ctx.add( "oauth", d.oauth );
        }

        order[ "order_id" ] = d.number;

        order[ "date" ] = "";
        order[ "date_desired" ] = "";

        order[ "sender_city" ] = d.senderCity;
        order[ "sender_company" ] = d.senderCompany;

        order[ "sender_address" ] = d.senderAddress;

        order[ "sender_contact" ] = d.senderContact;
        order[ "sender_phone" ] = d.senderPhone;

        order[ "rcpt_city_name" ] = d.city;
        order[ "rcpt_name" ] = d.name;

        order[ "rcpt_warehouse" ] = d.warehouse;
        // ���� � ���������� ����������� "�����"...
        const std::string lowerNote = boost::to_lower_copy( d.note, std::locale( "" ) );
        const bool hasDoor = boost::contains( lowerNote, "����" );
        if ( !hasDoor ) {
            // ������� �������� ����� �� �����������
            const bool hasStock = boost::contains( lowerNote, "�����" );
            if ( hasStock ) {
                // ����� �� ��������� � ������ ����������
                size_t store = 1;
                std::vector< std::string >  nv;
                boost::split( nv, d.note, boost::is_any_of( " -�\t" ), boost::algorithm::token_compress_on );
                for (auto ntr = nv.cbegin(); ntr != nv.cend(); ++ntr) {
                    if (store == 1) {
                        try {
                            store = boost::lexical_cast< size_t >( *ntr );
                            // ������������� ����� � ���� ����� ������
                            // break;  - ����������� ������� ������.
                        } catch ( ... ) {
                            // �� �����, ��������� N 1
                            store = 1;
                        }
                    }
                }
                order[ "rcpt_warehouse" ] = boost::lexical_cast< std::string >( store );
            }
        } // if ( !hasDoor )

        //order[ "rcpt_street_name" ] = d.address + " [" + d.note + "]";
        order[ "rcpt_street_name" ] = "";
        if ( hasDoor ) {
            // �������� �� �����, ��������� �����
            order[ "rcpt_street_name" ] = d.address;
            if ( !d.screamDoorNote.empty() ) {
                order[ "rcpt_street_name" ] += " " + d.screamDoorNote;
            }
        }

        order[ "rcpt_contact" ] = d.name;
        order[ "rcpt_phone_num" ] = d.phone;

        order[ "pay_type" ] = d.payType;
        order[ "payer" ] = d.payer;

        // ����. ��������� � ����������� �� ����� ������
        double defaultDeclareCost;
        try {
            defaultDeclareCost = boost::lexical_cast< double >( d.declareCost );
        } catch ( ... ) {
            defaultDeclareCost = 0.0;
        }

        double totalClear;
        try {
            totalClear = boost::lexical_cast< double >( d.totalClear );
        } catch ( ... ) {
            totalClear = 0.0;
        }

        order[ "cost" ] = boost::lexical_cast< std::string >( boost::math::round(
            (totalClear > defaultDeclareCost) ? totalClear : defaultDeclareCost
        ) );


        const bool redelivery =
            ( d.deliveryInOut.empty() && !d.redeliveryType.empty() );

        order[ "payer_city" ] = redelivery ? d.city : "";
        order[ "payer_company" ] = redelivery ? d.name : "";

        // � ���. ���. �� ����������� ��������� ��� ����������
        if ( !d.additionalInfo.empty() ) {
            order[ "additional_info" ] =
                d.additionalInfo + " " + d.city + " " + d.name;
        }

        order[ "documents" ] = "";
        order[ "pack_type" ] = d.packType;

        order[ "description" ] = d.item;
        if ( d.ignoreItem ) {
            if ( !d.description.empty() ) {
                order[ "description" ] = d.description;
            }
        }

        order[ "floor_count" ] = "0";
        order[ "saturday" ] = "0";

        // @todo ���� ����. �� ��� �� �������� � ����� �����. ������� ����?
        std::ostringstream ss;
        ss << (d.deliveryAmount.empty() ? "" : d.totalClear);
        order[ "delivery_amount" ] = ss.str();

        order[ "redelivery_type" ] = d.redeliveryType;

        order[ "delivery_in_out" ] = d.deliveryInOut;
        if ( redelivery ) {
            std::ostringstream ss;
            // # ����� ����� ������ �����
            //ss << "��������� �1� " << d.totalClear;
            //ss << "Dokum C1P " << d.totalClear;
            /* - ��� ���� "2" ��������� ������ ����� �����. ��. ����.
            ss << "���.�� " << d.totalClear;
            */
            ss << ( (d.redeliveryType == "2") ? "" : "���.�� " ) << d.totalClear;
            order[ "delivery_in_out" ] = ss.str();
        }

        if ( redelivery ) {
            order[ "redelivery_payment_city" ] =
                d.redeliveryPaymentCity.empty() ? d.city : d.redeliveryPaymentCity;
        } else {
            order[ "redelivery_payment_city" ] = "";
        }
        order[ "redelivery_payment_payer" ] = d.redeliveryPaymentPayer;

        order[ "weight" ] = d.weight;
        order[ "length" ] = d.length;
        order[ "width " ] = d.width;
        order[ "height " ] = d.height;

        order[ "cont_description" ] = d.item;
        if ( d.ignoreItem ) {
            if ( !d.containerDescription.empty() ) {
                order[ "cont_description" ] = d.containerDescription;
            }
        }

        l.push_back( order );
    }
    ctx.add( "order", l );


    // ����������
    template_t t;
    const std::string s = t.render( str, ctx );

    out << s;


    return out;
}
