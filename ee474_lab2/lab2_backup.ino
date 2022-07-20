//James Jhong 1939553
#define DDL2 1 << 2 //47
#define DDL1 1 << 1 //48
#define DDL0 1 << 0 //49
#define DDH3 1 << 3 //OCA4 output

//#define PORTH3 1 << 3
#define PORTL2 1 << 2
#define PORTL1 1 << 1
#define PORTL0 1 << 0

#define LED_PIN_A 47 
#define LED_PIN_B 48
#define LED_PIN_C 49
#define d_val 333

#define TIMER4_ON_BIT PRTIM4; //Writing logic one to this shuts Timer/Counter 4 module. WHen enabled, operates like before shutdown
#define TIMER5_ON_BIT PRTIM5;
#define TIMER3_ON_BIT PRTIM3;

//These values calculated using the formula in the datasheet
int ORCA_VAL; //400 HZ
//#define ORCA_VAL 124 //250 Hz 
//#define ORCA_VAL 31250 silence
//#define ORCA_VAL 38.5625 //400 HZ

void setup() {
// 1.1 Code
// pinMode(LED_PIN_A, OUTPUT);
// pinMode(LED_PIN_B, OUTPUT);
// pinMode(LED_PIN_C, OUTPUT);
  
  //For 1.4
  //Setting these pins here as outputs !
  DDRL |= DDL2; //activiating PA6, pin 47
  DDRL |= DDL0; //activating PA4, pin 49
  DDRL |= DDL1; //activating PA5, pin 48
  
   //Setting Pin 6 (timer pin) as an output
  DDRH |= 1 << PORTH3;


  //Setting up Timer 4 (Part 2)
  noInterrupts();
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4 = 0;
  
  OCR4A = 0;//Compare match register, look up... //change these values to change the frequency.
  TCNT4 = 0;
  TCCR4A |= (1 << COM4A0); //Toggling on compare match
  TCCR4B |= (1 << WGM42); //CTC MODE
  TCCR4B |= (1 << CS40); //Prescalar 1
  
}

//int orc_calc(int freq) {
//  int orc_val;
//  orc_val = (16000000 / 2 * freq) - 1 ;
//  return orc_val; 
//}

void orc_calc(int freq) {
  OCR4A = (16000000 / 2 * freq) - 1 ;
}

void loop() {
  //Code for Part 1.1 (Running LEDs on pins 47 - 49 in sequential order for 0.333 seconds each)
  //digWriteLEDs();
  //registerLEDs();
//  timersPart2();
  timersPart2();
}

//This task manipulates the timers to output square wave
void timersPart2() {
  //Part 2.3
  //I want to generate square wave on OC4A --> ABSTRACT PORT:PH3, HW PIN: 6, ADP: 7  (labeled as pin 6 on the board)
  //CTC mode
  //What scaler I want
  //modulus
  static int timer;
  timer++;
  if (timer % 4 == 0 ){
    orc_calc(400);
  } else { //0
    orc_calc(0);
  }
}

//Part 1.4 (Trying to do Part 1.1 without digitalWrite or pinMode)
    /*
    * I need PINS 47, 48, 49 from the board. 
    * 47, 48, 49 are all Port L
    * 47 : PL2 
    * 48 : PL1
    * 49 : PL0
    *
    * Setting an input would look like DDRA &= ~(1 << DDB4);
    *  
    */
//This is code functions the same as in the function digWriteLEDs but
//does not utilize digital write
void registerLEDs() {
  //Pin 47
  PORTL &= ~(PORTL2); //pin 47 low
  delay(333);
  PORTL |= PORTL2; //setting pin 47 to high
  delay(333);
  //Pin 48
  PORTL &= ~(PORTL1); //pin 47 low
  delay(333);
  PORTL |= PORTL1; //setting pin 47 to high
  delay(333);
  //Pin 49
  PORTL &= ~(PORTL0); //pin 47 low
  delay(333);
  PORTL |= PORTL0; //setting pin 47 to high
  delay(333);
}

//This is the code for part 1.1-1.2
void digWriteLEDs() {
  digitalWrite(LED_PIN_A, LOW);
  delay(d_val);
  digitalWrite(LED_PIN_A, HIGH);
  delay(d_val);
  
  //48
  digitalWrite(LED_PIN_B, LOW);
  delay(d_val);
  digitalWrite(LED_PIN_B, HIGH);
  delay(d_val);

  //48
  digitalWrite(LED_PIN_C, LOW);
  delay(d_val);
  digitalWrite(LED_PIN_C, HIGH);
  delay(d_val);
}
