#include <common.h>

//spectator mode text
#ifdef GASMOX_ENG
const char specting[] = "SPECTATING...";
const char finish_race[] = "FINISHING RACE!";

	const char s_switchCam[] = "Press R1 or L1 to change the camera";
    const char s_gg[] = "GG Thanks For Playing!";
	
#elif defined(GASMOX_ES)
	const char specting[] = "ESPECTEANDO..";
const char finish_race[] = "ACABANDO CARRERA!";

	const char s_switchCam[] = "Presiona L1 o R1 para cambiar camara";
    const char s_gg[] = "GG Gracias por jugar!";
	
#elif defined(GASMOX_BR)
	const char specting[] = "ESPECTANDO...";
const char finish_race[] = "FINALIZANDO CORRIDA!";

	const char s_switchCam[] = "Pressione R1 ou L1 para alternar cam";
    const char s_gg[] = "Terminado,Boa Corrida!!";
	
#endif
