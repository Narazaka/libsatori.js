#include "SakuraFMO.h"
#include	"satori.h"
//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////


bool	Satori::updateGhostsInfo() {

	if ( mExeFolder=="" )
	{
		return	false;
	}

	SakuraFMO theSakuraFMO;
	if ( !theSakuraFMO.update() )
	{
		return	false;
	}

	ghosts_info.clear();
	ghosts_info.push_back( strmap() );
	for( map<string, strmap>::iterator i=theSakuraFMO.begin() ; i!=theSakuraFMO.end() ; ++i ) {
		//�_�~�[���R��
		if ( i->first.find("ssp_fmo_header_dummyentry") != string::npos ) { continue; }
		if ( i->first.find("SSTPVIEWER-") != string::npos ) { continue; }
		if ( i->first.find("SSSB") != string::npos ) { continue; }

		strmap&	m = i->second;
		bool	isSelfData = false;// �������g�̃f�[�^���H

		if ( m.find("ghostpath") != m.end() ) {
			if ( stricmp((m["ghostpath"]+"ghost\\master\\").c_str(), mBaseFolder.c_str())==0 )
				isSelfData = true;
		}
		else {
			if ( stricmp(m["path"].c_str(), mExeFolder.c_str())==0 )
				isSelfData = true;
		}

		if ( isSelfData )
			ghosts_info[0]=m;	
		else
			ghosts_info.push_back(m);	// ���͏���������
	}

	return	true;
}
