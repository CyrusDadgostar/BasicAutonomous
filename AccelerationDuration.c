#define UPDATE_CYCLE_IN_MILLISECONDS 100

struct MotorMovementData
{
	tMotor port;
	int previousEncoderValue;
	float velocity;
	int previousTime;
	int stallPreviousTime;
}

void initMotorMovement(MotorMovementData moto, tMotor motorPort)
{
	moto.port = motorPort;
	moto.previousEncoderValue = 0;
	moto.velocity = 0;
	moto.previousTime = 0;
}

int EncoderDifference(MotorMovementData moto)
{

	return nMotorEncoder[moto.port]-moto.previousEncoderValue;
}

void trackVelocity(MotorMovementData moto)
{
	if(time1[T1] - moto.previousTime >= UPDATE_CYCLE_IN_MILLISECONDS)
	{
		moto.velocity = EncoderDifference(moto);
		moto.previousEncoderValue = nMotorEncoder[moto.port];
		moto.previousTime = time1[T1];
	}
}
task main()
{
	MotorMovementData left;
	MotorMovementData right;
	initMotorMovement(left, motorB);
	initMotorMovement(right, motorC);
	time1[T1] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	motor[motorB] = 5;
	motor[motorC] = 5;
	while(true)
	{
		trackVelocity(left);
		writeDebugStreamLine("%0.2f", left.velocity);
	}
}
