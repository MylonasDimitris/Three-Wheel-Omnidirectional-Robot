<h1 align="center">Three-Wheel-Omnidirectional-Robot</h1>


<p align="center">
    <br />
    Controlling a three wheel omnidirectional robot using an arduino UNO
</p>

<div align="center">
    <img src="images/image_1" alt="Description" width="300">
    <img src="images/image_2" alt="Description" width="300">
</div>


The onboard arduino was finally changed with an arduino MEGA due to the lack of interrupt pins on the UNO. A problem occured during development cause the pins 20 and 21 are bot the main I2C pins and interrupt pins (which are required for the encoders). Before the use of enableInterrupts library and using the hardware interrupt pins on the MEGA, measurements of the distance using encoders had a +-4cm per 30cm travelled accuracy, after that the accuracy dropped to +-15cm per 30cm travelled. The use of a better board like an esp32 would fix the issue.
