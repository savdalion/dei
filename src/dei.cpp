#include "../include/StdAfx.h"
#include "../include/configure.h"
#include "../include/Delivery.h"
#include "../include/Command.h"


/**
* ������ �� CSV (��� ������� �������� ����).
* -> �������� ������ ����� �����.
* -> ��������� ������.
* -> ������� � CSV.
*/
int main( int argc, char** argv ) {

    using namespace dei;
    namespace dc = dei::command;
    namespace dcn = dei::command::novaposhta;
    

    setlocale( LC_ALL, "Russian" );
    // ��� ����������� '.' ������ ','
    setlocale( LC_NUMERIC, "C" );


    // ����
    dc::listCommandPtr_t menu;
    menu.push_back( dc::CommandPtr( new command::novaposhta::CreateTTN1() ) );
    menu.push_back( dc::CommandPtr( new command::novaposhta::CreateTTN2() ) );
    menu.push_back( dc::CommandPtr( new command::novaposhta::FusionTTN1()  ) );
    menu.push_back( dc::CommandPtr( new command::novaposhta::FusionTTN2()  ) );

    static size_t rerun = 0;
    char ch = 0;
    while (ch != 13) {

        std::cout << "��� ��� �������" << ( (rerun > 0) ? " ������?" : "?") << std::endl << std::endl;
        rerun++;
        typelib::printPtr( std::cout, menu, "", "", "", "\n" );
        std::cout << std::endl << std::endl << "> ";
        //ch = std::getchar();
        //std::cout << ch << std::endl;
        std::cin >> ch;

        // ��������� �������
        const std::string skey( 1, ch );
        auto itr = menu.begin();
        for ( ; itr != menu.end(); ++itr) {
            const dc::CommandPtr cmd = *itr;
            auto sk = cmd->hotkey();
            if (sk == skey) {
                try {
                    cmd->run();
                } catch ( const Exception& ex ) {
                    std::cout << "������" << std::endl;
                    std::cout << std::endl << "(!) " << ex.what() << std::endl;
                }
                break;
            }
        }

        // ��������� ������ ��� ����� '0' ��� ��������� 'exit', 'quit'
        if ( (ch == '0')
            || (ch == 'e') || (ch == 'q') || (ch == 'E') || (ch == 'Q')
            || (ch == '-') || (ch == '.') || (ch == ',')
        ) {
            break;
        }

        if (itr == menu.cend()) {
            std::cerr << std::endl << ch << "? ������� �� ��������." << std::endl;
        }

        // ������������ ������� ��������� ����

        std::cout << std::endl << std::endl << std::endl;

    } // while ...



    std::cout << std::endl << "������ ���������." << std::endl;
    std::cout << std::endl << "���� ���-�� �������� �� ��� ��� ������� ����� > \n������ �����������, +38 050 335-16-18, dmotion@yandex.ru" << std::endl;
    std::cout << "^" << std::endl << std::endl;
    //std::cin.ignore();
    std::system( "pause.exe" );

    return 0;

} // main()
