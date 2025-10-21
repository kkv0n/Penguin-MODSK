

void ghost_OnBoot()
{
	//ghost fixes
	//these are writting somewhere in ghost_thtick func
	*(int*)0x800150c0 = 0;
	*(int*)0x800277c8 = 0;
	*(int*)0x800277d0 = 0;
}