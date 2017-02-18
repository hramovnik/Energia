/*
Pin Connections
===================================      
RFID Module       MSP430 LaunchPads        
--------------    -----------------
Pin 1  (SDA)      Pin 8  (CS)
Pin 2  (SCK)      Pin 7  (SCK)
Pin 3  (MOSI)     Pin 15 (MOSI)
Pin 4  (MISO)     Pin 14 (MISO)
Pin 5  (IRQ)      Not connected
Pin 6  (GND)      GND
Pin 7  (RST)      Pin 10
Pin 8  (3V3)      3V3

Addicore has a very good introduction to this module, written for Arduino.
Try the site below for additional detail on the module and examples
which include writing to a card, dumping detailed information, changing
the card user ID, etc.  It will run on LaunchPads or Arduinos with the
correct pin connections:  https://github.com/miguelbalboa/rfid
*/

#include <Mfrc522.h>
#include <SPI.h>

int CS = 8;                                 // chip select pin
int NRSTDP = 5;
Mfrc522 mfrc522(CS,NRSTDP);
unsigned char serNum[5];
unsigned char blueTabletOne[] = {230,151,123,161,171};
unsigned char blueTabletTwo[] = {38,49,127,161,201};

bool isValid(unsigned char * exemplarOne, unsigned char * exemplarTwo, int size){
  for(int i = 0; i < size; i++){
    if (exemplarOne[i] != exemplarTwo[i]) {return false;}
  }
  return true;
}

void setup() 
{             
  Serial.begin(9600);                        
  Serial.println("Starting RFID-RC522 MIFARE module demonstration...\n");

  SPI.begin();
  digitalWrite(CS, LOW);                    // Initialize the card reader
  pinMode(RED_LED, OUTPUT);                 // Blink LED if card detected
  mfrc522.Init();  
}

void loop()
{
  unsigned char status;
  unsigned char str[MAX_LEN];
    
  status = mfrc522.Request(PICC_REQIDL, str);
  if (status == MI_OK)
  {
    Serial.print("Card detected: ");
    Serial.print(str[0],BIN);
    Serial.print(" , ");
    Serial.print(str[1],BIN);
    Serial.println("");
  }

  status = mfrc522.Anticoll(str);
  memcpy(serNum, str, 5);
  if (status == MI_OK)
  {
    digitalWrite(RED_LED, HIGH);              // Card or tag detected!
    Serial.print("The card's number is: ");
    for(int i = 0; i < 5; i++){
      Serial.print(serNum[i]);
      Serial.print(" ");
    }
    Serial.println("");

    if (isValid(serNum, blueTabletOne, 5) || isValid(serNum, blueTabletTwo, 5)){
      Serial.println("Valid key");
      for(int i = 0; i < 10; i++){
        delay(50);
        digitalWrite(RED_LED, LOW);
        delay(50);
        digitalWrite(RED_LED, HIGH);
      }
    }else{
      delay(1000);
    }
    digitalWrite(RED_LED, LOW);
  }
  mfrc522.Halt();                         
}


