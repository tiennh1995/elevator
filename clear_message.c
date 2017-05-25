// Dung de xoa toan bo message dang trong hang doi
#include "message.h"

int main() {
  int i, key, msqid;
  msg rcvbuffer;
  for(i = 0; i < FLOOR_SIZE + 3; i++) {
    if(fork() == 0) {
      if(i == FLOOR_SIZE) {
        key = MSG_KEY_M;
      } else if(i == FLOOR_SIZE + 1) {
        key = MSG_KEY_C;
      } else if(i == FLOOR_SIZE + 2) {
        key = MSG_KEY_B;
      } else {
        key = MSG_KEYS[i];
      }
      while(1) {
        msqid = msgget(key, PERMISSION);
        msgrcv(msqid, &rcvbuffer, MSG_SIZE, 1, 0);
      }
    }
  }
  sleep(TIME_WAIT);
  system("pkill ./clear_message -f");
}
