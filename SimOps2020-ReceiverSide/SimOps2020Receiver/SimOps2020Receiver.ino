/*Receiver (valve) side Arduino. Includes RadioHead NRF24L01 library, valve, and LED controls.
 * This code contains multiple methods of doing the same task, such as flashing an LED, to 
 * show more ways of controlling the Arduino.
 */

/*
 * NRF24L01 > Arduino   (Description)
 * GND    >   GND       (Ground)
 * VCC    >   3.3V      (3.3 Volts)
 * CE     >   8         (Input to control data transmission and reception)
 * CSN    >   10         (Chip Select Not - enables the SPI bus when HIGH)
 * SCK    >   13        (Serial Clock - synchronizes data transmission)
 * MOSI   >   11        (Master Out Slave In - Master line for sending data to the slave)
 * MISO   >   12        (Master In Slave Out - Slave line for sending data to the master)
 * IRQ    >   Not Used  (Interrupt request, not used)
 */


// Will use SPI to communicate between Arduino and NRF24L01
#include <SPI.h>

// Add the library for the NRF24L01
#include <RH_NRF24.h>

// Singleton instance (meaning restricted to one instance) of the radio driver
RH_NRF24 radi0;

// Designate the pin that will open our valve
const int openV = 5;
// Designate the pin for our status LED
const int statusLed = 9;
// Create our time out variable for closing the valve
int timeOut = 0;
// Create our time check variable for holding the valve open
unsigned long howLongSince = 0;
// How long do we want the valve open?
const int openTime = 30000;

void setup() {
  // Set up our pins
  pinMode(openV, OUTPUT);
  pinMode(statusLed, OUTPUT);

  Serial.begin(9600);
  if (!radi0.init())
  {
    Serial.println("init failed");
    //Show a set of flashes if init fails using the StatBlink function defined below
    StatBlink(2, 1000);
  }
  if (!radi0.setChannel(111))     //set channel to 111
  {
    Serial.println("setChannel failed");
    //Show a set of flashes if setChannel fails by manually writing the blinks (functions save time)
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
    delay(1000);
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
    delay(1000);
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
  }

  /* //Uncomment this to set the data rate and transmission power
  if (!radi0.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");*/
}

void loop() {

  /*Set up our variable for tracking time. This will be done using the millis()
   * function instead of the delay() function as seen in the StatBlink function below.
   * The reason for using millis() here is that delay() will halt all other processing
   * while it runs but millis() lets our program continue. This means we can receive
   * another wireless message while our valve is open which just increases the time the valve
   * stays open. If we used delay() to hold the valve open, any additional wireless
   * messages would be ignored.
   */
  unsigned long currentRunTime = millis();
  
  // If our wireless has data available, process that data
  if (radi0.available())
  {
    // Create our buffer as an array of type unsigned integers with a
    // size of RH_NRF24_MAX_MESSAGE_LEN (a constant defined in the NRF24L01 library
    uint8_t buffy[RH_NRF24_MAX_MESSAGE_LEN];
    // Create a variable as type unsigned integer with size of one byte and store the size of our buffer in it
    uint8_t len = sizeof(buffy);
    
    //Read the message data
    if (radi0.recv(buffy, &len))
    {
      //Some output, useful for debugging
      Serial.print("got request: ");
      StatBlink(6, 100);
      Serial.println((char*)buffy);

      //Convert the message from characters to numbers
      int sig = atoi((const char*) buffy);
      //Check if we got the right number
      if (sig == 104)
      {
        // We got the right number so now we'll do something

        //Open the valve
        digitalWrite(openV, HIGH);
        // Set our time to check against
        howLongSince = currentRunTime;
      }
    }
  }

  // We will check if our valve is open (pin set to HIGH), and if we have waited long enough
  if (bitRead(PORTD, openV) == 1 && howLongSince + openTime < currentRunTime)
  {
    // Close the valve
    digitalWrite(openV, LOW);    
  }
}

void StatBlink(int blinkCount, int blinkSpeed)
{
  while (blinkCount >= 0)
  {
    digitalWrite(statusLed, HIGH);
    delay(blinkSpeed);
    digitalWrite(statusLed, LOW);
    delay(blinkSpeed);
    blinkCount--;
  }

}
