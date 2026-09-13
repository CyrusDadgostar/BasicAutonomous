#include "BaseCommand.h"

enum TrapezoidalStates
{
	Acc,
	Coast,
	Dcc
};

struct StallData
{
	int previousTime;
	bool isInitiated;
	bool isPassed;
}

struct TrapData
{
	int previousPosition;
	short accDuration;
	short dccDuration;
	short totalDuration;
	byte maxPower;
	TrapezoidalStates state;
	bool isConfigured;
}

struct MotorData
{
	tMotor motorPort;
	TrapData profile;
	int previousEncoderValue;
	float velocity;
	int previousTime;
	StallData stallData;
};

struct Bot
{
	int index;
	bool isRecovery;
	bool isCheckForRecovery;
	CommandType currentBotIndexCommand;
	BaseCommand commands[MAX_COMMAND_LENGTH];
	MotorData left;
	MotorData right;
	MotorData center;
};

Bot bot;
