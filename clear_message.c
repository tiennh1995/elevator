// Dung de xoa toan bo message dang trong hang doi
#include "message.h"

int main() {
  int i;
  for(i = 0; i <= FLOOR_SIZE; i++) {
    if(fork() == 0) {
      int key, msqid;
      msg rcvbuffer;
      if(i == FLOOR_SIZE) {
        key = MSG_KEY_S;
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
