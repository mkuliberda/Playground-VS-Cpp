#pragma once

#include "main.h"
#include <array>
#include <bitset>
#include <vector>
#include <iostream>
#include <chrono> //TODO: use UDLs for time counting and duration?


struct PumpInfo_s {
	uint32_t	state = 0;
	uint8_t		id = 0;
	bool		forced = false;
	bool		cmd_consumed = false;
};

enum class pump_state_t {
	init,
	running,
	reversing,
	stopped,
	waiting,
	fault,
	sleep
};

enum class pump_type_t {
	generic,
	binary,
	drv8833_dc,
	drv8833_bldc
};

enum class motortype_t {
	dc_motor,
	bldc_motor
};

enum class pump_cmd_t {
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
		//if (pBinPump != nullptr) delete pBinPump;
		//if (p8833Pump != nullptr) delete p8833Pump;
	};

	//BinaryPump							*pBinPump = nullptr; //TODO: implement Pumps as bridge/decorator design pattern?
	//DRV8833Pump							*p8833Pump = nullptr; //TODO: implement Pumps as bridge/decorator design pattern?

	bool								update(const double & _dt, bool & _activate_watering);
	bool								createPump(const pump_type_t & _pumptype);
	bool								setMode(const pumpcontrollermode_t & _mode);
	const pumpcontrollermode_t&			getMode(void);
	uint8_t&							getPumpStatusEncoded(void);


};

//Pumps based on Bridge Pattern (WIP)

