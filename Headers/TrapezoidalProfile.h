#pragma once
#define MIN_POWER 5
#define ACCELERATION_DURATION 250
#define DECELERATION_DURATION 250

#include "Bot.h"

//Every time you use nMotorEncoder, subtract the previous position so that everything work

//void resetEncoders(MotorData data)
//{
//	nMotorEncoder[data.motorPort] = 0;
//}

void resetCurrentPosition(MotorData& data)
{
	data.profile.previousPosition = nMotorEncoder[data.motorPort];
	writeDebugStreamLine("Previous Position: %i", data.profile.previousPosition);

}

int getcurrentPosition(MotorData& data)
{
	return nMotorEncoder[data.motorPort]-data.profile.previousPosition;
}

bool accelerate(MotorData& data)
{
	int realEncoder = nMotorEncoder[data.motorPort];
	writeDebugStreamLine("Real: %i", realEncoder);
	int currentPosition = getcurrentPosition(data);
	if(abs(currentPosition) >= abs(data.profile.accDuration))
	{
		return true;
	}

	float powerPercentage = (float)abs(currentPosition)/abs(ACCELERATION_DURATION);

	int power = (powerPercentage*(data.profile.maxPower-MIN_POWER))+MIN_POWER;

	power = abs(power) * sgn(data.profile.totalDuration);

	motor[data.motorPort] = power;


	return false;
}

bool stayAtSameSpeed(MotorData& data)
{
	int realEncoder = nMotorEncoder[data.motorPort];
	writeDebugStreamLine("Real: %i", realEncoder);
	int currentPosition = getcurrentPosition(data);
	int trapezoidalB1 = abs(data.profile.totalDuration)-data.profile.dccDuration;
	if(abs(currentPosition) >= abs(trapezoidalB1))
	{
		return true;
	}
	motor[data.motorPort] = 100 * sgn(data.profile.totalDuration);
	return false;
}

bool decelerate(MotorData& data)
{
	int currentPosition = getcurrentPosition(data); // No absolute value. Also, this should be a function, you shouldn't have this in 3 separate places
	writeDebugStreamLine("current: %i", currentPosition);
	writeDebugStreamLine("total: %i", data.profile.totalDuration);

	if(abs(currentPosition) >= abs(data.profile.totalDuration))
	{
		motor[data.motorPort] = 0;
		return true;
	}

	int maxpower = data.profile.maxPower;

	int adjustedPosition = (abs(data.profile.totalDuration) - DECELERATION_DURATION) - abs(currentPosition);
	int target = (abs(data.profile.totalDuration)-DECELERATION_DURATION);
	writeDebugStreamLine("adjusted: %i", adjustedPosition);
	int percentageValue = data.profile.maxPower - MIN_POWER;

	int power = ((1-abs(adjustedPosition)/(float)DECELERATION_DURATION)*percentageValue)+MIN_POWER;

	int direction = sgn(data.profile.totalDuration);

	power = abs(power) * direction;

	motor[data.motorPort] = power;


	return false;
}


/*
This NOT intMotorData. It is initTrapezoidalProfile.

Trap profile is very disorganized.
pull this into it's own program.
*/

bool initTrapezoidalProfileData(MotorData& data, short totalDuration, tMotor motorPort, TrapezoidalStates state)
{
	data.profile.state = state;
	data.motorPort = motorPort;
	data.profile.totalDuration = totalDuration;
	data.profile.accDuration = ACCELERATION_DURATION;
	data.profile.dccDuration = DECELERATION_DURATION;
	data.profile.maxPower = 100;
	return true;
}

void motorDataChecksAndProcedures(MotorData data)
{
	if(abs(data.profile.totalDuration) < data.profile.accDuration + data.profile.dccDuration)
	{
		data.profile.accDuration = ACCELERATION_DURATION*(abs(data.profile.totalDuration)/(float)(ACCELERATION_DURATION+DECELERATION_DURATION));
		data.profile.dccDuration = abs(data.profile.totalDuration)-data.profile.accDuration;


	}
	if(data.profile.totalDuration == 0)
	{
		//smartzero
	}
}

bool runMotorData(MotorData data)
{
	switch(data.profile.state)
	{
		case Acc:
		{
			bool isDone = accelerate(data);
			writeDebugStreamLine("%i", data.motorPort);
			writeDebugStreamLine("ACC");
			if(isDone)
			{
				data.profile.state = Coast;
			}

		}
			break;

		case Coast:
		{
			bool isDone = stayAtSameSpeed(data);
			writeDebugStreamLine("CCC");
			if(isDone)
			{
				data.profile.state = Dcc;
			}

		}
			break;

		case Dcc:
		{
			bool isDone = decelerate(data);
			writeDebugStreamLine("DCC");
			if(isDone)
			{
				data.stallData.isInitiated = false;
				writeDebugStreamLine("ENNNNNNNNNNNDDDDDDDDDDDEDDDDDD");
				return true;
			}
		}
			break;
	}
	return false;
}

bool TrapezoidalProfileMoveByDegrees(MotorData& data)
{
	bool isDone = runMotorData(data);
	if(isDone)
	{
		return true;
	}
	return false;
}
