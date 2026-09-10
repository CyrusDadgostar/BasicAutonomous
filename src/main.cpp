/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       lance                                                     */
/*    Created:      3/26/2026, 3:12:03 PM                                     */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;


#define KP 0.5
#define GYRO_SPEED 50
#define BUFFER 20
#define MOVEMENT_INCREASE_FORWARD_P 3


#define C  1
#define D  2
#define E  3
#define F  4
#define G  5
#define A  6
#define B  12


vex::brain Brain;

// Motors
vex::motor rightArmMotor(PORT9, true);
vex::motor leftArmMotor(PORT3, false);

vex::motor RightFrontChassisMotor(PORT8, false);
vex::motor RightBackChassisMotor(PORT1, true);
vex::motor LeftFrontChassisMotor(PORT2, true);
vex::motor LeftBackChassisMotor(PORT7, false);

// Pneumatics
vex::pneumatic armSolenoid(PORT10);

// Sensors
vex::gyro firstStageArmGyro(PORT5);
vex::bumper backTouchSensor(PORT12);

// Controller
vex::controller Controller;

// Timer
vex::timer Timer;

//Gyro
vex::inertial Gyro;

vex::touchled touchLED(PORT4);

//vex::aivision AiVision(PORT2, aivision::ALL_AIOBJS);
vex::optical OpticalSensor(PORT11);
    
enum GyroMovement
{
    Straight,
    Turn
};

enum ClawPneumaticStates
{
    Left,
    Right
};

//Chassis functions

void moveChassisMotors(int leftPower, int rightPower)
{
    LeftFrontChassisMotor.setVelocity(leftPower, pct);
    RightFrontChassisMotor.setVelocity(rightPower, pct);
    LeftBackChassisMotor.setVelocity(leftPower, pct);
    RightBackChassisMotor.setVelocity(rightPower, pct);
    LeftFrontChassisMotor.spin(fwd);
    RightFrontChassisMotor.spin(fwd);
    LeftBackChassisMotor.spin(fwd);
    RightBackChassisMotor.spin(fwd);
}

void stopChassisMotors()
{
    //Set the velocity of the motors to zero
    LeftFrontChassisMotor.setVelocity(0, pct);
    LeftBackChassisMotor.setVelocity(0, pct);
    RightFrontChassisMotor.setVelocity(0, pct);
    RightBackChassisMotor.setVelocity(0, pct);

    //Set the motors to hold the current position when stopped
    LeftFrontChassisMotor.setStopping(hold);
    LeftBackChassisMotor.setStopping(hold);
    RightFrontChassisMotor.setStopping(hold);
    RightBackChassisMotor.setStopping(hold);

    //Stop the motors
    LeftFrontChassisMotor.stop();
    LeftBackChassisMotor.stop();
    RightFrontChassisMotor.stop();
    RightBackChassisMotor.stop();
}

void resetChassisPosition()
{
    LeftFrontChassisMotor.resetPosition();
    LeftBackChassisMotor.resetPosition();
    RightFrontChassisMotor.resetPosition();
    RightBackChassisMotor.resetPosition();
}

//Arm movement functions

void moveArmMotors(int leftSpeed, int rightSpeed)
{
    leftArmMotor.setVelocity(leftSpeed, pct);
    rightArmMotor.setVelocity(rightSpeed, pct);
    leftArmMotor.spin(fwd);
    rightArmMotor.spin(fwd);
}

void stopArmMotors()
{
    //Set the velocity of the motors to zero
    leftArmMotor.setVelocity(0, pct);
    rightArmMotor.setVelocity(0, pct);

    //Set the brake type when stopping
    leftArmMotor.setStopping(hold);
    rightArmMotor.setStopping(hold);

    //stop the motors
    leftArmMotor.stop();
    rightArmMotor.stop();
}

void resetArmMotors()
{
    leftArmMotor.resetPosition();
    rightArmMotor.resetPosition();
}

//Driving forward using Gyro functions

