#include "common.h"
#include "utils.h"

void DrawReservesMeter(short posX, short posY, struct Driver* d)
{
	u_int colorAndCode;
	short reservesMeter;
	struct DB* backDB;
	u_long *primmemCurr;
	POLY_F4 *p;
	short bottomY;
	RECT box2;
	u_int auStack48 [2];
	int reservesMeterWidth;

	reservesMeter = posY - 0x2d;

	backDB = sdata->gGT->backBuffer;
	primmemCurr = backDB->primMem.curr;
	p = 0;

	// if there is room left for more
	if (primmemCurr <= (u_long *)backDB->primMem.endMin100)
	{
		// add primitives
		backDB->primMem.curr = &primmemCurr[6];
		p = (POLY_F4 *)primmemCurr;
	}

	if (p != 0)
	{
		box2.y = posY - 3;
		box2.w = 0x31;
		box2.h = 3;
		box2.x = posX - 0x31;
		memset(auStack48, 0, 4);

		CTR_Box_DrawWireBox(&box2, auStack48, sdata->gGT->pushBuffer_UI.ptrOT, &sdata->gGT->backBuffer->primMem);

		colorAndCode = 0x280000ff; // red
		
		
		if (d->reserves >= 32766 || d->reserves < 0)
		{
			d->reserves = 32766;
		}
		
		if (d->reserves > 3600)
		{
			if (d->reserves < 6840)
			{
				colorAndCode = 0x2800ffff; // yellow
			}
			else
			{
				colorAndCode = 0x2800ff00; // green
			}
		}
		*(u_int *)&p->r0 = colorAndCode;
		reservesMeterWidth = d->reserves * 0x4; //Meter maximum is 8000 reserves units
		bottomY = posY - 3;
		p->y0 = posY;
		p->y1 = posY;
		p->x1 = posX;
		p->y2 = bottomY;
		p->x3 = posX;
		p->y3 = bottomY;
		
		reservesMeter = posX - ((short)(reservesMeterWidth / 0x960));
		if(reservesMeter < (posX - 0x31))
		{
			reservesMeter = posX - 0x31;
			*(u_int *)&p->r0 = 0x28ff0000; // blue
		}
		p->x0 = reservesMeter;
		p->x2 = reservesMeter;

		primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

		*(int*)p = *primmemCurr | 0x5000000;
		*primmemCurr = (u_int)p & 0xffffff;

		backDB = sdata->gGT->backBuffer;
		primmemCurr = backDB->primMem.curr;
		p = 0;

		// If there is room to add more
		if (primmemCurr <= (u_long *)backDB->primMem.endMin100)
		{
			// Add more primitives
			backDB->primMem.curr = &primmemCurr[6];
			p = (POLY_F4 *)primmemCurr;
		}

		if (p != 0)
		{
			*(u_int *)&p->r0 = 0x28808080;
			p->y0 = posY;
			p->y1 = posY;
			p->x1 = posX;
			p->y2 = bottomY;
			p->x3 = posX;
			p->y3 = bottomY;
			p->x0 = posX - 0x31;
			p->x2 = posX - 0x31;

			// pointer to OT memory
			primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

			*(int*)p = *primmemCurr | 0x5000000;
			*primmemCurr = (u_int)p & 0xffffff;
		}
	}

	return;
}

// From Input display by niikasd
// https://github.com/CTR-tools/CTR-ModSDK/tree/94abf1a4753ed7de60dcc0877ab7c1cb74d03a5e/mods/Modules/InputDisplay

unsigned int original_map[] = {
//	RAW_BTN_UP,
//	RAW_BTN_DOWN,
//	RAW_BTN_LEFT,
//	RAW_BTN_RIGHT,
	RAW_BTN_CROSS,
	RAW_BTN_SQUARE,
	RAW_BTN_CIRCLE,
	RAW_BTN_TRIANGLE,
	RAW_BTN_R1,
	RAW_BTN_R2,
	RAW_BTN_L1,
	RAW_BTN_L2,
//	RAW_BTN_SELECT,
	RAW_BTN_START
};

