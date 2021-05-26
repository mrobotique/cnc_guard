
/*
* Interrupts HowTo: https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/
* 
*/
#include <Arduino.h>
unsigned long my_time;
unsigned long  oldtime;
float value;
float rpm;

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {34, 0, false};

void IRAM_ATTR isr() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 2) 
  {
    button1.numberKeyPresses += 1;
    button1.pressed = true;
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
  Serial.begin(115200);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
}

void loop() {

  delay(100);
  detachInterrupt(button1.PIN);           //detaches the interrupt
  my_time = millis()-oldtime;        //finds the time   
  //Serial.println(my_time);
  rpm=(float(button1.numberKeyPresses)/float(my_time))*6000.00;         //calculates rpm
  oldtime=millis();             //saves the current time
  
  Serial.print("button1.numberKeyPresses: ");
  Serial.print(button1.numberKeyPresses);
  Serial.print(" | RPM :  " );
  Serial.println(rpm);
  
  button1.numberKeyPresses = 0;
  attachInterrupt(button1.PIN, isr, FALLING);  
}