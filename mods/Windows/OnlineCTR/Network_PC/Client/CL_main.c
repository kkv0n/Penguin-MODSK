#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define WINDOWS_INCLUDE
#include "../../../../../decompile/General/AltMods/OnlineCTR/global.h"

char* pBuf;
struct OnlineCTR* octr;

struct SocketCtr
{
	SOCKET socket;
	int buttonPrev;
};

struct SocketCtr CtrMain;

int buttonPrev[8] = {0};

void ParseMessage()
{
	char recvBufFull[0x100];
	memset(recvBufFull, 0xFF, 0x100);

	// if send() happens 100 times, it all gets picked up
	// in one recv() call, so only call recv one time
	int numBytes = recv(CtrMain.socket, recvBufFull, 0x100, 0);

	if(numBytes == -1)
	{
		int err = WSAGetLastError();

		// This happens due to nonblock, ignore it
		if (err != WSAEWOULDBLOCK)
		{
			// if server is disconnected
			if ((err == WSAENOTCONN) || (err == WSAECONNRESET))
			{
				//Disconnect();

				// reboot
				octr->CurrState = -1;
				printf("Reboot\n");
			}

			return;
		}
	}

	// parse every message coming in
	for(int offset = 0; offset < numBytes; /**/)
	{
		struct SG_Header* recvBuf = &recvBufFull[offset];
		//printf("%d %d %d %d\n", numBytes, offset, recvBuf->size, recvBuf->type);
		offset += recvBuf->size;

		int slot;

		// switch will compile into a jmp table, no funcPtrs needed
		switch (((struct SG_Header*)recvBuf)->type)
		{
			case SG_NEWCLIENT:
			{
				// clientID is "you"
				octr->DriverID = ((struct SG_MessageClientStatus*)recvBuf)->clientID;
				octr->NumDrivers = ((struct SG_MessageClientStatus*)recvBuf)->numClientsTotal;
				
				if (octr->CurrState = LAUNCH_FIRST_INIT)
				{
					// choose to get host menu or guest menu
					octr->CurrState = LOBBY_ASSIGN_ROLE;
				}
				
				break;
			}
			
			case SG_DROPCLIENT:
			{
				int clientDropped = ((struct SG_MessageClientStatus*)recvBuf)->clientID;
				octr->NumDrivers = ((struct SG_MessageClientStatus*)recvBuf)->numClientsTotal;

				// fix driver IDs
				if (clientDropped == octr->DriverID) slot = 0;
				if (clientDropped < octr->DriverID) slot = clientDropped + 1;
				if (clientDropped > octr->DriverID) slot = clientDropped;

				for (int i = slot; i < octr->NumDrivers; i++)
				{
					*(short*)&pBuf[(0x80086e84 + 2 * (i)) & 0xffffff] =
						*(short*)&pBuf[(0x80086e84 + 2 * (i + 1)) & 0xffffff];

					octr->boolLockedInCharacter_Others[i] =
						octr->boolLockedInCharacter_Others[i + 1];
				}

				// clientID is the client disconnected
				if (octr->DriverID > clientDropped)
					octr->DriverID--;

				printf("New client, you are now: %d/%d\n", octr->DriverID, octr->NumDrivers);

				// if you are new host
				if (octr->DriverID == 0)
				{
					if (octr->CurrState == LOBBY_GUEST_TRACK_WAIT)
						octr->CurrState = LOBBY_HOST_TRACK_PICK;
				}

				break;
			}

			case SG_TRACK:
			{
				octr->boolLockedInTrack = 1;
				int trackID = ((struct SG_MessageTrack*)recvBuf)->trackID;
				printf("Got Track: %d\n", trackID);

				// set sdata->gGT->trackID
				*(char*)&pBuf[(0x80096b20 + 0x1a10) & 0xffffff] = trackID;
				octr->CurrState = LOBBY_CHARACTER_PICK;
				break;
			}

			case SG_CHARACTER:
			{
				int characterID = ((struct SG_MessageCharacter*)recvBuf)->characterID;
				int clientID = ((struct SG_MessageCharacter*)recvBuf)->clientID;

				if (clientID == octr->DriverID) slot = 0;
				if (clientID < octr->DriverID) slot = clientID + 1;
				if (clientID > octr->DriverID) slot = clientID;

				*(short*)&pBuf[(0x80086e84 + 2 * slot) & 0xffffff] = characterID;
				octr->boolLockedInCharacter_Others[clientID] = ((struct SG_MessageCharacter*)recvBuf)->boolLockedIn;
				break;
			}

			case SG_STARTLOADING:
			{
				// variable reuse, wait a few frames,
				// so screen updates with green names
				octr->CountPressX = 0;
				octr->CurrState = LOBBY_START_LOADING;
				break;
			}

			case SG_STARTRACE:
			{
				octr->CurrState = GAME_START_RACE;
				break;
			}

			case SG_RACEINPUT:
			{
				struct SG_MessageRaceInput* r =
					(struct SG_MessageRaceInput*)recvBuf;

				int clientID = r->clientID;
				if (clientID == octr->DriverID) slot = 0;
				if (clientID < octr->DriverID) slot = clientID + 1;
				if (clientID > octr->DriverID) slot = clientID;

				int curr = r->buttonHold;

				// sneak L1/R1 into one byte,
				// remove Circle/L2

				if ((curr & 0x40) != 0)
				{
					curr &= ~(0x40);
					curr |= 0x400;
				}

				if ((curr & 0x80) != 0)
				{
					curr &= ~(0x80);
					curr |= 0x800;
				}

				int prev = buttonPrev[slot];

				// tapped
				int tap = ~prev & curr;

				// released
				int rel = prev & ~curr;

				struct Gamepad
				{
					short unk_0;
					short unk_1;
					short stickLX;
					short stickLY;
					short stickLX_dontUse1;
					short stickLY_dontUse1;
					short stickRX;
					short stickRY;
					int buttonsHeldCurrFrame;
					int buttonsTapped;
					int buttonsReleased;
					int buttonsHeldPrevFrame;
				};

				struct Gamepad* pad = &pBuf[(0x80096804 + (slot * 0x50)) & 0xffffff];
				pad->buttonsHeldCurrFrame = curr;
				pad->buttonsTapped = tap;
				pad->buttonsReleased = rel;
				pad->buttonsHeldPrevFrame = prev;

				// In this order: Up, Down, Left, Right
				if ((pad->buttonsHeldCurrFrame & 1) != 0) pad->stickLY = 0;
				else if ((pad->buttonsHeldCurrFrame & 2) != 0) pad->stickLY = 0xFF;
				else pad->stickLY = 0x80;

				if ((pad->buttonsHeldCurrFrame & 4) != 0) pad->stickLX = 0;
				else if ((pad->buttonsHeldCurrFrame & 8) != 0) pad->stickLX = 0xFF;
				else pad->stickLX = 0x80;

				buttonPrev[slot] = curr;
				break;
			}

			case SG_RACEPOS:
			{
				struct SG_MessageRacePos* r =
					(struct SG_MessageRacePos*)recvBuf;

				// this one just uses ClientID directly
				// because of how kart spawning is modified
				int clientID = r->clientID;
				
				int psxPtr = *(int*)&pBuf[(0x8009900c + 4 * clientID) & 0xffffff];
				psxPtr &= 0xffffff;

				// 0x2D4, drop bottom byte
				*(unsigned char*)&pBuf[psxPtr + 0x2d4 + 1] = r->posX[0];
				*(unsigned char*)&pBuf[psxPtr + 0x2d4 + 2] = r->posX[1];
				*(unsigned char*)&pBuf[psxPtr + 0x2d4 + 3] = r->posX[2];

				// 0x2D8, drop bottom byte
				*(unsigned char*)&pBuf[psxPtr + 0x2d8 + 1] = r->posY[0];
				*(unsigned char*)&pBuf[psxPtr + 0x2d8 + 2] = r->posY[1];
				*(unsigned char*)&pBuf[psxPtr + 0x2d8 + 3] = r->posY[2];

				// 0x2DC, drop bottom byte
				*(unsigned char*)&pBuf[psxPtr + 0x2dc + 1] = r->posZ[0];
				*(unsigned char*)&pBuf[psxPtr + 0x2dc + 2] = r->posZ[1];
				*(unsigned char*)&pBuf[psxPtr + 0x2dc + 3] = r->posZ[2];
				break;
			}

		default:
			break;
		}
	}
}

