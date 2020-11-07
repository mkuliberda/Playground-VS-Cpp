#pragma once

#include "main.h"
#include <array>
#include <bitset>
#include <vector>
#include <iostream>

struct PumpInfo_s {
	uint32_t	state = 0;
	uint8_t		id = 0;
	bool		forced = false;
	bool		cmd_consumed = false;
};

enum class pumpstate_t {
	init,
	running,
	reversing,
	stopped,
	waiting,
	fault,
	sleep
};

enum class pumptype_t {
	generic,
	binary,
	drv8833_dc,
	drv8833_bldc
};

enum class motortype_t {
	dc_motor,
	bldc_motor
};

enum class pumpcmd_t {
	start,
	stop,
	reverse
};


enum class pumpcontrollermode_t {
	init,
	automatic,
	manual,
	external,
	sleep
};

void encodePumpStatus(const struct PumpInfo_s& _pump, uint32_t& status);
void decodePumpStatus(std::array<struct PumpInfo_s, 4>& a_pump, const std::bitset<32>& _status);

class Pump {

protected:

	pumptype_t					type = pumptype_t::generic;
	pumpstate_t 				state = pumpstate_t::init;		///< current pump's working state based on enum pumpstate_t
	struct PumpInfo_s	 		status;
	double 						runtime_seconds;				///< current runtime, incrementing in running state [seconds]
	double 						idletime_seconds;				///< current idletime incrementing in stopped and waiting state [seconds]
	uint32_t 					runtime_limit_seconds;			///< runtime limit for particular pump [seconds]
	uint32_t 					idletime_required_seconds; 		///< idletime required between two consecutive runs [seconds]

	pumptype_t&					getType(void);
	virtual bool 				start() = 0;
	virtual bool 				stop() = 0;

public:

	Pump() :
		runtime_seconds(0.0),
		idletime_seconds(0.0),
		runtime_limit_seconds(0),
		idletime_required_seconds(0)
	{};

	virtual ~Pump() {};

	bool						init(void);
	void						run(const double & _dt);
	virtual void 				setState(const pumpstate_t & _st) = 0;
	pumpstate_t&				getState(void);
	bool						isRunning(void);
	struct PumpInfo_s&			getStatus(void);
	void						resetRuntime(void);
	void						increaseRuntime(const double & _dt);
	double&						getRuntimeSeconds(void);
	void						resetIdletime(void);
	void						increaseIdletime(const double & _dt);
	double&						getIdletimeSeconds(void);

};

class BinaryPump : public Pump {

private:

	struct gpio_s 				pinout;
	struct gpio_s 				led;

protected:

	bool						start(void) override;
	bool						stop(void) override;

public:

	BinaryPump()
	{
		type = pumptype_t::binary;
	};

	bool init(const uint8_t & _id, const uint32_t & _idletime_required_seconds, const uint32_t & _runtime_limit_seconds, const struct gpio_s & _pinout, const struct gpio_s & _led);
	void run(const double & _dt, const pumpcmd_t & _cmd, bool & cmd_consumed);
	void forcestart(void);
	void forcestop(void);
	void setState(const pumpstate_t & _state);

};

class DRV8833Pump : public Pump {

private:

	std::array<struct gpio_s, 4> 	aIN;							///< in1, in2, in3, in4
	struct gpio_s 					led;
	struct gpio_s 					fault;
	struct gpio_s 					mode;

protected:

	bool 							start(void) override;
	bool 							stop(void) override;
	bool							reverse(void);


public:

	DRV8833Pump(const motortype_t & _type)
	{
		if (_type == motortype_t::bldc_motor) type = pumptype_t::drv8833_bldc;
		if (_type == motortype_t::dc_motor) type = pumptype_t::drv8833_dc;
	};

	bool 						init(const uint8_t & _id, const uint32_t & _idletime_required_seconds, const uint32_t & _runtime_limit_seconds, \
		const std::array<struct gpio_s, 4> & _pinout, const struct gpio_s & _led_pinout, \
		const struct gpio_s & _fault_pinout, const struct gpio_s & _mode_pinout);
	bool 						init(const uint8_t & _id, const uint32_t & _idletime_required_seconds, const uint32_t & _runtime_limit_seconds, \
		const std::array<struct gpio_s, 2> & _pinout, const struct gpio_s & _led_pinout, \
		const struct gpio_s & _fault_pinout, const struct gpio_s & _mode_pinout);
	void 						run(const double & _dt, const pumpcmd_t & _cmd, bool & cmd_consumed, bool & fault);
	void 						setState(const pumpstate_t & _st) override;
	bool 						forcestart(void);
	bool 						forcereverse(void);
	bool 						forcestop(void);
	void 						setSleep(void);
	void 						setEnable(void);
	bool						isFault(void);

};


class PumpController {

private:

	const uint8_t 						pumps_limit = 1;
	uint8_t								pumps_count;
	pumpcontrollermode_t				mode;
	uint8_t								pump_encoded_status;
	uint32_t							pump_fault_occurence_cnt;

public:

	PumpController() :
		pumps_count(0),
		mode(pumpcontrollermode_t::init),
		pump_encoded_status(255),
		pump_fault_occurence_cnt(0)
	{};

	~PumpController()
	{
		if (pBinPump != nullptr) delete pBinPump;
		if (p8833Pump != nullptr) delete p8833Pump;
	};

	BinaryPump							*pBinPump = nullptr;
	DRV8833Pump							*p8833Pump = nullptr;

	bool								update(const double & _dt, bool & _activate_watering);
	bool								createPump(const pumptype_t & _pumptype);
	bool								setMode(const pumpcontrollermode_t & _mode);
	const pumpcontrollermode_t&			getMode(void);
	uint8_t&							getPumpStatusEncoded(void);


};

