#include "pumps.h"


void encodePumpStatus(const struct PumpInfo_s & _pump, uint32_t & status) {

	switch (_pump.id)
	{
	case 0:
		status |= _pump.state;
		if (_pump.forced == true) 			status |= (1 << 6);
		if (_pump.cmd_consumed == true) 	status |= (1 << 7);
		break;
	case 1:
		status |= _pump.state << 8;
		if (_pump.forced == true) 			status |= (1 << 14);
		if (_pump.cmd_consumed == true) 	status |= (1 << 15);
		break;
	case 2:
		status |= (_pump.state << 16);
		if (_pump.forced == true) 			status |= (1 << 22);
		if (_pump.cmd_consumed == true) 	status |= (1 << 23);
		break;
	case 3:
		status |= (_pump.state << 24);
		if (_pump.forced == true) 			status |= (1 << 30);
		if (_pump.cmd_consumed == true) 	status |= (1 << 31);
		break;
	default:
		break;
	}

}

void decodePumpStatus(std::array<struct PumpInfo_s, 4> & a_pump, const std::bitset<32> & _status) {

	const std::bitset<32> pumpstatemask(0x0000000F);
	std::bitset<32> tmp;

	for (uint8_t i = 0; i < 4; i++)
	{
		tmp = _status;
		if (i > 0) tmp >>= 8 * i;
		tmp &= pumpstatemask;
		a_pump[i].id = i;
		a_pump[i].state = tmp.to_ulong();
		if (_status.test(6)) a_pump[i].forced = true;
		if (_status.test(7)) a_pump[i].cmd_consumed = true;
	}
}

/************************************/
/*! Pump class implementation */
/************************************/

pumptype_t& Pump::getType(void) {
	return type;
}

bool Pump::init(void) {
	return true;
}

void Pump::run(const double & _dt) {}

pumpstate_t& Pump::getState(void) {
	return state;
}

bool Pump::isRunning(void) {
	return getState() == pumpstate_t::running || getState() == pumpstate_t::reversing ? true : false;
}

struct PumpInfo_s& Pump::getStatus(void) {
	return status;
}

void Pump::resetRuntime(void) {
	runtime_seconds = 0.0;
}

void Pump::increaseRuntime(const double & _dt) {
	runtime_seconds += _dt;
}

double& Pump::getRuntimeSeconds(void) {
	return runtime_seconds;
}

void Pump::resetIdletime(void) {
	idletime_seconds = 0.0;
}

void Pump::increaseIdletime(const double & _dt) { 
	idletime_seconds += _dt;
}

double&	Pump::getIdletimeSeconds(void) { 
	return idletime_seconds;
}


/************************************/
/*! BinaryPump class implementation */
/************************************/

bool BinaryPump::start(void) {

	bool success = false;
	status.forced = false;

	if (isRunning() == false) {
		std::cout << "BinPump running" << std::endl;
		//HAL_GPIO_WritePin(pinout.port, pinout.pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(led.port, led.pin, GPIO_PIN_SET);
		setState(pumpstate_t::running);
		resetIdletime();
		resetRuntime();
		success = true;
	}

	return success;
}

bool BinaryPump::stop(void){

	bool success = false;
	status.forced = false;

	if (isRunning() == true) {
		std::cout << "BinPump stopped" << std::endl;
		//HAL_GPIO_WritePin(pinout.port, pinout.pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(led.port, led.pin, GPIO_PIN_RESET);
		setState(pumpstate_t::stopped);
		resetIdletime();
		resetRuntime();
		success = true;
	}

	return success;
}

bool BinaryPump::init(const uint8_t & _id, const uint32_t & _idletime_required_seconds, const uint32_t & _runtime_limit_seconds, const struct gpio_s & _pinout, const struct gpio_s & _led) {
	status.id = _id;
	pinout.pin = _pinout.pin;
	pinout.port = _pinout.port;
	led.pin = _led.pin;
	led.port = _led.port;
	idletime_required_seconds = _idletime_required_seconds;
	idletime_seconds = idletime_required_seconds;
	runtime_limit_seconds = _runtime_limit_seconds;
	//HAL_GPIO_WritePin(pinout.port, pinout.pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(led.port, led.pin, GPIO_PIN_RESET);
	setState(pumpstate_t::stopped);

	return true;
}

