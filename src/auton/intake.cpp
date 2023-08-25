#include "main.h"
#include "okapi/api.hpp"
#include "auton/intake.hpp"

using namespace okapi;

void pressure(double time, double speed, double pressure_time) {
	MotorGroup left({3, -11, -10});
	MotorGroup right({-5, 18, 4});
	Motor intake(-9);

	IMU inertial(16, IMUAxes::z);
	IMU inertial2(12, IMUAxes::z);
	IMU inertial3(19, IMUAxes::z);
	inertial.reset(0);
	inertial2.reset(0);
	inertial3.reset(0);

	left.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
	right.setEncoderUnits(AbstractMotor::encoderUnits::degrees);

	left.setBrakeMode(AbstractMotor::brakeMode::coast);
	right.setBrakeMode(AbstractMotor::brakeMode::coast);

	left.moveVoltage(-speed * 120);
	right.moveVoltage(-speed * 120);
	pros::delay(time);

	changing = true;
	intake_voltage = 11999;
	pros::delay(pressure_time);
	
	intake_voltage = 0;
	left.moveVoltage(0);
	right.moveVoltage(0);

	last_turn_direction = r;
	angle_error = (inertial.get() + inertial2.get() + inertial3.get()) / 3;
}

void start_intake(void) {
	changing = true;
	intake_voltage = 12000;
}

void stop_intake(void) {
	intake_voltage = 0;
}

void slow_intake(void) {
	changing = true;
	intake_voltage = 8000;
}

void reverse_intake(void) {
	intake_voltage = -12000;
}

void intake_handler(void*) {
	Motor intake(-9);
	ControllerButton R2(ControllerDigital::R2);
	intake.setGearing(AbstractMotor::gearset::blue);
	intake_voltage = 0;
	speed = 2;

	bool past_change = false;

	while (true) {
		if (!R2.isPressed()) {
			intake.moveVoltage(intake_voltage);
			if (fabs(intake.getActualVelocity()) < speed && fabs(intake_voltage) > 5000 && !changing && intake_voltage != 11999)  {
				intake.moveVoltage(-12000);
				pros::delay(200);
				intake.moveVoltage(intake_voltage);
				pros::delay(500);
			}
		} else {
			intake.moveVoltage(-12000);
		}
		if (changing && !past_change) {
			changing = false;
			pros::delay(700);
		}
		past_change = changing;
		pros::delay(10);
	}
}