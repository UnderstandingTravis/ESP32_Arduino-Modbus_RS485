//Travis Eddy a.k.a Travis Proctor
//See https://github.com/UnderstandingTravis/ESP32_Arduino-Modbus_RS485 for License and Copywrite infomaion. 

// This is a test and "notes" program to get an ESP32 to talk to a PZEM-016 
//via RS485 and ModBus. Using the 2nd serial port on the ESP32 connected to
//an RS485 level converter (drok 3.3v model from amazon I think)
//Hopefully eventually this will get worked into a libary or added to another
//project at some point. As I made this because I couldn't find anything to 
//make the ESP32 talk to modbus devices.

//quality disclaimer: I am not a programmer, I just a guy who gets things done
//and makes things happen. So it works, but it may not be as elegant as i would like.


//maybe i should put variables up here?
//unsigned long amps ;

void setup()
{
  // use Serial 1 (Serial1, yes thats the number one) ; Modbus baud rate
  Serial1.begin(9600); //RS485 (level converter used), UART, baud rate 9600, 8 data bits, 1 stop bit, no parity

  Serial.begin(115200); //normal serial terminal for data viewing pleasure
  Serial.println("Serial Terminal is working");
}


void loop()
{
    Serial.printf("\n manually sending request for info :");
    
    uint8_t buf1[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x70, 0x0D}; //8 bytes?
    //TODO: make CRC stuff work. 16bit CRC 
    
    Serial1.write(buf1,8); // send the bytes from above out to the RS485 - Modbus
    Serial.printf(" Message sent \n");
    
    //Read in from the serial input buffer... one byte at a time :-/   
    uint8_t buf2[63]; //probably to large
    //looks like it sends 25 bytes max? should probably check this
    for (int j = 0; j < 63; j++)
    {
      buf2[j] = Serial1.read();
      //TODO: make CRC stuff work. 16bit CRC 
    }
    //wipe out read buffer becauae its not clearing out?? 
    //maybe my buffer isn't clearing out ??? TODO

    //serial.peek useful here? 
    
  //now lets find the voltage... should be the 

  //first byte is slave address, 2nd byte is what is comming out (0x04 = read input registers)
  //below maybe tells us how much data is comming out?
  //3rd byte (if your counting from zero) is number of bytes sent? TODO check this
  //3rd byte is register addr high byte, 4th is that but low byte.
  //this is the data
  //5th is number of registers high byte, 6th is that but low byte..
  
  //4th and 5th look like voltage?

  //TODO: make CRC stuff work. 16bit CRC 
  
  uint16_t voltage =((uint8_t)buf2[3] <<  8) | buf2[4];
  uint8_t voltageD = voltage % 10;
  voltage = voltage / 10;
  Serial.printf("Voltage : %d.%d ", voltage, voltageD);
  //the order is low 16 bits first, AND THEN the high 16 bits... so 7,8,5,6
  uint32_t amps = ((uint8_t)buf2[7] << 24) | ((uint8_t)buf2[8] << 16) | ((uint8_t)buf2[5] <<  8) | buf2[6];
  uint16_t ampsD = amps % 1000;
  amps = amps / 1000;
  Serial.printf(" Amps : %d.%d ",amps, ampsD); 
  //now energy
  uint32_t watts = ((uint8_t)buf2[11] << 24) | ((uint8_t)buf2[12] << 16) | ((uint8_t)buf2[9] <<  8) | buf2[10];
  uint8_t wattsD = watts % 10;
  watts = watts / 10;
  Serial.printf(" Watts : %d ", watts); 
  uint32_t Wh = ((uint8_t)buf2[15] << 24) | ((uint8_t)buf2[16] << 16) | ((uint8_t)buf2[13] <<  8) | buf2[14];
  Serial.printf(" Wh : %d ", Wh); 
  //insert witty comment here... also specs stuff
  uint16_t Hz = ((uint8_t)buf2[17] <<  8) | buf2[18];
  uint8_t HzD = Hz % 10;
  Hz = Hz / 10;
  Serial.printf(" Hz : %d.%d ", Hz, HzD);
  uint16_t Pf = ((uint8_t)buf2[19] <<  8) | buf2[20];
  uint8_t PfD = Pf % 100;
  Pf = Pf /100;
  Serial.printf(" Power Factor : %d.%d ", Pf, PfD);

   //alarm codes?

   delay(1000);
  
}  //end of main void LOOP