void BinaryPump::run(const double & _dt, const pumpcmd_t & _cmd, bool & cmd_consumed) {

	if (_cmd != pumpcmd_t::reverse) {

		switch (getState())
		{
		case pumpstate_t::init:
			std::cout << "BinPump init" << std::endl;
			stop();
			cmd_consumed = true;
			break;

		case pumpstate_t::waiting:
			std::cout << "BinPump waiting" << std::endl;
			increaseIdletime(_dt);
			if (getIdletimeSeconds() > idletime_required_seconds) {
				start();
				cmd_consumed = true;
			}

			break;

		case pumpstate_t::stopped:
			std::cout << "BinPump stopped" << std::endl;
			increaseIdletime(_dt);
			if ((_cmd == pumpcmd_t::start) && (getIdletimeSeconds() > idletime_required_seconds)) {
				start();
				cmd_consumed = true;
			}
			else if ((_cmd == pumpcmd_t::start) && (getIdletimeSeconds() <= idletime_required_seconds)) {
				setState(pumpstate_t::waiting);
			}
			break;

		case pumpstate_t::running:
			std::cout << "BinPump running" << std::endl;
			increaseRuntime(_dt);
			if (_cmd == pumpcmd_t::start) {
				cmd_consumed = true;
			}
			else {
				stop();
				cmd_consumed = true;
			}
			if (getRuntimeSeconds() > runtime_limit_seconds && status.forced == false) stop(); //TODO: how to handle force.... commands?
			break;

		default:
			break;
		}
	}
	else {
		//Invalid command, not consumed, let's stop pump for safety
		stop();
	}
}

void BinaryPump::forcestart(void) {

	if (isRunning() == false) resetRuntime();
	std::cout << "BinPump forcestart" << std::endl;
	//HAL_GPIO_WritePin(pinout.port, pinout.pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(led.port, led.pin, GPIO_PIN_SET);
	setState(pumpstate_t::running);
	status.forced = true;
}
void BinaryPump::forcestop(void) {

	if (isRunning() == true) resetIdletime();
	std::cout << "BinPump forcestop" << std::endl;
	//HAL_GPIO_WritePin(pinout.port, pinout.pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(led.port, led.pin, GPIO_PIN_RESET);
	setState(pumpstate_t::stopped);
	status.forced = true;
}

void BinaryPump::setState(const pumpstate_t & _state) {
	state = _state;
	status.state = static_cast<uint32_t>(_state);
}


/************************************/
/*! DRV8833Pump class implementation */
/************************************/

bool DRV8833Pump::init(const uint8_t & _id, const uint32_t & _idletime_required_seconds, const uint32_t & _runtime_limit_seconds, \
	const std::array<struct gpio_s, 4> & _pinout, const struct gpio_s & _led_pinout, \
	const struct gpio_s & _fault_pinout, const struct gpio_s & _mode_pinout) {

	bool success = true;

	this->status.id = _id;
	this->fault.pin = _fault_pinout.pin;
	this->fault.port = _fault_pinout.port;
	this->mode.pin = _mode_pinout.pin;
	this->mode.port = _mode_pinout.port;
	this->led.pin = _led_pinout.pin;
	this->led.port = _led_pinout.port;
	this->idletime_required_seconds = _idletime_required_seconds;
	this->idletime_seconds = this->idletime_required_seconds;
	this->runtime_limit_seconds = _runtime_limit_seconds;

	switch (this->getType()) {
	case pumptype_t::drv8833_dc:
		success = false;
		break;
	case pumptype_t::drv8833_bldc:
		this->aIN[0].pin = _pinout[0].pin;
		this->aIN[0].port = _pinout[0].port;
		this->aIN[1].pin = _pinout[1].pin;
		this->aIN[1].port = _pinout[1].port;
		this->aIN[2].pin = _pinout[2].pin;
		this->aIN[2].port = _pinout[2].port;
		this->aIN[3].pin = _pinout[3].pin;
		this->aIN[3].port = _pinout[3].port;
		break;
	default:
		success = false;
		break;
	}

	this->setEnable();

	if (this->isFault() == false) {
		//TODO:
	}
	else {
		this->setState(pumpstate_t::fault);
		success = false;
	}

	return success;
}

