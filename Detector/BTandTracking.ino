#include <SoftwareSerial.h> 
int buttonpinR = 12; //定义寻线传感器接口
int buttonpinL = 13; 
int buttonpinLL = 7;
int buttonpinRR = 8;  
int valR;
int valL;
int valLL;
int valRR;
int k=0;
#include <Servo.h>
Servo myservo;
int input1 = 5; // 定义uno的pin 5 向 input1 输出
int input2 = 6; // 定义uno的pin 6 向 input2 输出
int input3 = 9; // 定义uno的pin 9 向 input3 输出
int input4 = 10; // 定义uno的pin 10 向 input4 输出

const int TrigPin = 2;
const int EchoPin = 3;
float distance;
int pos = 0;
int turn_check=0;
int judgeF=0;
int judgeS=0;
int delay_time=0;
float * list;
int choose;
void setup()
{ // 初始化串口通信及连接SR04的引脚

  myservo.attach(11);
  Serial.begin(38400);
  pinMode(TrigPin, OUTPUT);
  // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
  pinMode(EchoPin, INPUT);
  Serial.println("Ultrasonic sensor:");
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  pinMode(input3, OUTPUT);
  pinMode(input4, OUTPUT);
  pinMode(buttonpinL, INPUT);
  pinMode(buttonpinR, INPUT);
  pinMode(buttonpinRR, INPUT);
  pinMode(buttonpinLL, INPUT);
  list = (float*)malloc(140 * sizeof(float));
  choose=0;
}

void loop()
{
 while(Serial.available())
  {
    char c=Serial.read();
      if(c=='0')
      {
        Serial.write("idle");
        choose=0;
      }
      if(c=='1')
      {
        Serial.println("BT is ready!");
        // 返回到手机调试程序上
        Serial.write("forward");
        choose=1;
      }
     if(c=='2')
     {
       Serial.write("backward");
       choose=2; 
     }
     if(c=='3')
     {
      Serial.write("TurnLeft");
       choose=3;
     }
     if(c=='4')
     {
      Serial.write("TurnRight");
       choose=4;
     }
     if(c=='5')
     {
      Serial.write("Tracking");
       choose=5;
     }
     if(c=='6')
     {
      Serial.write("Add Speed");
       k+=50;
       if(k>1000) k=1000;
     }
     if(c=='7')
     {
      Serial.write("Slow Speed");
       k-=50;
       if(k<0) k=0;
     }
  }
  if(choose==0)
  {
    Idle();
  }
  if(choose==1)
  {
     Forward();
  }
  if(choose==2)
  {
    Backward();
  }
  if(choose==3)
  {
    TurnLeft(10);
  }
  if(choose==4)
  {
    TurnRight(10);
  }
  if(choose==5)
  {
    k=600;
    valL = digitalRead(buttonpinL); //将数字接口3的值读取赋给val
    valR = digitalRead(buttonpinR);
    valLL = digitalRead(buttonpinLL); //将数字接口3的值读取赋给val
    valRR = digitalRead(buttonpinRR);
    judgeF=0;
    judgeS=0;
    if(valL==HIGH) judgeS+=1;
    if(valR==HIGH) judgeS+=2;
    if(valLL==HIGH) judgeF+=1;
    if(valRR==HIGH) judgeF+=2;
    switch(judgeF)
    {
      case 0:
            switch(judgeS)
            {
              case 0:
                    delay_time=0;
                    Forward();
                    break;
              case 1:
                    delay_time=0;
                    TurnLeft(delay_time);
                    break;
              case 2:
                    delay_time=0;
                    TurnRight(delay_time);
                    break;
              case 3:
                    delay_time=0;
                    Forward();
                    break;
            }
            break;
      case 1:
            TurnLeft(500);
            break;
      case 2:
            TurnRight(500);
            break;
      case 3:        
            delay_time=0;
            Idle();
            break; 
    }
    turn_check=judgeS;
  }
}

float Search()
{

  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  // 检测脉冲宽度，并计算出距离
  distance = pulseIn(EchoPin, HIGH) / 58.00;
  return distance;
}
void sweep()
{
  int i = 0;
  for (pos = 15; pos < 155; pos += 1) {
    // in steps of 1 degree
    myservo.write(pos);
    delay(5);
    distance = Search();
    list[i] = distance;
    i++;
  }
}

void TurnLeft(int theta)
{
  analogWrite(5, 255);
  analogWrite(6, 0);
  analogWrite(9, 255);
  analogWrite(10, 0);
  delay(theta);
}
void TurnRight(int theta)
{
  analogWrite(5, 0);
  analogWrite(6, 255);
  analogWrite(9, 0);
  analogWrite(10, 255);
  delay(theta);
}

void MoveTurnLeft()
{
  analogWrite(5, 255);
  analogWrite(6, 255);
  analogWrite(9, 255);
  analogWrite(10, 0);
  delay(400);
}
void MoveTurnRight()
{
  analogWrite(5, 0);
  analogWrite(6, 255);
  analogWrite(9, 255);
  analogWrite(10, 255);
  delay(150);
}

void Idle()
{
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, 0);
  analogWrite(10, 0);
  delay(150);
  
}
void Forward()
{
//  analogWrite(5, 0);
//  analogWrite(6, 255);
//  analogWrite(9, 255);
//  analogWrite(10, 0);
  digitalWrite(5, LOW);
 digitalWrite(6, HIGH);
 digitalWrite(9, HIGH);
 digitalWrite(10, LOW);
 delayMicroseconds(k);
 digitalWrite(5, LOW);
 digitalWrite(6, LOW);
 digitalWrite(9, LOW);
 digitalWrite(10, LOW);
 delayMicroseconds(1000-k);
}
void Backward()
{
  analogWrite(5, 255);
  analogWrite(6, 0);
  analogWrite(9, 0);
  analogWrite(10, 255);
  //delay(300);
}
