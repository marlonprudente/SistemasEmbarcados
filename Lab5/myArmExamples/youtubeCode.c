#define Uservo 0   // define NO. of up servo  
#define Bservo 1  // define NO. of bottom servo
#define Rservo 2  // define NO. of right servo
#define Lservo 3  // define NO. of lift servo

int servoConfig[4][4]={
      // Pin,    min,      max,     initialAngle
        { 2,       0,       50,               40},   // UpServo
        { 3,      10,      170,      Org[0] },  // ButtomServo
        {4,      40,      160,      Org[1] },   // RightServo
        {5,      80,      170,      Org[2]}    // LeftServo
}; 

Servo mearm[4];   
int x0,y0;  // the coordinate of the mearm

void setup() {
  for(int i=0;i<4;i++){
    mearm[i].attach(servoConfig[i][0]);
    mearm[i].write(servoConfig[i][3]);
    delay(150);
  }
 
}


// move servo form st angle to ed angle

void mearmmove(int servoNO, int st, int ed) {
  int ds=1;
  int dsNum=abs(ed-st);
  int dt=1;
  
  if(ed<st)dt=-1;
  
  for(int i=0;i<=dsNum;i+=ds){
    mearm[servoNO].write(st+i*dt);
    delay(15);
  }
 
}



// move meArm to the point (x,y)
// x be the angle of bottom servo
// y be the angle of right servo

void movetopoint(int x, int y) {
  z1 = 106 +  (y - 126);  // set the height of the pen
  motormove(Bservo, x0, x);
  motormove(Rservo, y0, y);
  if(y<122)z1=90;
  if (pendown) {
    motormove(Lservo, z0, z1); //move the pen
    z0 = z1;
  }
  
  y0 = y;
  x0 = x;

}


// move the meArm to draw a line form (x,y) to (xx,yy)

void drawline(int x, int y, int xx, int yy) {
  pendown = true;
  movetopoint(x, y);
  
  if (xx > x) {
    float dy = (float)(yy - y) / (xx - x);
    for (int i = x; i < xx; i++) {
       movetopoint(i, y + dy * (i - x));
    }
  }
  else if (xx < x) {
    float dy = (float)(yy - y) / (xx - x);
    for (int i = x; i > xx; i--) {
      movetopoint(i, y + dy * (i - x));
    }
  }
  else {
    if(y<yy){
      for(int i=y;i<yy;i++){
        movetopoint(x,i);
      }
    }
    else{
      for(int i=y;i>yy;i--){
        movetopoint(x,i);
      }
    }

  }

}﻿
 

