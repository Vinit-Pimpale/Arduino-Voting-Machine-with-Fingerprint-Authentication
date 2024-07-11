#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <EEPROM.h>

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
uint8_t getFingerprintEnroll(int id);
uint8_t id;
SoftwareSerial mySerial(7, 6);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define enroll A0
#define del A1
#define up A2
#define down A3
#define match A4

#define sw1 3
#define sw2 4
#define sw3 5
#define resultsw 2

#define indVote 0
#define indFinger 1
#define buzzer A5

int vote1, vote2, vote3;
int flag;
int bin;

#define records 25

byte p20[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};
byte p40[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};
byte p60[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};
byte p80[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};
byte p100[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

void setup() {
  delay(1000);
  pinMode(enroll, INPUT_PULLUP);
  pinMode(del, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(match, INPUT_PULLUP);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(resultsw, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(indVote, OUTPUT);
  pinMode(indFinger, OUTPUT);


  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Voting Machine");
  lcd.createChar(0, p20);
  lcd.createChar(1, p40);
  lcd.createChar(2, p60);
  lcd.createChar(3, p80);
  lcd.createChar(4, p100);
  lcd.print("                ");
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      lcd.setCursor(i, 1);
      lcd.write(j);
      delay(100);
    }
  }
  lcd.clear();
  finger.begin(57600);

  if (EEPROM.read(0) == 0xff)
    EEPROM.write(0, 0);
  if (EEPROM.read(1) == 0xff)
    EEPROM.write(1, 0);

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Finding Module");
  if (finger.verifyPassword())
  {
    lcd.clear();
    lcd.print("Found");
  }
  else {
    lcd.clear();
    lcd.print("Not Found");
  }

if(digitalRead(resultsw) ==1)
  {
       for(int i=0;i<records;i++)
          EEPROM.write(i+10,0xff);
       EEPROM.write(0,0);
       EEPROM.write(1,0);
       EEPROM.write(2,0);
       lcd.clear();
       lcd.print("System Reset");
       delay(1000);
  }



}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Match Key");
  lcd.setCursor(0, 1);
  lcd.print("to start system");


  digitalWrite(indVote, LOW);
  digitalWrite(indFinger, LOW);

 

  if (digitalRead(match) == 0)
  {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    digitalWrite(indFinger, HIGH);

    for (int i = 0; i < 3; i++)
    {
      lcd.clear();
      lcd.print("Place Finger");
      delay(2000);
      int result = getFingerprintIDez();
      if (result >= 0)
      {
        flag = 0;
        for (int j = 0; j < records; j++)
        {
          if (result == EEPROM.read(j + 10))
          {
            lcd.clear();
            lcd.print("Authorized Voter");
            lcd.setCursor(0, 1);
            lcd.print("Please Wait");
            delay(1000);
            Vote();
            EEPROM.write(j + 10, 0xff);
            flag = 1;
            return;
          }
        }

        if (flag == 0)
        {
          lcd.clear();
          lcd.print("Already Voted");
          digitalWrite(buzzer, HIGH);
          delay(5000);
          digitalWrite(buzzer, LOW);
          return;
        }
      }
    }
    lcd.clear();
  }
  checkKeys();
  delay(1000);


  if (digitalRead(resultsw) == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cn1");
    lcd.setCursor(4, 0);
    lcd.print("Cn2");
    lcd.setCursor(8, 0);
    lcd.print("Cn3");

    lcd.setCursor(0, 1);
    vote1 = EEPROM.read(0);
    lcd.print(vote1);
    lcd.setCursor(4, 1);
    vote2 = EEPROM.read(1);
    lcd.print(vote2);
    lcd.setCursor(8, 1);
    vote3 = EEPROM.read(2);
    lcd.print(vote3);
    delay(2000);

    if (vote1>0||vote2>0||vote3>0)
{ 
  if (vote1>=vote2)
  {
    if (vote1==vote2)
    {
      if (vote1==vote3){bin = 7;}
      else if (vote3>vote1) {bin= 3;}
      else {bin = 4;}
    }
    else 
    {
      if (vote1>vote3){bin=1;}
      else if (vote1==vote3){bin=6;}
      else {bin=3;}
      }
    }

//------------------------------------------------

  if (vote2>=vote3)
  {
    if (vote2==vote3)
    {
      if (vote1==vote2){bin = 7;}
      else if (vote1>vote2) {bin= 1;}
      else {bin = 5;}
    }
    else 
    {
      if (vote2>vote1){bin=2;}
      else if (vote2==vote1){bin=4;}
      else {bin=1;}
      }
    }
//--------------------------------------------

  if (vote3>=vote1)
  {
    if (vote3==vote1)
    {
      if (vote3==vote2){bin = 7;}
      else if (vote2>vote3) {bin= 2;}
      else {bin = 6;}
    }
    else 
    {
      if (vote3>vote2){bin=3;}
      else if (vote3==vote2){bin=2;}
      else {bin=2;}
      }
    }
}

else {bin = 8;}


      switch(bin)
      {
        case 1: lcd.clear();
                lcd.print("Can 1 Wins");
                break;
        case 2: lcd.clear();
                lcd.print("Can 2 Wins");
                break;
        case 3: lcd.clear();
                lcd.print("Can 3 Wins");
                break;
        case 4: lcd.clear();
                lcd.print("Can1 Can2 Equal");
                break;
        case 5: lcd.clear();
                lcd.print("Can2 Can3 Equal");
                break;
        case 6: lcd.clear();
                lcd.print("Can1 Can3 Equal");
                break;        
        case 7: lcd.clear();
                lcd.print("Equal Votes");
                break; 
        default: lcd.clear();
                 lcd.print("No Voting");
                 break;
        
        }  
       
      delay(3000);
  }
}

