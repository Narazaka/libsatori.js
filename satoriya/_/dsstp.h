/*
	ClearBrainSystems�́u�Ȉ�SSTP�N���C�A���g�v���DirectSSTP���������p���ρB

	http://clearbrain.khsoft.gr.jp/
*/

/*----------------------------------------------------------------------------

  Light SSTP Client
  Copyright(C) 2001 Koumei Hashimoto, all rights reserved.

  header.h

----------------------------------------------------------------------------*/
#ifndef	STRICT
#define STRICT
#endif
#ifndef	_WIN32_IE
#define _WIN32_IE 0x200
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <winsock.h>
#include <mbstring.h>

// STL
#include <deque>
#include <vector>
#include <string>
using namespace std;

/*----------------------------------------------------------------------------
	���C�u���������N
----------------------------------------------------------------------------*/
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "wsock32.lib")

BOOL sendDirectSSTP_for_NOTIFY(string client, string id, deque<string>& refs);

BOOL CheckSakuraMutex();
