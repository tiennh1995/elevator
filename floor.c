#include "message.h"

int valid_argument(int argc, char const *argv[]);
void listenMsg(int floor);
void sigHandle(int sigNo);
void menu(int floor);
void menu_bar(int floor);
int valid_choose(int floor, int choose);

msg sndbuffer;
int current_floor = 1;

int main(int argc, char const *argv[]) {
  int floor = valid_argument(argc, argv);
  sndbuffer.mtype = 1;

  signal(SIGINT, sigHandle);
  if(fork() == 0)
    listenMsg(floor);

  menu(floor);
  return 0;
}

// Kiem tra dau vao tham so hop le
int valid_argument(int argc, char const *argv[]) {
  if(argc != 2) {
    printf("Syntax: ./floor number_of_floor!\n");
    exit(1);
  }
  int floor = atoi(argv[1]);
  if(!floor) {
    printf("ERROR: number_of_floor is not valid!\n");
    exit(1);
  } else if(floor < 1 || floor > FLOOR_SIZE) {
    printf("ERROR: number_of_floor is not valid!\n");
    exit(1);
  }

  return floor;
}

// Lang nghe tin nhan de cap nhat vi tri cua thang may
void listenMsg(floor) {
  int msqid;
  msg rcvbuffer;
  while(1) {
    msqid = msgget(floor, PERMISSION);
    if(msqid >= 0) {
      if(msgrcv(msqid, &rcvbuffer, MSG_SIZE, 1, 0) >= 0) {
        current_floor = rcvbuffer.mtext[0] - '0';
        menu_bar(floor);
      }
    }
  }
}

// Khi an ctrl + c thi tat het tien trinh lien quan toi floor
void sigHandle(int sigNo) {
  system("killall ./floor");
  return;
}

// Menu de quan ly cac tang
void menu(int floor) {
  int choose = 0;
  do {
    menu: menu_bar(floor);
    scanf("%d", &choose);
    if(!valid_choose(floor, choose)) {
      printf("Your choose is not valid. Please choose again!\n");
      goto menu;
    }

    if(choose){
      sndbuffer.mtext[0] = choose + '0';
      sendController(sndbuffer);
    }
  } while(choose);
  system("killall ./floor");
}

void menu_bar(int floor) {
  printf("\n\n----- Floor: %d -----\n", floor);
  printf("The elevator is at floor: %d\n", current_floor);
  if(floor == 1) {
    printf("2 - Call floor 2\n");
    printf("3 - Call floor 3\n");
    printf("4 - Call floor 4\n");
    printf("5 - Call floor 5\n");
  } else {
    printf("1 - Call floor 1\n");
  }
  printf("0 - Exit\n");
  printf("==> Your choose: \n");
}

// Kiem tra dau vao cua lua chon menu la hop le
int valid_choose(int floor, int choose) {
  if(0 <= choose && choose <= FLOOR_SIZE) {
    if(floor == 1) {
      if((2 <= choose && choose <= FLOOR_SIZE) || choose == 0)
        return 1;
    } else if(choose == 0 || choose == 1) {
      return 1;
    }
  }
  return 0;
}