void checkKeys() {
  if (digitalRead(enroll) == 0)
  {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while (digitalRead(enroll) == 0)
      Enroll();
  }

  else if (digitalRead(del) == 0)
  {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    delet();
  }
}

void Enroll() {
  int count = 0;
  lcd.clear();
  lcd.print("Enter Finger ID:");

  while (1)
  {
    lcd.setCursor(0, 1);
    lcd.print(count);
    if (digitalRead(up) == 0)
    {
      count++;
      if (count > 25)
        count = 0;
      delay(500);
    }

    else if (digitalRead(down) == 0)
    {
      count--;
      if (count < 0)
        count = 25;
      delay(500);
    }
    else if (digitalRead(del) == 0)
    {
      int id;
      id = count;
      getFingerprintEnroll(id);
      for (int i = 0; i < records; i++)
      {
        if (EEPROM.read(i + 10) == 0xff)
        {
          EEPROM.write(i + 10, id);
          break;
        }
      }
      return;
    }

    else if (digitalRead(enroll) == 0)
    {
      return;
    }
  }
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();

  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.print("Finger Not Found");
    lcd.setCursor(0, 1);
    lcd.print("Try Later");
    delay(2000);
    return -1;
  }
  return finger.fingerID;
}

void Vote() {
  lcd.clear();
  lcd.print("Please Place");
  lcd.setCursor(0, 1);
  lcd.print("Your Vote");
  digitalWrite(indVote, HIGH);
  digitalWrite(indFinger, LOW);
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(1000);
  while (1)
  {
    if (digitalRead(sw1) == 0)
    {
      vote1++;
      voteSubmit(1);
      EEPROM.write(0, vote1);
      while (digitalRead(sw1) == 0);
      return;
    }
    if (digitalRead(sw2) == 0)
    {
      vote2++;
      voteSubmit(2);
      EEPROM.write(1, vote2);
      while (digitalRead(sw2) == 0);
      return;
    }
    if (digitalRead(sw3) == 0)
    {
      vote3++;
      voteSubmit(3);
      EEPROM.write(2, vote3);
      while (digitalRead(sw3) == 0);
      return;
    }

    if (digitalRead(resultsw) == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Can1");
      lcd.setCursor(6, 0);
      lcd.print("Can2");
      lcd.setCursor(12, 0);
      lcd.print("Can3");
      for (int i = 0; i < 3; i++)
      {
        lcd.setCursor(i * 6, 1);
        lcd.print(EEPROM.read(i));
      }
      delay(2000);
      int vote = vote1 + vote2 + vote3;
      if (vote)
      {
        if ((vote1 > vote2 && vote1 > vote3))
        {
          lcd.clear();
          lcd.print("Can1 Wins");
          delay(2000);
          lcd.clear();
        }
        else if (vote2 > vote1 && vote2 > vote3)
        {
          lcd.clear();
          lcd.print("Can2 Wins");
          delay(2000);
          lcd.clear();
        }
        else if ((vote3 > vote1 && vote3 > vote2))
        {
          lcd.clear();
          lcd.print("Can3 Wins");
          delay(2000);
          lcd.clear();
        }

        else
        {
          lcd.clear();
          lcd.print("   Tie Up Or   ");
          lcd.setCursor(0, 1);
          lcd.print("   No Result   ");
          delay(1000);
          lcd.clear();
        }

      }
      else
      {
        lcd.clear();
        lcd.print("No Voting....");
        delay(1000);
        lcd.clear();
      }
      vote1 = 0; vote2 = 0; vote3 = 0; vote = 0;
      lcd.clear();
      return;
    }
  }
  digitalWrite(indVote, LOW);
}

