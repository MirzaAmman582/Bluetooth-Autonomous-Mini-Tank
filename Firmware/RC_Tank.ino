

unsigned char start01[] = {0x00, 0x7c, 0x12, 0x12, 0x7c, 0x00, 0x7e, 0x02, 0x7e, 0x02, 0x7e, 0x00, 0x44, 0x7e, 0x40, 0x00};
unsigned char front[] = {0x00, 0x00, 0x80, 0xf0, 0xf8, 0x78, 0x2e, 0xf3, 0xf3, 0x2e, 0x78, 0xf8, 0xf0, 0x80, 0x00, 0x00};
unsigned char back[] = {0x00, 0x00, 0x01, 0x1a, 0x0e, 0x36, 0x62, 0x82, 0x82, 0x62, 0x36, 0x0e, 0x1a, 0x01, 0x00, 0x00};
unsigned char left[] = {0x00, 0x00, 0x00, 0x18, 0x3c, 0x7e, 0xe7, 0xc3, 0x81, 0x00, 0x18, 0x18, 0x18, 0x7e, 0xdb, 0x99};
unsigned char right[] = {0x99, 0xdb, 0x7e, 0x18, 0x18, 0x18, 0x00, 0x81, 0xc3, 0xe7, 0x7e, 0x3c, 0x18, 0x00, 0x00, 0x00};
unsigned char STOP01[] = {0x7e, 0x08, 0x7e, 0x00, 0x7e, 0x0a, 0x7e, 0x00, 0x7e, 0x40, 0x40, 0x02, 0x02, 0x7e, 0x02, 0x02};
unsigned char followmode[] = {0x00, 0x00, 0xf8, 0x48, 0x50, 0xf0, 0x14, 0x04, 0x04, 0x1f, 0x0e, 0x04, 0x60, 0x7c, 0x7c, 0x7c};
unsigned char light[] = {0x00, 0x08, 0x49, 0x2a, 0x00, 0x00, 0x3c, 0x7e, 0x7e, 0x3c, 0x00, 0x00, 0x2a, 0x49, 0x08, 0x00};
unsigned char obstAvoid[] = {0x00, 0x00, 0xf8, 0x48, 0x50, 0xf0, 0x10, 0x00, 0x28, 0x44, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc};

