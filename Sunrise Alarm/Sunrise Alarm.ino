
/*********************************************************** TO DO LIST ****************************************************************************  
 * 
 * X 1. TURN OFF CHALLENGE
 * X 2. PHOTORESISTR analog 
 * X 3. I/O SNOOZE: interrupt and button activated
 * X 4. SPEAKER output (concurrently)
 * X 5. Tri-LED nightlight output that reflects analog I/O from photoresistor
 * X 6. UNIVERSAL TIMER: keep track of time when Arduino is first hooked up
 * X 7. SNOOZE TIMER: keep track of time until alarm turns on again
 * 
 */

//Includes the time library to use functions to output universal clock time
#include <Time.h>

/**************************************************************VARIABLE DECLARATIONS*************************************************************/

//LEDs for TURN OFF function
const int ledPin1 = 8; //Lowest significant bit for LEDs 
const int ledPin2 = 9; 
const int ledPin3 = 10; //Highest significant bit for LEDs

//LEDs for tricolor which will be attached to PWM (pulse width modulation) pins. These pins can be invoked to rapidly change the power (i.e. time) applied at the pin
//Middle pins are common cathodes that have to be grounded
//Red pin is the pin on the right side of the flat edge
//Blue pin is the pin on the left side of the flat edge (NOTE: this blue pin is not used)
//2nd blue pin is the pin on the right side of the round edge
//Green pin is the pin on the left side of the round edge
const int triLedPinR = 3; //Red pin
const int triLedPinB = 5; //Blue pin
const int triLedPinG = 6; //Green pin

//Speaker
const int speakerPin = 7;

//Snooze interrupt
const int snoozePin = 2;

//Time variables: these variables are used to compare present and past time 
int prevHour = 0;
int prevMinute = 0;
int prevSecond = 0;

//Other variables
int randomNumber = -1; //Random number dictates which decimal number (i.e. 0-7) th system will output as binary; intialized to -1 because it is not a case that can be generated
boolean alarmOn = false; //A variable that describes the main state of the system (i.e. alarm is on, or alarm is off); initialized to off because alarm is initially off
const int analogPin = A0; //Aalog pin for the photoresistor; this pin will read in analog values from the photoresistor 

//Volatile variables; these variables tell the interrupt to grab the values of the variables from the memory rather than from the register since these variables' values will be changed beyond the control of the code
volatile boolean snoozeOn = false; //Variable that describes the state of the snooze function; this is a volatile variable because the interrupt manipulates it and uses it
volatile long timer1_millis = 0; //Counts the numbers of ticks generated by the Arduino
volatile long timer1_fract = 0; //Keeps traack of the error when counting overflows 

/************************************************************ SETUP *******************************************************************************/

void setup()
{  
  //Initialize values for the following registers to be used with timer1 (i.e. 16 bit register)
  TCNT1=0x0000; //Saves counter value, set to 0x0000 to make sure timer starts at 0 
  TCCR1B=0x03; //Prescale to 64
  TIMSK1=0x01; //Timer or Counter Interrupt Mask Register which enables/disables timer interrupts; setting to 1 to enable global and timer overflow interrupt     
  TCCR1A=0x00; //Timer/Counter Control Register, prescaler can be configured here; initializes timer to 0
  
  
  setTime(12,0,0,0,0,0); //function belonging to time library; Sets time to 12:00:00. Last 3 variables read in initial values of 0 for day, month, year (not of interest) 
  randomSeed(analogRead(A1)); //Seeds a random number from analog I/O pin A1 because A1 is not being used and whose value will be unknown -> almost guaranteeing random numbers every time
  
  //Set up the pins as output or input
  pinMode(triLedPinR, OUTPUT);
  pinMode(triLedPinB, OUTPUT);
  pinMode(triLedPinG, OUTPUT);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  
  pinMode(speakerPin, OUTPUT);
  
  pinMode(snoozePin, INPUT);
  
  attachInterrupt(0,snoozeHandler, RISING); //0 -> pin 2; snoozeHandler function will be called upon a rising edge because the user presses down on a button (0->1) first before releasing it (more accurate)
  
  Serial.begin(9600); //opens serial port, sets data rate to 9600 bps
}

/**************************************************************************** MAIN FUNCTION *********************************************************************/

