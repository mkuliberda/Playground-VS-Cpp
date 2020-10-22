#pragma once

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

struct gpio_s {
	uint32_t port;
	uint16_t pin;
};

