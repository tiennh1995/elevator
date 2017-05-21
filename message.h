#include "constant.h"

typedef struct msgbuf {
  long mtype;
  char mtext[MSG_SIZE];
} msg;

void die(char *s, char *excute);
void sendAll(msg s);
void sendFloor(int floor, msg s);
void sendController(msg s);
void kill_all(char *s);

void die(char *s, char *excute) {
  perror(s);
  system(excute);
}

// Gui tin hieu cho tat ca cac tang
void sendAll(msg s) {
  int msqid, s_size = strlen(s.mtext) + 1, i;
  for(i = 0; i < FLOOR_SIZE; i++) {
    if((msqid = msgget(MSG_KEYS[i], IPC_CREAT | PERMISSION)) < 0)
      kill_all("msgget()");
    if(msgsnd(msqid, &s, s_size, IPC_NOWAIT) < 0)
      kill_all("msgsnd");
  }
}

// Gui tin hieu cho 1 tang bat ki
void sendFloor(int floor, msg s) {
  int msqid, s_size = strlen(s.mtext) + 1;
  if((msqid = msgget(floor, IPC_CREAT | PERMISSION)) < 0)
    kill_all("msgget()");
  if(msgsnd(msqid, &s, s_size, IPC_NOWAIT) < 0)
    kill_all("msgsnd");
}

// Tu floor gui toi floor_controller
void sendController(msg s) {
  int msqid, s_size = strlen(s.mtext) + 1;
  if((msqid = msgget(MSG_KEY_S, IPC_CREAT | PERMISSION)) < 0)
    kill_all("msgget()");
  if(msgsnd(msqid, &s, s_size, IPC_NOWAIT) < 0)
    kill_all("msgsnd()");
}

// Kill cac tien trinh chay khi gap loi
void kill_all(char *s) {
  perror(s);
  system("killall ./floor");
  system("killall ./floor_controller");
}
