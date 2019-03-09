#include <usbhub.h>

#include <ptp.h>
#include <canoneos.h>

#include <Wire.h> 
#include <LCD.h> 
#include <LiquidCrystal_I2C.h> 

#define SHUTTER_SPEED_BULB      0x0c
#define SHUTTER_SPEED_30S       0x10 //   30"
#define SHUTTER_SPEED_25S       0x13 //   25"
#define SHUTTER_SPEED_20S       0x14 //   20"
//#define SHUTTER_SPEED_15S       0x15:   20"
#define SHUTTER_SPEED_15S       0x18 //   15"
#define SHUTTER_SPEED_13S       0x1B //   13"
#define SHUTTER_SPEED_10S       0x1C //  10"
//#define SHUTTER_SPEED_10S       0x1D:   10"
#define SHUTTER_SPEED_8S        0x20 //   8"
#define SHUTTER_SPEED_6S        0x23 //   6"
//#define SHUTTER_SPEED_6S        0x24 //   6"
#define SHUTTER_SPEED_5S        0x25 //   5"
#define SHUTTER_SPEED_4S        0x28 //   4"
#define SHUTTER_SPEED_3_2S      0x2B //   3"2
#define SHUTTER_SPEED_3S        0x2C //   3"
#define SHUTTER_SPEED_2_5S      0x2D //   2"5
#define SHUTTER_SPEED_2S        0x30 //   2"
#define SHUTTER_SPEED_1_6S      0x33 //   1"6
#define SHUTTER_SPEED_1_5S      0x34 //   1"5
#define SHUTTER_SPEED_1_3S      0x35 //   1"3
#define SHUTTER_SPEED_1S        0x38 //   1"
#define SHUTTER_SPEED_0_8S      0x3B //   0"8
#define SHUTTER_SPEED_0_7S      0x3C //   0"7
#define SHUTTER_SPEED_0_6S      0x3D //   0"6
#define SHUTTER_SPEED_0_5S      0x40 //   0"5
#define SHUTTER_SPEED_0_4S      0x43 //   0"4
#define SHUTTER_SPEED_0_3S      0x44 //   0"3
//#define SHUTTER_SPEED_0_3S 0x45:   0"3
#define SHUTTER_SPEED_1_4THS    0x48 //   1/4
#define SHUTTER_SPEED_1_5THS    0x4B //   1/5
#define SHUTTER_SPEED_1_6THS    0x4C //   1/6
//#define SHUTTER_SPEED_1_6THS    0x4D //   1/6
#define SHUTTER_SPEED_1_8THS    0x50 //   1/8
#define SHUTTER_SPEED_1_10THS   0x53 //   1/10
//#define SHUTTER_SPEED_1_10THS   0x54:   1/10
#define SHUTTER_SPEED_1_13TH5S  0x55 //   1/13
#define SHUTTER_SPEED_1_15THS   0x58 //   1/15
#define SHUTTER_SPEED_1_20THS   0x5B //   1/20
//#define SHUTTER_SPEED_1_20THS   0x5C //   1/20
#define SHUTTER_SPEED_1_25THS   0x5D  //   1/25
#define SHUTTER_SPEED_1_30THS   0x60 //   1/30
#define SHUTTER_SPEED_1_40THS   0x63 //   1/40
#define SHUTTER_SPEED_1_45THS   0x64 //   1/45
#define SHUTTER_SPEED_1_50THS   0x65 //   1/50
#define SHUTTER_SPEED_1_60THS   0x68 //   1/60
#define SHUTTER_SPEED_1_80THS   0x6B //   1/80
#define SHUTTER_SPEED_1_90THS   0x6C //   1/90
#define SHUTTER_SPEED_1_100THS  0x6D //   1/100
#define SHUTTER_SPEED_1_125THS  0x70 //   1/125
#define SHUTTER_SPEED_1_160THS  0x73 //   1/160
#define SHUTTER_SPEED_1_180THS  0x74 //   1/180
#define SHUTTER_SPEED_1_200THS  0x75 //   1/200
#define SHUTTER_SPEED_1_250THS  0x78 //   1/250
#define SHUTTER_SPEED_1_320THS  0x7B //   1/320
#define SHUTTER_SPEED_1_350THS  0x7C //   1/350
#define SHUTTER_SPEED_1_400THS  0x7D //   1/400
#define SHUTTER_SPEED_1_500THS  0x80 //   1/500
#define SHUTTER_SPEED_1_640THS  0x83 //   1/640
#define SHUTTER_SPEED_1_750THS  0x84 //   1/750
#define SHUTTER_SPEED_1_800THS  0x85 //   1/800
#define SHUTTER_SPEED_1_1000THS 0x88 //   1/1000
#define SHUTTER_SPEED_1_1250THS 0x8B //   1/1250
#define SHUTTER_SPEED_1_1500THS 0x8C //   1/1500
#define SHUTTER_SPEED_1_1600THS 0x8D //   1/1600
#define SHUTTER_SPEED_1_2000THS 0x90 //   1/2000
#define SHUTTER_SPEED_1_2500THS 0x93 //   1/2500
#define SHUTTER_SPEED_1_3000THS 0x94 //   1/3000
#define SHUTTER_SPEED_1_3200THS 0x95 //   1/3200
#define SHUTTER_SPEED_1_4000THS 0x98 //   1/4000
#define SHUTTER_SPEED_1_5000THS 0x9B //   1/5000
#define SHUTTER_SPEED_1_6000THS 0x9C //   1/6000
#define SHUTTER_SPEED_1_6400THS 0x9D //   1/6400
#define SHUTTER_SPEED_1_8000THS 0xA0 //   1/8000

