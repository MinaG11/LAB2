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
#define LED_PIN_B 49
#define LED_PIN_C 48
#define d_val 777

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

#define NOTE_c 3830 // 261 Hz
#define NOTE_d 3400 // 294 Hz
#define NOTE_e 3038 // 329 Hz
#define NOTE_f 2864 // 349 Hz
#define NOTE_g 2550 // 392 Hz
#define NOTE_a 2272 // 440 Hz
#define NOTE_b 2028 // 493 Hz
#define NOTE_C 1912 // 523 Hz
#define R 0

int melody[] = { NOTE_e, R, NOTE_d, R, NOTE_c, R, NOTE_d, R, NOTE_e, R, NOTE_e, R, NOTE_e, R, NOTE_d, R, NOTE_d, R, NOTE_d, R, NOTE_e, R, NOTE_g, R, NOTE_g, R, NOTE_e, R, NOTE_d,                  
                  R, NOTE_c, R, NOTE_d, R, NOTE_e, R, NOTE_e, R, NOTE_e, R, NOTE_e, R, NOTE_d, R, NOTE_d, R, NOTE_e, R, NOTE_d, R, NOTE_c, R, NOTE_c };


// change these pins as necessary
int DIN = 12; //PB7
int CS =  11; //PB6
int CLK = 10; //PB5

byte spidata[2]; //spi shift register uses 16 bits, 8 for ctrl and 8 for data

//Transfers 1 SPI command to LED Matrix for given row
//Input: row - row in LED matrix
//       data - bit representation of LEDs in a given row; 1 indicates ON, 0 indicates OFF
void spiTransfer(volatile byte row, volatile byte data);
unsigned long prev ;
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

 
  //Setting up Timer 4 (Part 2)

  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4 = 0;
  
  OCR4A = 0;//Compare match register, look up... //change these values to change the frequency.
  TCNT4 = 0;
  TCCR4A |= (1 << COM4A0); //Toggling on compare match
  TCCR4B |= (1 << WGM42); //CTC MODE
  TCCR4B |= (1 << CS40); //Prescalar 1
  DDRH |= 1<<PH3;
  //LED matrix setup
  DDRB |= (1 << DDB6); //DIN
  DDRB |= (1 << DDB5); //CS
  DDRB |= (1 << DDB4); //CLOCK
  PORTH |= (1 << PORTB5);
  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);
  prev = millis();
}


//This method calculates the ocr4A value based on the equation given in the hardware book.
void orc_calc(int freq) {
  if (freq == 0) {
    OCR4A = 0;
  } else {
    OCR4A = (16000000 / 2 * freq) - 1 ;
  }
}
int lamb_time = 188;
int speak_flag = 1; //Change this on for the single demo of this
int led_flag = 1;
int lamb_flag = 0; //Change this on
int c_flag = 1;


void loop() { 
  
  //matrix(); //if ( time <= 200)
  //taskA();
  taskB(); 
  //littleLamb();

  //taskC();
  delay(1);
}


//This task manipulates the timers to output square wave
void taskB() {
  //Part 2.3/2.4
  //I want to generate square wave on OC4A --> ABSTRACT PORT:PH3, HW PIN: 6, ADP: 7  (labeled as pin 6 on the board);
  static int time;
  if (speak_flag == 0) {
    PORTH &= ~(PH3);
    orc_calc(0);  
    } else {  
      if (time % 4000 == 0){
  //    Serial.print("Time TASK 2:");
  //    Serial.print(time, DEC);
        orc_calc(400); //its running 400 HZ
      } 
      if (time % 4000 == 1000) {
  //    Serial.print("Time TASK 2:");
  //    Serial.print(time, DEC);
        orc_calc(250); //800 Hz
      } 
    
      if (time % 4000 == 2000) {
  //    Serial.print("Time TASK 2:");
  //    Serial.print(time, DEC);
        orc_calc(800);
      }
    
       if (time % 4000 == 3000) {
  //    Serial.print("Time TASK 2:");
  //    Serial.print(time, DEC);
        orc_calc(0);
      } 
      
      time++;
  }

}


void taskA() {

  static int time;   
  time++;
  Serial.begin(9600);
  if (led_flag == 0) {
    PORTL |= (PORTL2); //Turn on only LED 47
    PORTL |= (PORTL2); //Turn on only LED 47
    PORTL |= (PORTL2); //Turn on only LED 47
    DDRL &= ~(DDL2);
    DDRL &= ~(DDL1);
    DDRL &= ~(DDL0);
  } else {
      if (time >= 0) {
      
      PORTL &= ~(PORTL2); //Turn on only LED 47
      PORTL |= PORTL1;
      PORTL |= PORTL0;
    }  
    if (time >= 666) {
      PORTL &= ~(PORTL1); 
      PORTL |= (PORTL0);
      PORTL |= PORTL2;
      
    }
     if (time >= 1334) {
      PORTL &= ~(PORTL0);
      PORTL |= PORTL2;
      PORTL |= PORTL1;
    }
     if (time == 1999) {
      time = -1;
      led_flag = 0;
      lamb_flag = 1;
      speak_flag = 1;
      Serial.print(lamb_flag, DEC);
    }
  }
  
}


void littleLamb(){
   if (lamb_flag == 0) {
    PORTH &= ~(PH3);
   } else {
   int note;
   static int counter;
   counter++;
   //ICR4 = note;
   note = melody[counter / lamb_time];
   if (counter >= 11000) {
    note = 0;
    counter = 0;
    lamb_flag = 0;
    led_flag = 1;
   }
   OCR4A = note;
   }
}



void taskC() {
  static int time;
  time++;
  if (time >= 7000) {
    led_flag = 1;
    speak_flag = 1;
    taskA();
    taskB();
  } if (time <= 17000) {
    led_flag = 0;
    speak_flag = 0;
    time = 0;
  }
  delay(1);
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
  return;
}





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
  delay(d_val * 50);
  digitalWrite(LED_PIN_A, HIGH);
  delay(d_val * 50);
  
  //48
  digitalWrite(LED_PIN_B, LOW);
  delay(d_val * 50);
  digitalWrite(LED_PIN_B, HIGH);
  delay(d_val * 50);

  //480
  digitalWrite(LED_PIN_C, LOW);
  delay(d_val * 50);
  digitalWrite(LED_PIN_C, HIGH);
  delay(d_val * 50);
  return;
}