void StatePC_Launch_EnterPID()
{
	// if client connected to DuckStation
	// before game booted, wait for boot
	if (octr->IsBootedPS1)
	{
		octr->CurrState = LAUNCH_ENTER_IP;
	}
}

void StatePC_Launch_EnterIP()
{
	struct sockaddr_in socketIn;
	struct hostent* hostinfo;
	int result = 0;

	socketIn.sin_family = AF_INET;
	socketIn.sin_port = htons(1234);

	// Try Niko's IP, then only enter manually
	// if this server is not open (temporary test)
	socketIn.sin_addr.S_un.S_un_b.s_b1 = 24;
	socketIn.sin_addr.S_un.S_un_b.s_b2 = 187;
	socketIn.sin_addr.S_un.S_un_b.s_b3 = 10;
	socketIn.sin_addr.S_un.S_un_b.s_b4 = 49;

	CtrMain.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	result = connect(CtrMain.socket, (struct sockaddr*)&socketIn, sizeof(socketIn));

	if ((result < 0) || CtrMain.socket == INVALID_SOCKET)
	{
		char ip[100];

		printf("\n");
		printf("Enter IP Address: ");
		scanf_s("%s", ip, sizeof(ip));

		hostinfo = gethostbyname(ip);
		if (hostinfo == NULL)
		{
			printf("Unknown host\n");
			octr->CurrState = LAUNCH_CONNECT_FAILED;
			return;
		}

		socketIn.sin_addr = *(struct in_addr*)hostinfo->h_addr;
		printf("URL converts to IP: %d.%d.%d.%d\n",
			socketIn.sin_addr.S_un.S_un_b.s_b1,
			socketIn.sin_addr.S_un.S_un_b.s_b2,
			socketIn.sin_addr.S_un.S_un_b.s_b3,
			socketIn.sin_addr.S_un.S_un_b.s_b4);

		// Create a SOCKET for connecting to server
		CtrMain.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		// Setup the TCP listening socket
		result = connect(CtrMain.socket, (struct sockaddr*)&socketIn, sizeof(socketIn));


		// failed connection
		if (result < 0)
		{
			printf("WSAGetLastError: %d\n", WSAGetLastError());
			octr->CurrState = LAUNCH_CONNECT_FAILED;
			return;
		}

		// failed connection
		if (CtrMain.socket == INVALID_SOCKET)
		{
			octr->CurrState = LAUNCH_CONNECT_FAILED;
			return;
		}
	}
	
	unsigned long nonBlocking = 1;
	ioctlsocket(CtrMain.socket, FIONBIO, &nonBlocking);

	int flag = 1;
	setsockopt(CtrMain.socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));

	octr->DriverID = -1;
	octr->CurrState = LAUNCH_FIRST_INIT;
}