#define ISO_AUTO                 0x00
#define ISO_100                  0x48
#define ISO_200                  0x50
#define ISO_400                  0x58
#define ISO_800                  0x60
#define ISO_1600                 0x68
#define ISO_3200                 0x70
#define ISO_6400                 0x78
#define ISO_12800                0x80

#define MAX_SS 10

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7);

int iInterval = 2; // 2 sec per photo
int iNumProminence = 6;
int iNumCoronaInner = 6;
int iNumCoronaOutter = 6;

int iShutterProminence = 2; // 1/500 s
int iShutterCoronaInner = 100; // 1/10 s
int iShutterCoronaOutter = 4; // 4 s

struct PhotoParm
{
  String strDispText; // "Part 1", "Part 2"...
  String strShutterText; // "1/1000s", "1/10s"...
  int iEosShutterSpeed;  // SHUTTER_SPEED_1_1000THS,...
  int iShutterTime;      // in milli-seconds
  int iInterval;         // intervals between each photo, seconds, better to leave 2 sec between each
  int iNumPhotos;  
};

PhotoParm SolarParm[] = {
                         {"Prominence",     "1/1000s", SHUTTER_SPEED_1_1000THS, 1,    1, 15},  //日珥        15s
                         {"Corona-Inner1",  "1/250s",  SHUTTER_SPEED_1_250THS,  4,    1, 10},  //日冕（内）  10s + 2
                         {"Corona-Inner2",  "1/125s",  SHUTTER_SPEED_1_125THS,  8,    1, 10},  //日冕（内    10s + 2
                         {"Corona-Outter1", "1/2s",    SHUTTER_SPEED_0_5S,      500,  1, 10},  //日冕（外）  15s + 2
                         {"Moon",           "4s",      SHUTTER_SPEED_4S,        4000, 2, 6},   //月面        36s + 2
                         {"Corona-Outter2", "1s",      SHUTTER_SPEED_1S,        1000, 1, 10},  //日冕（外）  20s + 2
                         {"Prominence",     "1/1000s", SHUTTER_SPEED_1_1000THS, 1,    1, 20}   //日珥        20s
};

bool ShutterStarted = false;
bool PhotoFinished  = false;
bool ButtonReleased = true;

int StartButtonPin = 2;
int ShutterPin = 3;


String Part1Text = "Part 1";
String Part1ShutterText = "1/1000s";


class CamStateHandlers : public EOSStateHandlers
{
      enum CamStates { stInitial, stDisconnected, stConnected };
      CamStates stateConnected;

public:
      CamStateHandlers() : stateConnected(stInitial) {};

      virtual void OnDeviceDisconnectedState(PTP *ptp);
      virtual void OnDeviceInitializedState(PTP *ptp);
} CamStates;

USB                 Usb;
USBHub              Hub1(&Usb);
CanonEOS            Eos(&Usb, &CamStates);

long iCount = 0;

void DetectStartButton()
{  
  int i = 0;
  i = digitalRead(StartButtonPin);
  Serial.print("StartButton: ");
  Serial.println(i);  
  
  if (i == HIGH)
  {
    ShutterStarted = true;
    if (ButtonReleased)
    {
      PhotoFinished = false;
      ButtonReleased = false;
    }
  }
  else
  {
    ShutterStarted = false;
    ButtonReleased = true;
    PhotoFinished = true;
  }   
}

void ShutterTime (int iDelay)
{
  int i = 0;
  int iNumSec = 0;

  if (iDelay < 1000) // within 1 seconds
  {
    delay(iDelay);
  }
  else
  {
    iNumSec = (int) (iDelay/1000 + 0.5f);

    while (ShutterStarted && i<iNumSec) // countdown for seconds
      {
        lcd.setCursor (14,1);
        lcd.print(iNumSec - i);
        delay(1000);
        i++;
        DetectStartButton();
      }
      lcd.setCursor (14,1);
      lcd.print("  ");
  }
}

