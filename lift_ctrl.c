#include "message.h"

void listenMsg();
void sigHandle(int sigNo);
int min(int a, int b);
int max(int a, int b);
void calculate();

int begin_floor, end_floor;

int main(int argc, char const *argv[]) {
  signal(SIGINT, sigHandle);
  listenMsg();
  return 0;
}

// Lang nghe tin nhan de cap nhat vi tri cua thang may
void listenMsg() {
  int msqid;
  msg rcvbuffer;
  while(1) {
    msqid = msgget(MSG_KEY_C, PERMISSION);
    if(msgrcv(msqid, &rcvbuffer, MSG_SIZE, 1, 0) >= 0) {
      if(rcvbuffer.mtext[0] == 'm') {
        begin_floor = rcvbuffer.mtext[1] - '0';
        end_floor = rcvbuffer.mtext[2] - '0';
        calculate();
      } else if(rcvbuffer.mtext[0] == 's') {
        msg s;
        s.mtype = 1;
        s.mtext[0] = 'c';
        s.mtext[1] = rcvbuffer.mtext[1];
        s.mtext[2] = '\0';
        sendMessage(MSG_KEY_M, s);
      } else {
        system("killall ./lift_ctrl");
      }
    }
  }
}

// Khi an ctrl + c thi tat het tien trinh lien quan toi floor
void sigHandle(int sigNo) {
  kill_all("kill all");
  return;
}

// Tinh toan thoi gian chuyen dong
void calculate() {
  msg s;
  s.mtype = 1;
  int i = 0;
  int timeEachFloor = (int)(HIGHT_FLOOR / SPEED);
  int distance = begin_floor - end_floor;
  if(distance < 0) distance = 0 - distance;
  if(begin_floor == 1) {
    s.mtext[0] = 's';
    s.mtext[1] = '3';
    s.mtext[2] = '\0';
    sendMessage(MSG_KEY_B, s);
  }

  for(i = 0; i < distance; i++) {
    s.mtext[0] = 'u';
    s.mtext[1] = timeEachFloor + '0';
    s.mtext[2] = '\0';
    sendMessage(MSG_KEY_B, s);
  }

  s.mtext[0] = 's';
  s.mtext[1] = '3';
  s.mtext[2] = '\0';
  sendMessage(MSG_KEY_B, s);

  for(i = 0; i < distance; i++) {
    s.mtext[0] = 'd';
    s.mtext[1] = timeEachFloor + '0';
    s.mtext[2] = '\0';
    sendMessage(MSG_KEY_B, s);
  }

  if(end_floor == 1) {
    s.mtext[0] = 's';
    s.mtext[1] = '3';
    s.mtext[2] = '\0';
    sendMessage(MSG_KEY_B, s);
  }

}

int max(int a, int b) {
  if(a > b) return a;
  else return b;
}

int min(int a, int b) {
  if(a < b) return a;
  else return b;
}