void StatePC_Launch_ConnectFailed()
{
	printf("Unable to connect to server!\n");
	system("pause");
	octr->CurrState = LAUNCH_ENTER_IP;
}

void StatePC_Launch_FirstInit()
{
	ParseMessage();
}

void StatePC_Lobby_AssignRole()
{
	ParseMessage();
}

void StatePC_Lobby_HostTrackPick()
{
	ParseMessage();

	if (!octr->boolLockedInTrack) return;

	printf("Sending Track to Server\n");

	struct CG_MessageTrack mt;
	mt.type = CG_TRACK;
	mt.size = sizeof(struct CG_MessageTrack);

	// sdata->gGT->levelID
	mt.trackID = *(char*)&pBuf[(0x80096b20 + 0x1a10) & 0xffffff];

	send(CtrMain.socket, &mt, mt.size, 0);

	octr->CurrState = LOBBY_CHARACTER_PICK;
}

int prev_characterID = -1;
int prev_boolLockedIn = -1;
void StatePC_Lobby_GuestTrackWait()
{
	ParseMessage();

	prev_characterID = -1;
	prev_boolLockedIn = -1;
}

void StatePC_Lobby_CharacterPick()
{
	ParseMessage();

	struct CG_MessageCharacter mc;
	mc.type = CG_CHARACTER;
	mc.size = sizeof(struct CG_MessageCharacter);

	// data.characterIDs[0]
	mc.characterID = *(char*)&pBuf[0x80086e84 & 0xffffff];
	mc.boolLockedIn = octr->boolLockedInCharacter;

	if(
		(prev_characterID != mc.characterID) ||
		(prev_boolLockedIn != mc.boolLockedIn)
	  )
	{
		prev_characterID = mc.characterID;
		prev_boolLockedIn = mc.boolLockedIn;

		send(CtrMain.socket, &mc, mc.size, 0);
	}
	
	if (mc.boolLockedIn == 1)
	{
		octr->CurrState = LOBBY_WAIT_FOR_LOADING;
	}
}

