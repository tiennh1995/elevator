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
  int old, new;
  if(str[0] == 's') {
    printf("Waiting for luggage...\n");
    sleep(str[1] - '0');
    printf("Start delivering\n");
  } else if(str[0] == 'u') {
    old = (int) (position - MIN_POSITION) / HIGHT_FLOOR + 1;
    position += HIGHT_FLOOR;
    new = (int) (position - MIN_POSITION) / HIGHT_FLOOR + 1;
    printf("Upping from FLOOR %d to %d\n", old, new);
    sleep(str[1] - '0');
    printf("Upped to FLOOR %d\n", new);

    if(position > MAX_POSITION)
      system("killall ./lift_body");
    else
      shm[0] = position;
  } else if(str[0] == 'd') {
    old = (int) (position - MIN_POSITION) / HIGHT_FLOOR + 1;
    position -= HIGHT_FLOOR;
    new = (int) (position - MIN_POSITION) / HIGHT_FLOOR + 1;
    printf("Downing from FLOOR %d to %d\n", old, new);
    sleep(str[1] - '0');
    printf("Downed to FLOOR %d\n", new);

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
