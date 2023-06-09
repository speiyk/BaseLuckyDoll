/**
 * @file .ino
 *
 * Project Base Lucky Doll
 *
 * @author  S.Sarawut
 * @date    2223/06/08
 */
#define PINA_MOVE_X   1
#define PINA_MOVE_Y   2
#define PIN_INTOKEN   3
#define PIN_BUTTON_OK 4
#define PIN_OUTPWM_MOTOR_X     5
#define PIN_OUTPWM_MOTOR_Y     6
#define PIN_OUTFORWARD_MOTOR_X 7
#define PIN_OUTREVERSE_MOTOR_X 8
#define PIN_OUTFORWARD_MOTOR_Y 9
#define PIN_OUTREVERSE_MOTOR_Y 10
#define PIN_OUTFORWARD_MOTOR_Z 11
#define PIN_OUTREVERSE_MOTOR_Z 12
#define PIN_OUTGRAB            13
#define PIN_LIMIT_AXIS_X0      14
#define PIN_LIMIT_AXIS_X1      15
#define PIN_LIMIT_AXIS_Y0      16
#define PIN_LIMIT_AXIS_Y1      17
#define PIN_LIMIT_AXIS_Z0      18
#define PIN_LIMIT_AXIS_Z1      19

bool isToken = false;
bool isOK = false;
bool recordt_old = false;
int ix = 0;
int num_Token = 0;
int t_countdown = 0;
const int pwm_channel_x = 0;
const int pwm_channel_y = 0;
const int frequence = 5000;
const int resolution = 8;
unsigned long t_old = 0;

void readToken(){
  isToken = true;
  if(isToken) num_Token++; isToken = false;
}

void readButtonOK(){
  if(ix == 1 && recordt_old == true){ isOK = true; }
}

void motor_stop_x() { 
  digitalWrite(PIN_OUTFORWARD_MOTOR_X, LOW);
  digitalWrite(PIN_OUTREVERSE_MOTOR_X, LOW);
  ledcWrite(PIN_OUTPWM_MOTOR_X, 00);
}

void motor_stop_y() { 
  digitalWrite(PIN_OUTFORWARD_MOTOR_Y, LOW);
  digitalWrite(PIN_OUTREVERSE_MOTOR_Y, LOW);
  ledcWrite(PIN_OUTPWM_MOTOR_Y, 00);
}

void motor_forward_x(uint8_t speed, uint8_t st_brake) { 
  if(st_brake == LOW) {     
  	digitalWrite(PIN_OUTFORWARD_MOTOR_X, LOW);
  	digitalWrite(PIN_OUTREVERSE_MOTOR_X, HIGH);
  	ledcWrite(PIN_OUTPWM_MOTOR_X, speed);
  }
  else { motor_stop_x(); }	  
}

void motor_reverse_x(uint8_t speed, uint8_t st_brake) { 
  if(st_brake == LOW) {
    digitalWrite(PIN_OUTFORWARD_MOTOR_X, HIGH);
    digitalWrite(PIN_OUTREVERSE_MOTOR_X, LOW);
    ledcWrite(PIN_OUTPWM_MOTOR_X, speed);
  }
  else { motor_stop_x(); }
}

void motor_forward_y(uint8_t speed, uint8_t st_brake) {  
  if(st_brake == LOW) {
    digitalWrite(PIN_OUTFORWARD_MOTOR_Y, LOW);
  	digitalWrite(PIN_OUTREVERSE_MOTOR_Y, HIGH);
  	ledcWrite(PIN_OUTPWM_MOTOR_Y, speed);
  }
  else { motor_stop_y(); }
}

void motor_reverse_y(uint8_t speed, uint8_t st_brake) { 
  if(st_brake == LOW) {
    digitalWrite(PIN_OUTFORWARD_MOTOR_Y, HIGH);
    digitalWrite(PIN_OUTREVERSE_MOTOR_Y, LOW);
    ledcWrite(PIN_OUTPWM_MOTOR_Y, speed);
  }
  else { motor_stop_y(); }
}

