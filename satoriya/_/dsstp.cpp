/*----------------------------------------------------------------------------

  Light SSTP Client
  Copyright(C) 2001 Koumei Hashimoto, all rights reserved.

  dsstp.cpp

----------------------------------------------------------------------------*/
#include "dsstp.h"
#pragma warning( disable : 4786 ) //�u�f�o�b�O�����ł̎��ʎq�؎̂āv
#pragma warning( disable : 4503 ) //�u�������ꂽ���O�̒��������E���z���܂����B���O�͐؂�̂Ă��܂��B�v

/*----------------------------------------------------------------------------
	���[�J����`
----------------------------------------------------------------------------*/
#define MY_DIRECTSSTP_PORT	9801
#define MY_EXIST_FILEMAP	"Sakura"

/*----------------------------------------------------------------------------
	lstrchr()
	�}���`�o�C�gstrchr
----------------------------------------------------------------------------*/
LPSTR lstrchr(LPCSTR sz, CHAR ch)
{
	return (LPSTR)_mbschr((LPBYTE)sz, (UINT)ch);
}

/*----------------------------------------------------------------------------
	CheckSakuraFileMapping()
	�t�@�C���}�b�s���O�I�u�W�F�N�g�̑��݊m�F
----------------------------------------------------------------------------*/
BOOL CheckSakuraFileMapping(HWND hParentWnd, vector<HWND>& vec)
{
	HANDLE hFileMap;
	LPVOID lpBasePtr;
	LPSTR lpBuffer;
	LPSTR lpType1;
	LPSTR lpType2;
	CHAR szTemp[200];
	DWORD dwSize;
	HWND hWnd = NULL;
	BOOL bRet;

	vec.clear();

	// �t�@�C���}�b�v���J��
	hFileMap = OpenFileMapping(FILE_MAP_READ, FALSE, MY_EXIST_FILEMAP);
	if(hFileMap == NULL) 
	{
		// ���݂��Ȃ�
		//MessageBox(hParentWnd, "�t�@�C���}�b�s���O�I�u�W�F�N�g���J���܂���B", "TestSSTP", MB_ICONSTOP);
		return FALSE;
	}

	// �x�[�X�A�h���X�擾
	lpBasePtr = (LPVOID)MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	if(lpBasePtr == NULL)
	{
		// ���s
		//MessageBox(hParentWnd, "�t�@�C���}�b�s���O�I�u�W�F�N�g������ł��܂���B", "TestSSTP", MB_ICONSTOP);
		CloseHandle(hFileMap);
		return FALSE;
	}
	lpBuffer = (LPSTR)lpBasePtr;

	// �f�[�^�ǂݍ���
	bRet = TRUE;
	try
	{
		CopyMemory(&dwSize, lpBuffer, sizeof(DWORD));
		lpBuffer += sizeof(DWORD);

		while(*lpBuffer)
		{
			// �G���g�����
			lpType1 = lstrchr(lpBuffer, '.');
			lpType1++;
			lpType2 = lstrchr(lpType1, '\01');
			lpType2++;
			lstrcpyn(szTemp, lpType1, lpType2 - lpType1);

			// �G���g���̎�ނ��Ƃɕ���
			if(lstrcmpi(szTemp, "hwnd") == 0)
			{
				// �f�[�^�擾
				lpType1 = lstrchr(lpType2, '\r');
				lpType1++;
				lstrcpyn(szTemp, lpType2, lpType1 - lpType2);
				hWnd = (HWND)atoi(szTemp);

				vec.push_back(hWnd);
			}

			// \r\n �܂łP�G���g��
			lpBuffer = lstrchr(lpBuffer, '\n');
			if(lpBuffer)
			{
				lpBuffer++;
			}
		}

		bRet = vec.size() > 0 ? TRUE : FALSE;
	}
	catch(...)
	{
		bRet = FALSE;
	}

	UnmapViewOfFile(lpBasePtr);
	CloseHandle(hFileMap);
	return bRet;
}



/*----------------------------------------------------------------------------
	DirectSSTPSendMessage()
	DirectSSTP ���b�Z�[�W���M
----------------------------------------------------------------------------*/
BOOL sendDirectSSTP_for_NOTIFY(string client, string id, deque<string>& refs) 
//BOOL DirectSSTPSendMessage(HWND hParentWnd, LPCSTR szClient, LPCSTR szMessage, LPCSTR szOption)
{
	HWND hParentWnd=NULL;
	LPCSTR szClient=client.c_str();
	LPCSTR szOption="";
	CHAR szBuffer[100];
	COPYDATASTRUCT cds;
	DWORD dwRet;
	vector<HWND> vec;
	vector<HWND>::iterator it;
	string strSendBuffer;

	// ���݂��`�F�b�N
	if(!CheckSakuraMutex())
	{
		//MessageBox(hParentWnd, "SSTP�T�[�o�[���N�����Ă��܂���B", "TestSSTP", MB_ICONSTOP);
		return FALSE;
	}

	// �E�B���h�E�擾
	if(!CheckSakuraFileMapping(hParentWnd, vec))
	{
		//MessageBox(hParentWnd, "DirectSSTP���g�p�ł��܂���B", "TestSSTP", MB_ICONSTOP);
		return FALSE;
	}

	wsprintf(szBuffer, "%d", hParentWnd);

	strSendBuffer = "NOTIFY SSTP/1.5\r\n";
	strSendBuffer += "Sender: ";
	strSendBuffer += szClient;
	strSendBuffer += "\r\n";
	strSendBuffer += "Event: ";
	strSendBuffer += id;
	strSendBuffer += "\r\n";
	int	n='0';
	for (deque<string>::iterator i=refs.begin() ; i!=refs.end() ; ++i, ++n) {
		strSendBuffer += "Reference";
		strSendBuffer += (char)n;
		strSendBuffer += ": ";
		strSendBuffer += *i;
		strSendBuffer += "\r\n";
	}
	strSendBuffer += "Charset: Shift_JIS";
	strSendBuffer += "\r\n";
	strSendBuffer += "\r\n";

	// ���M
	cds.dwData = MY_DIRECTSSTP_PORT;
	cds.cbData = strSendBuffer.size();
	cds.lpData = (LPVOID)strSendBuffer.c_str();
	for(it = vec.begin(); it != vec.end(); it++)
	{
		// WM_COPYDATA �͔ėp�Ȃ̂ŁAHWND_BROADCAST ���Ă͂����܂���B
		SendMessageTimeout(*it, WM_COPYDATA, (WPARAM)hParentWnd, (LPARAM)&cds,
			SMTO_ABORTIFHUNG, 1000, &dwRet);
	}

	return TRUE;
}

/*----------------------------------------------------------------------------
	���[�J����`
----------------------------------------------------------------------------*/
#define MY_LOCALHOST	"127.0.0.1"
#define MY_SAKURA_MUTEX	"sakura"
#define MY_SSP_MUTEX	"ssp"


/*----------------------------------------------------------------------------
	CheckSakuraMutex()
	SSTP �T�[�o�[���݊m�F
----------------------------------------------------------------------------*/
BOOL CheckSakuraMutex()
{
	//return TRUE;
	HANDLE hMutex;

	hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MY_SAKURA_MUTEX);
	if(hMutex == NULL) 
	{
		hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MY_SSP_MUTEX);
		if(hMutex == NULL) 
		{
			return FALSE;
		}

		CloseHandle(hMutex);
		return TRUE;
	}

	CloseHandle(hMutex);
	return TRUE;
}

