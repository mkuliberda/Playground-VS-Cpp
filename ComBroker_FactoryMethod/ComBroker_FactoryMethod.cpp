// ComBroker_FactoryMethod.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "HAL_UART_MsgBroker.h"
#include "MsgBrokerFactory.h"

int main()
{
    std::cout << "Hello World!\n";
	MsgBrokerPtr p_broker;
	p_broker = MsgBrokerFactory::create(msg_broker_type_t::hal_uart);

	std::map<std::string, double> items;
	uint8_t test_val = 7;

	items["item1"] = 2.4;
	items["item2"] = 2.7;

	for (const auto&[key, value] : items) {
		std::cout << key << ":" << value << std::endl;
	}

	p_broker->publishData(recipient_t::google_home, "Pelargonia", { { "Soil moisture", 67.0 }, { "is exposed", 0.0 } });
	p_broker->sendMsg(recipient_t::google_home, "Pelargonia");
	p_broker->requestData(recipient_t::ntp_server, "CurrentTime");
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
