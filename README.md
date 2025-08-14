This is my line follower robot project — a small but fast bot that can follow a black track on a white surface all by itself.
It uses Arduino Nano for the brain, QTR-8A sensors for “eyes,” and a PID control system to keep it smooth and steady.

I built it to learn more about robotics, sensors, and control systems.

# What’s Inside

Arduino Nano – the robot’s brain

QTR-8 A reflectance sensor array – detects the track

TB6612 motor driver – Tells the motors how fast to spin

N20 12V 600 RPM motors – Moves the robot forward

44mm wheels + caster wheel – Lets it roll freely

12V LiPo battery – Gives it the power to run

A simple chassis (perforated board) to hold it all together

# Basic Mechanism of the Line Follower Robot

Calibrating the sensors so it knows what’s “black” and what’s “white.”

The QTR sensor tells you the reflectance value of the black and white line where black has low reflectance value and white has high reflectance value. Arduino uses the reflectance values given by the sensor to evaluate how far off is the bot from the center line.

The PID control system decides how much to speed up or slow down each motor to correct the positional error of the bot from the center line.

The robot makes tiny adjustments due to the corressponding changes in Kp,Kc and Kd values in the PID controller which helps it to stay on course and make proper turns.

Upload the code from Line-Follower/Pathfinder.ino using Arduino IDE.

If it’s zig-zagging too much → lower Kp or increase Kd.

If it’s slow to correct → increase Kp.

If it always drifts off in the same direction → add some Ki.

