#pragma once


namespace dei {

struct Delivery;
typedef std::shared_ptr< Delivery >  DeliveryPtr;
typedef std::unique_ptr< Delivery >  DeliveryUPtr;
typedef std::vector< DeliveryPtr >  listDeliveryPtr_t;

}




namespace std {

ostream& operator<<( std::ostream& out, const dei::Delivery& );
ostream& operator<<( std::ostream& out, const std::vector< dei::DeliveryPtr >& );

} // std





namespace dei {


/**
* ��������.
*
* @see http://orders.novaposhta.ua/api.php?todo=api_form
*/
struct Delivery {
    // ������ ���������� (����������� �� �����)
    // @see data/1.csv
    std::string date;
    std::string number;     //+
    std::string item;       //+
    std::string name;       //+
    std::string phone;      //+
    std::string postcode;
    std::string city;       //+
    std::string address;    //+
    std::string total;
    std::string totalClear; //+
    std::string note;       //+


    // ������������� ���������
    std::string declareCost;

    // ����� ��������� ��� �������� ����������
    std::string warehouse;

    // ��� ��������, ����� - "�������"
    std::string packType;

    // �������� (1) ��� ����������� (2) ������
    std::string payType;

    // ��� ������: ���������� (0), ����������� (1), ������ ���� (3)
    std::string payer;

    // �������������� ���������� (� ���. �������, "��������� �1� XXX"
    std::string additionalInfo;

    // �������� ��������
    // @see ���� 'ignoreItem'
    std::string description;

    // ��� �������� ��������
    // [1 - ��������� | 2 - ����� | 3 - ������ (����) | 4 - ����� | 5 - ����]
    std::string redeliveryType;

    // ����� ����������� �������� ��������
    std::string redeliveryPaymentCity;

    // ���������� �������� ��������
    // [1 - ��������� | 2 - ���������]
    std::string redeliveryPaymentPayer;

    // ��� ���� ����������� �� ���������
    // @todo ������� � ���� ������������.
    // @todo fine ��� ���� ���������� ����� Key.
    std::string senderCity;
    std::string senderCompany;
    std::string senderAddress;
    std::string senderContact;
    std::string senderPhone;

    // ������ �� ����������, ���� ������ ��������� ������
    std::string deliveryAmount;

    // �� ���� � �������� ��������
    std::string deliveryInOut;

    // ����� � ��������
    std::string weight;
    std::string length;
    std::string width;
    std::string height;

    // �������� ������� ����� ��������
    // @see ���� 'ignoreItem'
    std::string containerDescription;


    // ������ ��������������
    std::string oauth;


    // ���������� �� "����� �����" ����� ����������� ������
    std::string novaposhtaNumber;


    // ���� 'true', ������ �������� ����� ����� ��������� �������� �����
    // 'description' � 'cont_description' �� ���������.
    bool ignoreItem;

    // ��� ����� ����������� � ���� 'note' ��� ������ � ���� ����������� ����� '����'
    // ������: "�������� �� �����" ���������� ��� "�������� �� ����� �����������".
    std::string screamDoorNote;


    // � ���� ��� ������� � ������ ������ ����������� (1) ��� � (2) ����� ��������.
    //typedef std::pair< std::string, std::string >  typeCostDelivery_t;
    typedef std::vector< std::string >  typeCostDelivery_t;
    typeCostDelivery_t doorDoorDelivery;
    typeCostDelivery_t doorStockDelivery;
    typeCostDelivery_t stockDoorDelivery;
    typeCostDelivery_t stockStockDelivery;



    inline Delivery( const std::string& initFile ) {
        initDefault( this, initFile );
    }




    virtual inline ~Delivery() {
    }



    /**
    * @return ���� �� ����� ��������, �������� �� ������ �� ������.
    *         � ����� ������ �������� ������ ����. �����:
    *           - stock-door
    *           - stock-stock
    */
    std::string typeDelivery() const;



    /**
    * ��������� �������� �� ���������� ����� � 'r'.
    * ������ ����� - ��. data/1.csv
    *
    * @param configure ���� ������������, � ������� ����� ������� ��������.
    */
    static void load(
        listDeliveryPtr_t& r,
        const std::string& source,
        const std::string& configure
    );





protected:
    /**
    * ��������� �������� ���������� �� ���������.
    */
    static void initDefault(
        Delivery* d,
        const std::string&  initFile
    );



    /**
    * @return ����, ��������� �� ������� ��������.
    */
    static std::string correctField( const std::string& s );



    /**
    * @return ���� � ��������� ������, ��������� �� ������.
    */
    static std::string correctCity( const std::string& s );

};



} // dei