void driveUsingYaw(int targetYaw, int timeDuration, int leftSpeed, int rightSpeed, int targetDegrees)
{
    Brain.resetTimer();
    resetChassisPosition();
    int startYaw = Gyro.yaw();

    while(abs(LeftFrontChassisMotor.position(deg)) < targetDegrees &&
          abs(LeftBackChassisMotor.position(deg)) < targetDegrees &&
          abs(RightFrontChassisMotor.position(deg)) < targetDegrees &&
          abs(RightBackChassisMotor.position(deg)) < targetDegrees &&
          Brain.timer(msec) < timeDuration)
    {
        int error = Gyro.yaw() - startYaw;

        int correction = error * KP;

        int leftPower = leftSpeed - correction;
        int rightPower = rightSpeed + correction;

        moveChassisMotors(leftPower, rightPower);
    }

    stopChassisMotors();
}

//Pneumatic functions

void resetPneumatics()
{
    // Control the front claw
    armSolenoid.extend((vex::cylinderType)ClawPneumaticStates::Right);

    //Control the hinge
    armSolenoid.retract((vex::cylinderType)ClawPneumaticStates::Left);
}

//Turning using the Gyro functions

void gyroTurn(int target, int maxSpeed, int minSpeed)
{
    int startingRotations = Gyro.rotation(degrees);

    int error = target - (Gyro.rotation(degrees) - startingRotations);

    while((Gyro.rotation(degrees) - startingRotations < target - 5) ||
          (Gyro.rotation(degrees) - startingRotations > target + 5))
    {
        error = target - (Gyro.rotation(degrees) - startingRotations);
        //error is how much left you have to move

        int speed = ((error * (maxSpeed - minSpeed)) / target)+minSpeed;

        if(target > 0)
        {
            moveChassisMotors(speed , -1*speed);
        }
        else
        {
            moveChassisMotors(-1*speed, speed);
        }
    }

    stopChassisMotors();
}
void moveArm (int target, int maxSpeed, int minSpeed)
{
    int startingRotations = firstStageArmGyro.rotation(degrees);
    Brain.resetTimer();
    int time = 2000;

    int error = target - (firstStageArmGyro.rotation(degrees) - startingRotations);

    while((firstStageArmGyro.rotation(degrees) - startingRotations < target - 5) ||
          (firstStageArmGyro.rotation(degrees) - startingRotations > target + 5) && Brain.timer(msec) < time)
    {
        error = target - (firstStageArmGyro.rotation(degrees) - startingRotations);
        //error is how much left you have to move

        int speed = ((error * (maxSpeed - minSpeed)) / target)+minSpeed;

        if(target > 0)
        {
            moveArmMotors(speed , speed);
        }
        else
        {
            moveArmMotors(-1*speed, -1*speed);
        }
    }

    stopArmMotors();
}
//Music functions

