# EnergyMonitor
The power consumption of our house is made available to a dashboard so I can keep an eye on its power usage and how much money we're spending.  It works by monitoring the LED flashes from the consumer unit and transmits to a listening server (Raspberry Pi).  The sensor is just a light sensitive resistor connected to an Arduino mini pro with a transmitter.


Stuff...

Both arduino and pi use the same github repo -  https://github.com/TMRh20/RF24.git

Arduino
-------
- Take a look at WiringDiagram.jpg in root of project for how I initially set the arduino hardware up
- Publish the file /GettingStarted/GettingStarted.ino to <home>/Arduino/projects/GettingStarted

If not already done so:
- Download the zip from  https://github.com/TMRh20/RF24.git and import it into the Arduino ide

- This helped a lot, all about getting NRF2401 going on Arduino: https://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo

Raspberry pi
------------
- Basic workflow that just works is:
-- logon to pi
-- cd to repos/RF24/examples_linux (pull down if required - could fork from original to start all over again - https://github.com/TMRh20/RF24.git)
-- then simply 'make' to create 'gettingstarted'
- to run at start-up on Raspberry pi, add this line to /etc/rc.local:
    sudo /home/pi/repos/RF24/examples_linux/gettingstarted &

- Compile: sudo docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp gcc gcc -o gettingstarted examples_linux/gettingstarted.cpp
- Push to git hub and pull down 

Just in case it is useful, the start of this project is derived from this:
- Use the pin diagram here: http://tmrh20.github.io/RF24/
- Clone the repo:  https://github.com/TMRh20/RF24.git

-------------
A lot of things I've forgotten here - will fill in blanks!  Finishes with a cmake in the examples_linux folder I think

- I think this was important: http://tmrh20.github.io/RF24/RPi.html

- enable spi http://www.raspberrypi-spy.co.uk/2014/08/enabling-the-spi-interface-on-the-raspberry-pi/
- sudo apt-get update
- sudo apt-get upgrade
- sudo nano /boot/config.txt
- dtparam=spi=on
- sudo reboot
- lsmod | grep spi_ ... should see spi_bcm2708 or spi_bcm2835 - 

- install cmake to allow compilation, this might help https://www.raspberrypi.org/forums/viewtopic.php?f=66&t=62344
- probably need to do:
  - sudo apt-get update && sudo apt-get upgrade
  - sudo apt-get install cmake
-------------

- cd examples_linux/
- make
- sudo ./gettingstarted
At the command prompt choose either 0: pong_back (ie listening) or 1: ping_out

