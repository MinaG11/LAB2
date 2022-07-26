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

//For part 1.1/1.2
#define LED_PIN_A 47 
#define LED_PIN_B 49
#define LED_PIN_C 48
#define d_val 777

#define TIMER4_ON_BIT PRTIM4; 
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


// Pins for SPI/LED matrix
int DIN = 12; //PB7
int CS =  11; //PB6
int CLK = 10; //PB5

byte spidata[2]; //spi shift register uses 16 bits, 8 for ctrl and 8 for data

//Transfers 1 SPI command to LED Matrix for given row
//Input: row - row in LED matrix
//       data - bit representation of LEDs in a given row; 1 indicates ON, 0 indicates OFF
void spiTransfer(volatile byte row, volatile byte data);

/** Setup function is where all pin registers used in this lab, registers for the timer4, and spiTransfer settings
 *	  are declared.
 */	
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
  DDRH |= 1<<PH3;
 
  //Setting up Timer 4 (Part 2)
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
  prev = millis();
}


/** This function calculates the OCRnA value for the timer4 using the given formula in the hardware sheet.
*   It is to be used only when the prescalar value is 1. 
*/
void orc_calc(int freq) {
  if (freq == 0) {
    OCR4A = 0;
  } else {
    OCR4A = (16000000 / 2 * freq) - 1 ;
  }
}

//Time duration for each note of little lamb song
int lamb_time = 188;

//Flags for tasks
int speak_flag = 1; 
int led_flag = 1;
int lamb_flag = 0; 
int c_flag = 1;

/** Loop runs the various tasks from the lab. 
*/
void loop() { 
  //taskA();
  taskB(); 
  //littleLamb();
  //matrix(); 
  //taskC();
  //part_3_1();
  delay(1);
}

/** This function is for part 3.1 in the lab, running task A alone, task B alone, and then waiting a second.
    Repeats when called in the loop. 
*/
void part_3_1() {
	static int time;
	time++;
	if (time == 0) { //Run both tasks 
		taskA();
		taskB();
	} else if (time >= 6500) { //After both tasks done (it takes about 6 seconds to complete both)
		time = 0;			   //Reset timer and flags so it can restart. 
		led_flag = 1;
		speak_flag = 0;
	}
}

/** This task manipulates the timers to output a square wave of frequency 400, 800, 250 and 0 for one second each
*	over a 4 second period.
*/
void taskB() {
  //Part 2.3/2.4
  //I want to generate square wave on OC4A --> ABSTRACT PORT:PH3, HW PIN: 6, ADP: 7  (labeled as pin 6 on the board);
  static int time;
  if (speak_flag == 0) { //If  flag is off the port is shut off. 
    PORTH &= ~(PH3);
    orc_calc(0);  
    } else {
      PORTH |= (PH3); //Turning port back on. 
      if (time % 4000 == 0){ //Play 400 Hz for one second
        orc_calc(400); 
      } 
      if (time % 4000 == 1000) {
        orc_calc(250); //250 Hz for one second
      } 
    
      if (time % 4000 == 2000) {
        orc_calc(800); //800 Hz next
      }
       if (time % 4000 == 3000) {
        orc_calc(0); //Silence for one second
      } 
      time++;
  }

}

/** Code for 1.4, turns 3 LEDs off and on in sequence over a 2 second period, and then completely off. 
*/
void taskA() {
  static int time;   
  time++;
  if (led_flag == 0) { //If flag is off turn off the lights and the ports.
      PORTL |= (PORTL2); 
      PORTL |= (PORTL2); 
      PORTL |= (PORTL2); 
      DDRL &= ~(DDL2);
      DDRL &= ~(DDL1);
      DDRL &= ~(DDL0);
  } else {
      DDRL |= (DDL2); //Turning ports back on for operation
      DDRL |= (DDL1);
      DDRL |= (DDL0);
	  
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
      
	  if (time == 1999) { //Reseting timer to 0 and the flags
      	time = -1; 		  //This is currently setup to run LED task alone, and then the speaker task alone (or lamb)
      	led_flag = 0;     //Changes depending on what part needs to be demoed. 
      	lamb_flag = 1;
      	speak_flag = 1;
     }
  }
  
}

/** This function plays the notes of the Mary's Little Lamb on a speaker. */
void littleLamb(){
   if (lamb_flag == 0) { //Shuts speaker port off when flag is off
       PORTH &= ~(PH3);
   } else { //Playing the song
       PORTH |= (PH3);
       int note;
       static int counter;
       counter++;
       note = melody[counter / lamb_time]; //Playing each note of the song
       if (counter >= 11000) { //Song plays for approximately 10 seconds, restarting the timer and counter at end of duration.
          note = 0;
          counter = 0;
          //lamb_flag = 0; 
       }
       OCR4A = note; //Setting the OCR4A of the timer to the desired note. 
   }
}

/** Task C was incomplete, but the general idea is that it should run both tasks simulateneously for 10 seconds,
*   And then at the ten second mark, the ports are turned off so they no longer run. 
*/
void taskC() {
  static int time;
  time++;
  if (time >= 0000) { //Turning ports back on for operation
    DDRH |= (DDH3);
	DDRL |= (DDL2); 
    DDRL |= (DDL1);
    DDRL |= (DDL0);
    taskA(); //Our code for these tasks is set up to run using flags, but what we would do is have flagless versions
    taskB(); //that run regardless, that way the speakers and leds could run simulatenously in the loop.  
  } if (time <= 10000) {
  	DDRH &=  ~(DDH3); //Turning off ports and resetting the timers. 
	DDRL &= ~(DDL2);
    DDRL &= ~(DDL1);
    DDRL &= ~(DDL0);  
    time = 0;
  }
  delay(1);
}

/**This function contains the code to display on 
* the 8x8 LED display based on inputs from the user controlled joystick.
*/
void matrix() {
  int vert = analogRead(A0) >> 7; //Converts values to 0 to 7
  int horiz = analogRead(A1) >> 7;

  //HEX instead of binary
  for (int i = 0; i < 8; i++){
    if(i == horiz){ //Turns on row and column of coordinate only
      spiTransfer(i, 1<<vert); //Column value obtained by shifting
    }else{
      // Turn off all other lights. 
      spiTransfer(i, 0x00);
    }
  }
  return;
}


/** This is the provided code for the SPI method for the LED matrix */
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

/** This is the code for 1.1/1.2, lighting up LEDs in sequence using arduino macro */ 
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
