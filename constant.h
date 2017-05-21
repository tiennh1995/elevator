#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define PERMISSION 0666
#define FLOOR_SIZE 5
const int MSG_KEYS[5] = {1, 2, 3, 4, 5};
#define MSG_KEY_S 12345
#define MSG_SIZE 8
#define MAX_REQUEST 50
#define SPEED 0.5
#define TIME_WAIT 3
