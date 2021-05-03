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
		p_broker->requestData({ ExternalObject_t::ntp_server, 999 }, "CurrentTime", true);
		//Message msg = p_broker->readData(EXT_TIME_STR_LEN);
		//msg.apply();
	}

	int query = 3;

	while (true)
	{
		if (query == 1 /*some task requested sth */) {
			p_broker->requestData({ExternalObject_t::raspberry_pi, 999}, "sth", true);
			p_broker->readData(255);
		}
		else if (query == 2 /*another task requested sth */) {
			p_broker->requestData({ ExternalObject_t::raspberry_pi, 999 }, "sth else", true);
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

struct JsonSerializer :Visitor {

	void visit(const Header& p) override {
		oss << "{\"" << p.text << "\":";
	}

	void visit(const ListItem& li) override {
		oss << "{\"" << li.text <<"\":\""<< li.text << "\"},";
	}

	void visit(const Footer& li) override {
		oss << "}\n";
	}

	void visit(const List& l) override {
		//oss << "{";
		for (const auto& item : l) {
			item.accept(*this);
		}
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
		Header p{ "Hi" };
		ListItem l1{ "red" };
		ListItem l2{ "blue" };
		Footer end{ "dummy" };

		List list{ l1, l2 };
		std::vector<Element*> document{ &p, &list, &end };
		JsonSerializer jsonizer;

		for (auto item : document)
		{
			item->accept(jsonizer);
		}
		std::cout << jsonizer.str() << std::endl;
	}

	{
		Header p{ "Hello" };
		ListItem l3{ "redd" };
		ListItem l4{ "blu" };
		Footer end{ "dummy" };

		List list{ l3, l4 };
		std::vector<Element*> document{ &p, &list, &end };
		JsonSerializer jsonizer;

		for (auto item : document)
		{
			item->accept(jsonizer);
		}
		std::cout << jsonizer.str() << std::endl;
	}


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