void StatePC_Lobby_WaitForLoading()
{
	ParseMessage();

	// if recv message to start loading,
	// change state to StartLoading,
	// this check happens in ParseMessage
}

int prevClockSecond = 0;
unsigned char prevHold1 = 0;
unsigned char prevHold2 = 0;
int boolAlreadySent_StartRace = 0;
void StatePC_Lobby_StartLoading()
{
	ParseMessage();

	prevHold1 = 0;
	prevHold2 = 0;
	boolAlreadySent_StartRace = 0;
}

void SendKartInput()
{
	struct CG_MessageRaceInput cg;
	cg.type = CG_RACEINPUT;
	cg.size = sizeof(struct CG_MessageRaceInput);

	int hold = *(int*)&pBuf[(0x80096804 + 0x10) & 0xffffff];

	// ignore Circle/L2
	hold &= ~(0xC0);

	// put L1/R1 into one byte

	if ((hold & 0x400) != 0)
	{
		hold |= 0x40;
	}

	if ((hold & 0x800) != 0)
	{
		hold |= 0x80;
	}

	cg.buttonHold = (unsigned char)hold;

	// dont send duplicate input, if input hasnt changed in 3 frames,
	// this way "curr" and "prev" are both set by client recv
	if ((cg.buttonHold == prevHold1) && (prevHold1 == prevHold2))
		return;

	prevHold2 = prevHold1;
	prevHold1 = cg.buttonHold;

	send(CtrMain.socket, &cg, cg.size, 0);
}

void SendKartPos()
{
	struct CG_MessageRacePos cg;
	cg.type = CG_RACEPOS;
	cg.size = sizeof(struct CG_MessageRacePos);

	int psxPtr = *(int*)&pBuf[0x8009900c & 0xffffff];
	psxPtr &= 0xffffff;

	// 0x2D4, drop bottom byte
	cg.posX[0] = *(unsigned char*)&pBuf[psxPtr + 0x2d4 + 1];
	cg.posX[1] = *(unsigned char*)&pBuf[psxPtr + 0x2d4 + 2];
	cg.posX[2] = *(unsigned char*)&pBuf[psxPtr + 0x2d4 + 3];

	// 0x2D8, drop bottom byte
	cg.posY[0] = *(unsigned char*)&pBuf[psxPtr + 0x2d8 + 1];
	cg.posY[1] = *(unsigned char*)&pBuf[psxPtr + 0x2d8 + 2];
	cg.posY[2] = *(unsigned char*)&pBuf[psxPtr + 0x2d8 + 3];

	// 0x2DC, drop bottom byte
	cg.posZ[0] = *(unsigned char*)&pBuf[psxPtr + 0x2dc + 1];
	cg.posZ[1] = *(unsigned char*)&pBuf[psxPtr + 0x2dc + 2];
	cg.posZ[2] = *(unsigned char*)&pBuf[psxPtr + 0x2dc + 3];

	send(CtrMain.socket, &cg, cg.size, 0);
}

void SendKartMain()
{
	int gGT_elapsedEventTime = *(int*)&pBuf[(0x80096b20 + 0x1d10) & 0xffffff];

	// divide by 1024, even though one second is 1000,
	// that way nobody notices it's "exactly" every second
	int approxSecond = gGT_elapsedEventTime >> 10;

	if (prevClockSecond < approxSecond)
	{
		prevClockSecond = approxSecond;

		SendKartPos();
	}

	SendKartInput();
}

