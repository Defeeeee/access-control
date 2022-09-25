// MFRC522 - Version: Latest
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 
Servo servo1;

int mod;

int pos = 0;

MFRC522 rfid(SS_PIN, RST_PIN);

byte keyTagUID[4] = {0x04, 0x62, 0x07, 0x6A}; // 0xE7, 0xEC, 0xBD, 0xB4
byte keyTagUID1[4] = {0xE7, 0xEC, 0xBD, 0xB4};

void setup()
{
  lcd.init(); 
  Serial.begin(9600);
  SPI.begin();     // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  servo1.attach(8);
  lcd.backlight();
  servo1.write(90);
  mod = 1;
}

void loop()
{
  switch (mod)
  {
  case 1:
    printRfid();
    break;
  case 2:
    buscarr();
    break;
  case 3:
    checkValid();
    break;
  case 4:
    accesoPermitido();
    break;
  case 5:
    accesoDenegado();
    break;
  default:
    break;
  }
}

void printRfid()
{
  lcd.clear();
  lcd.print("Esperando RFID..");
  mod = 2;
}

void buscarr()
{
  if (rfid.PICC_IsNewCardPresent())
  {
    mod = 3;
  }
  else
  {
    mod = 2;
  }
}

void checkValid()
{
  if (rfid.PICC_ReadCardSerial()) // NUID has been readed
  {
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    if (rfid.uid.uidByte[0] == keyTagUID[0] &&
            rfid.uid.uidByte[1] == keyTagUID[1] &&
            rfid.uid.uidByte[2] == keyTagUID[2] &&
            rfid.uid.uidByte[3] == keyTagUID[3] ||
        rfid.uid.uidByte[0] == keyTagUID1[0] &&
            rfid.uid.uidByte[1] == keyTagUID1[1] &&
            rfid.uid.uidByte[2] == keyTagUID1[2] &&
            rfid.uid.uidByte[3] == keyTagUID1[3])
    {
      mod = 4;
    }
    else
    {
      mod = 5;
    }
  }
}

void accesoPermitido()
{
  Serial.println("Access is granted");
  lcd.clear();
  lcd.print("Acceso Permitido");
  servo1.write(180);
  delay(3000);
  while (rfid.PICC_IsNewCardPresent())
  {
    delay(1000);
  }
  if (!rfid.PICC_IsNewCardPresent())
  {
    servo1.write(90);
    lcd.clear();
    mod = 1;
  }
}

void accesoDenegado()
{
  Serial.println("Access denied");
  lcd.clear();
  lcd.print("Acceso Deneagdo");
  servo1.write(100);
  delay(500);
  servo1.write(90);
  delay(1000);
  lcd.clear();
  mod = 1;
}