char button[14][2] = {
//	"U",//up
//	"D",//down
//	"L",//left
//	"R",//right
	"*",//cross
	"[",//square
	"@",//circle
	"^",//triangle
	"1",//R1
	"2",//R2
	"1",//L1
	"2",//L2
//	"-",//Select
	"-"//Start
};
unsigned int coords[14][2] = {
//	{20,170},//up
//	{20,190},//down
//	{10,180},//left
//	{30,180},//right
	{80,187},//cross
	{68,180},//square
	{92,180},//circle
	{80,173},//triangle
	{80,160},//R1
	{80,150},//R2
	{20,160},//L1
	{20,150},//L2
//	{45,180},//Select
	{51,180}//Start
};

void draw(int i)
{
   DecalFont_DrawLine(button[i], coords[i][0], coords[i][1], FONT_SMALL, ORANGE);
}


void DrawButtons(unsigned short * buttons)
{
	unsigned short curr_buttons = *buttons;
	for (int i = 0; i < RAW_BTN_COUNT-5; i++)
	{
		if ((curr_buttons & original_map[i]) == 0)
			draw(i);
		else
			 DecalFont_DrawLine(".", coords[i][0]+3, coords[i][1]-2, FONT_SMALL, ORANGE);
	}
}

void DrawAnalog(unsigned short * X, unsigned short * Y)
{

    {
    	int Xcoord = ((*X)/9)+9;
    	int Ycoord = ((*Y)/16)+170;
        DecalFont_DrawLine(".", Xcoord, Ycoord, FONT_SMALL, ORANGE);
    }	
}

void inputDisplay_Main()
{
	if ((sdata->gGT->gameMode1 & (GAME_CUTSCENE | LOADING)) == 0)
	{	
		DecalFont_DrawLine(".", 23,178, FONT_SMALL, ORANGE);

		struct GamepadBuffer * controller = &sdata->gGamepads->gamepad[0];
		DrawButtons(&controller->ptrControllerPacket->controllerInput);
		DrawAnalog(&controller->stickLX, &controller->stickLY);
	}
}

void NumbersToString(char * str, int value, int decimals)
{
	int i;
	int j = decimals;
	char isNegative = 0;

	if (value < 0)
	{
		isNegative = 1;
		value *= -1;
	}

	for (i = 0; i < decimals; i++)
	{
		str[decimals - i] = '0' + (value % 10);
		value /= 10;
		if (value == 0)
		{
			j = i + 1;
			break;
		}
	}

	if (isNegative)
	{
		str[decimals - j] = '-';
	}
	else
	{
		str[decimals - j] = ' ';
	}

	for (j = j + 1; j < decimals + 1; j++)
		str[decimals - j] = ' ';
}

// From practice rom by redhot
// https://github.com/CTR-tools/CTR-ModSDK/tree/94abf1a4753ed7de60dcc0877ab7c1cb74d03a5e/mods/Standalones/PracticeROM

#define textDuration 3 * 30
#define numSpeedColor 10
#define jump 0x2500
// Strings positions
// x
#define x_speed 0x190 - 15
#define x_reserves 0x1AA - 15
#define x_checkpointStr 0x14
#define x_checkpoint 0x50
#define x_progressStr 0x14
#define x_progress 0x50
#define x_engine 0x14
#define x_frames 196
#define x_numFrames 196 + 0x3C + 15
#define x_total 171
#define x_lag 171 + 0x3C

