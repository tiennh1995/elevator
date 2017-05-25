#include "message.h"

void listenMsg();
void sigHandle(int sigNo);
void excuteRequest(char *str);

int shmid;
float *shm, position = MIN_POSITION;

int main(int argc, char const *argv[]) {
  signal(SIGINT, sigHandle);
  shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | PERMISSION);
  shm = (float*) shmat(shmid, 0, 0);
  shm[0] = MIN_POSITION;
  listenMsg();
  return 0;
}

// Lang nghe tin nhan de thuc hien chuyen dong thang may
void listenMsg() {
  int msqid;
  msg rcvbuffer;
  while(1) {
    msqid = msgget(MSG_KEY_B, PERMISSION);
    if(msgrcv(msqid, &rcvbuffer, MSG_SIZE, 1, 0) >= 0) {
      excuteRequest(rcvbuffer.mtext);
    }
  }
}

void excuteRequest(char *str) {
  if(str[0] == 's') {
    sleep(str[1] - '0');
  } else if(str[0] == 'u') {
    sleep(str[1] - '0');
    position += HIGHT_FLOOR;

    if(position > MAX_POSITION)
      system("killall ./lift_body");
    else
      shm[0] = position;
  } else if(str[0] == 'd') {
    sleep(str[1] - '0');
    position -= HIGHT_FLOOR;

    if(position < MIN_POSITION)
      system("killall ./lift_body");
    else
      shm[0] = position;
  } else {
    system("killall ./lift_body");
  }
}

// Khi an ctrl + c thi tat het tien trinh lien quan toi floor
void sigHandle(int sigNo) {
  system("killall ./lift_body");
  return;
}
