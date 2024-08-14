#include <AFMotor.h>
#include <Servo.h>

// DC 모터 제어를 위한 객체 생성
AF_DCMotor motor1(1); // 1번 모터
AF_DCMotor motor2(2); // 2번 모터

// 서보 모터 제어를 위한 객체 생성
Servo ms;

int pi = 3.1415926535;
int wheelLeng = pi * 65; // mm


void setup() {
  Serial.begin(115200);  // 시리얼 통신 시작
  Serial.println("Motor and Servo control ready!");
  Serial3.begin(9600);
  

  // 모터 초기 설정
  motor1.setSpeed(200); // 1번 모터 속도 설정 (0-255)
  motor2.setSpeed(200); // 2번 모터 속도 설정 (0-255)
  motor1.run(RELEASE);  // 1번 모터 초기 상태를 RELEASE로 설정
  motor2.run(RELEASE);  // 2번 모터 초기 상태를 RELEASE로 설정

  // 서보 모터 초기 설정
  ms.attach(9);         // 서보 모터를 9번 핀에 연결
  ms.write(50);         // 서보 모터 초기 방향 설정 (50도)
}
int flag = 0;
void loop(){
  /*
  if (Serial3.available()) {
    String input = Serial3.readString();
    input.trim();
    if (input.length() > 0 && isDigit(input.charAt(0))) { // 입력이 숫자인 경우
      int distance = input.toInt();
      moveExactDistance(distance);
    }
  }*/
  if(Serial3.available()) {
    char cmd = Serial3.read();
    if(cmd == 'p'){
      complexMovement();
    }
    if(cmd == 'm'){
      moCtrl();
    }
  }
}








void moCtrl() {
  if (Serial3.available()) { // 시리얼 입력이 있을 경우
    String command = Serial3.readString(); // 시리얼로부터 문자열 읽기
    command.trim(); // 공백 제거
    
    if (command.equalsIgnoreCase("f")) { // 'f' 또는 'F' 입력 시
      moCtrl_F();
      
    }
    else if (command.equalsIgnoreCase("b")) { // 'b' 또는 'B' 입력 시
      moCtrl_B();
      
    }
    else if (command.equalsIgnoreCase("lf")) { // 'lf' 또는 'LF' 입력 시
      moCtrl_LF();
      
    }
    else if (command.equalsIgnoreCase("rf")) { // 'rf' 또는 'RF' 입력 시
      moCtrl_RF();
      
    }
    else if (command.equalsIgnoreCase("lb")) { // 'lb' 또는 'LB' 입력 시
      moCtrl_LB();
      
    }
    else if (command.equalsIgnoreCase("rb")) { // 'rb' 또는 'RB' 입력 시
      moCtrl_RB(); 
      
    }
    else {
      motor1.run(RELEASE); // 입력이 없거나 다른 입력이 들어오면 모터 정지
      motor2.run(RELEASE); 
      
    }
  }
}
void moCtrl_F(){
  motor1.run(BACKWARD);  // 1번 모터 전진
  motor2.run(BACKWARD);  // 2번 모터 전진
  delay(500);          // 5초간 대기
  motor1.run(RELEASE);   // 1번 모터 정지
  motor2.run(RELEASE);   // 2번 모터 정지
  return;
}
void moCtrl_B(){
  motor1.run(FORWARD);  // 1번 모터 후진
  motor2.run(FORWARD);  // 2번 모터 후진
  delay(500);         // 5초간 대기
  motor1.run(RELEASE);  // 1번 모터 정지
  motor2.run(RELEASE);  // 2번 모터 정지
  return;    
}
void moCtrl_LF(){
  ms.write(70);        // 서보 모터 왼쪽으로 조향 (80도)
  delay(500);         // 0.5초 대기
  motor1.run(BACKWARD);  // 1번 모터 전진
  motor2.run(BACKWARD);  // 2번 모터 전진
  delay(1000);          // 5초간 대기
  motor1.run(RELEASE);   // 1번 모터 정지
  motor2.run(RELEASE);   // 2번 모터 정지
  ms.write(50);         // 서보 모터 중앙으로 복귀
  return;
}
void moCtrl_RF(){
  ms.write(30);        // 서보 모터 오른쪽으로 조향 (20도)
  delay(500);         // 0.5초 대기
  motor1.run(BACKWARD);  // 1번 모터 전진
  motor2.run(BACKWARD);  // 2번 모터 전진
  delay(1000);          // 10초간 대기
  motor1.run(RELEASE);   // 1번 모터 정지
  motor2.run(RELEASE);   // 2번 모터 정지
  ms.write(50);         // 서보 모터 중앙으로 복귀
  return;
}
void moCtrl_LB(){
  ms.write(70);        // 서보 모터 왼쪽으로 조향 (80도)
  delay(1000);         // 0.5초 대기
  motor1.run(FORWARD);  // 1번 모터 후진
  motor2.run(FORWARD);  // 2번 모터 후진
  delay(1000);          // 5초간 대기
  motor1.run(RELEASE);   // 1번 모터 정지
  motor2.run(RELEASE);   // 2번 모터 정지
  ms.write(50);         // 서보 모터 중앙으로 복귀
  return;
}
void moCtrl_RB(){
  ms.write(25);        // 서보 모터 오른쪽으로 조향 (20도)
  delay(100);         // 0.5초 대기
  motor1.run(FORWARD);  // 1번 모터 후진
  motor2.run(FORWARD);  // 2번 모터 후진
  delay(1000);          // 5초간 대기
  motor1.run(RELEASE);   // 1번 모터 정지
  motor2.run(RELEASE);   // 2번 모터 정지
  ms.write(50);         // 서보 모터 중앙으로 복귀
  return;
}

void moveExactDistance(int requestedDistance) {
  float distanceErrorFactor = 0.1; // 10cm당 1cm의 오차 보정
  int baseDistance = 10; // 기준 거리 (cm)
  int baseTime = 500; // 기준 이동 시간 (ms) for 10cm

  // 보정된 거리 계산
  float correctedDistance = requestedDistance * (1.0 - (requestedDistance / baseDistance) * distanceErrorFactor);

  // 이동 시간 계산
  int timeToMove = (int)(baseTime * (correctedDistance / baseDistance));

  // 모터를 작동하여 이동
  motor1.run(BACKWARD); // 1번 모터 전진
  motor2.run(BACKWARD); // 2번 모터 전진
  delay(timeToMove); // 계산된 시간만큼 이동
  motor1.run(RELEASE); // 1번 모터 정지
  motor2.run(RELEASE); // 2번 모터 정지
}
void complexMovement() {
  motor2.setSpeed(220);
  for(int ang = 20;ang<=80;ang+=30){    
    // 좌측으로 20도 조정 후 전진
    ms.write(ang);         // 서보 모터를 좌측 70도로 조정
    delay(500);
    motor1.run(FORWARD);  // 모터 정지
    motor2.run(FORWARD);  // 모터 정지
    delay(1000);           // 잠시 대기
    motor1.run(BACKWARD); // 1번 모터 전진
    motor2.run(BACKWARD); // 2번 모터 전진
    delay(500);           // 짧은 시간 전진
    motor1.run(RELEASE);  // 모터 정지
    motor2.run(RELEASE);  // 모터 정지
    delay(500);
  }
  // 서보 모터를 초기 위치(중앙)로 복귀
  ms.write(50);         
}
