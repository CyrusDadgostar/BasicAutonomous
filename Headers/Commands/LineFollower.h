#pragma once
struct LineFollowData
{
	CommandType commandType;
	bool isConfigured;
	byte leftPower;
	byte rightPower;
	unsigned short duration;
};

void initLineFollowData(byte leftPower, byte rightPower, unsigned short duration, bool isRecovery = false)
{
	VERIFY(sizeof(LineFollowData) <= sizeof(BaseCommand));

	LineFollowData data;
	data.commandType = isRecovery ? LineFollowRec : LineFollow;
	data.isConfigured = false;
	data.leftPower = leftPower;
	data.rightPower = rightPower;
	data.duration = duration;

	memcpy(&bot.commands[bot.index], &data, sizeof(LineFollowData));

	bot.index++;
	writeDebugStreamLine("NextCommand: %i CommandType: %i", bot.index, bot.commands[bot.index].commandType);
}

void configure(LineFollowData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;


	BaseCommand* command = &bot.commands[bot.index];
	memcpy(command, data, sizeof(LineFollowData));

	nMotorEncoder[leftMotor] = 0;
	nMotorEncoder[rightMotor] = 0;
}

bool hasRan(LineFollowData* data)
{
	displayTextLine(2, "Target: %i", data->duration);
	displayTextLine(3, "R Deg Moved: %i", abs(nMotorEncoder[rightMotor]));
	displayTextLine(4, "L Deg Moved: %i", abs(nMotorEncoder[leftMotor]));
	writeDebugStreamLine("Left: %i, Right: %i", nMotorEncoder[leftMotor], nMotorEncoder[rightMotor]);
	if(!(nMotorEncoder[leftMotor] < data->duration && nMotorEncoder[rightMotor] < data->duration)) return true;
	if((TColors)SensorValue[color] == BLACKCOLOR)
	{
		motor[leftMotor] = data->leftPower;
		motor[rightMotor] = 0;
	}
	else if(SensorValue[color] != BLACKCOLOR)
	{
		motor[leftMotor] = 0;
		motor[rightMotor] = data->rightPower;
	}


	BaseCommand* command = &bot.commands[bot.index];
	memcpy(command, data, sizeof(LineFollowData));

	return false;
}

void cleanup(LineFollowData* data)
{
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
}
