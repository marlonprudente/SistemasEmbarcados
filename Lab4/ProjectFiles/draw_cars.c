#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include "carros.h"
#include "inimigos.h"
#include "draw_cars.h"

void draw_cars(inimigo ini[4],int x,int y, int quantidade_i,const tContext sContext){
	int i;
	for(i=0;i<quantidade_i;i++){
			GrImageDraw(&sContext,ini[i].img,ini[i].xi,ini[i].yi);
		}
		GrImageDraw(&sContext,carro,x,90);
}