void loop() 
{
  clockDisplay(); //Displays univeral clock time everytime the loop is reiterated

  checkAlarm(); //Checks if the alarm is on or not via the photoresistor

  if (alarmOn) //Main branch: when the alarm is on 
  {
    
    tone(speakerPin, 100); //Turns on the speakers and will output a sound with frequency of 100 Hz

    randomNumber = random(8); //Intialize randomNumber with a random integer between 0-7
    
    //NOTE: turnOffTest()interacts with the user to see if they can turn off the alarm
    switch(randomNumber)
    {
    case 0:
      {
        //Serial.println("Case 0!");
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        turnOffTest();
        break;
      }

    case 1:
      {
        //Serial.println("Case 1!");

        //Reset the LEDs
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        turnOffTest();
        break;
      }

    case 2:
      {
        //Serial.println("Case 2!");

        //Reset the LEDs
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, LOW);

        turnOffTest();
        break;
      }

    case 3:
      {
        //Serial.println("Case 3!");

        //Reset the LEDs
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, LOW);

        turnOffTest();
        break;
      }

    case 4:
      {
        //Serial.println("Case 4!");

        //Reset the LEDs
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, HIGH);

        turnOffTest();
        break;
      }

    case 5:
      {
        //Serial.println("Case 5!");

        //Reset the LEDs
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, HIGH);

        turnOffTest();
        break;
      }

    case 6:
      {
        //Serial.println("Case 6!");

        //Reset the LEDs
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);

        turnOffTest();
        break;
      }

    case 7:
      {
        //Serial.println("Case 7!");

        //Reset the LEDs
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);

        turnOffTest();
        break;
      }
    }
  }
}

/**************************************************************************** FUNCTIONS *************************************************************************/
//Listed in alphabetical order:

//Function checks if surrounding environment is bright via the photoresistor and turns on the alarm in such an event
void checkAlarm()
{
  
  int brightness = analogRead(analogPin); //Reads and maps input voltages between 0 and 5 volts into integer values between 0 and 1023 yieling a resolution between readings of 5 volts/1024 units via the analog pin and a resistor

  if(brightness > 200) //The brightness of a well-lit room has a read-in value of approximately 200 (i.e. value determined by experimentation)
  {
    alarmOn = true; //Keeps track of the alarm state 
  }

  else
  {
    nightlight(); //If it's dark, turn on the nightlight
  }
}

//Function checks if the user has pushed the snooze button
void checkSnooze()
{
   if (snoozeOn)
    {
      Serial.println("Snoozing!"); //Notifies the user that they are in the snooze state
  
      //Read and guard the current state of LED  pins to turn them back on once snooze is finished
      int ledState1 = digitalRead(ledPin1);
      int ledState2 = digitalRead(ledPin2);
      int ledState3 = digitalRead(ledPin3);
       
      noTone(speakerPin); //Momentarily snoozes the alarm 
      
      //Turning off LEDs
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      
      myDelay(10); //Snoozes the system for x seconds
      
      tone(speakerPin, 100); //Turns on the alarm again once snoozing has finished

      //Return system state prior to snooze
      digitalWrite(ledPin1, ledState1);
      digitalWrite(ledPin2, ledState2);
      digitalWrite(ledPin3, ledState3);
      
      Serial.println("Finished snoozing!"); //Notifies the user snoozing state is off
      Serial.println(); //Skips a line for formatting purposes
      Serial.println("What is the decimal representation of the binary number as represented by the LEDs (i.e. 0-7)?");
      Serial.println("Note: left most LED is most significant bit, right most LED is least significant bit and an LED that is on represents 1.");
    }
}

//Function persistently updates the time displayed on the serial output until alarm goes off
void clockDisplay()
{
  if(hour() != prevHour || minute() != prevMinute || second() != prevSecond) //update the display only if the time has changed
  {
    digitalClockDisplay();  
  }
}

//Displays time in format x:y:z and guards recorded time for future calculations
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
  
  //Keeps track of previous time 
  prevHour = hour();
  prevMinute = minute();
  prevSecond = second();
}

//Interrupt Service Routine that counts the time
ISR(TIMER1_OVF_vect) {
  timer1_millis += 262; //Number of milliseconds that the 16-bit timer1 will count until an overflow has occured ((4 us *2^16)/1000 us = 262.144 ms)
  timer1_fract += 144; //Number of microseconds kept from the round off error because it actually takes 262.144 microiseconds for an overflow to occur

  if (timer1_fract >= 1000) //If the error has reached 1000 us or 1 ms, add millisecond to the count
  {
    timer1_fract -= 1000; //Removes 1 millisecond 
    timer1_millis +=1; //Add one millisecond to the millisecond count
  }
}

//Our own version of the delay() using timer1 because using timer0 would interrupt with the Arduino's system
void myDelay(int duration) //Reads in duration value in seconds
{  
  while(myMillis() < duration) //While the time snapped is less than the inputted time, do nothing
  {
    //Do nothing, stalling
  }
  
  snoozeOn = false; //Indicates that the snoozing period has finished
}

