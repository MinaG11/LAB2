//James Jhong 1939553
//Mina Gao 1973927

#define DDL2 1 << 2 //47
#define DDL1 1 << 1 //48
#define DDL0 1 << 0 //49
#define DDH3 1 << 3 //OCA4 output

//#define PORTH3 1 << 3
#define PORTL2 1 << 2
#define PORTL1 1 << 1
#define PORTL0 1 << 0
#define PORTH3 1 << 3;

#define LED_PIN_A 47 
#define LED_PIN_B 48
#define LED_PIN_C 49
#define d_val 333

#define TIMER4_ON_BIT PRTIM4; //Writing logic one to this shuts Timer/Counter 4 module. WHen enabled, operates like before shutdown
#define TIMER5_ON_BIT PRTIM5;
#define TIMER3_ON_BIT PRTIM3;

//SPI define calls
#define OP_DECODEMODE  8
#define OP_SCANLIMIT   10
#define OP_SHUTDOWN    11
#define OP_DISPLAYTEST 14
#define OP_INTENSITY   10

// joystick define
#define VERT 1 << 0
#define HORZ 1 << 1

// change these pins as necessary
int DIN = 12; //PB7
int CS =  11; //PB6
int CLK = 10; //PB5

byte spidata[2]; //spi shift register uses 16 bits, 8 for ctrl and 8 for data

//Transfers 1 SPI command to LED Matrix for given row
//Input: row - row in LED matrix
//       data - bit representation of LEDs in a given row; 1 indicates ON, 0 indicates OFF
void spiTransfer(volatile byte row, volatile byte data);

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
  DDRH |= DDH3;
  
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

  //LED matrix setup
  DDRB |= (1 << DDB6); //DIN
  DDRB |= (1 << DDB5); //CS
  DDRB |= (1 << DDB4); //CLOCK
  PORTH |= (1 << PORTB5);
  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);
}


//This method calculates the ocr4A value based on the equation given in the hardware book.
void orc_calc(int freq) {
  if (freq == 0) {
    OCR4A = 0;
  } else {
    OCR4A = (16000000 / 2 * freq) - 1 ;
  }
}

void loop() {
  //Code for Part 1.1 (Running LEDs on pins 47 - 49 in sequential order for 0.333 seconds each)
  //digWriteLEDs();
  //registerLEDs();
  //timersPart2();

  //timerPart3_1();
  //LEDs_task3();
  speakers_task2_4();
  matrix();
}

//This functions runs task A for 2 second
//Task B for one cycle
//No outups for 1
//Repeat
void timerPart3_1() {
  static int time;
  time++;
  LEDs_task3(); //running task A and Task bB
  speakers_task2_4(); //6000 because they take about 6 seconds to complete
  if (time >= 7000) { //pausing for 1 second and resestting the timer
    time = 0;
  }
  delay(1);
  return;
}

//This task manipulates the timers to output square wave
void speakers_task2_4() {
  //Part 2.3/2.4
  //I want to generate square wave on OC4A --> ABSTRACT PORT:PH3, HW PIN: 6, ADP: 7  (labeled as pin 6 on the board);
  static int time = 0;
  
  time++;
  if (time == 3000){
    orc_calc(400); //its running 400 HZ
  } 
 
  if (time == 4000) {
    orc_calc(800); //800 Hz
  } 
  
  if (time == 5000) {
    orc_calc(250);
  }
  
  if (time == 6000) {
    orc_calc(0);
    time = 0;
  }
  delay(1);
  return;
}

//Same LED structure as Part 1.4, except runs only for 2 seconds at a time (not infinte).
void LEDs_task3() {
  static int time = 0;
  time++;
  if (time == 0) {
    PORTL &= ~(PORTL2);
    PORTL |= PORTL0;
  }  
  if (time == 500) {
    PORTL |= PORTL2;
    PORTL &= ~(PORTL1); 
  }
  if (time == 1000) {
    PORTL &= ~(PORTL0);
    PORTL |= PORTL1;
  }
  if (time == 1500) {
    PORTL |= (PORTL0);
    PORTL |= (PORTL1); 
    PORTL |= (PORTL2);
    time = 0;
  }
  delay(1);
  return;
}

void timerPart3_2() {
  static int time;
  time++;
  LEDs_task3();
  speakers_task2_4();
  taskC();
  if (time >= 17000) {
    time = 0;
  }
  delay(1);
  return;
}

void taskC() {
  static int time;
  time++;
  if (time == 5000) {
  LEDs_task3();
  speakers_task2_4();
  } 
  if (time >= 10000) {
    time = 0;
  }
  delay(1);
  return;
}

void registerLEDS_1_4() {
  static int timer;
  if (timer % 1000 == 0) {
    PORTL &= ~(PORTL2);
    PORTL |= PORTL0;
  } else if (timer % 1000 == 333) {
    PORTL |= PORTL2;
    PORTL &= ~(PORTL1); 
  } else if (timer % 1000 == 666) {
    PORTL &= ~(PORTL0);
    PORTL |= PORTL1;
  }
  
  delay(100);
  timer++;
}



//This function contains the code to display on the 8x8 LED display based on movements from the joystick. 
void matrix() {
  int vert = analogRead(A0) >> 7;
  int horiz = analogRead(A1) >> 7;
//  int vert = 3;
//  int horiz = 3;

  //HEX instead of binary
  for (int i = 0; i < 8; i++){
    if(i == horiz){
      spiTransfer(i, 1<<vert);
    }else{
      // instead of using clearBoard, we turn it off
      spiTransfer(i, 0x00);
    }
  }
}

//// do not need it any more
//void clearBoard() {
//  int i;
//  for (i = 0; i < 8; i++) { //clears the rest of the rows
//    spiTransfer(i, 0b00000000);  
//  }
//}

//spiTransfer 
void spiTransfer(volatile byte opcode, volatile byte data){
  int offset = 0; //only 1 device
  int maxbytes = 2; //16 bits per SPI command
  
  for(int i = 0; i < maxbytes; i++) { //zero out spi data
    spidata[i] = (byte)0;
  }
  //load in spi data
  spidata[offset+1] = opcode+1;
  spidata[offset] = data;
  digitalWrite(CS, LOW); //
  for(int i=maxbytes;i>0;i--)
    shiftOut(DIN,CLK,MSBFIRST,spidata[i-1]); //shift out 1 byte of data starting with leftmost bit
  digitalWrite(CS,HIGH);
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
