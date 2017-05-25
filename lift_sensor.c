#include "message.h"

void listenMsg();
void sigHandle(int sigNo);
void readShareMemory();

int main(int argc, char const *argv[]) {
  signal(SIGINT, sigHandle);
  readShareMemory();
  return 0;
}

void readShareMemory() {
  int msqid, shmid, floor = 1;
  float *shm, current_position = MIN_POSITION, new_position;
  msg sndbuffer;
  sndbuffer.mtype = 1;
  shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | PERMISSION);
  shm = (float*) shmat(shmid, 0, 0);
  while(1) {
    new_position = shm[0];
    if(new_position != current_position) {
      floor = (int) (new_position - MIN_POSITION) / HIGHT_FLOOR;
      floor++;
      sndbuffer.mtext[0] = 's';
      sndbuffer.mtext[1] = floor + '0';
      sndbuffer.mtext[2] = '\0';
      sendMessage(MSG_KEY_C, sndbuffer);
      current_position = new_position;
    }
  }
}

// Khi an ctrl + c thi tat het tien trinh lien quan toi floor
void sigHandle(int sigNo) {
  system("killall ./lift_sensor");
  return;
}
