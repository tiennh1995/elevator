#include "constant.h"

typedef struct msgbuf {
  long mtype;
  char mtext[MSG_SIZE];
} msg;

void die(char *s, char *excute);
void sendAll(msg s);
void sendMessage(int ms_key, msg s);
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

//Gui tin nhan toi hang doi co key la ms_key
void sendMessage(int ms_key, msg s) {
  int msqid, s_size = strlen(s.mtext) + 1;
  if((msqid = msgget(ms_key, IPC_CREAT | PERMISSION)) < 0)
    kill_all("msgget()");
  if(msgsnd(msqid, &s, s_size, IPC_NOWAIT) < 0)
    kill_all("msgsnd");
}

// Kill cac tien trinh chay khi gap loi
void kill_all(char *s) {
  perror(s);
  system("killall ./lift_body");
  system("killall ./lift_ctrl");
  system("killall ./lift_manager");
  system("killall ./lift_sensor");
  system("killall ./floor");
}
