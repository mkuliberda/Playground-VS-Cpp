// MsgParser_WithDesignPattern.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <sstream>
#include "Messages.h"
#include "MsgBroker.h"
#include "MsgBrokerFactory.h"
#include "MsgEncoder.h"
#include "stm32f4xx_hal.h"
#include "utilities.h"


UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
#define EXT_TIME_FORMAT "{\"NTP999>SYS000\":{\"CTD\":\"200914Mon185950\"}}" //Current Time Date
#define EXT_TIME_STR_LEN (sizeof(EXT_TIME_FORMAT)-1)

//{\"MAN999>SEC000\":{\"CMD\":\"Irrigation start\"}}
//{\"MAN999>SEC002\":{\"CMD\":\"Irrigation stop\"}}

void WirelessTask()
{
	MsgBrokerPtr p_broker = MsgBrokerFactory::create(MsgBrokerType::hal_uart_dma, &huart2);

	{
		//p_broker->requestData({ ExternalObject_t::ntp_server, 999 }, "CurrentTime", true);
		//Message msg = p_broker->readData(EXT_TIME_STR_LEN);
		//msg.apply();
	}

	int query = 3;

	while (true)
	{
		if (query == 1 /*some task requested sth */) {
			//p_broker->requestData({ExternalObject_t::raspberry_pi, 999}, "sth", true);
			p_broker->readData(255);
		}
		else if (query == 2 /*another task requested sth */) {
			//p_broker->requestData({ ExternalObject_t::raspberry_pi, 999 }, "sth else", true);
			p_broker->readData(128);
		}
		//...
	}
}

void GsmTask()
{
	MsgBrokerPtr p_broker = MsgBrokerFactory::create(MsgBrokerType::hal_uart_dma, &huart3);

	while (true)
	{
		p_broker->readData(255);
		
		
	}
	
}

struct JsonPublisher :Encoder {

	void visit(const Header& p) override {
		oss << "$PUB{\"" << p.part1 <<">"<< p.part2 << "\":";
	}

	void visit(const DataItem& li) override {
		oss << "{\"" << li.part1 << "\":\"" << li.part2 << "\"},";
	}

	void visit(const Footer& li) override {
		oss << li.part1 << "}\n"  << li.part2;
	}

	void visit(const Data& l) override {
		for (const auto& item : l) {
			item.accept(*this);
		}
		oss.seekp(-1, std::ios_base::end);		
	}

	std::string str() const override {
		return oss.str();
	}

private:
	std::ostringstream oss;
};

struct JsonRequester :Encoder {

	void visit(const Header& p) override {
		oss << "$GET{\"" << p.part1 << ">" << p.part2 << "\":";
	}

	void visit(const DataItem& li) override {
		oss << "{\"" << li.part1 << "\":\"" << li.part2 << "\"},";
	}

	void visit(const Footer& li) override {
		oss << li.part1 << "}\n"  << li.part2;
	}

	void visit(const Data& l) override {
		for (const auto& item : l) {
			item.accept(*this);
		}
		oss.seekp(-1, std::ios_base::end);
	}

	std::string str() const override {
		return oss.str();
	}

private:
	std::ostringstream oss;
};

struct SmsSerializer :Encoder {

	void visit(const Header& p) override {
		oss << "[" << p.part1 <<" "<< p.part2 << ": ";
	}

	void visit(const DataItem& li) override {
		oss << li.part1 << "-" << li.part2 << ", ";
	}

	void visit(const Footer& li) override {
		oss << li.part1 << "]\n" << li.part2;
	}

	void visit(const Data& l) override {
		for (const auto& item : l) {
			item.accept(*this);
		}
		oss.seekp(-2, std::ios_base::end);
	}

	std::string str() const override {
		return oss.str();
	}

private:
	std::ostringstream oss;
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		Header p{ "Hi" , "Asia"};
		DataItem l1{ "red", "1" };
		DataItem l2{ "blue", "0" };
		Footer end{ "", "" };

		Data list{ l1, l2 };
		list.push_back(l1);
		std::vector<Element*> document{ &p, &list, &end };
		JsonPublisher json_pub_serializer;

		for (auto item : document)
		{
			item->accept(json_pub_serializer);
		}
		std::cout << json_pub_serializer.str();
	}

	{
		Header p{ "Hello" , "Mateusz"};
		DataItem l3{ "redd", "" };
		DataItem l4{ "blu", "" };
		Footer end{ "", "" };

		Data list{ l3, l4 };
		std::vector<Element*> document{ &p, &list, &end };
		SmsSerializer smsizer;

		for (auto item : document)
		{
			item->accept(smsizer);
		}
		std::cout << smsizer.str();
	}

	MsgBrokerPtr json_msg_broker = MsgBrokerFactory::create(MsgBrokerType::hal_uart_dma, &huart2);
	JsonPublisher json_pub_serializer;
	JsonRequester json_get_serializer;
	json_msg_broker->setEncoder(&json_pub_serializer);
	json_msg_broker->setInternalAddresses(&internal_entities);
	json_msg_broker->setExternalAddresses(&esp01s_external_addresses);
	json_msg_broker->publishData({ ExternalObject_t::raspberry_pi, 899 }, { InternalObject_t::plant, 16 },  { {"Soil moisture", 18}, {"Type", 2} }, false);

	json_msg_broker->publishData({ ExternalObject_t::raspberry_pi, 999 }, { InternalObject_t::sector, 16 }, { {"Soil moisture", 18}, {"Type", 2} }, false, &json_get_serializer);


	getchar();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
