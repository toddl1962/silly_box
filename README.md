# Silly Box

This is a "useless box" project with some added personality!  It is based on
the Arduino useless box project found at:

   [Useless Box with Arduino](https://create.arduino.cc/projecthub/viorelracoviteanu/useless-box-with-arduino-d67b47)

However, I added light, sound, and an ultrasonic sensor for proximity detection.  I also
created new software in C++ and "table-ized" all of the light, sound, and movement
sequences in order to make them easily modifiable and executable in parallel.

In it's current state the software still fits in an Arduino Uno or Nano but the concession
is that most of the tables are placed in PROGMEM.

# LICENSE

"Silly Box" is free software: you can redistribute it and/or modify it under the terms 
of the GNU General Public License as published by the Free Software Foundation, 
version 3 of the License.

"Silly Box" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with "Silly Box" 
in a file named gpl-3.0.txt.  If not, see [https://www.gnu.org/licenses/](https://www.gnu.org/licenses/).

