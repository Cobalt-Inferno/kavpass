#include <stdio.h>
#include "color.h"

void switch_color(int col) {
    col = col >= 0 ? col : 0;
    switch(col) {
        case 0:
            printf("%s",RESET);
            break;
        case 1:
            printf("%s",RED);
            break;
        case 2:
            printf("%s",GREEN);
            break;
        case 3:
            printf("%s",YELLOW);
            break;
        case 4:
            printf("%s",BLUE);
            break;
        case 5:
            printf("%s",MAGENTA);
            break;
        case 6:
            printf("%s",CYAN);
            break;
        case 7:
            printf("%s",WHITE);
            break;
        default:
            return;
    }
}