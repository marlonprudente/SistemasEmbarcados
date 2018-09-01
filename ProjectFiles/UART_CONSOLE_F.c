#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include "UARTDriver.h"
#include "cfaf128x128x16.h"
#include "UART_CONSOLE_F.h"
void pag_ini_con(tContext sContext){
	GrStringDraw(&sContext,"Ola, bem vindo       ", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	GrStringDraw(&sContext,"------------------ ", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"1 - Tons de Azul     ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - Tons de Verde    ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - Tons de Marrom   ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - Tons de Roxo     ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - Tons de Ciano    ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - Tons de Laranja  ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - Tons de Amarelo  ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - Tons de Rosa     ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - Tons de Vermelho ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"                     ", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"                     ", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_az1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - SlateBlue              ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - DarkSlateBlue          ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - MidnightBlue           ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - Navy                   ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - DarkBlue               ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - MediumBlue             ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - Blue                   ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - CornFlowerBlue         ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - RoyalBlue              ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"p - Prox.", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_az2_con(tContext sContext){
	GrStringDraw(&sContext,"1 - DodgerBlue             ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - DeepSkyBlue            ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - LightSkyBlue           ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - SkyBlue                ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - LightBlue              ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - SteelBlue              ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - LightSteelBlue         ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - SlateGray              ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - LightSlateGray         ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"a- ant.                    ", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ve1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - DarkSlateGray          ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - MediumSpringGreen      ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - SpringGreen            ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - PaleGreen              ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - LightGreen             ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - DarkSeaGreen           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - MediumSeaGreen         ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - SeaGreen               ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - DarkGreen              ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"p - Prox.                  ", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ve2_con(tContext sContext){
	GrStringDraw(&sContext,"1 - Green                  ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - ForestGreen            ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - LimeGreen              ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - Lime                   ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - LawnGreen              ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - Chartreuse             ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - GreenYellow            ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - YellowGreen            ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - OliveDrab              ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"p - Prox. | a- ant.", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ve3_con(tContext sContext){
	GrStringDraw(&sContext,"1 - DarkOliveGreen         ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - Olive                  ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"a- ant.                    ", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ma1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - DarkKhaki			         ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - Goldenrod				       ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - DarkGoldenrod	         ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - SaddleBrown            ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - Sienna		             ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - RosyBrown		           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - Peru					         ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - Chocolate              ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - SandyBrown             ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"p - Prox.                  ", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ma2_con(tContext sContext){
	GrStringDraw(&sContext,"1 - NavajoWhite		         ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - Wheat                  ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - BurlyWood              ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - Tan                    ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"a- ant.											", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ro1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - MediumSlateBlue        ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - MediumPurple		       ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - BlueViolet		         ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - Indigo			           ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - DarkViolet             ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - DarkOrchid	           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - MediumOrchid	         ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - Purple		             ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - DarkMagenta            ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"p - Prox.                  ", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ro2_con(tContext sContext){
	GrStringDraw(&sContext,"1 - Fuchsia				         ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - Violet                 ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - Orchid		             ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - Plum                   ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"a- ant.											", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ci1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - Cyan					         ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - DarkTurquoise		       ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - Turquoise			         ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - MediumTurquoise        ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - LightSeaGreen          ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - DarkCyan		           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - Teal					         ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - Aquamarine	           ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - MedAquamarine          ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"l - CadetBlue              ", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_la1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - OrangeRed			         ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - DarkOrange             ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - Orange		             ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"				                   ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"a- ant.											", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_lo1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - Gold					         ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - Yellow		             ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - Khaki			             ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"				                   ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"                           ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"a- ant.											", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ra1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - MediumVioletRed        ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - DeepPink				       ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - HotPink				         ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - PaleVioletRed          ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - LightPink	             ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - Pink				           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - LightCoral		         ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - IndianRed	             ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - Crimson			           ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_vr1_con(tContext sContext){
	GrStringDraw(&sContext,"1 - Maroon				         ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"2 - DarkRed					       ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"3 - FireBrick			         ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"4 - Brown						       ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"5 - Salmon				         ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"6 - DarkSalmon	           ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"7 - LightSalmon		         ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
	GrStringDraw(&sContext,"8 - Coral				           ", -1, 0, (sContext.psFont->ui8Height+2)*9, true);
	GrStringDraw(&sContext,"9 - Tomato			           ", -1, 0, (sContext.psFont->ui8Height+2)*10, true);
	GrStringDraw(&sContext,"l - Red				             ", -1, 0, (sContext.psFont->ui8Height+2)*11, true);
	GrStringDraw(&sContext,"i - Inicio", -1, 0, (sContext.psFont->ui8Height+2)*12, true);
}
void pag_ini_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - Tons de Azul\n\r");
	UARTprintstring("2 - Tons de Verde\n\r");
	UARTprintstring("3 - Tons de Marrom\n\r");
	UARTprintstring("4 - Tons de Roxo\n\r");
	UARTprintstring("5 - Tons de Ciano\n\r");
	UARTprintstring("6 - Tons de Laranja\n\r");
	UARTprintstring("7 - Tons de Amarelo\n\r");
	UARTprintstring("8 - Tons de Rosa\n\r");
	UARTprintstring("9 - Tons de Vermelho\n\r");
}
void pag_az1_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - SlateBlue\n\r");
	UARTprintstring("2 - DarkSlateBlue\n\r");
	UARTprintstring("3 - MidnightBlue\n\r");
	UARTprintstring("4 - Navy\n\r");
	UARTprintstring("5 - DarkBlue\n\r");
	UARTprintstring("6 - MediumBlue\n\r");
	UARTprintstring("7 - Blue\n\r");
	UARTprintstring("8 - CornFlowerBlue\n\r");
	UARTprintstring("9 - Royal\n\r");
	UARTprintstring("p - prox.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_az2_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - DodgerBlue\n\r");
	UARTprintstring("2 - DeepSkyBlue\n\r");
	UARTprintstring("3 - LightSkyBlue\n\r");
	UARTprintstring("4 - SkyBlue\n\r");
	UARTprintstring("5 - LightBlue\n\r");
	UARTprintstring("6 - SteelBlue\n\r");
	UARTprintstring("7 - LightSteelBlue\n\r");
	UARTprintstring("8 - SlateGray\n\r");
	UARTprintstring("9 - LightSlateGray\n\r");
	UARTprintstring("a - ant.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ve1_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - DarkSlateGray\n\r");
	UARTprintstring("2 - MediumSpringGreen\n\r");
	UARTprintstring("3 - SpringGreen\n\r");
	UARTprintstring("4 - PaleGreen\n\r");
	UARTprintstring("5 - LightGreen\n\r");
	UARTprintstring("6 - DarkSeaGreen\n\r");
	UARTprintstring("7 - MediumSeaGreen\n\r");
	UARTprintstring("8 - SeaGreen\n\r");
	UARTprintstring("9 - DarkGreen\n\r");
	UARTprintstring("p - prox.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ve2_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - Green\n\r");
	UARTprintstring("2 - ForestGreen\n\r");
	UARTprintstring("3 - LimeGreen\n\r");
	UARTprintstring("4 - Lime\n\r");
	UARTprintstring("5 - LawnGreen\n\r");
	UARTprintstring("6 - Chartreuse\n\r");
	UARTprintstring("7 - GreenYellow\n\r");
	UARTprintstring("8 - YellowGreen\n\r");
	UARTprintstring("9 - OliveDrab\n\r");
	UARTprintstring("p - prox. | a - ant.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ve3_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - DarkOliveGreen\n\r");
	UARTprintstring("2 - Olive\n\r");
	UARTprintstring("a - ant.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ma1_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - DarkKhaki\n\r");
	UARTprintstring("2 - Goldenrod\n\r");
	UARTprintstring("3 - DarkGoldenrod\n\r");
	UARTprintstring("4 - SaddleBrown\n\r");
	UARTprintstring("5 - Sienna\n\r");
	UARTprintstring("6 - RosyBrown\n\r");
	UARTprintstring("7 - Peru\n\r");
	UARTprintstring("8 - Chocolate\n\r");
	UARTprintstring("9 - SandyBrown\n\r");
	UARTprintstring("p - prox.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ma2_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - NavajoWhite\n\r");
	UARTprintstring("2 - Wheat\n\r");
	UARTprintstring("3 - BurlyWood\n\r");
	UARTprintstring("4 - Tan\n\r");
	UARTprintstring("a - ant.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ro1_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - MediumSlateBlue\n\r");
	UARTprintstring("2 - MediumPurple\n\r");
	UARTprintstring("3 - BlueViolet\n\r");
	UARTprintstring("4 - Indigo\n\r");
	UARTprintstring("5 - DarkViolet\n\r");
	UARTprintstring("6 - DarkOrchid\n\r");
	UARTprintstring("7 - MediumOrchid\n\r");
	UARTprintstring("8 - Purple\n\r");
	UARTprintstring("9 - DarkMagenta\n\r");
	UARTprintstring("p - prox.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ro2_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - Fuchsia\n\r");
	UARTprintstring("2 - Violet\n\r");
	UARTprintstring("3 - Orchid\n\r");
	UARTprintstring("4 - Plum\n\r");
	UARTprintstring("a - ant.\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ci1_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - Cyan\n\r");
	UARTprintstring("2 - DarkTurquoise\n\r");
	UARTprintstring("3 - Turquoise\n\r");
	UARTprintstring("4 - MediumTurquoise\n\r");
	UARTprintstring("5 - LightSeaGreen\n\r");
	UARTprintstring("6 - DarkCyan\n\r");
	UARTprintstring("7 - Teal\n\r");
	UARTprintstring("8 - Aquamarine\n\r");
	UARTprintstring("9 - MedAquamarine\n\r");
	UARTprintstring("l - CadetBlue\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_la1_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - OrangeRed\n\r");
	UARTprintstring("2 - DarkOrange\n\r");
	UARTprintstring("3 - Orange\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_lo1_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - Gold\n\r");
	UARTprintstring("2 - Yellow\n\r");
	UARTprintstring("3 - Khaki\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_ra1_uart(){
	UARTprintstring("Ola, bem vindo\n\r");
	UARTprintstring("1 - MediumVioletRed\n\r");
	UARTprintstring("2 - DeepPink\n\r");
	UARTprintstring("3 - HotPink\n\r");
	UARTprintstring("4 - PaleVioletRed\n\r");
	UARTprintstring("5 - LightPink\n\r");
	UARTprintstring("6 - Pink\n\r");
	UARTprintstring("7 - LightCoral\n\r");
	UARTprintstring("8 - IndianRed\n\r");
	UARTprintstring("9 - Crimson\n\r");
	UARTprintstring("i - inicio\n\r");
}
void pag_vr1_uart(){
	UARTprintstring("Ola, bem vindo \n\r");
	UARTprintstring("1 - Maroon\n\r");
	UARTprintstring("2 - DarkRed\n\r");
	UARTprintstring("3 - FireBrick\n\r");
	UARTprintstring("4 - Brown\n\r");
	UARTprintstring("5 - Salmon\n\r");
	UARTprintstring("6 - DarkSalmon\n\r");
	UARTprintstring("7 - LightSalmon\n\r");
	UARTprintstring("8 - Coral\n\r");
	UARTprintstring("9 - Tomato\n\r");
	UARTprintstring("l - Red\n\r");
	UARTprintstring("i - inicio\n\r");
}