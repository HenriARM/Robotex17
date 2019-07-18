#include <EEPROM.h>

//NEW
#define BUT_1 A7
#define BUT_2 A6

#define SDA A5
#define SCL A4

#define LED_DATA
#define LED_SCK
#define LED_RCK



//---------------
#define GSL A2  //G. sensors LEFT 
#define GSR A1  //G. sensors RIGHT
#define GSB A0  //Gridas sensors BACK


#define DIP1 D2
#define DIP2 D3
#define DIP3 D4
#define DIP4 D5

#define ML_PWM D9   //MOTOR LEFT (PWM) ML1
#define ML_DIR D8   //direction ML2
#define MR_PWM D10  //MOTOR RIGHT (PWM) MR1
#define MR_DIR D11  //direction MR2

//R-READ
#define button  !digitalRead(BUT) //nolasa kad pirma poga ir ieslegta

#define DIP1R digitalRead(DIP1) //nolasa kad  DIP ir ieslegts
#define DIP2R digitalRead(DIP2)
#define DIP3R digitalRead(DIP3)
#define DIP4R digitalRead(DIP4)

#define GSLR analogRead(GSL)<minn-10  //jesli vidit belij cvet to vivodit 1
#define GSRR analogRead(GSR)<minn-10
#define GSBR analogRead(GSB)<minn-10

//#define sensors     ( (SHARPCR || SHARPRR || SHARPLR || SHARPSLR || SHARPSRR) || (GSLR || GSRR || GSBR) ) //visi sensori
//#define ssensors    (SHARPCR || SHARPRR || SHARPLR || SHARPSLR || SHARPSRR) //sharp sensori
//#define gsensors    (GSLR || GSRR || GSBR) //gridas sensori
//#define gsensorsand (GSLR && GSRR && GSBR) //gridas sensori
//#define fsensors    (SHARPCR || SHARPRR || SHARPLR)  //forward sensori
//#define fsensorsand (SHARPCR && SHARPRR && SHARPLR)  //forward sensori

int minn;

void setup()
{
  pinMode(BUT, INPUT);

  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED13, OUTPUT);

  pinMode(DIP1, INPUT);
  pinMode(DIP2, INPUT);
  pinMode(DIP3, INPUT);
  pinMode(DIP4, INPUT);

  pinMode(GSL, INPUT);
  pinMode(GSR, INPUT);
  pinMode(GSB, INPUT);

  pinMode(SHARPL, INPUT);
  pinMode(SHARPR, INPUT);
  pinMode(SHARPC, INPUT);
  pinMode(SHARPSL, INPUT);
  pinMode(SHARPSR, INPUT);

  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);

  digitalWrite(BUT, HIGH); //pull-up rezistors

  digitalWrite(DIP1, HIGH); //pull-up rezistors
  digitalWrite(DIP2, HIGH); //pull-up rezistors
  digitalWrite(DIP3, HIGH); //pull-up rezistors
  digitalWrite(DIP4, HIGH); //pull-up rezistors

  digitalWrite(GSL, HIGH); //pull-up rezistors
  digitalWrite(GSR, HIGH); //pull-up rezistors
  digitalWrite(GSB, HIGH); //pull-up rezistors

  minn = 1023;
  while (!button)
  {
    digitalWrite(LEDL, HIGH);
    digitalWrite(LEDR, HIGH);

    if (DIP3R)
    {
      int xx = analogRead(GSL);
      if (xx < minn)
      {
        minn = xx;
      }
      if ( (minn < 1023) && (minn > 0) )
      {
        digitalWrite(LEDB, HIGH);
      }
    }
  }

  delay(5000); // 5 sekund

  if (DIP3R)
  {
    EEPROM.put(0, minn);
  }
  if (!DIP3R)
  {
    EEPROM.get(0, minn);
  }

  digitalWrite(LEDB, LOW);
  digitalWrite(LEDL, LOW);
  digitalWrite(LEDR, LOW);

  minn = 100;

  int sstate = 0; //strategijas stavoklis

  if (DIP1R) {
    sstate = sstate + 1;
  }
  if (DIP2R) {
    sstate = sstate + 2;
  }

  switch (sstate) { //nosakam ko darit katra stavokli

    case 0 :// (medlenno jedet vpered)
      while (!sensors)
      {
        Forward(80, 80); //pereproveritj
      }
      break;

    case 1 : //DIP1(bistro jedet vpered)
      while (!sensors)
      {
        Forward(140, 140); //pereproveritj
      }
      break;


    case 2: { // DIP2 (krutitsja odin raz i jedjet vpered chutj-chutj)
        int counteroid = 0;
        while ( (!sensors) && (counteroid < 4 ) ) //3 reizes darisim to
        {
          for ( int x = 100; (x > 0) && (!sensors); x = x - 50 ) //malenkije shazhki vpered
          {
            Forward(x, x);
            delay(120);
          }
          unsigned long turn = millis();
          while ( (!sensors) && ((turn + 750) > millis()) ) //odin povorot
          {
            Right(80, 80);
          }
          counteroid += 1;
        }
      } break;

    case 3 : { //DIP2 and DIP1 (malenkimi shazhkami vpered)
        int counterx = 1;
        while ( (!sensors) && (counterx < 4 ) )
        {
          for (int x = 105; (x >= 0) && (!sensors) ; x = x - 35)
          {
            Forward(x, x);
            delay(80);
          }
          int sg = 1;
          while ( (!sensors) && (sg == 1) )
          {
            delay(180);
            sg += 1;
          }
          counterx += 1;
        }
      } break;


  } //switch sstate

  Stop();

} //void setup


