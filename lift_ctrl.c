#include "message.h"

void listenMsg();
void sigHandle(int sigNo);
void excuteRequest(char *str);
void calculate();
int min(int a, int b);
int max(int a, int b);

int begin_floor, end_floor;

int main(int argc, char const *argv[]) {
  signal(SIGINT, sigHandle);
  listenMsg();
  return 0;
}

// Khi an ctrl + c thi tat het tien trinh lien quan toi floor
void sigHandle(int sigNo) {
  system("killall ./lift_ctrl");
  return;
}

// Lang nghe tin nhan de cap nhat vi tri cua thang may
void listenMsg() {
  int msqid;
  msg rcvbuffer;
  while(1) {
    msqid = msgget(MSG_KEY_C, PERMISSION);
    if(msgrcv(msqid, &rcvbuffer, MSG_SIZE, 1, 0) >= 0) {
      excuteRequest(rcvbuffer.mtext);
    }
  }
}

void excuteRequest(char *str) {
  if(str[0] == 'm') {
    begin_floor = str[1] - '0';
    end_floor = str[2] - '0';
    printf("Receive a request, from FLOOR %d to FLOOR %d\n", begin_floor,
      end_floor);
    calculate();
  } else if(str[0] == 's') {
    msg s;
    s.mtype = 1;
    s.mtext[0] = 'c';
    s.mtext[1] = str[1];
    s.mtext[2] = '\0';
    sendMessage(MSG_KEY_M, s);
    if(str[1] == '1') {
      printf("Finish the request, from FLOOR %d to FLOOR %d\n",
        begin_floor, end_floor);
    }
  } else {
    system("killall ./lift_ctrl");
  }
}

// Tinh toan thoi gian chuyen dong
void calculate() {
  msg s;
  s.mtype = 1;
  int i = 0;
  int timeEachFloor = (int)(HIGHT_FLOOR / SPEED);
  int distance = begin_floor - end_floor;
  if(distance < 0) distance = 0 - distance;

  printf("Excuting the request, from FLOOR %d to FLOOR %d\n", begin_floor,
    end_floor);

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

