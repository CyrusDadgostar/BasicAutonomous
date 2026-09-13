The Ducks Autonomous, by Ishaan Subin Gopinathan

This is the autonomous project for VIQRC for our robot. This is for the team "The Ducks", with the members being
Cyrus Dadgostar,
Milo Hua,
Lucas Jacky,
John Barsoumian,
and Ishaan Subin Gopinathan. 
We will try to reach and win VIQRC Worlds with the best of our ability. This is the purpose of this code. I doubt anyone will read this code, as it is very specific to us, but if you are interested, let me tell you in detail how this works.

1.

The first part is how the VEX Application Programming Interface works. I will not go into too much detail, so I suggest you check this link: https://api.vex.com/iq2/home/cpp/index.html

2.

The second part is the first code that we used. Currently, we are reusing code from the last season, as that was a crude effort to educate myself on how to use the API. It consists of proportional turning, driving, and arm movement. This will not be used in the future.

3.

The third part is the new code that is supposed to be an improvement over the old one, as it has more efficient and accurate turning, driving, and arm movement. However, it was programmed using the ROBOTC IDE, so some parts need to be edited for a successful autonomous function.
In this program, the system that is being used is called Command Struct. Simply put, Command Struct is a way to write simpler programs using a more complex system. The code is non-blocking, so different functions may run at the same time. 
The programs written in the system can also have special "recovery commands", which are a type of command that brings the robot from a different path back to the correct path. 

Think of the simple program like a story, and the main protagonist is the robot. 
If the protagonist veers off of their designated path due to external means, the recovery commands are there to help bring it back. Sometimes, the recovery commands activate; sometimes, they do not. In a perfect world, they should never activate, but as we all know, there are many external factors in the "real world".
Finally, the most important part of this entire setup is the addition of a special way of moving a motor. It is known as trapezoidal-profile motion. Our mentor considers this a must-have for this competition. 

![Alt text](https://mechatronics.studio/content/images/2024/02/Trapezoidal-Velocity-2.png)

As you can see, it uses a three-phase system starting with acceleration, staying at a constant rate, and decelerating to the destination. 
Once this is tuned properly with the correct acceleration and deceleration distances for this robot, it will be the most accurate way of movement that is physically and also logistically possible.
There are also commands for following a line, moving the end effector, and the recovery command versions of those commands.

4.

The fourth part is the system that we are using. We are using a different type of system than what the Command Struct program had, so most of the work now is to edit and correct the program to work with this robot. There is also the fact that more and more edits will be made to this specific Command Struct program, as it is not finished yet.
Some improvements that are being planned are individual motor stall checks, stall recovery, and the implementation of PID. PID stands for Proportional-Integral-Derivative, and the essence of this is just to make sure that everything is controlled. This controller could be used for any process that requires strict maintenance and care.
The VEX system also has more and more accurate sensors that will be very useful as an aid for this program, some of which are the gyro and inertial sensors, the optical sensor, the laser distance sensor, and the AI Vision Sensor. Hopefully, none of these are copyrighted. It would be a shame.

5.

The fifth part is our goal. The current plan is to score at least 100 points by scoring nine level 3s. With the old code, we were able to make at least one L3, but that all changed with a different robot and different power values, which was the reason that the Command Struct program was created.
Our team's strategist is John Barsoumian, and he is familiar with what is needed and expected of this competition. I cannot describe the steps in detail, as we may have a different robot in the future, one that uses a catapult. This is an image of the VEX pyramid goal. The small ledge at the top is level 3.

![Alt text](https://th.bing.com/th/id/OIP.XdEWvTHdBU_HTRDB37EfRgAAAA?w=207&h=183&c=7&r=0&o=7&dpr=1.5&pid=1.7&rm=3)

Finally...

...thank you for taking the time to read this, whoever you are. This is not a real README file, as this would be terrible in the eyes of other developers. I can already hear their crying from where I sit. So I ask you, do not take this file seriously. It was there to record for all of us on this team what is required,
especially for something like this.