void setup(){
  pinMode(PIN_INTOKEN, INPUT);
  pinMode(PIN_BUTTON_OK, INPUT); 
  pinMode(PIN_LIMIT_AXIS_X0, INPUT);  
  pinMode(PIN_LIMIT_AXIS_X1, INPUT);  
  pinMode(PIN_LIMIT_AXIS_Y0 , INPUT);  
  pinMode(PIN_LIMIT_AXIS_Y1, INPUT);  
  pinMode(PIN_LIMIT_AXIS_Z0, INPUT);
  pinMode(PIN_LIMIT_AXIS_Z1, INPUT);

  pinMode(PIN_OUTPWM_MOTOR_X, OUTPUT); 
  pinMode(PIN_OUTPWM_MOTOR_Y, OUTPUT); 
  pinMode(PIN_OUTFORWARD_MOTOR_X, OUTPUT); 
  pinMode(PIN_OUTREVERSE_MOTOR_X, OUTPUT); 
  pinMode(PIN_OUTFORWARD_MOTOR_Y, OUTPUT); 
  pinMode(PIN_OUTREVERSE_MOTOR_Y, OUTPUT); 
  pinMode(PIN_OUTFORWARD_MOTOR_Z, OUTPUT); 
  pinMode(PIN_OUTREVERSE_MOTOR_Z, OUTPUT); 
  pinMode(PIN_OUTGRAB, OUTPUT); 

  ledcSetup(pwm_channel_x, frequence, resolution);
  ledcSetup(pwm_channel_y, frequence, resolution);
  ledcAttachPin(PIN_OUTPWM_MOTOR_X, pwm_channel_x);
  ledcAttachPin(PIN_OUTPWM_MOTOR_Y, pwm_channel_y);
  
  attachInterrupt(digitalPinToInterrupt(PIN_INTOKEN), readToken, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_OK), readButtonOK, CHANGE);

  Serial.begin(115200);
  Serial.println("- Starting Lucky Doll...");
}

void loop(){
  //1023, 515, 0
  int var_move_x = analogRead(PINA_MOVE_X);
  int var_move_y = analogRead(PINA_MOVE_Y);

  int var_safety_x0 = digitalRead(PIN_LIMIT_AXIS_X0);
  int var_safety_x1 = digitalRead(PIN_LIMIT_AXIS_X1);
  int var_safety_y0 = digitalRead(PIN_LIMIT_AXIS_Y0 );
  int var_safety_y1 = digitalRead(PIN_LIMIT_AXIS_Y1);
  int var_move_z0   = digitalRead(PIN_LIMIT_AXIS_Z0);
  int var_move_z1   = digitalRead(PIN_LIMIT_AXIS_Z1);
  
  switch (ix) {
    case 0:
      if(num_Token > 0){ ix++; }
      break;

    case 1:
      if(recordt_old == false && (var_move_x != 515 || var_move_y != 515)){ 
        recordt_old = true;
        num_Token--; 
        t_countdown = 30;      
      }

      if(var_move_x < 515){
        var_move_x = map(var_move_x, 515, 0, 50, 255);
        motor_forward_x(var_move_x, var_safety_x0);
      }
      else if(var_move_x > 515){
        var_move_x = map(var_move_x, 515, 1023, 50, 255);
        motor_reverse_x(var_move_x, var_safety_x1);
      }
      else{
        motor_stop_x();
      }

      if(var_move_y < 515){
        var_move_y = map(var_move_y, 515, 0, 50, 255);
        motor_forward_y(var_move_y, var_safety_y0);
      }
      else if(var_move_y > 515){
        var_move_y = map(var_move_y, 515, 1023, 50, 255);
        motor_reverse_y(var_move_y, var_safety_y1);
      }
      else{
        motor_stop_y();
      }

      if(isOK || t_countdown == 0){
        isOK = false;
        recordt_old = false;
        ix++;      
        t_countdown = 0;                        
        
        motor_stop_x();
        motor_stop_y();
      }    
      break;

    case 2:
      if(var_move_z0 == LOW){
        ix++;
        t_countdown = 10;

        digitalWrite(PIN_OUTFORWARD_MOTOR_Z, LOW);
        digitalWrite(PIN_OUTREVERSE_MOTOR_Z, LOW);
      }
      else{
        digitalWrite(PIN_OUTFORWARD_MOTOR_Z, LOW);
        digitalWrite(PIN_OUTREVERSE_MOTOR_Z, HIGH); 
      }     
      break;

    case 3:
      if(t_countdown == 0){ ix++; }
      break;

    case 4:
      ix++;
      t_countdown = 10;
      digitalWrite(PIN_OUTGRAB, LOW); 
      break;

    case 5:
      if(t_countdown == 0){ ix++; }
      break;

    case 6:
      if(var_move_z1 == LOW){
        ix++;

        digitalWrite(PIN_OUTFORWARD_MOTOR_Z, LOW);
        digitalWrite(PIN_OUTREVERSE_MOTOR_Z, LOW);
      }
      else{
        digitalWrite(PIN_OUTFORWARD_MOTOR_Z, HIGH);
        digitalWrite(PIN_OUTREVERSE_MOTOR_Z, LOW); 
      }     
      break;

    case 7:    
      if(var_safety_x1 == LOW && var_safety_y1 == LOW){
        ix++;
        t_countdown = 10;

        motor_stop_x();
        motor_stop_y();
        digitalWrite(PIN_OUTGRAB, HIGH);
      }
      else{
        motor_reverse_x(100, var_safety_x1);      
        motor_reverse_y(100, var_safety_y1);
      }
      break;

    case 8:
      if(t_countdown == 0){ ix=0; }
      break;
  }

  while ((micros() - t_old) < 1000000L); t_old = micros();
  if(t_countdown > 0) { t_countdown--; }
}