bool DRV8833Pump::init(const uint8_t & _id, const uint32_t & _idletime_required_seconds, const uint32_t & _runtime_limit_seconds, \
	const std::array<struct gpio_s, 2> & _pinout, const struct gpio_s & _led_pinout, \
	const struct gpio_s & _fault_pinout, const struct gpio_s & _mode_pinout) {

	bool success = true;

	this->status.id = _id;
	this->fault.pin = _fault_pinout.pin;
	this->fault.port = _fault_pinout.port;
	this->mode.pin = _mode_pinout.pin;
	this->mode.port = _mode_pinout.port;
	this->led.pin = _led_pinout.pin;
	this->led.port = _led_pinout.port;
	this->idletime_required_seconds = _idletime_required_seconds;
	this->idletime_seconds = this->idletime_required_seconds;
	this->runtime_limit_seconds = _runtime_limit_seconds;

	switch (this->getType()) {
	case pumptype_t::drv8833_dc:
		this->aIN[0].pin = _pinout[0].pin;
		this->aIN[0].port = _pinout[0].port;
		this->aIN[1].pin = _pinout[1].pin;
		this->aIN[1].port = _pinout[1].port;
		this->aIN[2].pin = 0;
		this->aIN[2].port = 0;// = nullptr;
		this->aIN[3].pin = 0;
		this->aIN[3].port = 0;// = nullptr;
		break;
	case pumptype_t::drv8833_bldc:
		success = false;
		break;
	default:
		success = false;
		break;
	}

	this->setEnable();

	if (this->isFault() == false) {
		//HAL_GPIO_WritePin(this->aIN[0].port, this->aIN[0].pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(this->aIN[1].port, this->aIN[1].pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(this->led.port, this->led.pin, GPIO_PIN_RESET);
	}
	else {
		this->setState(pumpstate_t::fault);
		success = false;
	}

	return success;
}

