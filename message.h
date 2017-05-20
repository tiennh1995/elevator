#include "constant.h"

typedef struct msgbuf {
  long mtype;
  char mtext[MSG_SIZE];
} msg;

void die(char *s) {
  perror(s);
  exit(1);
}

void sendAll(msg s) {
  int msqid, s_size = strlen(s.mtext) + 1, i;
  for(i = 0; i < FLOOR_SIZE; i++) {
    msqid = msgget(MSG_KEYS[i], IPC_CREAT | PERMISSION);
    msgsnd(msqid, &s, s_size, IPC_NOWAIT);
  }
}

void sendFloor(int floor, msg s) {
  int msqid, s_size = strlen(s.mtext) + 1;
  msqid = msgget(floor, IPC_CREAT | PERMISSION);
  msgsnd(msqid, &s, s_size, IPC_NOWAIT);
}

void sendController(msg s) {
  int msqid, s_size = strlen(s.mtext) + 1;
  msqid = msgget(MSG_KEY_S, IPC_CREAT | PERMISSION);
  msgsnd(msqid, &s, s_size, IPC_NOWAIT);
}
