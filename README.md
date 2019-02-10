# Local Weather Station


/*************************************************** 

  LOCAL WEATEHR STATION:::
  
  This is an example for the BMP180 Barometric Pressure & Temp Sensor and DHT11 temp and Humidity sensor
  
  BMP180 useS I2C to communicate, 2 pins are required to  
  interface
  // Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
  // Connect GND to Ground
  // connect i2c bus with SDA=D2 and SCL=D1 of NodeMCU 

  // Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
  // Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
  // EOC is not used, it signifies an end of conversion
  // XCLR is a reset pin, also not used here

  DHT11 uses normal gpio pins to interface with NodeMCU
  // Connect VCC or + of the DHT11 sensor to 3.3V (NOT 5.0V!)
  // Connect GND or - of the DHT11 to Ground
  // connect out of DHT11 to any gpio of nodeMCU (here to  d4 of nodemcu)

  #returns    
	Humidity in percentage.
	Temperature in degree celsius.  
	Heat index in celsius.  
	pressure in pascal.  
	real altitude in meters.   
  
 ****************************************************/