void playTwinkleTwinkleLittleStar()
{
    Brain.playTone(C, 80, 300); wait(500, msec);
    Brain.playTone(C, 80, 300); wait(350, msec);
    Brain.playTone(G, 80, 300); wait(500, msec);
    Brain.playTone(G, 80, 300); wait(350, msec);
    Brain.playTone(A, 80, 300); wait(500, msec);
    Brain.playTone(A, 80, 300); wait(350, msec);
    Brain.playTone(G, 80, 300); wait(500, msec);
    Brain.playTone(F, 80, 300); wait(500, msec);
    Brain.playTone(F, 80, 300); wait(350, msec);
    Brain.playTone(E, 80, 300); wait(500, msec);
    Brain.playTone(E, 80, 300); wait(350, msec);
    Brain.playTone(D, 80, 300); wait(500, msec);
    Brain.playTone(D, 80, 300); wait(350, msec);
    Brain.playTone(C, 80, 300); wait(500, msec);
    Brain.playTone(G, 80, 300); wait(500, msec);
    Brain.playTone(G, 80, 300); wait(350, msec);
    Brain.playTone(F, 80, 300); wait(500, msec);
    Brain.playTone(F, 80, 300); wait(350, msec);
    Brain.playTone(E, 80, 300); wait(500, msec);
    Brain.playTone(E, 80, 300); wait(350, msec);
    Brain.playTone(D, 80, 300); wait(500, msec);
    Brain.playTone(G, 80, 300); wait(500, msec);
    Brain.playTone(G, 80, 300); wait(350, msec);
    Brain.playTone(F, 80, 300); wait(500, msec);
    Brain.playTone(F, 80, 300); wait(350, msec);
    Brain.playTone(E, 80, 300); wait(500, msec);
    Brain.playTone(E, 80, 300); wait(350, msec);
    Brain.playTone(D, 80, 300); wait(500, msec);
    Brain.playTone(C, 80, 300); wait(500, msec);
    Brain.playTone(C, 80, 300); wait(350, msec);
    Brain.playTone(G, 80, 300); wait(500, msec);
    Brain.playTone(G, 80, 300); wait(350, msec);
    Brain.playTone(A, 80, 300); wait(500, msec);
    Brain.playTone(A, 80, 300); wait(350, msec);
    Brain.playTone(G, 80, 300); wait(500, msec);
    Brain.playTone(F, 80, 300); wait(500, msec);
    Brain.playTone(F, 80, 300); wait(350, msec);
    Brain.playTone(E, 80, 300); wait(500, msec);
    Brain.playTone(E, 80, 300); wait(350, msec);
    Brain.playTone(D, 80, 300); wait(500, msec);
    Brain.playTone(D, 80, 300); wait(350, msec);
    Brain.playTone(C, 80, 300); wait(500, msec);
    Brain.soundOff();
}

//Triangle drives using the gyro

void triangleDriveUsingYawAi(int targetDegrees, int maxPower, int minPower)
{
    Brain.resetTimer();
    resetChassisPosition();

    int startingYaw = Gyro.yaw();
    int halfOfTargetDegrees = targetDegrees / 2;

    int lastSpeed = minPower;

    while ((RightFrontChassisMotor.position(degrees) +
            LeftFrontChassisMotor.position(degrees)) / 2 < targetDegrees)
    {
        int pos =
            (LeftBackChassisMotor.position(degrees) +
             RightBackChassisMotor.position(degrees) +
             LeftFrontChassisMotor.position(degrees) +
             RightFrontChassisMotor.position(degrees)) / 4;

        int half = halfOfTargetDegrees;

        int percent;

        if (pos < half)
            percent = (pos * 100) / half;
        else
            percent = ((targetDegrees - pos) * 100) / half;

        if (percent > 100) percent = 100;
        if (percent < 0) percent = 0;

        int rawSpeed = minPower + ((maxPower - minPower) * percent) / 100;

        int speed = (rawSpeed + lastSpeed) / 2;
        lastSpeed = speed;

        int error = Gyro.yaw() - startingYaw;
        int correction = error * KP;

        if (correction > 20) correction = 20;
        if (correction < -20) correction = -20;

        moveChassisMotors(speed - correction, speed + correction);

        wait(15, msec);
    }

    stopChassisMotors();
}

void lucasTriangleDrive(int targetDegrees, int maxPower, int minPower)
{
    resetChassisPosition();

    int halfOfTargetDegrees = targetDegrees / 2;

    int startingYaw = Gyro.yaw();

    while(LeftBackChassisMotor.position(degrees) < halfOfTargetDegrees)
    {
        double percent = (double)LeftBackChassisMotor.position(degrees) / targetDegrees;

        int forwardSpeed = minPower + (maxPower - minPower) * percent;


        int error = Gyro.yaw() - startingYaw;

        int correction = error * KP;


        int leftPower = forwardSpeed - correction;
        int rightPower = forwardSpeed + correction;

        moveChassisMotors(leftPower, rightPower);
    }
    
    while(LeftBackChassisMotor.position(degrees) < targetDegrees)// && LeftChassisMotorBack.position(degrees) > halfOfTargetDegrees)
    {
        double percent = (double)LeftBackChassisMotor.position(degrees) / targetDegrees;    

        int forwardSpeed = maxPower - (maxPower - minPower) * percent;

        /*
        //int mathForForwardSpeed = ((LeftChassisMotorBack.position(degrees) * (maxPower - minPower)) / (targetDegrees * (maxPower - minPower)));
        //int forwardSpeed = 100 - (minPower + (maxPower * mathForForwardSpeed));
        //double percentage = LeftChassisMotorBack.position(degrees) / (double)halfOfTargetDegrees;
        //int forwardSpeed = maxPower - ((maxPower - minPower) * percentage);
        //double percentage = LeftChassisMotorBack.position(degrees) / (halfOfTargetDegrees - minPower);

        //int forwardSpeed = 100 - (minPower + (maxPower * percentage));
        */
        
        int error = Gyro.yaw() - startingYaw;

        int correction = error * KP;

        int leftPower = forwardSpeed - correction;

        int rightPower = forwardSpeed + correction;

        moveChassisMotors(leftPower, rightPower);
    }

    stopChassisMotors();
}

