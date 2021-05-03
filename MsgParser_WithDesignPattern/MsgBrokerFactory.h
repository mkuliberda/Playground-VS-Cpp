#ifndef __MsgBrokerFactory_H
#define __MsgBrokerFactory_H

#pragma once
#include "MsgBroker.h"
#include <string_view>

enum class MsgBrokerType {
	hal_uart_dma
};

class MsgBrokerFactory
{
public:
	static MsgBrokerPtr create(const MsgBrokerType& _type, void* _dev_handle=nullptr);
};

#endif