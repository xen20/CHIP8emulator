#include <cstdint>
#include <ctime>
#include <cstdlib>

uint8_t genRandomNum(void){
    uint8_t randomNum = 0;

    srand(time(NULL));

    randomNum = rand() % 256;

    return randomNum;
}