void DRV8833Pump::run(const double & _dt, const pumpcmd_t & _cmd, bool & cmd_consumed, bool & fault) {

	if (this->isFault() == true) this->setState(pumpstate_t::fault);

	switch (this->getState()) {
	case pumpstate_t::init:
		std::cout << "DRV8833 state init" << std::endl;
		this->setState(pumpstate_t::stopped);
		if (_cmd == pumpcmd_t::stop) cmd_consumed = true;
		else cmd_consumed = false;
		break;

	case pumpstate_t::waiting:
		std::cout << "DRV8833 state waiting" << std::endl;
		this->increaseIdletime(_dt);
		if (this->getIdletimeSeconds() > this->idletime_required_seconds) {
			if (_cmd == pumpcmd_t::start) this->start();
			else this->stop();
			cmd_consumed = true;
		}
		break;

	case pumpstate_t::stopped:
		std::cout << "DRV8833 state stopped" << std::endl;
		this->increaseIdletime(_dt);
		if (_cmd == pumpcmd_t::start) {
			if (this->getIdletimeSeconds() > this->idletime_required_seconds) {
				this->start();
				cmd_consumed = true;
			}
			else if (this->getIdletimeSeconds() <= this->idletime_required_seconds) {
				this->setState(pumpstate_t::waiting);
			}
		}
		else {
			cmd_consumed = true;
		}
		break;

	case pumpstate_t::running:
		std::cout << "DRV8833 state running" << std::endl;
		this->increaseRuntime(_dt);
		if (_cmd == pumpcmd_t::start) {
			cmd_consumed = true;
		}
		else if (_cmd == pumpcmd_t::stop) {
			this->stop();
			cmd_consumed = true;
		}
		else {
			this->stop();
			cmd_consumed = false;
		}
		if (this->getRuntimeSeconds() > this->runtime_limit_seconds && this->status.forced == false) this->stop(); //TODO: how to handle force.... commands?
		break;

	case pumpstate_t::reversing:
		std::cout << "DRV8833 state reversing" << std::endl;
		this->increaseRuntime(_dt);
		if (_cmd == pumpcmd_t::start) {
			this->start();
			cmd_consumed = true;
		}
		else {
			cmd_consumed = true;
		}
		if (this->getRuntimeSeconds() > 30.0 && this->status.forced == false) this->stop(); //TODO: how to handle force.... commands?
		break;

	case pumpstate_t::fault:
		std::cout << "DRV8833 state fault" << std::endl;
		fault = true;
		this->increaseIdletime(_dt);
		this->stop(); //TODO: or forcestop?
		if (_cmd == pumpcmd_t::stop) cmd_consumed = true;
		else cmd_consumed = false;
		break;

	case pumpstate_t::sleep:
		std::cout << "DRV8833 state sleep" << std::endl;
		this->increaseIdletime(_dt);
		this->stop(); //TODO: or forcestop?
		if (_cmd == pumpcmd_t::stop) cmd_consumed = true;
		else cmd_consumed = false;
		break;

	default:
		break;
	}


}

bool DRV8833Pump::start(void) {

	bool success = false;
	this->status.forced = false;

	if (this->isRunning() == false) {
		this->resetIdletime();
		this->resetRuntime();

		switch (this->getType()) {
		case pumptype_t::drv8833_dc:
			std::cout << "DRV8833 start" << std::endl;
			//HAL_GPIO_WritePin(this->aIN[0].port, this->aIN[0].pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(this->aIN[1].port, this->aIN[1].pin, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(this->led.port, this->led.pin, GPIO_PIN_SET);
			success = true;
			break;
		case pumptype_t::drv8833_bldc:
			//TODO:
			break;
		default:
			break;
		}
	}

	this->setState(pumpstate_t::running);

	return success;
}

bool DRV8833Pump::stop(void) {

	bool success = false;
	this->status.forced = false;

	if (this->isRunning() == true) {
		this->resetIdletime();
		this->resetRuntime();

		switch (this->getType()) {
		case pumptype_t::drv8833_dc:
			std::cout << "DRV8833 stop" << std::endl;
			//HAL_GPIO_WritePin(this->aIN[0].port, this->aIN[0].pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(this->aIN[1].port, this->aIN[1].pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(this->led.port, this->led.pin, GPIO_PIN_RESET);
			success = true;
			break;
		case pumptype_t::drv8833_bldc:
			//TODO:
			break;
		default:
			break;
		}
	}

	this->setState(pumpstate_t::stopped);

	return success;
}

bool DRV8833Pump::reverse(void) {

	bool success = false;
	this->status.forced = false;


	this->resetIdletime();
	this->resetRuntime();

	switch (this->getType()) {
	case pumptype_t::drv8833_dc:
		std::cout << "DRV8833 reverse" << std::endl;
		//HAL_GPIO_WritePin(this->aIN[0].port, this->aIN[0].pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(this->aIN[1].port, this->aIN[1].pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(this->led.port, this->led.pin, GPIO_PIN_SET);
		success = true;
		break;
	case pumptype_t::drv8833_bldc:
		//TODO:
		break;
	default:
		break;
	}

	this->setState(pumpstate_t::reversing);


	return success;

}