//void PhotoMode (CanonEOS *thisEos, String strDispText, int iMode, int iShutterSpeed, int iDelay, String strShutterText, int iRepeat)
void PhotoMode (CanonEOS *thisEos, PhotoParm parm)
{
  int i;
  int j;
  uint16_t rc = 0;

  DetectStartButton();

  rc = thisEos->SetProperty(EOS_DPC_ShutterSpeed, parm.iEosShutterSpeed);
  if (rc != PTP_RC_OK)
  {
     ErrorMessage<uint16_t>("Error", rc);
     //lcd.setCursor (0,0);
     //lcd.print(rc);
  }
  delay(200);
  
  i=0;
  while(ShutterStarted && i<parm.iNumPhotos)
  {
    lcd.setCursor (0,0);  // go to start of 1st line

    lcd.print("                ");
    lcd.setCursor (0,0);
    lcd.print(parm.strDispText);
    //lcd.print(parm.iEosShutterSpeed, HEX);
    //lcd.setCursor (4,0);
    //lcd.print(rc);

    j=0;
    while (ShutterStarted && j< parm.iInterval && i>0) //wait for the interval, no need for the first picture
    {
      lcd.setCursor (14,0);  // print the interval count down
      lcd.print(parm.iInterval - j);
      delay(1000);
      j++;
      DetectStartButton();
    } // while for interval
    lcd.setCursor (14,0);
    lcd.print("  ");

    lcd.setCursor (0,1);  // go to start of 2nd line
    lcd.print(i+1);
    lcd.print(" of ");
    lcd.print(parm.iNumPhotos);

    lcd.print(" ");
    lcd.print(parm.strShutterText);

    // control the shutter LED here
    digitalWrite(ShutterPin, HIGH);

    thisEos->Capture();

    ShutterTime(parm.iShutterTime);

    // release the shutter here
    digitalWrite(ShutterPin, LOW);
      
    DetectStartButton();

    i++;
  }

  lcd.setCursor (0,0);  // go to start of 1st line
  lcd.print("Ready           ");
  lcd.setCursor (0,1);  // go to start of 1st line
  lcd.print("                ");
}

void CamStateHandlers::OnDeviceDisconnectedState(PTP *ptp)
{
    if (stateConnected == stConnected || stateConnected == stInitial)
    {
        stateConnected = stDisconnected;
        E_Notify(PSTR("Camera disconnected\r\n"),0x80);  

        lcd.setCursor (0,0);
        lcd.print("EOS disconnected");
    }
}

void CamStateHandlers::OnDeviceInitializedState(PTP *ptp)
{
    if (stateConnected == stDisconnected || stateConnected == stInitial)
    {
        //int16_t rc = 0;
        stateConnected = stConnected;
        E_Notify(PSTR("Camera connected\r\n"),0x80);

        lcd.setCursor (0,0);
        lcd.print("EOS connected   ");

        //int16_t rc = ((CanonEOS*)ptp)->SetProperty(EOS_DPC_Iso,ISO_100); // try to set ISO=100
        //rc = ((CanonEOS*)ptp)->SetProperty(EOS_DPC_ShutterSpeed,SHUTTER_SPEED_1_1000THS);
    }
    else
    {
        DetectStartButton();

        if (ShutterStarted && (!PhotoFinished))
        {
            PhotoFinished = false;
            int i = 0;
    
            //PhotoMode (1, iShutterProminence, iNumProminence);
            //PhotoMode (2, iShutterCoronaInner, iNumCoronaInner);
            //PhotoMode (3, iShutterCoronaOutter, iNumCoronaOutter);
            
            //PhotoMode (CanonEos *thisEos, String *strDispText, int iMode, int iShutterSpeed, int iDelay, String *strShutterText, int iRepeat)
            //PhotoMode (&Eos, Part1Text, 1, SHUTTER_SPEED_1_1000THS, iShutterProminence, Part1ShutterText, iNumProminence);

            

            //((CanonEOS*)ptp)->SetProperty(EOS_DPC_ShutterSpeed,SolarParm[0].iEosShutterSpeed);

            for (int i=0; i<7; i++)
            {
                PhotoMode (&Eos,SolarParm[i]);
                delay(2000);
            }

            // if we can hit here, all photos have been taken
            //ShutterStarted = false;
            PhotoFinished = true;
        } //if (ShutterStarted)
    }
}


        
void setup()
{
  // put your setup code here, to run once:
    Serial.begin( 115200 );
    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    lcd.begin (16,2); // for 16 x 2 LCD module 
    lcd.setBacklightPin(3,POSITIVE);
    lcd.setBacklight(HIGH);

    lcd.setCursor (0,0);  // go to start of 1st line
    lcd.print("Ready           ");
    lcd.setCursor (0,1);  // go to start of 1st line
    lcd.print("                ");

    pinMode(StartButtonPin, INPUT);
    pinMode(ShutterPin, OUTPUT);
    digitalWrite(ShutterPin, LOW);
  
    delay( 200 );
}

void loop()
{
  // put your main code here, to run repeatedly:
  Usb.Task();  
}



