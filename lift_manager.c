#include "message.h"

void sigHandle(int sigNo);
void listenMsg();
void excuteRequest();
int insertRequest(int floor);
void nextRequest();
void changeRequest();
void sendCurrentfloor();

int requests[MAX_REQUEST], requests_size = 0, current_floor = 1, can_send = 1,
  old_begin_floor, old_end_floor;

int main(int argc, char const *argv[]) {
  int i;
  for (i = 0; i < MAX_REQUEST; i++) {
    requests[i] = 0;
  }
  signal(SIGINT, sigHandle);
  listenMsg();
  return 0;
}

// Khi an ctrl + c thi tat het tien trinh lien quan toi floor
void sigHandle(int sigNo) {
  system("killall ./lift_manager");
  return;
}

// Lang nghe yeu cau
void listenMsg() {
  int msqid;
  msg rcvbuffer;
  while(1) {
    msqid = msgget(MSG_KEY_M, PERMISSION);
    if(msgrcv(msqid, &rcvbuffer, MSG_SIZE, 1, 0) >= 0) {
      excuteRequest(rcvbuffer.mtext);
    }
  }
}

// Thuc hien yeu cau tu ben floor, ctrl
void excuteRequest(char *str) {
  if(str[0] == 'f') {
    int begin_floor = str[1] - '0';
    int end_floor = str[2] - '0';
    printf("Receive a request, from FLOOR %d to FLOOR %d\n", begin_floor,
      end_floor);
    if(!insertRequest(begin_floor * 10 + end_floor)) {
      int req_key = str[1] - '0';
      msg s;
      s.mtype = 1;
      s.mtext[0] = 'r';
      sendMessage(req_key, s);
    }
  } else if(str[0] == 'c') {
    current_floor = str[1] - '0';
    sendCurrentfloor();

    if(current_floor == 1) {
      printf("Finish the request, from FLOOR %d to FLOOR %d\n",
        old_begin_floor, old_end_floor);
      can_send = 1;

      if(requests_size)
        nextRequest();
    }
  } else {
    system("killall ./lift_manager");
  }
}

// Chen request vao hang doi
int insertRequest(int floor) {
  if(requests_size < MAX_REQUEST) {
    requests[requests_size] = floor;
    requests_size++;

    if(requests_size)
      nextRequest();
    return 1;
  }

  return 0;
}

// Thuc hien request tiep theo
void nextRequest() {
  if(can_send) {
    int num = requests[0];
    old_begin_floor = num / 10;
    old_end_floor = num % 10;
    msg s;
    s.mtype = 1;
    s.mtext[0] = 'm';
    s.mtext[1] = old_begin_floor + '0';
    s.mtext[2] = old_end_floor + '0';
    s.mtext[3] = '\0';
    sendMessage(MSG_KEY_C, s);
    changeRequest();
    printf("Excuting the request, from FLOOR %d to FLOOR %d\n",
      old_begin_floor, old_end_floor);
    can_send = 0;
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
  msg sndbuffer;
  sndbuffer.mtype = 1;
  sndbuffer.mtext[0] = 'm';
  sndbuffer.mtext[1] = current_floor + '0';
  sndbuffer.mtext[2] = '\0';
  sendAll(sndbuffer);
}