bool DRV8833Pump::forcestart(void) {

	bool success = true;

	this->status.forced = true;

	switch (this->getType()) {
	case pumptype_t::drv8833_dc:
		std::cout << "DRV8833 forcestart" << std::endl;
		//HAL_GPIO_WritePin(this->aIN[0].port, this->aIN[0].pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(this->aIN[1].port, this->aIN[1].pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(this->led.port, this->led.pin, GPIO_PIN_SET);
		this->setState(pumpstate_t::running);
		break;
	case pumptype_t::drv8833_bldc:
		//TODO:
		success = false;
		break;
	default:
		success = false;
		break;
	}

	if (this->isRunning() == false) {
		this->resetRuntime();
	}
	else {
		success = false;
	}

	return success;
}
bool DRV8833Pump::forcestop(void) {

	bool success = true;

	this->status.forced = true;

	switch (this->getType()) {
	case pumptype_t::drv8833_dc:
		std::cout << "DRV8833 forcestop" << std::endl;
		//HAL_GPIO_WritePin(this->aIN[0].port, this->aIN[0].pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(this->aIN[1].port, this->aIN[1].pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(this->led.port, this->led.pin, GPIO_PIN_RESET);
		this->setState(pumpstate_t::stopped);
		break;
	case pumptype_t::drv8833_bldc:
		//TODO:
		success = false;
		break;
	default:
		success = false;
		break;
	}

	if (this->isRunning() == true) {
		this->resetIdletime();
	}
	else {
		success = false;
	}

	return success;
}

bool DRV8833Pump::forcereverse(void) {

	bool success = true;

	this->status.forced = true;

	switch (this->getType()) {
	case pumptype_t::drv8833_dc:
		std::cout << "DRV8833 forcereverse" << std::endl;
		//HAL_GPIO_WritePin(this->aIN[0].port, this->aIN[0].pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(this->aIN[1].port, this->aIN[1].pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(this->led.port, this->led.pin, GPIO_PIN_SET);
		this->setState(pumpstate_t::reversing);
		break;
	case pumptype_t::drv8833_bldc:
		//TODO:
		success = false;
		break;
	default:
		success = false;
		break;
	}

	if (this->isRunning() == false) {
		this->resetRuntime();
	}
	else {
		success = false;
	}

	return success;
}

void DRV8833Pump::setSleep(void) {
	this->stop();
	//HAL_GPIO_WritePin(this->mode.port, this->mode.pin, GPIO_PIN_RESET);
	this->setState(pumpstate_t::sleep);
}

void DRV8833Pump::setEnable(void) {
	this->stop();
	//HAL_GPIO_WritePin(this->mode.port, this->mode.pin, GPIO_PIN_SET);
}
bool DRV8833Pump::isFault(void) {
	return false;// HAL_GPIO_ReadPin(this->fault.port, this->fault.pin) == GPIO_PIN_RESET ? true : false;
}

void DRV8833Pump::setState(const pumpstate_t & _state) {
	this->state = _state;
	this->status.state = static_cast<uint32_t>(_state);
}

/***********************************/
/*! PumpController class implementation */
/***********************************/

