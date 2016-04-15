#include <cstdio>
#include <Windows.h>
#include "Map.h"
#include "Screen.h"
#include "Utils.h"
#include "Settings.h"

struct Cursor 
{
	int x = 1;
	int y = 1;

	void fixPosition(int maxWidth, int maxHeight)
	{
		y = min(y, maxHeight - 1);
		y = max(y, 0);

		x = min(x, maxWidth - 1);
		x = max(x, 0);
	}

};

void main()
{
	// console warning if I forget release ram
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(758);

	bool _running = true;
	Cursor _cursor;
	Vector _screenPosition = { 0,0 };

	Settings _settings;
	_settings.load();

	Screen* _screen = new Screen( _settings.getScreenWidth(), _settings.getScreenHeight() );
	_screen->init();
	
	Map<char> _mapTile;
	// Tile represent the terrain material
	_mapTile.init(1920, 1980);
	
	while (_running)
	{
		int _screenWidth = _screen->getBackBufferWidth();
		int _screenHeight = _screen->getBackBufferHeight();

		_mapTile.print(_screen->getPixels(), _screenWidth, (_screenHeight - 1) * _screenWidth, _screenPosition);
		
		// move the cursor in base of the keys up, down, right and left
		if (GetAsyncKeyState(VK_UP))
			_cursor.y -= 1;

		if (GetAsyncKeyState(VK_DOWN))
			_cursor.y += 1;

		if (GetAsyncKeyState(VK_RIGHT))
			_cursor.x += 1;

		if (GetAsyncKeyState(VK_LEFT))
			_cursor.x -= 1;

		// check the limit and fix the position
		_cursor.fixPosition(_screenWidth , _screenHeight);

		// draw the cursor
		_screen->getPixels() [_cursor.y * _screenWidth + _cursor.x] = '#';
				
		// draw the screen
		_screen->print();
				
		// this is necesary to don't see tearing. This happent because we don't write directly in the console
		Sleep(20);

		// press the scape key to exit
		if ( GetAsyncKeyState(VK_ESCAPE) )
			_running = false;
	}
	
	if( _screen)
		delete(_screen);
}