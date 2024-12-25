#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum tx { NONE, ROT90, ROT180, ROT270, FLIPH, FLIPV, TRANS };

bool changes_aspect(enum tx transformation) 
{
        switch (transformation) {
        case NONE:   return false;
        case ROT90:  return true;
        case ROT180: return false;
        case ROT270: return true;
        case FLIPH:  return false;
        case FLIPV:  return false;
        case TRANS:  return true;
        }
        return false;
}

int main(){
        enum tx my_tx = FLIPH;
        bool my_bool = changes_aspect(my_tx);

        if(my_bool){
                printf("yay :D\n");
        }
        else{
                printf("nay D:\n");
        }
        exit(0);
}