//lucasTriangleDrive(500, 100, 20);

void driveUntilPressed()
{
    while(!backTouchSensor.pressing())
    {
        moveChassisMotors(-100, -100);
    }
    stopChassisMotors();
}

void driveUntilSight()
{
    while(OpticalSensor.color() == vex::color::yellow || OpticalSensor.color() == vex::color::black)
    {
        moveChassisMotors(75, 75);
    }
    stopChassisMotors();
}

void auton()
{
    driveUntilPressed();
    driveUsingYaw(0, 500, 40, 40, 90);
    gyroTurn(-45, 50, 10);
    driveUntilPressed();
    moveChassisMotors(-5,-10);
    moveArm(150, 100, 50);
    armSolenoid.extend((vex::cylinderType)ClawPneumaticStates::Left);
    wait(250, msec);
    armSolenoid.extend((vex::cylinderType)ClawPneumaticStates::Right);
    moveArm(-150, 100, 50);
    armSolenoid.retract((vex::cylinderType)ClawPneumaticStates::Right);
    driveUsingYaw(0, 1000, 45, 55, 75);
    driveUsingYaw(0, 1000, 80, 90, 75);
    driveUsingYaw(0, 3500, 100, 100, 900);
    //driveUntilSight();

    resetPneumatics();
    armSolenoid.retract((vex::cylinderType)ClawPneumaticStates::Right);
    driveUntilPressed();
    moveChassisMotors(-5,-10);
    moveArm(150, 100, 50);
    armSolenoid.extend((vex::cylinderType)ClawPneumaticStates::Left);
    wait(250, msec);
    armSolenoid.extend((vex::cylinderType)ClawPneumaticStates::Right);
    moveArm(-145, 100, 50);
    resetPneumatics();
    driveUsingYaw(0, 4000, 100, 95, 1200);
    //driveUntilSight();

    armSolenoid.retract((vex::cylinderType)ClawPneumaticStates::Right);
    driveUntilPressed();
    moveArm(150, 100, 50);
    armSolenoid.extend((vex::cylinderType)ClawPneumaticStates::Left);
    wait(250, msec);
    armSolenoid.extend((vex::cylinderType)ClawPneumaticStates::Right);
}

int main() 
{
    LeftFrontChassisMotor.setStopping(hold);
    LeftBackChassisMotor.setStopping(hold);
    RightFrontChassisMotor.setStopping(hold);
    RightBackChassisMotor.setStopping(hold);
    OpticalSensor.setLightPower(50, percentUnits::pct);
    OpticalSensor.setLight(vex::ledState::on);
    touchLED.on(vex::color::red);
    touchLED.setBrightness(100);
    resetPneumatics();
    while(!touchLED.pressing())
    {
        touchLED.setColor(vex::color::red);
    }
    while(touchLED.pressing())
    {
    }
    armSolenoid.retract((vex::cylinderType)ClawPneumaticStates::Right);
    while(!touchLED.pressing())
    {
        touchLED.setColor(vex::color::orange);
    }
    touchLED.setColor(vex::color::green);    
    auton();

}