// y
#if BUILD == EurRetail
#define y_speed 0x8C + 0x14
#define y_reserves 0xBF + 0x14
#define y_checkpointStr 0xC8 + 0x14
#define y_checkpoint 0xC8 + 0x14
#define y_progressStr 0xBF + 0x14
#define y_progress 0xBF + 0x14
#define y_engine 0xB6 + 0x14
#else
#define y_speed 0xAD
#define y_reserves 0xBF
#define y_checkpointStr 0xC8
#define y_checkpoint 0xC8
#define y_progressStr 0xBF
#define y_progress 0xBF
#define y_engine 0xB6
#endif
#define y_frames 17
#define y_numFrames 17
#define y_total 26
#define y_lag 26

// String variables
unsigned char speedVal[numSpeedColor] = {151, 165, 175, 185, 196, 207, 218, 229, 240, 255};
int speedColorVal[numSpeedColor] = {FOREST_GREEN, TINY_GREEN, PAPU_YELLOW, ROO_ORANGE, RED, COCO_MAGENTA, N_GIN_PURPLE, PURA_VIOLET, CRASH_BLUE, POLAR_CYAN};

char s_numbers[] = " 0000";
char s_checkpointStr[] = "Check:";
char s_progressStr[] = "Progr:";

force_inline int Div10(int value)
{
	return (value / 10);
}

void DrawDebugString()
{
	int i;
	int speedColor = SILVER;
    int turboColor = FOREST_GREEN;
    int turboFill = 0;
	struct Driver* driver = sdata->gameTracker.drivers[0];
	int xSpeed = driver->xSpeed;
	int zSpeed = zSpeed = driver->zSpeed;
	int progress = driver->distanceToFinish_curr;
	int checkpoint = driver->distanceToFinish_checkpoint;
	int speed = MATH_FastSqrt((xSpeed * xSpeed) + (zSpeed * zSpeed), 0);

    for (i = 0; i < numSpeedColor; i++)
	{
		if (speed < (int)speedVal[i]*100)
		{
			speedColor = speedColorVal[i];
			break;
		}
	}
    
    if ((driver->kartState == KS_DRIFTING) || (driver->kartState == KS_ENGINE_REVVING))
    {
        turboFill = (960 - driver->turbo_MeterRoomLeft);

        if (turboFill >= 480)
            turboColor = RED;
    }

	DecalFont_DrawLine(s_progressStr, x_progressStr, y_progressStr, FONT_SMALL, PERIWINKLE);
	DecalFont_DrawLine(s_checkpointStr, x_checkpointStr, y_checkpointStr, FONT_SMALL, PERIWINKLE);
	// DecalFont_DrawLine(p_rom->currEngine, x_engine, y_engine, FONT_SMALL, PERIWINKLE);

	NumbersToString(s_numbers, Div10(driver->reserves), 4);
	DecalFont_DrawLine(s_numbers, x_reserves, y_reserves, FONT_SMALL, DINGODILE_OLIVE);

    NumbersToString(s_numbers, Div10(turboFill), 4);
	DecalFont_DrawLine(s_numbers, 360, 200, FONT_SMALL, turboColor);

	NumbersToString(s_numbers, Div10(driver->turnAngleCurr), 4);
	DecalFont_DrawLine(s_numbers, 240, y_checkpoint, FONT_SMALL, (DINGODILE_OLIVE | JUSTIFY_CENTER));

	NumbersToString(s_numbers, Div10(driver->axisRotationY), 4);
	DecalFont_DrawLine(s_numbers, x_progressStr, 114, FONT_SMALL, (DINGODILE_OLIVE | JUSTIFY_CENTER));
	
	NumbersToString(s_numbers, Div10(speed), 4);
	DecalFont_DrawLine(s_numbers, x_speed, y_speed, FONT_BIG, speedColor);
	
	NumbersToString(s_numbers, Div10(progress), 5);
	DecalFont_DrawLine(s_numbers, x_progress, y_progress, FONT_SMALL, PERIWINKLE);
	
	NumbersToString(s_numbers, Div10(checkpoint), 5);
	DecalFont_DrawLine(s_numbers, x_checkpoint, y_checkpoint, FONT_SMALL, PERIWINKLE);

	inputDisplay_Main();
}