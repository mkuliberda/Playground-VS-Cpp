#pragma once

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef char int8;

#define GPIO_PIN_RESET 0
#define GPIO_PIN_SET 1

struct gpio_s {
	uint32_t port;
	uint16_t pin;
};

typedef struct
{
	uint16_t                 *Instance;     /*!< Register base address             */
	uint16_t         Init;          /*!< TIM Time Base required parameters */
	uint16_t        Channel;       /*!< Active channel                    */
	uint16_t            *hdma[7];      /*!< DMA Handlers array
													This array is accessed by a @ref DMA_Handle_index */
	uint16_t              Lock;          /*!< Locking object                    */
	uint16_t    State;         /*!< TIM operation state               */
} TIM_HandleTypeDef;

inline bool HAL_GPIO_ReadPin(uint32_t port, uint16_t pin) {
	return true;
}

inline void HAL_GPIO_WritePin(uint32_t port, uint16_t pin, uint8_t val) {

}