//motora funkcijas
void Forward (int leftSpeed, int rightSpeed) {
  analogWrite(ML1, leftSpeed);
  digitalWrite(ML2, LOW);
  analogWrite(MR1, rightSpeed);
  digitalWrite(MR2, LOW);
}
void Backward (int leftSpeed, int rightSpeed) {
  analogWrite(ML1, leftSpeed);
  digitalWrite(ML2, HIGH);
  analogWrite(MR1, rightSpeed);
  digitalWrite(MR2, HIGH);
}
void Left (int leftSpeed, int rightSpeed) {
  analogWrite(ML1, leftSpeed);
  digitalWrite(ML2, HIGH);
  analogWrite(MR1, rightSpeed);
  digitalWrite(MR2, LOW);
}
void Right (int leftSpeed, int rightSpeed) {
  analogWrite(ML1, leftSpeed);
  digitalWrite(ML2, LOW);
  analogWrite(MR1, rightSpeed);
  digitalWrite(MR2, HIGH);
}
void Stop ()
{
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, LOW);
}


void loop()
{
  int mstate = 0; // motora stāvoklis

  if (GSLR) {
    mstate = mstate + 1;
  }
  if (GSRR) {
    mstate = mstate + 2;
  }
  if (GSBR) {
    mstate = mstate + 4;
  }
  if (SHARPLR) {
    mstate = mstate + 8;
  }
  if (SHARPRR) {
    mstate = mstate + 16;
  }
  if (SHARPCR) {
    mstate = mstate + 32;
  }
  if (SHARPSLR) {
    mstate = mstate + 64;
  }
  if (SHARPSRR) {
    mstate = mstate + 128;
  }


  switch (mstate) { //nosakam ko darit katra stavokli

    case 0 ://neviens nav nostradajis
      while (!sensors)
      {
        Forward(120, 120);
      }
      break;


    case 1 :  // gridas sensors left
      while ((!ssensors) && (GSLR))
      {
        Backward(180, 240); //pereproveritj
      }
      while ( (!sensors) && (!gsensors) )
      {
        Right(100, 100);
      }
      break;


    case 2 :  // gridas sensors right
      while ((!ssensors) && (GSRR))
      {
        Backward(180, 240); //pereproveritj
      }
      while ( (!sensors) && (!gsensors) )
      {
        Left(100, 100);
      }
      break;



    case 3 : { //gridas sensori left and right
        while ( (GSLR) && (GSRR) && (!ssensors) )
        {
          Backward(240, 240); //pereproveritj
        }
        unsigned long ggqagqg = millis();
        while ( (!ssensors) && ((millis() - ggqagqg) < 230) ) //poloborota sdelatj
        {
          Left(100, 100);
        }
      } break;



    case 4:  // gridas back sensors
      while ( (!ssensors) && (GSBR) )
      {
        Forward(240, 240);
      }
      break;


    case 5  :   //gridas sensors back and left
      while ((GSBR) && (GSLR))
      {
        Forward(240, 240);
      }
      break;


    case 6 : //gridas sensors back and right
      while ((GSBR) && (GSRR))
      {
        Forward(240, 240);
      }
      break;

    case 37 :  //gridas sensors back and left + sharp center sensors
    case 61 : //gridas sensors back and left + sharp center,right,left sensori
    case 133 : //gridas sensors back and left + sharp side sensors right
      while ((GSBR) && (GSLR) && (fsensorsand) )
      {
        Forward(240, 240);
      }
      break;

    case  38 : //gridas sensors back and right + sharp center sensors
    case  62 : //gridas sensors back and right + sharp center,right,left sensori
    case  70: //gridas sensors back and right + sharp side sensors left
      while ((GSBR) && (GSRR) && (fsensorsand))
      {
        Forward(240, 240);
      }
      break;



    case 19 : //gridas sensors left and right + sharp right
    case 11 : //gridas sensors left and right + sharp left
    case 27 : //gridas sensor  left and right + levij,pravij sharp sensor
    case 35 : //tozhe samoje + centralnij sharp
    case 43 : //tozhe samoje + centralnij,levij sharp sensor
    case 51 : //tozhe samoje + centralnij,pravij sensor
    case 59 : //tozhe samoje + vse perednije sharpi
    case 187 : //sharp side right + right,left,center sharp sensori + gridas sensors right,left
    case 123 : //sharp side left  + right,left,center sharp sensori + gridas sensors right,left
      while ( (fsensors) && (GSLR) && (GSRR) )
      {
        Backward(240, 240);
      }
      break;


    case 20 : //sharp right + gridas sensors back
      while ( (!SHARPCR) && (GSBR) )
      {
        Forward(200, 240);
      }
      break;


    case 12 : //sharp left + gridas sensors back
      while ( (!SHARPCR) && (GSBR) )

      {
        Forward(240, 200);
      }
      break;



    case 129 : //sharp side right + gridas sensors left
      while ( (!fsensorsand) && (SHARPSRR) && (!GSBR))
      {
        Right(40, 160);
      }
      break;



    case 66 : //sharp side left + gridas sensors right
      while ( (!fsensorsand) && (SHARPSLR) && (!GSBR))
      {
        Left(160, 40);
      }
      break;

    case 9 : //sharp left + gridas sensors left
    case 25 : //sharp left,right + gridas sensors left
    case 41 : //sharp center,left + gridas sensors left
    case 57 : //sharp center,right,left + gridas sensors left
      while ( (fsensors) && (GSLR) && (!GSRR) && (!GSBR) )
      {
        Forward(240, 160);
      }
      break;


    case 18 : //sharp right + gridas sensors right
    case 26 : //sharp left,right + gridas sensors right
    case 49 : //sharp center,right + gridas sensors right
    case 58 : //sharp center,right,left + gridas sensors right
      while ( (fsensors) && (GSRR) && (!GSBR) && (!GSLR) )
      {
        Forward(160, 240 );
      }
      break;


    case 185 : //sharp side right + right,left,center sharp sensori + gridas sensors left
    case 121 : //sharp side left  + right,left,center sharp sensori + gridas sensors left
    case 17  : //sharp sensors right + gridas sensors left
      while ( (fsensors) && (!GSRR) )
      {
        Forward(60, 160); //pereproveritj
      }
      break;



    case 10  : //sharp sensors left + gridas sensors right
    case 186 : //sharp side right + right,left,center sharp sensori + gridas sensors right
    case 122 : //sharp side left  + right,left,center sharp sensori + gridas sensors right
      while ( (fsensors) && (!GSLR) )
      {
        Forward(160, 60); //pereproveritj
      }
      break;



    case 44 : // gridas sensors back + centralnij,levij sharp sensor
    case 52 : //tozhe samoje  + centralnij,pravij sensor
    case 28 : //tozhe samoje + levij,pravij sharp sensor
    case 36 : //tozhe samoje + centralnij sharp
    case 60 : //tozhe samoje + vse perednije sharpi
    case 188 : //sharp side right + right,left,center sharp sensori + gridas sensors back
    case 124 : //sharp side left  + right,left,center sharp sensori + gridas sensors back
      while ( (ssensors) && (GSBR) )
      {
        Forward(240, 240);
      }
      break;


    case 8 :  //left sharp sensor
      while ((!SHARPCR) && (!gsensors))
      {
        Right(70, 70);
      }
      break;


    case 16 :  //right sharp sensor
      while ((!SHARPCR) && (!gsensors))
      {
        Left(70, 70);
      }
      break;


    case 24 : //left,right sharp sensor
    case 32 : //center sharp sensor
    case 56 : //left,right,center sharp sensor
      if (DIP4R)
      {
        while ((!SHARPSLR) && (!gsensors))
        {
          Forward(250, 110);
        }

        while ( (!fsensorsand) && (!gsensors) )
        {
          Left (60, 60);
        }
      }

      while ( (SHARPCR) && (!gsensors) )
      {
        Forward(240, 240);
      }


    case 40 : //left,center sharp sensors
      while ( (fsensors) && (!gsensors) && (!SHARPRR))
      {
        Forward(240, 160);
      }
      break;


    case 48 : // right,left  sharp sensors
      while ((fsensors) && (!gsensors) && (!SHARPLR) )
      {
        Forward(160, 240);
      }
      break;


    case 64 : { //sharp side left  sensor
        unsigned long tiwwi = millis();
        while ((!fsensors) && (!gsensors) && ( (millis() - tiwwi ) < 1000))
        {
          Left(60, 60);
        }
        while ( (!gsensors) && (!fsensors) )
        {
          Backward(100, 20);
        }
      } break;


    case 128 : { //sharp side right  sensor
        unsigned long tiwwi2 = millis();
        while ((!fsensors) && (!gsensors) && ( (millis() - tiwwi2 ) < 1000))
        {
          Right(60, 60);
        }
        while ( (!gsensors) && (!fsensors) )
        {
          Backward(20, 100);
        }
      } break;


    case 120 :  //sharp side left + right,left,center sharp sensori
    case 80  :  //sharp side left + right sharp sensori
    case 112 :  //sharp side left + right,center sharp sensori
    case 72  :  //sharp side left + left sharp sensori
    case 104 : {//sharp side left + left,center sharp sensori
        unsigned long evev2 = millis();
        while ( (!gsensors) && (SHARPSLR) && (fsensors) && ((millis() - evev2) < 1000) )
        {
          Forward(200, 200);
        }
        while ( (!gsensors) && (fsensors) && (SHARPCR) )
        {
          Backward(120, 40);
        }
      } break;


    case 184 : //sharp side right + right,left,center sharp sensori
    case 144 : //sharp side right + right sharp sensori
    case 176 : //sharp side right + right,center sharp sensori
    case 136 : //sharp side right + left sharp sensori
    case 168 : { //sharp side right + left,center sharp sensori
        unsigned long evev = millis();
        while ( (!gsensors) && (SHARPSRR) && (fsensors) && ((millis() - evev) < 1000) )
        {
          Forward(200, 200);
        }
        while ( (!gsensors) && (fsensors) && (SHARPCR) )
        {
          Backward(40, 120);
        }
      } break;



    case 248 : //visi sharp sensori
      while ( (ssensors)  && (!gsensors) )
      {
        Forward(240, 240);
      }
      break;



  } //switch mstate

  Stop();
  //
  //  if (button)
  //  {
  //    while (1)
  //    {
  //      Stop();
  //    }
  //  }

} //void loop