void StatePC_Game_WaitForRace()
{
	ParseMessage();

	if (!boolAlreadySent_StartRace)
	{
		boolAlreadySent_StartRace = 1;

		struct CG_Header cg;
		cg.type = CG_STARTRACE;
		cg.size = sizeof(struct CG_Header);

		send(CtrMain.socket, &cg, cg.size, 0);
	}

	SendKartMain();
}

void StatePC_Game_StartRace()
{
	ParseMessage();

	SendKartMain();
}

void (*ClientState[]) () =
{
	StatePC_Launch_EnterPID,
	StatePC_Launch_EnterIP,
	StatePC_Launch_ConnectFailed,
	StatePC_Launch_FirstInit,
	StatePC_Lobby_AssignRole,
	StatePC_Lobby_HostTrackPick,
	StatePC_Lobby_GuestTrackWait,
	StatePC_Lobby_CharacterPick,
	StatePC_Lobby_WaitForLoading,
	StatePC_Lobby_StartLoading,
	StatePC_Game_WaitForRace,
	StatePC_Game_StartRace
};

// for EnumProcessModules
#pragma comment(lib, "psapi.lib")

int main()
{
	printf(__DATE__);
	printf("\n");
	printf(__TIME__);
	printf("\n\n");

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 480, 240 + 35, TRUE);

	int numDuckInstances = 0;
	char* duckTemplate = "duckstation";
	int duckPID = -1;

	// copy from
	// https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
	cProcesses = cbNeeded / sizeof(DWORD);
	for (int i = 0; i < cProcesses; i++)
	{
		DWORD processID = aProcesses[i];

		if (processID != 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
				PROCESS_VM_READ,
				FALSE, processID);

			if (NULL != hProcess)
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
					&cbNeeded))
				{
					TCHAR szProcessName[MAX_PATH];
					GetModuleBaseNameA(hProcess, hMod, szProcessName,
						sizeof(szProcessName) / sizeof(TCHAR));

					char* procName = (char*)&szProcessName[0];

					if (
						(*(int*)&procName[0] == *(int*)&duckTemplate[0]) &&
						(*(int*)&procName[4] == *(int*)&duckTemplate[4])
						)
					{
						numDuckInstances++;
						duckPID = processID;
					}
				}
			}
		}
	}

	if (numDuckInstances == 0)
	{
		printf("DuckStation not found\n");
		system("pause");
		exit(0);
	}

	char pidStr[16];
	if (numDuckInstances > 1)
	{
		printf("Multiple ducks detected\n");
		printf("Please use PID manually\n\n");

		printf("Enter DuckStation PID: ");
		scanf_s("%s", pidStr, sizeof(pidStr));
	}
	else
	{
		sprintf_s(pidStr, 100, "%d", duckPID);
	}

	char duckName[100];
	sprintf_s(duckName, 100, "duckstation_%s", pidStr);

	TCHAR duckNameT[100];
	swprintf(duckNameT, 100, L"%hs", duckName);

	// 8mb RAM
	const unsigned int size = 0x800000;
	HANDLE hFile = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, duckNameT);
	pBuf = (char*)MapViewOfFile(hFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, size);

	if (pBuf == 0)
	{
		printf("Error: Failed to open DuckStation\nTry again\n");
		system("pause");
		system("cls");
		main();
	}

	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		system("pause");
		return;
	}

	octr = (struct OnlineCTR*)&pBuf[0x8000C000 & 0xffffff];

	while (1)
	{
		// To do: Check for PS1 system clock tick,
		// then run client update

		octr->time[0]++;
		ClientState[octr->CurrState]();
		
		void FrameStall(); FrameStall();
	}

	system("pause");
}

#pragma optimize("", off)
int gGT_timer = 0;
void FrameStall()
{
	// wait for next frame
	while (gGT_timer == *(int*)&pBuf[(0x80096b20 + 0x1cec) & 0xffffff]) {}
	gGT_timer = *(int*)&pBuf[(0x80096b20 + 0x1cec) & 0xffffff];
}
#pragma optimize("", on)