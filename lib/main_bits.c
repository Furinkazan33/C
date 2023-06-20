#include <stdio.h>

#define INIT 0x0        //0
#define WP1  0x1        //1
#define WP2  0x2        //10
#define MASQ 0xF000000000000000 //1111000...000000

void printBits(unsigned long long num)
{
   int i;

   for (i = sizeof(unsigned long long) * 8 - 1; i >= 0; i--)
   {
      printf("%c", num & (1LU << i) ? '1' : '0');
   }
   printf("\n");
}


int main(void) {
    unsigned long long player_infos;
    player_infos = INIT;
    player_infos = player_infos | WP1 | WP2 | MASQ;

    printBits(player_infos);

    if (player_infos & WP1) printf("WP1\n");
    if (player_infos & WP2) printf("WP2\n");


    return 0;
}

