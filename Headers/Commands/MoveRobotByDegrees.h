#pragma once
#include "TrapezoidalProfile.h"
struct RobotDrivetrainData
{
	CommandType commandType;
	bool isConfigured;
	TrapezoidalStates leftState;
	TrapezoidalStates rightState;
	short leftDuration;
	short rightDuration;
};
const int commandSize = sizeof(RobotDrivetrainData);
void initMoveByDegreeData(short leftDuration, short rightDuration, TrapezoidalStates leftState, TrapezoidalStates rightState, bool isRecovery = false)
{
	VERIFY(commandSize <= sizeof(BaseCommand));

	RobotDrivetrainData data;
	data.commandType = isRecovery ? MoveByDegreesRec : MoveByDegrees;
	data.isConfigured = false;
	data.leftDuration = leftDuration;
	data.rightDuration = rightDuration;
	data.leftState = leftState;
	data.rightState = rightState;

	writeDebugStreamLine("LEFTDURATION: %i", data.leftDuration);
	writeDebugStreamLine("RIGHTDURATION: %i", data.rightDuration);

	memcpy(&bot.commands[bot.index], &data, commandSize);

	bot.index++;
	writeDebugStreamLine("NextCommand: %i CommandType: %i", bot.index, bot.commands[bot.index].commandType);
}

void configure(RobotDrivetrainData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	initTrapezoidalProfileData(bot.left, data->leftDuration, leftMotor, data->leftState);
	initTrapezoidalProfileData(bot.right, data->rightDuration, rightMotor, data->rightState);
	motorDataChecksAndProcedures(bot.left);
	motorDataChecksAndProcedures(bot.right);
	resetCurrentPosition(bot.left);
	resetCurrentPosition(bot.right);

	BaseCommand* command = &bot.commands[bot.index];
	memcpy(command, data, sizeof(RobotDrivetrainData));
}

bool hasRan(RobotDrivetrainData* data)
{
	bool isLeftDone = TrapezoidalProfileMoveByDegrees(bot.left);
	bool isRightDone = TrapezoidalProfileMoveByDegrees(bot.right);
	writeDebugStreamLine("~~~~~~~~~~~~~~~~~~~~~ CURRENT COMMAND INDEX: %i ~~~~~~~~~~~~~~~~~~~~~~~", bot.index);
	writeDebugStreamLine("IsLeftDone: %i", isLeftDone);
	writeDebugStreamLine("IsRightDone: %i", isRightDone);

	if(isLeftDone && isRightDone)
	{
		return true;
	}

	return false;
}

void cleanup(RobotDrivetrainData* data)
{
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
}
