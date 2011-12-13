/*----------------------------------------------------------------------*
 * Remote sensor for XBee sensor network                                *
 *                                                                      *
 * Sends sensor data once a minute to the base station.  The sensor     *
 * in this example is just a CdS photocell to sense brightness.         *
 * The base station forwards the data to Pachube.                       *
 *                                                                      *
 * Jack Christensen                                                     *
 * V1.0 28Apr2011                                                       *
 *                                                                      *
 * Hardware:                                                            *
 * (1) Arduino Uno or similar                                           *
 * (1) XBee ZB low power Zigbee Module, Digi Product ID XB24-Z7CIT-004, *
 *     XB24-Z7WIT-004, XB24-Z7SIT-004, or XB24-Z7UIT-004.               *
 * (1) Adafruit Industries XBee Adapter, ID 126                         *
 * (1) CdS photocell                                                    *
 * (1) 10K resistor                                                     *
 *                                                                      *
 * XBee configuration:                                                  *
 * 1. Load the Zigbee Router API firmware (I used Version 2370).        *
 * 2. Set the PAN ID as desired, to match that of the base unit.        *
 * 3. Set API mode 2 (AP=2).                                            *
 * 4. Set baud rate to 9600.                                            *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/

