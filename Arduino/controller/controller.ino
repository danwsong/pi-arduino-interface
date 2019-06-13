const int MOTOR_ENABLE = 11;
const int MOTOR_BOARD_IN1 = 2;
const int MOTOR_BOARD_IN2 = 3;

const int STEPPER1 = 4;
const int STEPPER2 = 5;
const int STEPPER3 = 6;
const int STEPPER4 = 7;
const int STEPPER_DELAY = 5;

int currentSpeed = 120;
int currentAngle = 0;
int desiredAngle = 0;

void setup() {
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_BOARD_IN1, OUTPUT);
  pinMode(MOTOR_BOARD_IN2, OUTPUT);
  
  pinMode(STEPPER1, OUTPUT);
  pinMode(STEPPER2, OUTPUT);
  pinMode(STEPPER3, OUTPUT);
  pinMode(STEPPER4, OUTPUT);
  
  digitalWrite(MOTOR_BOARD_IN1, HIGH);
  digitalWrite(MOTOR_BOARD_IN2, LOW);
  
  Serial.begin(9600);
}

void stepperWrite(int val1, int val2, int val3, int val4) {
  digitalWrite(STEPPER1, val1);
  digitalWrite(STEPPER2, val2);
  digitalWrite(STEPPER3, val3);
  digitalWrite(STEPPER4, val4);
  delay(STEPPER_DELAY);
}

void stepLeft() {
  stepperWrite(HIGH, HIGH, LOW, LOW);
  stepperWrite(LOW, HIGH, HIGH, LOW);
  stepperWrite(LOW, LOW, HIGH, HIGH);
  stepperWrite(HIGH, LOW, LOW, HIGH);
  currentSpeed--;
  analogWrite(MOTOR_ENABLE, currentSpeed);
}

void stepRight() {
  stepperWrite(HIGH, LOW, LOW, HIGH);
  stepperWrite(LOW, LOW, HIGH, HIGH);
  stepperWrite(LOW, HIGH, HIGH, LOW);
  stepperWrite(HIGH, HIGH, LOW, LOW);
  currentSpeed--;
  analogWrite(MOTOR_ENABLE, currentSpeed);
}

void loop() {
    Serial.print("DESIRED: ");
  Serial.println(desiredAngle);
  while (Serial.available() > 0) {
    int val = Serial.read();
    Serial.print("VAL: ");
    Serial.println(val);
    if (val == 120) {
      currentSpeed = 120;
    } else if (val >= 20 && val <= 100) {
      // SET NEW ANGLE
      desiredAngle = val - 60;
    } else if (val == 240 && currentSpeed < 240) {
      // SET NEW SPEED
      currentSpeed += 5;
    }
  }
  if (currentAngle < desiredAngle) {
    stepLeft();
    currentAngle++;
  }
  if (currentAngle > desiredAngle) {
    stepRight();
    currentAngle--;
  }
  currentSpeed--;
  if (currentSpeed < 120) {
    currentSpeed = 120;
  }
  analogWrite(MOTOR_ENABLE, currentSpeed);
  delay(STEPPER_DELAY * 4);
}