void voteSubmit(int cn) {
  lcd.clear();
  if (cn == 1)
    lcd.print("Can1");
  else if (cn == 2)
    lcd.print("Can2");
  else if (cn == 3)
    lcd.print("Can3");
  lcd.setCursor(0, 1);
  lcd.print("Vote Submitted");
  digitalWrite(buzzer , HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  digitalWrite(indVote, LOW);
  return;
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK)
  {
    //Serial.println("Deleted!");
    lcd.clear();
    lcd.print("Figer Deleted");
    lcd.setCursor(0, 1);
    lcd.print("Successfully");
    delay(1000);
  }

  else
  {
    //Serial.print("Something Wrong");
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0, 1);
    lcd.print("Try Again Later");
    delay(2000);
    return p;
  }
}

void delet() {
  int count = 0;
  lcd.clear();
  lcd.print("Enter Finger ID");

  while (1)
  {
    lcd.setCursor(0, 1);
    lcd.print(count);
    if (digitalRead(up) == 0)
    {
      count++;
      if (count > 25)
        count = 0;
      delay(500);
    }

    else if (digitalRead(down) == 0)
    {
      count--;
      if (count < 0)
        count = 25;
      delay(500);
    }
    else if (digitalRead(del) == 0)
    {
      id = count;
      deleteFingerprint(id);
      for (int i = 0; i < records; i++)
      {
        if (EEPROM.read(i + 10) == id)
        {
          EEPROM.write(i + 10, 0xff);
          break;
        }
      }
      return;
    }
    else if (digitalRead(enroll) == 0)
    {
      return;
    }
  }
}

uint8_t getFingerprintEnroll(int id) {
  uint8_t p = -1;
  lcd.clear();
  lcd.print("Finger ID:");
  lcd.print(id);
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.print("Image Taken");
        break;
      case FINGERPRINT_NOFINGER:
        lcd.clear();
        lcd.print("No Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        lcd.print("Comm Error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.print("Imaging Error");
        break;
      default:
        lcd.clear();
        lcd.print("Unknown Error");
        break;
    }
  }


  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image Converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Comm Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    default:
      lcd.clear();
      lcd.print("Unknown Error");
      return p;
  }

  lcd.clear();
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  lcd.clear();
  lcd.print("Place Finger");
  lcd.setCursor(0, 1);
  lcd.print("   Again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        break;
      case FINGERPRINT_IMAGEFAIL:
        break;
      default:
        break;
    }
  }

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:;
      return p;
    default:
      return p;
  }

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    return p;
  } else {
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Stored");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    return p;
  } else {
    return p;
  }
}