class PumpImp {
public:
	PumpImp(const uint8_t& _id, const pump_type_t& _type) {
		type = _type;
		status.id = _id;
		std::cout << "PumpImp standard constructor " << std::endl;//TODO: delete on STM32
	}
	PumpImp(const uint8_t&& _id, const pump_type_t&& _type) {
		type = std::move(_type);
		status.id = std::move(_id);
		std::cout << "PumpImp move constructor " << std::endl;//TODO: delete on STM32
	}
	~PumpImp() {
		std::cout << "PumpImp dtor " << std::endl;//TODO: delete on STM32
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	PumpImp(PumpImp const&) = delete;
	PumpImp& operator=(PumpImp const&) = delete;

	virtual pump_type_t					getType() const;
	virtual bool						isValid() const;
	virtual bool 						start();
	virtual bool 						stop();
	virtual bool						init(const uint32_t& _idletime_required_seconds, const uint32_t& _runtime_limit_seconds);
	virtual void						run(const double& _dt, bool& cmd_consumed, const pump_cmd_t& _cmd = pump_cmd_t::stop);
	virtual void 						setState(const pump_state_t& _state);
	virtual pump_state_t				getState(void) const;
	virtual bool						isRunning(void) const;
	virtual struct PumpInfo_s			getStatus(void) const;
	virtual void						resetRuntime(void);
	virtual void						increaseRuntime(const double& _dt);
	virtual double						getRuntimeSeconds(void) const;
	virtual void						resetIdletime(void);
	virtual void						increaseIdletime(const double& _dt);
	virtual double						getIdletimeSeconds(void) const;

protected:
	pump_type_t							type = pump_type_t::generic;
	pump_state_t 						state = pump_state_t::init;		///< current pump's working state based on enum pump_state_t
	struct PumpInfo_s	 				status;
	double 								runtime_seconds;				///< current runtime, incrementing in running state [seconds]
	double 								idletime_seconds;				///< current idletime incrementing in stopped and waiting state [seconds]
	uint32_t 							runtime_limit_seconds;			///< runtime limit for particular pump [seconds]
	uint32_t 							idletime_required_seconds; 		///< idletime required between two consecutive runs [seconds]

};

/*class BinaryDcPumpImp : public PumpImp {
public:
	BinaryDcPumpImp(const struct gpio_s& _pinout, const pump_type_t& _type = pump_type_t::binary) : PumpImp(_type) {
		type = _type;
		pinout = _pinout;
		std::cout << "BinaryDcPumpImp standard constructor " << std::endl;//TODO: delete on STM32
	}

	BinaryDcPumpImp(const struct gpio_s&& _pinout, const pump_type_t&& _type = pump_type_t::binary) : PumpImp(_type) {
		type = std::move(_type);
		pinout = std::move(_pinout);
		std::cout << "BinaryDcPumpImp move constructor " << std::endl;//TODO: delete on STM32
	}
	~BinaryDcPumpImp() {
		std::cout << "BinaryDcPumpImp dtor " << std::endl;//TODO: delete on STM32
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	BinaryDcPumpImp(BinaryDcPumpImp const&) = delete;
	BinaryDcPumpImp& operator=(BinaryDcPumpImp const&) = delete;

	// virtual
	float									read(const double& _dt = 0);
	void									init(const float& _mount_height_meters, const struct gpio_s& _pinout);
	//const float&							getMountHeightMeters(void);
	bool									isSubmersed(void);

protected:
	struct gpio_s 							pinout;
	struct gpio_s 							led;
	//struct gpio_s 						pinout = { 0, 0 };

};*/


class DRV8833DcPumpImp : public PumpImp {
public:
	DRV8833DcPumpImp(const uint8_t& _id, const uint32_t & _idletime_required_seconds, const uint32_t & _runtime_limit_seconds, \
		const std::array<struct gpio_s, 2>& _pinout, const struct gpio_s& _led_pinout, \
		const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout, \
		const pump_type_t& _type = pump_type_t::drv8833_dc) : PumpImp(_id, _type) {

		status.id = _id;
		type = _type;
		init(_idletime_required_seconds, _runtime_limit_seconds, _pinout, _led_pinout, _fault_pinout, _mode_pinout);
		std::cout << "DRV8833DcPumpImp standard constructor " << std::endl;//TODO: delete on STM32
	}
	DRV8833DcPumpImp(const uint8_t&& _id, const uint32_t&& _idletime_required_seconds, const uint32_t&& _runtime_limit_seconds, \
		const std::array<struct gpio_s, 2>&& _pinout, const struct gpio_s&& _led_pinout, \
		const struct gpio_s&& _fault_pinout, const struct gpio_s&& _mode_pinout, \
		const pump_type_t&& _type = pump_type_t::drv8833_dc) : PumpImp(_id, _type) {

		status.id = std::move(_id);
		type = std::move(_type);
		init(std::move(_idletime_required_seconds), std::move(_runtime_limit_seconds), std::move(_pinout), std::move(_led_pinout), std::move(_fault_pinout), std::move(_mode_pinout));
		std::cout << "DRV8833DcPumpImp move constructor " << std::endl;//TODO: delete on STM32
	}
	~DRV8833DcPumpImp() {
		std::cout << "DRV8833DcPumpImp dtor " << std::endl;//TODO: delete on STM32
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	DRV8833DcPumpImp(DRV8833DcPumpImp const &) = delete;
	DRV8833DcPumpImp& operator=(DRV8833DcPumpImp const&) = delete;

	virtual void					run(const double& _dt, bool& cmd_consumed, const pump_cmd_t& _cmd = pump_cmd_t::stop);
	bool							init(const uint32_t& _idletime_required_seconds, const uint32_t& _runtime_limit_seconds, \
										const std::array<struct gpio_s, 2>& _pinout, const struct gpio_s& _led_pinout, \
										const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout);
	void							setEnable(void);
	bool							isFault(void);


protected:
	std::array<struct gpio_s, 2> 	aIN;							///< in1, in2
	struct gpio_s 					led;
	struct gpio_s 					fault;
	struct gpio_s 					mode;

	char zone_[3] = "-1";//TODO: delete on STM32

};

/*class DRV8833BldcPumpImp : public PumpImp {
public:
	DRV8833BldcPumpImp(const struct gpio_s& _pinout, const pump_type_t& _type = pump_type_t::drv8833_dc) : PumpImp(_type) {
		type = _type;
		//init(_pinout, 0);
		std::cout << "DRV8833BldcPumpImp standard constructor " << std::endl;//TODO: delete on STM32
	}
	DRV8833BldcPumpImp(const struct gpio_s&& _pinout, const pump_type_t&& _type = pump_type_t::drv8833_dc) : PumpImp(_type) {
		type = std::move(_type);
		//init(std::move(_pinout), 0);
		std::cout << "DRV8833BldcPumpImp move constructor " << std::endl;//TODO: delete on STM32
	}
	~DRV8833BldcPumpImp() {
		std::cout << "DRV8833BldcPumpImp dtor " << std::endl;//TODO: delete on STM32
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	DRV8833BldcPumpImp(DRV8833BldcPumpImp const &) = delete;
	DRV8833BldcPumpImp& operator=(DRV8833BldcPumpImp const&) = delete;

	// virtual
	float									read(const double& _dt = 0);

protected:
	char zone_[3] = "-1";//TODO: delete on STM32

};*/

class Pump {
public:
	Pump() {}
	Pump(const uint8_t& _id, const pump_type_t& _type) {
		std::cout << "Pump standard constructor " << std::endl;//TODO: delete on STM32
		imp_ = new PumpImp(_id, _type);
	}
	Pump(const uint8_t&& _id, const pump_type_t&& _type) {
		std::cout << "Pump move constructor " << std::endl;//TODO: delete on STM32
		imp_ = new PumpImp(std::move(_id), std::move(_type));
	}
	~Pump() {
		std::cout << "Pump dtor " << std::endl;//TODO: delete on STM32
		delete imp_;
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	Pump(Pump const&) = delete;
	Pump& operator=(Pump const&) = delete;

	virtual pump_type_t					getType() const;
	virtual bool						isValid() const;
	virtual bool 						start();
	virtual bool 						stop();
	virtual bool						init(const uint32_t& _idletime_required_seconds, const uint32_t& _runtime_limit_seconds);
	virtual void						run(const double& _dt, bool& cmd_consumed, const pump_cmd_t& _cmd);
	virtual void 						setState(const pump_state_t& _state);
	virtual pump_state_t				getState(void) const;
	virtual bool						isRunning(void) const;
	virtual struct PumpInfo_s			getStatus(void) const;
	virtual void						resetRuntime(void);
	virtual void						increaseRuntime(const double& _dt);
	virtual double						getRuntimeSeconds(void) const;
	virtual void						resetIdletime(void);
	virtual void						increaseIdletime(const double& _dt);
	virtual double						getIdletimeSeconds(void) const;

protected:
	PumpImp								*imp_;
};

/*class BinaryDcPump : public Pump {
public:
	BinaryDcPump(const struct gpio_s& _pinout) {
		imp_ = new BinaryDcPumpImp(_pinout);
		std::cout << "BinaryDcPump standard constructor " << std::endl;//TODO: delete on STM32
	}
	BinaryDcPump(const struct gpio_s&& _pinout) {
		std::cout << "BinaryDcPump move constructor " << std::endl;//TODO: delete on STM32
		imp_ = new BinaryDcPumpImp(std::move(_pinout));
	}
	~BinaryDcPump() {
		std::cout << "BinaryDcPump dtor " << std::endl;//TODO: delete on STM32
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	BinaryDcPump(BinaryDcPump const&) = delete;
	BinaryDcPump& operator=(BinaryDcPump const&) = delete;
};*/

class DRV8833DcPump : public Pump {
public:
	DRV8833DcPump(const uint8_t& _id, const uint32_t & _idletime_required_seconds, const uint32_t & _runtime_limit_seconds, \
		const std::array<struct gpio_s, 2>& _pinout, const struct gpio_s& _led_pinout, \
		const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout, \
		const pump_type_t& _type = pump_type_t::drv8833_dc) {
		imp_ = new DRV8833DcPumpImp(_id, _idletime_required_seconds, _runtime_limit_seconds, _pinout, _led_pinout, _fault_pinout, _mode_pinout);
	}
	DRV8833DcPump(const uint8_t&& _id, const uint32_t && _idletime_required_seconds, const uint32_t && _runtime_limit_seconds, \
		const std::array<struct gpio_s, 2>&& _pinout, const struct gpio_s&& _led_pinout, \
		const struct gpio_s&& _fault_pinout, const struct gpio_s&& _mode_pinout, \
		const pump_type_t&& _type = pump_type_t::drv8833_dc) {
		imp_ = new DRV8833DcPumpImp(std::move(_id), std::move(_idletime_required_seconds), std::move(_runtime_limit_seconds), std::move(_pinout), std::move(_led_pinout), std::move(_fault_pinout), std::move(_mode_pinout));
	}
	~DRV8833DcPump() {
		std::cout << "DRV8833DcPump dtor " << std::endl;//TODO: delete on STM32
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	DRV8833DcPump(DRV8833DcPump const&) = delete;
	DRV8833DcPump& operator=(DRV8833DcPump const&) = delete;

};

/*class DRV8833BldcPump : public Pump {
public:
	DRV8833BldcPump(const struct gpio_s& _pinout) {
		imp_ = new DRV8833BldcPumpImp(_pinout);
	}
	DRV8833BldcPump(const struct gpio_s&& _pinout) {
		imp_ = new DRV8833BldcPumpImp(std::move(_pinout));
	}
	~DRV8833BldcPump() {
		std::cout << "DRV8833BldcPump dtor " << std::endl;//TODO: delete on STM32
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	DRV8833BldcPump(DRV8833BldcPump const&) = delete;
	DRV8833BldcPump& operator=(DRV8833BldcPump const&) = delete;
};*/

