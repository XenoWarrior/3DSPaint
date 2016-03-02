#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <inttypes.h>
#include <map>
#include <3ds.h>

int main()
{
	// Initializations
	srvInit();			// services
	aptInit();			// applets
	hidInit();			// input
	
	gfxInitDefault();	// graphics
	gfxSet3D(true);	// 3d mode

	consoleInit(GFX_TOP, NULL);

	u32 keyDown;		// keys down
	u32 keyHeld;		// keys pressed
	u32 keyUp;			// keys up

	std::map<int, int> renderPos;

	int dotPos = 144000;
	int size_z = 12;
	int size_x = 6;
	int colour = 0xFFFFFF;

	bool isEraser = false;
	bool colourLoop = false;
	bool colourLoopU = false;

	printf("Hello World!\n");
	printf("Press START to exit.\n");

	while (aptMainLoop())
	{
		//Read the touch screen coordinates
		touchPosition touch;
		hidTouchRead(&touch);

		// Wait for next frame
		gspWaitForVBlank();
	
		// Read button presses
		hidScanInput();
		keyDown = hidKeysDown();
		keyHeld = hidKeysHeld();
		//keyUp = hidKeysUp();

		// Button press handlers
		if (keyDown & KEY_START)
		{
			break;
		}
		if (keyHeld & KEY_SELECT)
		{
			renderPos.clear();
		}
		if (keyDown & KEY_L)
		{
			isEraser = (!isEraser);
			printf("Toggled pen mode.\n");
		}
		if (keyDown & KEY_R)
		{
			colour = rand() % 16777215;
			printf("Changed to a random colour.\n");
		}
		if (keyDown & KEY_ZR)
		{
			if(!colourLoopU)
			{
				if (colourLoop)
				{
					colourLoopU = (!colourLoopU);
					printf("Toggled random colour loop ultimate.\n");
				}
				else
				{
					colourLoop = (!colourLoop);
					printf("Toggled random colour loop.\n");
				}
			}
			else
			{
				colourLoop = false;
				colourLoopU = false;
				printf("Turned off random colour loop.\n");
			}
		}

		if (keyHeld & KEY_X)
		{
			size_x++;
		}
		if (keyHeld & KEY_B)
		{
			size_x--;
		}

		if (keyHeld & KEY_A)
		{
			size_z++;
		}
		if (keyHeld & KEY_Y)
		{
			size_z--;
		}

		// Example rendering code that displays a white pixel
		// Please note that the 3DS screens are sideways (thus 240x400 and 240x320)
		u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		memset(fb, 0, 240 * 320 * 3);

		dotPos = (720*touch.px) - (3*touch.py);
		
		if (isEraser)
		{
			renderPos[dotPos] = 0x000000;

			for (int i = 1; i < size_x; i++)
			{
				for (int l = 0; l < size_z; l++)
				{
					renderPos[dotPos + (1 * i) + (720 * l)] = 0x000000;
				}
			}
		}
		else
		{
			int nc = rand() % 16777215;
			for (int i = 1; i < size_x; i++)
			{
				for (int l = 0; l < size_z; l++)
				{
					if (colourLoop)
					{
						if(colourLoopU)
						{
							renderPos[dotPos + (1 * i) + (720 * l)] = rand() % 16777215;
						}
						else
						{
							renderPos[dotPos + (1 * i) + (720 * l)] = nc;
						}
					}
					else
					{
						renderPos[dotPos + (1 * i) + (720 * l)] = colour;
					}
				}
			}
		}

		for (std::map<int, int>::iterator it = renderPos.begin(); it != renderPos.end(); ++it)
		{
			fb[it->first] = it->second;
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	// Exit
	gfxExit();
	hidExit();
	aptExit();
	srvExit();

	// Return to hbmenu
	return 0;
}