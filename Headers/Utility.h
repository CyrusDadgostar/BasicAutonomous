#include "bot.h"

#pragma once
void finalize()
{
	writeDebugStreamLine("Stoppedddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd");

	bot.index++;
	if(bot.isRecovery)
	{

		writeDebugStreamLine("NO REC: NextCommand: %i CommandType: %i", bot.index, bot.commands[bot.index].commandType);
		return;
	}

	while( (bot.commands[bot.index].commandType & 0x80) > 0)
	{
		bot.index++;
		writeDebugStreamLine("NextCommand: %i CommandType: %i", bot.index, bot.commands[bot.index].commandType);
	}


}


int distanceToDegrees(int mm)
{
	writeDebugStreamLine("DegreesFromDistance: %i", radiansToDegrees(mm/RADIUS_OF_WHEEL));
	return (mm*180)/(PI*RADIUS_OF_WHEEL);
}

float degreesToMove(int angle)
{
 	float length = RADIUS_OF_ROBOT * angle;
 	float degrees = length/RADIUS_OF_WHEEL;
 	writeDebugStreamLine("DegreesToMove: %i", degrees);
 	return degrees;
}
