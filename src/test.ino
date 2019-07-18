#include <SPI.h>

#define buttonKR 21
#define buttonKR2 20
#define buttonL2 12
#define VBAT_PIN     A0 // PC0
#define dist_pwm 3
#define batt A0 //pilns-976.5, zem 814-tukss

#define LED_DATA_PIN          11 // PB5
#define LED_SCK_PIN           13 // PB3

#define BUTTON1_PIN           // PB7 (PCINT7)
#define BUTTON2_PIN           // PB6 (PCINT6)
#define BUTTON3_PIN           12 // PB4 (PCINT4)
 
#define GSL              A4
#define GSR              A2 
#define GSB              A1 

#define SHARPSL          8 //SHARP sensori 
#define SHARPL           2 
#define SHARPC           4
#define SHARPSR          7
#define SHARPR           A3

#define PIEZO_PIN        A5
#define OP_SENS_LED_PIN  3 // PD3 (OC2B)

#define MR1 5 //motors
#define MR2 6
#define ML1 9
#define ML2 10

#define SHARPLR  !digitalRead(SHARPL) // nolasa SHARP sensorus
#define SHARPRR  !digitalRead(SHARPR)
#define SHARPCR  !digitalRead(SHARPC)
#define SHARPSLR !digitalRead(SHARPSL)
#define SHARPSRR !digitalRead(SHARPSR)

void Buttons_Init(void) {
  DDRB &= ~((1<<PINB6) | (1<<PINB7)); // Input
  PORTB |= ((1<<PINB6) | (1<<PINB7)); // Pullup 
  PCICR |= (1<<PCIE0);                // Enable Pin Change Interrupt
  PCMSK0 |= (1<<PCINT7) | (1<<PCINT6);
}
void OpSensor_Init(void) {
  pinMode(OP_SENS_LED_PIN, OUTPUT);
  TCCR2A = (1<<COM2B0) | (1<<WGM21);  // CTC, toggle 0C2B on Compare Match
  TCCR2B = (1<<CS20);                 // No prescaler
  OCR2A = 105;                        // Compare A register value
}
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




void setup() {
  Serial.begin(9600);
  SPI.begin();
  Buttons_Init();
  OpSensor_Init();
  
  pinMode(13, OUTPUT);  // SCK
  pinMode(11, OUTPUT);  // MOSI

  pinMode(PIEZO_PIN, OUTPUT);
  digitalWrite(PIEZO_PIN, 0);

  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
   
   /*tone(OP_SENS_LED_PIN, 38000);

    for(byte i=0;i<2;i++) {
    digitalWrite(PIEZO_PIN, 1);
    delay(50);
    digitalWrite(PIEZO_PIN, 0);
    delay(50);
  } */

  pinMode(SHARPL, INPUT);
  pinMode(SHARPR, INPUT);
  pinMode(SHARPC, INPUT);
  pinMode(SHARPSL, INPUT);
  pinMode(SHARPSR, INPUT);

}//void setup

/*
int x = 0;
byte test_drive = 1;
byte op_sen[5];
*/

void loop() {

/*op_sen[0] =SHARPCR; 
  op_sen[1] =SHARPLR; 
  op_sen[2] =SHARPRR;
  op_sen[3] =SHARPSRR;
  op_sen[4] =SHARPSLR;

  x = (op_sen[0]<<1) | (op_sen[1]<<2) | (op_sen[2]<<3) | (op_sen[2]<<4) | (op_sen[3]<<5) | (op_sen[4]<<6);
  SPI.transfer(x);  

 /* if(test_drive) {
    test_drive = 0;
    delay(500);
    analogWrite(MOTOR_A1_PIN, 255);
    analogWrite(MOTOR_B2_PIN, 255);
    delay(1000);
    analogWrite(MOTOR_A1_PIN, 0);
    analogWrite(MOTOR_B2_PIN, 0);
    delay(100);
  } */
 Serial.println(SHARPSRR);
 }//void loop

/*
ISR (PCINT0_vect)
{
  static volatile uint8_t portb_history;
  uint8_t changed_bits = PINB ^ portb_history;
  portb_history = PINB;
  if(changed_bits & (1<<PINB6) && !(PINB & (1<<PINB6))) {
    Serial.println("Button 1");
    test_drive = 1;
  }
  if(changed_bits & (1<<PINB7) && !(PINB & (1<<PINB7))) {
    Serial.println("Button 2");
  } 
}
*/
