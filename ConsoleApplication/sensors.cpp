#include "sensors.h"

void SensorImp::read() {
	std::cout << "SensorImp read sensor_type: " << 0 << std::endl;
}

sensor_type_t SensorImp::getType() {
	return sensor_type;
}

void SensorImp::getResult(bool& val) {
	val = false;
}
void SensorImp::getResult(float& val) {
	val = -1;
}

void OpticalWaterLevelSensorImp::read() {
	std::cout << "OpticalWaterLevelSensorImp read: " << mount_pos_meters << std::endl;
}

void OpticalWaterLevelSensorImp::getResult(bool& val) {
	val = state == fixedwaterlevelsensorstate_t::wet ? true : false;
}

void WaterFlowSensorImp::read() {
	std::cout << "WaterFlowSensorImp read: " << zone_ << std::endl;
}

void WaterFlowSensorImp::getResult(float& val) {
	val = 6.5;
}

void TemperatureSensorImp::read() {

}
void TemperatureSensorImp::getResult(float& val) {

}

void Sensor::read() {
	imp_->read();
}
sensor_type_t Sensor::getType() {
	return imp_->getType();
}

void Sensor::getResult(bool& val) {
	imp_->getResult(val);
}

void Sensor::getResult(float& val) {
	imp_->getResult(val);
}