unsigned char clear[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

#define SCL_Pin  A5
#define SDA_Pin  A4
#define ML_Ctrl 13
#define ML_PWM 11
#define MR_Ctrl 12
#define MR_PWM 3
#define Trig 5
#define Echo 4
#define servoPin 9
#define light_L_Pin A1
#define light_R_Pin A2

int distance, a, a1, a2, random2, left_light, right_light;
char bluetooth_val;
int flag;

unsigned long lastStopTime = 0;
bool isStopped = false;
bool isAutoMode = false;

void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(ML_Ctrl, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(MR_Ctrl, OUTPUT);
  pinMode(MR_PWM, OUTPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(SCL_Pin, OUTPUT);
  pinMode(SDA_Pin, OUTPUT);
  pinMode(light_L_Pin, INPUT);
  pinMode(light_R_Pin, INPUT);
  procedure(90);
  matrix_display(clear);
  matrix_display(start01);
}

void loop() {
  if (Serial.available()) {
    bluetooth_val = Serial.read();
    Serial.println(bluetooth_val);
  }

  switch (bluetooth_val) {
    case 'F': isAutoMode = false; Tank_front(); matrix_display(front); break;
    case 'B': isAutoMode = false; Tank_back();  matrix_display(back); break;
    case 'L': isAutoMode = false; Tank_left();  matrix_display(left); break;
    case 'R': isAutoMode = false; Tank_right(); matrix_display(right); break;
    case 'S': isAutoMode = false; Tank_Stop(); break;

    case 'Y': isAutoMode = true; matrix_display(followmode); follow(); break;
    case 'U': isAutoMode = true; matrix_display(obstAvoid); avoid(); break;
    case 'X': isAutoMode = true; matrix_display(light); light_track(); break;
  }

  // After 5 seconds of being stopped, show start sign again
  if (isStopped && (millis() - lastStopTime >= 1000)) {
    matrix_display(start01);
    isStopped = false;
  }
}

void avoid() {
  isStopped = false;
  flag = 0;
  while (flag == 0) {
    random2 = random(1, 100);
    a = checkdistance();

    if (a < 20) {
      Tank_Stop();
      delay(200);

      procedure(160);
      a1 = 0;
      for (int j = 0; j < 5; j++) a1 += checkdistance();
      a1 /= 5;

      delay(200);
      procedure(20);
      a2 = 0;
      for (int k = 0; k < 5; k++) a2 += checkdistance();
      a2 /= 5;

      procedure(90);

      if (a1 > a2) {
        Tank_left();
        delay(500);
      } else {
        Tank_right();
        delay(500);
      }
      Tank_front();
    } else {
      Tank_front();
    }

    if (Serial.available()) {
      bluetooth_val = Serial.read();
      if (bluetooth_val == 'Z') flag = 1;
    }
  }
}

void follow() {
  flag = 0;
  while (flag == 0) {
    distance = checkdistance();

    if (distance >= 10 && distance <= 20) {
      Tank_front();
    } else {
      Tank_Stop();
      isStopped = false;
    }

    if (Serial.available()) {
      bluetooth_val = Serial.read();
      if (bluetooth_val == 'Z') {
        flag = 1;
      }
    }
  }
}

float checkdistance() {
  long sum = 0;
  for (int i = 0; i < 5; i++) {
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    sum += pulseIn(Echo, HIGH);
    delay(10);
  }
  return (sum / 5.0) / 58.0;
}

void procedure(int myangle) {
  int pulsewidth;  
  for (int i = 0; i <= 50; i++) {
    pulsewidth = myangle * 11 + 500;
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servoPin, LOW);
    delay(20 - pulsewidth / 1000);
  }
}

void light_track() {
  flag = 0;
  while (flag == 0) {
    left_light = analogRead(light_L_Pin);
    right_light = analogRead(light_R_Pin);

    if (left_light > 650 && right_light > 650) Tank_front();
    else if (left_light > 650) Tank_left();
    else if (right_light > 650) Tank_right();
    else Tank_Stop();
    isStopped = false; 

    if (Serial.available()) {
      bluetooth_val = Serial.read();
      if (bluetooth_val == 'Z') flag = 1;
    }
  }
}

void matrix_display(unsigned char matrix_value[]) {
  IIC_start();
  IIC_send(0xc0);
  for (int i = 0; i < 16; i++) IIC_send(matrix_value[i]);
  IIC_end();
  IIC_start();
  IIC_send(0x8A);
  IIC_end();
}

void IIC_start() {
  digitalWrite(SCL_Pin, HIGH); delayMicroseconds(3);
  digitalWrite(SDA_Pin, HIGH); delayMicroseconds(3);
  digitalWrite(SDA_Pin, LOW); delayMicroseconds(3);
}

void IIC_send(unsigned char send_data) {
  for (char i = 0; i < 8; i++) {
    digitalWrite(SCL_Pin, LOW); delayMicroseconds(3);
    digitalWrite(SDA_Pin, (send_data & 0x01) ? HIGH : LOW);
    delayMicroseconds(3);
    digitalWrite(SCL_Pin, HIGH); delayMicroseconds(3);
    send_data >>= 1;
  }
}

void IIC_end() {
  digitalWrite(SCL_Pin, LOW); delayMicroseconds(3);
  digitalWrite(SDA_Pin, LOW); delayMicroseconds(3);
  digitalWrite(SCL_Pin, HIGH); delayMicroseconds(3);
  digitalWrite(SDA_Pin, HIGH); delayMicroseconds(3);
}

void Tank_front() {
  digitalWrite(MR_Ctrl, LOW); analogWrite(MR_PWM, 200);
  digitalWrite(ML_Ctrl, LOW); analogWrite(ML_PWM, 200);
}

void Tank_back() {
  digitalWrite(MR_Ctrl, HIGH); analogWrite(MR_PWM, 200);
  digitalWrite(ML_Ctrl, HIGH); analogWrite(ML_PWM, 200);
}

void Tank_left() {
  digitalWrite(MR_Ctrl, LOW); analogWrite(MR_PWM, 255);
  digitalWrite(ML_Ctrl, HIGH); analogWrite(ML_PWM, 255);
}

void Tank_right() {
  digitalWrite(MR_Ctrl, HIGH); analogWrite(MR_PWM, 255);
  digitalWrite(ML_Ctrl, LOW); analogWrite(ML_PWM, 255);
}

void Tank_Stop() {
  digitalWrite(MR_Ctrl, LOW); analogWrite(MR_PWM, 0);
  digitalWrite(ML_Ctrl, LOW); analogWrite(ML_PWM, 0);

  if (!isAutoMode) {
    matrix_display(STOP01);  
    lastStopTime = millis();
    isStopped = true;
  } else {
     
  }
}