bool PumpController::update(const double& _dt, bool& _activate_watering) {

	bool consumed = false;
	bool fault = false;
	std::bitset<8> errcode;
	/*******errcode**********
	 * 00000000
	 * ||||||||->(0) 1 if cmd not consumed
	 * |||||||-->(1) 1 if active, 0 if stopped
	 * ||||||--->(2) 1 if runtime timeout
	 * |||||---->(3) 1 if fault occurred at least once
	 * ||||----->(4)
	 * |||------>(5) 1 if none of avbl pumps was correctly initialized/created
	 * ||------->(6) 1 if controller is in wrong or not avbl mode
	 * |-------->(7) 1 if pumps_count is 0
	 *************************/

	if (this->pumps_count > 0)
	{
		switch (this->mode)
		{

		case pumpcontrollermode_t::init:
			std::cout << "PumpCtrl init" << std::endl;
			errcode.set(6, true);
			break;

		case pumpcontrollermode_t::external:
			std::cout << "PumpCtrl external" << std::endl;
			if (this->pBinPump != nullptr)
			{
				std::cout << "PumpCtrl pBinPump != nullptr" << std::endl;
				if (_activate_watering)
				{
					this->pBinPump->run(_dt, pumpcmd_t::start, consumed);
					if (consumed == false) errcode.set(0, true);
				}
				else
				{
					this->pBinPump->run(_dt, pumpcmd_t::stop, consumed);
					if (consumed == false) errcode.set(0, true);
				}
			}
			else if (this->p8833Pump != nullptr)
			{
				std::cout << "PumpCtrl p8833Pump != nullptr" << std::endl;
				if (_activate_watering == true && this->p8833Pump->getState() != pumpstate_t::waiting)
				{
					this->p8833Pump->run(_dt, pumpcmd_t::start, consumed, fault);
					if (consumed == false) errcode.set(0, true);
				}
				else if (_activate_watering == true && this->p8833Pump->getState() == pumpstate_t::waiting)
				{
					this->p8833Pump->run(_dt, pumpcmd_t::stop, consumed, fault);
					if (consumed == false) errcode.set(0, true);
					errcode.set(2, true); //runtime timeout
					_activate_watering = false;
				}
				else if (_activate_watering == false)
				{
					this->p8833Pump->run(_dt, pumpcmd_t::stop, consumed, fault);
				}
			}
			else errcode.set(5, true);

			break;

		case pumpcontrollermode_t::manual:
			errcode.set(6, true);
			break;

		case pumpcontrollermode_t::automatic:
			errcode.set(6, true);
			break;

		case pumpcontrollermode_t::sleep:
			errcode.set(6, true);
			break;

		default:
			errcode.set(6, true);
			break;
		}

		if (this->pBinPump != nullptr)
		{
			if (this->pBinPump->getState() == pumpstate_t::running) errcode.set(1, true);
		}
		else if (this->p8833Pump != nullptr)
		{
			if (this->p8833Pump->getState() == pumpstate_t::running) errcode.set(1, true);
		}

		if (fault == true) {
			if (++this->pump_fault_occurence_cnt > 0) errcode.set(3, true);
		}

	}
	else errcode.set(7, true);

	this->pump_encoded_status = static_cast<uint8_t>(errcode.to_ulong());

	return consumed;
}

bool PumpController::createPump(const pumptype_t & _pumptype) {

	bool success = true;

	switch (_pumptype) {
	case pumptype_t::binary:
		if (this->pumps_count < (this->pumps_limit + 1))
		{
			this->pBinPump = new BinaryPump();
			this->pumps_count++;
		}
		else
		{
			std::cout << "Pump count limit reached" << std::endl;
			success = false;
		}
		break;

	case pumptype_t::generic:
		success = false;
		break;

	case pumptype_t::drv8833_dc:
		if (this->pumps_count < (this->pumps_limit + 1))
		{
			this->p8833Pump = new DRV8833Pump(motortype_t::dc_motor);
			this->pumps_count++;
		}
		else
		{
			std::cout << "Pump count limit reached" << std::endl;
			success = false;
		}
		break;

	case pumptype_t::drv8833_bldc:
		if (this->pumps_count < (this->pumps_limit + 1))
		{
			this->p8833Pump = new DRV8833Pump(motortype_t::bldc_motor);
			this->pumps_count++;
		}
		else
		{
			std::cout << "Pump count limit reached" << std::endl;
			success = false;
		}
		break;

	default:
		std::cout << "Incorrect pump type" << std::endl;
		success = false;
		break;
	}

	return success;
}

bool PumpController::setMode(const pumpcontrollermode_t & _mode) {

	bool changed = true;

	if (this->mode != _mode && _mode != pumpcontrollermode_t::init)
	{
		this->mode = _mode;
	}
	else changed = false;

	return changed;
}

const pumpcontrollermode_t&	PumpController::getMode(void) {
	return this->mode;
}

uint8_t& PumpController::getPumpStatusEncoded(void) {
	return this->pump_encoded_status;
}

