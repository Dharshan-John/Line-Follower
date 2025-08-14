This is my line follower robot project — a small but fast bot that can follow a black track on a white surface all by itself.
It uses Arduino Nano for the brain, QTR-8A sensors for “eyes,” and a PID control system to keep it smooth and steady.

I built it to learn more about robotics, sensors, and control systems, and also because… well… watching it zoom around the track is just plain fun.

🔧 What’s Inside

Arduino Nano – the robot’s brain

QTR-8 A reflectance sensor array – detects the track

TB6612 motor driver – Tells the motors how fast to spin

N20 12V 600 RPM motors – Moves the robot forward

44mm wheels + caster wheel – Lets it roll freely

12V LiPo battery – Gives it the power to run

A simple chassis (perforated board) to hold it all together

# Basic Mechanism of the Line Follower Robot

The sensors look at the ground and figure out where the black line is.

Arduino figures out how far off the robot is from the center of the line.

The PID control system decides how much to speed up or slow down each motor to correct the error.

The robot makes tiny adjustments and stays glued to the track.

Upload the code from Line-Follower/Pathfinder.ino using Arduino IDE.

Calibrate the sensors so it knows what’s “black” and what’s “white.”

If it’s zig-zagging too much → lower P or increase D.
If it’s slow to correct → increase P.
If it always drifts off in the same direction → add some I.
