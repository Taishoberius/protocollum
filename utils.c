#include "utils.h"

void exit_with(int errorCode, char* message) {
    printf("%s\n", message);
    exit(errorCode);
}
