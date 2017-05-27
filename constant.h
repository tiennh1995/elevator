#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <sys/msg.h>

#define PERMISSION 0666
#define FLOOR_SIZE 5
const int MSG_KEYS[5] = {1, 2, 3, 4, 5};
#define MSG_KEY_M 12345
#define MSG_KEY_C 1234
#define MSG_KEY_B 123
#define SHM_KEY 11111
#define SHM_SIZE 128
#define MSG_SIZE 50
#define MAX_REQUEST 10
#define SPEED 0.5
#define TIME_WAIT 3
#define HIGHT_FLOOR 3
#define MAX_POSITION 13.5
#define MIN_POSITION 1.5
