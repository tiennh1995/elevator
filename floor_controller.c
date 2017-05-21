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

int requests[MAX_REQUEST];
int requests_size = 0;
msg sndbuffer;
int current_floor = 1;

int main(int argc, char const *argv[]) {
  int i;
  sndbuffer.mtype = 1;
  for (i = 0; i < MAX_REQUEST; i++) {
    requests[i] = 0;
  }

  signal(SIGINT, sigHandle);
  if(fork() == 0)
    listenMsg();
  menu();
  return 0;
}

// Lang nghe tin nhan de cap nhat vi tri cua thang may
void listenMsg() {
  int msqid;
  msg rcvbuffer;
  while(1) {
    if((msqid = msgget(MSG_KEY_S, PERMISSION)) < 0)
      die("msgget()", "killall ./floor_controller");

    if(msgrcv(msqid, &rcvbuffer, MSG_SIZE, 1, 0) >= 0) {
      int floor = rcvbuffer.mtext[0] - '0';
      insertRequest(floor);
      excuteRequest();
    }
  }
}

// Khi an ctrl + c thi tat het tien trinh lien quan toi floor
void sigHandle(int sigNo) {
  system("killall ./floor_controller");
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

// Thuc hien yeu cau
void excuteRequest() {
  if(requests_size) {
    int floor = requests[0];
    excuteCtrl(floor);
    changeRequest();
  }
}

// Tinh toan thoi gian di chuyen
void excuteCtrl(int floor) {
  int distance, times;
  if(floor != 1) {
    while(current_floor !=  floor) {
      sleep(TIME_WAIT);
      current_floor++;
      sendCurrentfloor();
    }
    sleep(TIME_WAIT);
    while(current_floor != 1) {
      sleep(TIME_WAIT);
      current_floor--;
      sendCurrentfloor();
    }
  }
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
  sndbuffer.mtext[0] = current_floor + '0';
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
  system("killall ./floor_controller");
}

void menu_bar() {
  printf("\n\nFloor controller is running\n");
  printf("0 - Exit\n");
  printf("==> Your choose: \n");
}
