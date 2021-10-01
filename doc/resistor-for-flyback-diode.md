## Role of resistor in a flyback diode configuration

Use of a proper reistor in series with flyback diode gives two useful benefits:

* Reduce [discharge time](http://hades.mech.northwestern.edu/index.php/RC_and_RL_Exponential_Responses#RL_Circuits) of electromagnet (i.e. inductor).

  *Fig.1* Discharge time when **no resistor** is used (nearly **15 ms**)
  
  <img src="/doc/electromagnet-discharge-initial.png" width="500" />
  
  *Fig.2* Discharge time when a **330 ohm** resistor is used (nearly **2.5 ms**)
  
  <img src="/doc/electromagnet-discharge-R330.png" width="500" />
  
  (*time-response graphs are produced by fantastic [Arduino Oscilloscope](https://www.instructables.com/Another-Arduino-Oscilloscope/) )*

* Reduce heat produced by electromagnet.
  
  During discharge phase energy accumulated by inductor is dissipated mainly by the 330 ohm resistor. This results in a cooler electromagnet durign execution.