//Function returns snapshot in time in seconds
unsigned long myMillis () //Unsigned because it cannot have a negative value (no such thing as a negative time)
{
  return timer1_millis/1000; //Returns the time calculated by the ISR in seconds
}

//Function reads in brightness of light from external environment via analog I/O and adjusts triLED output accordingly
void nightlight(){
  int brightness = analogRead(analogPin); //reads the analog value of the photoresistor 

  //constrains the brightness level to be in a range from 10 to 200
  //returns the analog value read from photoresistor if the value is between the range 10 and 200
  //returns a value of 10 if value read is less than 10
  //returns a value of 200 if value read is greater than 200
  brightness = constrain(brightness, 10, 200); 

  //maps a number from one range to another
  //maps brightness level of 10 to led level of 255 and 200 to 0
  //255 means that there is 5V going to triLED, 0 means there is 0V going to triLED
  int ledLevel = map(brightness, 10, 200, 255, 0); 

  //sets the led level depending on the brightness in the room to each colour
  //Using analogWrite allows the PWM (pulse with modulation) pins to rapidly change the power applied at the pin
  if (brightness <=200)
  {
    analogWrite(triLedPinR, ledLevel);
    analogWrite(triLedPinB, ledLevel);
    analogWrite(triLedPinG, ledLevel);
  }
}

//Prints digits for time in proper format which takes in the minute, or second as a variable 
//Function ensures that the time is displayed in format: x:xx:xx or xx:xx:xx 
void printDigits(int digits){
  Serial.print(":");
  if(digits < 10) //if the digit will only take up 1 space
    Serial.print('0'); //print a preceding 0 before it to ensure it is displayed as in format xx
  Serial.print(digits);
}

//Function interacts with the user to see if they can turn off the alarm
void turnOffTest()
{ 
  
  do //Loop quizzes the user to turn off the alarm by entering the correct decimal representation of the binary number displayed by the LEDs. Loop will reiterate until user has entered the correct answer
  {
    Serial.println(); //Skips a line for formatting purposes
    Serial.println("What is the decimal representation of the binary number as represented by the LEDs (i.e. 0-7)?");
    Serial.println("Note: left most LED is most significant bit, right most LED is least significant bit and a bright LED represents 1.");
    
    //The following three blocks allows the user to snooze the alarm a maximum of three times before forcing them to turn off the alarm 
    //The alarm will be off at this point and while the user is not doing anything, the program will wait until the user either enters something in the serial or presses the snooze button
    while (!Serial.available() && snoozeOn == false) //if the user has not inputted anything into the serial or if the user has not hit the snooze button, execute the while loop and wait for user commands
    {
      //Waiting for user input
    }
    
    checkSnooze(); //Checks to see if snooze is on; if it is, system will switch to snooze mode
    
    while (!Serial.available() && snoozeOn == false)
    {
      //Waiting for user input
    }
    checkSnooze();
    
    while (!Serial.available() && snoozeOn == false)
    {
      //Waiting for user input
    }
    checkSnooze(); 
    
    while(!Serial.available()) //Waiting for the user to finish inputting something
    {
      //Waiting for user input in the serial
    }
    
    int turnOffInput = Serial.read() - 48; //turnOffInput is the decimal number corresponding to the binary number displayed by the LEDs; the input is subtracted by 48 because the serial reads in a number x+48

    if (turnOffInput == randomNumber) //If the user is correct, turn off the alarm speakers, the LEDs and continue displaying time; user will have to reset the alarm system to provoke another alarm
    {
      noTone(speakerPin); // Turns off the speaker
      digitalWrite(ledPin1, LOW); //Turns off the LEDs
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      alarmOn = false; //Alarm is turned off
      Serial.println("Alarm turned off!"); //Notifies user alarm is turned off
      while(1)
      {
        clockDisplay(); //The system will be stuck in an infinite loop and will act as a regular clock until the user resets the system (at night) to prepare for the next sunrise.
      } 
    }

    else
    {
      Serial.println("Oops! Try again."); //Will cause the loop to reiterate and user has another chance to input the correct answer to turn off th alarm
    }
  }
  while(alarmOn);
}

/*************************************************************************** HANDLERS**********************************************************************************/

//Handler for the snooze function 
void snoozeHandler()
{
  snoozeOn = true; //Notifies the rest of the functions that the system is in the snooze state
  timer1_millis = 0; //Restarts the count to allow myDelay() to work repeatively
  timer1_fract = 0; //Restarts the count to work repeatively
}


