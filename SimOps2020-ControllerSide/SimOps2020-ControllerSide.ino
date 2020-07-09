//Controller (button) side Arduino. Includes RadioHead NRF24L01 library, button, and LED controls.

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

// Set which pin # we will read for our button press
const int button = 2;

// Set which pin # we will use for indicator LED
const int led = 3;


void setup() {

  pinMode(led, OUTPUT);
  pinMode(10, INPUT);
  
  // Set up our wireless connection and check each step for failure
  Serial.begin(9600);
  if (!radi0.init())
  {
    Serial.println("init fail");
    //Show a set of flashes if init fails
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
  }
    
  // If init works, default channel, data rate, and transmit power are set
  // Defaults: 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!radi0.setChannel(111))
  {
    Serial.println("SetChannel fail");
    //Show a set of flashes if setChannel fails
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
  }
  /*if (!radi0.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("serRF fail");*/

  Serial.println("Success");
}

void loop() {
  // Watch for the button to be pressed, then send a wireless signal to the Receiver
  if (digitalRead(button) == HIGH)
  {
    uint8_t data[] = "104";
    radi0.send(data, sizeof(data));
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
  }
  //Wait here for a moment and then start again
  delay(60);

}
