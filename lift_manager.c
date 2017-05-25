#include "message.h"

void listenMsg();
void sigHandle(int sigNo);
int insertRequest(int floor);
void excuteRequest();
void excuteCtrl(int floor);
void changeRequest();
void sendCurrentfloor();
void menu();
void menu_bar();
void nextRequest();

int requests[MAX_REQUEST];
int requests_size = 0;
int current_floor = 1;

int main(int argc, char const *argv[]) {
  int i;
  for (i = 0; i < MAX_REQUEST; i++) {
    requests[i] = 0;
  }

  signal(SIGINT, sigHandle);
  if(fork() == 0)
    listenMsg();

  menu();
  return 0;
}


void listenMsg() {
  int msqid, mscid;
  msg rcvbuffer;
  while(1) {
    msqid = msgget(MSG_KEY_M, PERMISSION);
    if(msgrcv(msqid, &rcvbuffer, MSG_SIZE, 1, 0) >= 0) {
      char str[MSG_SIZE];
      strcpy(str, rcvbuffer.mtext);

      if(str[0] == 'f') {
        int begin_floor = str[1] - '0';
        int end_floor = str[2] - '0';
        if(current_floor != 1)
          insertRequest(begin_floor * 10 + end_floor);
        else {
          msg s;
          s.mtype = 1;
          s.mtext[0] = 'm';
          s.mtext[1] = begin_floor + '0';
          s.mtext[2] = end_floor + '0';
          s.mtext[3] = '\0';
          sendMessage(MSG_KEY_C, s);
        }
      } else if(str[0] == 'c') {
        current_floor = str[1] - '0';
        sendCurrentfloor();
        if(current_floor == 1 && requests_size > 0) {
          nextRequest();
        }
      } else {
        system("killall ./lift_manager");
      }
    }
  }
}

void nextRequest() {
  int num = requests[0];
  int begin_floor = num / 10;
  int end_floor = num % 10;
  msg s;
  s.mtype = 1;
  s.mtext[0] = 'm';
  s.mtext[1] = begin_floor + '0';
  s.mtext[2] = end_floor + '0';
  s.mtext[3] = '\0';
  sendMessage(MSG_KEY_C, s);
  changeRequest();
}

// Khi an ctrl + c thi tat het tien trinh lien quan toi floor
void sigHandle(int sigNo) {
  kill_all("kill all");
  return;
}

//Chen request vao hang doi
int insertRequest(int floor) {
  if(requests_size < MAX_REQUEST) {
    requests[requests_size] = floor;
    requests_size++;
    return 1;
  }
  return 0;
}

//Giam hang doi xuong sau khi da xu li
void changeRequest() {
  int i;
  for (i = 0; i < MAX_REQUEST - 1; i++) {
    requests[i] = requests[i + 1];
  }
  requests[MAX_REQUEST - 1] = 0;
  requests_size--;
}

// Gui cho cac tang vi tri thang may hien tai
void sendCurrentfloor() {
  msg sndbuffer;
  sndbuffer.mtype = 1;
  sndbuffer.mtext[0] = 'm';
  sndbuffer.mtext[1] = current_floor + '0';
  sndbuffer.mtext[2] = '\0';
  sendAll(sndbuffer);
}

// Menu de quan ly cac tang
void menu(int floor, int current_floor) {
  int choose = 0;
  do {
    menu: menu_bar();
    scanf("%d", &choose);
    if(choose) {
      printf("Your choose is not valid. Please choose again!\n");
      goto menu;
    }
  } while(choose);
  system("killall ./lift_manager");
}

void menu_bar() {
  printf("\n\nFloor controller is running\n");
  printf("0 - Exit\n");
  printf("==> Your choose: \n");
}
