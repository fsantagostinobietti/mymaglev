# MyMagLev
Yet another magnetic levitation project. This is my implementation!

<img src="/doc/demo-image.png" width="300" height="300"/><img src="/doc/hall-sensor-position.png" height="300" />

<i>LEVITATIO NON DISPUTANDUM EST!</i>

As you can see the main difference between mine and similar projects is the position of hall sensor. 

Usually it is placed just below electromagnet  with its sensing face pointing down.
It would be the best solution, but it **didn't work** for me!
Magnetic field generated by electromagnet was detected by hall sensor, so that it was not able to measure levitating magnet position correctly.
For this reason I finally came with the idea to place sensor under levitating magnet (with sensing face pointing up). This way hall sensor 
is free from electromagnet interference.

## Demo
https://user-images.githubusercontent.com/6057026/135050719-c478f0a1-0ded-451e-b2f9-01925771c97f.mp4

## Circuit schema
<img src="/doc/circuit.png" width="800" />

## Bill of materials
* Electromagnet ZYE1-P50/27 (DC=12V, R=22ohm, L=30mH (?))
* Neodymium magnets N52 (12x3 mm)
* Hall sensor 49E (TO-92 package)
* Arduino Uno R3 (compatible clone)
* Transistor NPN PN2222A (TO-92 package)
* Diode 1N4007
* Resistors - 5k ohm, 330 ohm 
* Power supply 12V DC

## Control algorithm
To obtain levitation even for a small permanent magnet a [closed control loop](https://en.wikipedia.org/wiki/Control_theory#Open-loop_and_closed-loop_(feedback)_control) is needed.

Simplest algoritm is a switch on/off near the equilibrium position. Sometimes it is called [the easy way]()
I was not able to implement this control in my project, since it was too instable.

I had to implement a more sofisticated [PID](https://en.wikipedia.org/wiki/Control_theory#PID_feedback_control) control algorithm.

*TODO*

## Next steps
* Improve stability
* Understand why hall sensor placed just below electromagnet doesn't work for me
* Understand why simple "switch on/switch off" control algorithm is not enough to maintain stable levitation in my project
