//Mary Had a Little Lamb
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


void setup() {
  // put your setup code here, to run once:
  TCCR4A = 0;
  TCCR4B = 0;
  TCCR4C = 0;

  TCCR4B |= (1 << WGM43); //CTC MODE
  TCCR4B |= (1 << CS41); //Prescalar 1
  TCCR4A |= (1 << COM4A1 | 0 << COM4A0); //Toggling on compare match
//  TCCR4A |= (1 << COM4A0); //Toggling on compare match
//  TCCR4B |= (1 << WGM42); //CTC MODE
//  TCCR4B |= (1 << CS40); //Prescalar 1

  DDRH |= 1<<PH3;

  littleLamb();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void littleLamb(){
  int note;
  for (int i = 0; i < 53; i++){
    note = melody[i];
    ICR4 = note;
    OCR4A = note/2;
    delay(200);
  }
  DDRH &= !1<<PH3;
}
