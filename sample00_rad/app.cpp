#include "app.h"
#include "util.h"

#include "Motor.h"
#include "Clock.h"
#include "math.h"

using namespace ev3api;

// static float targetDist = 0.0;
// static float targetRad = 0.0;
// void calcTargetDist(int x1, int y1, int x2, int y2);
// void calcTargetRad(int x1, int y1, int x2, int y2);

/**
 * メインタスク
 */
void main_task(intptr_t unused) {

  Motor leftWheel(PORT_C); // <1>
  Motor rightWheel(PORT_B);  // <2>
  Clock clock; // <3>

#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif

  const uint32_t duration = 2000*1000; // <1>

  init_f(__FILE__);

  int pre_left_angle= leftWheel.getCount();
  int pre_right_angle= rightWheel.getCount();
  float distance_left = 0;
  float distance_right = 0;
  float distance = 0;
  float rad = 0;
  const int wheel_diameter = 100;

  leftWheel.setPWM(pwm);
  rightWheel.setPWM(pwm);

  

  while(1) {
    //msg_f("Forwarding...", 1);
    //leftWheel.setPWM(pwm);
    //rightWheel.setPWM(pwm);
    //clock.sleep(duration); // <1>

    //msg_f("Backwarding...", 1);
    //leftWheel.setPWM(-pwm);
    //rightWheel.setPWM(-pwm);
    //clock.sleep(duration);

    int cur_left_angle = leftWheel.getCount();
    int cur_right_angle = rightWheel.getCount();
    //int surrent_right_angle = rightWheel.getCount();
    //distance_left = distance_left + ((81*3)/360)*(cur_left_angle - pre_left_angle);
    distance_left = wheel_diameter*M_PI*(cur_left_angle - pre_left_angle)/360;
    distance_right = wheel_diameter*M_PI*(cur_right_angle - pre_right_angle)/360;
    distance += (distance_left + distance_right)/2;
    rad += (distance_left - distance_right)*180/(M_PI*145);
    if(distance > 400.0){
      leftWheel.setPWM(0);
      rightWheel.setPWM(0);
    }
    if(rad > 90){
      leftWheel.setPWM(0);
      rightWheel.setPWM(0);
    }

    printf("distance_left = %f\n",distance_left);
    printf("distance_right = %f\n",distance_right);
    printf("distance = %f\n",distance);
    printf("rad = %f\n",rad);
    pre_left_angle = cur_left_angle;
    pre_right_angle = cur_right_angle;
    uint64_t time = clock.now();
    printf("time =%llu\n",time);

    if(time > 24000000){
      leftWheel.setPWM(0);
      rightWheel.setPWM(0);
    }

    clock.sleep(duration/10);

    // 左ボタンを長押し、それを捕捉する
    if (ev3_button_is_pressed(LEFT_BUTTON)) {
      break;
    }
  }

  msg_f("Stopped.", 1);
  leftWheel.stop();
  rightWheel.stop();
  while(ev3_button_is_pressed(LEFT_BUTTON)) {
    ;
  }

  ext_tsk();
}


// void calcTargetDist(int x1, int y1, int x2, int y2){
//   targetDist = sqrt(pow((float)(x2 - x1),2) + pow((float)(y2 - y1),2));
// }


// void calsTargetRad(int x1, int y1, int x2, int y2){

//   targetRad = atan2((float)(y2 - y1),(float)(x2 - x1));
//   targetRad = targetRad * 180.0/M_PI;
// }