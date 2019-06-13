// set constants for the arduino pins to use
const int MOTOR_ENABLE = 11;
const int MOTOR_BOARD_IN1 = 2;
const int MOTOR_BOARD_IN2 = 3;

const int STEPPER1 = 4;
const int STEPPER2 = 5;
const int STEPPER3 = 6;
const int STEPPER4 = 7;

// constant delay between each step that the stepper makes
const int STEPPER_DELAY = 5;

// create a speed variable to indicate how fast the DC motor will run and set it to 120 (120 = stop)
int currentSpeed = 120;
// create a position variable to indicate the position of the stepper motor and set it to 0 (0 = straight)
int stepperPos = 0;
/* create a variable that indicates the position that the stepper motor should be at,
   so the current position will go towards tthe desired position */
int desiredPos = 0;

void setup() {
  // set the stepper pins and motorboard pins to output
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_BOARD_IN1, OUTPUT);
  pinMode(MOTOR_BOARD_IN2, OUTPUT);
  
  pinMode(STEPPER1, OUTPUT);
  pinMode(STEPPER2, OUTPUT);
  pinMode(STEPPER3, OUTPUT);
  pinMode(STEPPER4, OUTPUT);

  // set the direction of the motor to spin
  digitalWrite(MOTOR_BOARD_IN1, HIGH);
  digitalWrite(MOTOR_BOARD_IN2, LOW);

  // start reading from usb port (connected to raspberry pi)
  Serial.begin(9600);
}

void stepperWrite(int val1, int val2, int val3, int val4) {
  // pre:  takes in 4 integer values that correspond to the 4 values to send to the stepper motor
  // post: change the position of the stepper motor
  
  digitalWrite(STEPPER1, val1);
  digitalWrite(STEPPER2, val2);
  digitalWrite(STEPPER3, val3);
  digitalWrite(STEPPER4, val4);
  delay(STEPPER_DELAY);
}

void stepLeft() {
  // pre:  none
  // post: stepper moves 1 step to the left

  // calls stepperWrite function 4 times with different values to increase torque
  stepperWrite(HIGH, HIGH, LOW, LOW);
  stepperWrite(LOW, HIGH, HIGH, LOW);
  stepperWrite(LOW, LOW, HIGH, HIGH);
  stepperWrite(HIGH, LOW, LOW, HIGH);

  // reduce currentSpeed by 1 as the speed must always be decreasing when the user is not pressing a button to accelerate the motor
  currentSpeed--;
  analogWrite(MOTOR_ENABLE, currentSpeed);
}

void stepRight() {
  // pre:  none
  // post: stepper moves 1 step to the right

  // calls stepperWrite function 4 times with different values to increase torque
  stepperWrite(HIGH, LOW, LOW, HIGH);
  stepperWrite(LOW, LOW, HIGH, HIGH);
  stepperWrite(LOW, HIGH, HIGH, LOW);
  stepperWrite(HIGH, HIGH, LOW, LOW);

  // reduce currentSpeed by 1 as the speed must always be decreasing when the user is not pressing a button to accelerate the motor
  currentSpeed--;
  analogWrite(MOTOR_ENABLE, currentSpeed);
}

void loop() {

  // while loop that runs as long as there is still data being sent to process from the raspberry pi
  while (Serial.available() > 0) {
    // store pi data as val integer
    int val = Serial.read();

    // if val is 120, set current speed to 120
    if (val == 120)
      currentSpeed = 120;
    /* if val is between 20 and 100, it means that the raspberry pi wants to change the position of the stepper
       as values between 200 and 100 are reserved for the changing the stepper motor */
    else if (val >= 20 && val <= 100)
      // set the desired position to value - 60 as the stepper motor should step between -40 to 40
      desiredPos = val - 60;
    /* if val is 240 and the current speed is less than 240, it means that the raspberry pi wants to increase the speed of
       the DC motor as 240 is reserved for changing the speed. Since 240 is the max speed of the motor, the speed should cap at 240 */
    else if (val == 240 && currentSpeed < 240)
      // increase currentSpeed by 5 as long as 240 is being read and the currentSpeed is less than 240
      currentSpeed += 5;
  }

  /* if the stepperPos is less than desiredPos, it means the stepper motor must step left to go to the desiredPos
     so call stepLeft() to go left and increase the stepperPos by 1  */
  if (stepperPos < desiredPos) {
    stepLeft();
    stepperPos++;
  }

  
  /* if the stepperPos is greater than desiredPos, it means the stepper motor must step right to go to the desiredPos
     so call stepRight() to go right and decrease the stepperPos by 1  */
  if (stepperPos > desiredPos) {
    stepRight();
    stepperPos--;
  }

  // reduce currentSpeed by 1 as the speed must always be decreasing when the user is not pressing a button to accelerate the motor
  currentSpeed--;

  // if currentSpeed is less than 120, set it to 120 which is the stop value
  if (currentSpeed < 120) {
    currentSpeed = 120;
  }
  // write the currentSpeed to the DC motor to update the speed that it is running at
  analogWrite(MOTOR_ENABLE, currentSpeed);
  // delay for 20 ms
  delay(STEPPER_DELAY * 4);
}
