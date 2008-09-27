/* -*- c++ -*- */
#ifndef POSIX
#  include <windows.h>	// HMODULE,BOOL,HGLOBAL�Ƃ�
#endif

#include "SaoriClient.h"

class Satori;

// �v���O�C���̑����Ǘ�
class ShioriPlugins {

	struct CallData {	// �Ăяo�������Ƃ̏��
		string	mDllPath;
		strvec	mPreDefinedArguments;
		bool	mIsBasic;
	};
	struct DllData {	// DLL���Ƃ̏��
		SaoriClient	mSaoriClient;
		int	mRefCount;
	};
	map<string, CallData>	mCallData;	// �Ăяo�����G�Ăяo�������Ƃ̏��
	map<string, DllData>	mDllData;	// DLL�̃t���p�X�GDLL���Ƃ̏��

	string	mBaseFolder;

	Satori *pSatori;

	ShioriPlugins(void) { }

public:
	ShioriPlugins(Satori *pSat) : pSatori(pSat) {
	}

	bool	load(const string& iBaseFolder);
	bool	load_a_plugin(const string& iPluginLine);

	string	request(const string& iCallName, const strvec& iArguments, strvec& oResults, const string& iSecurityLevel);
	void	unload();

	bool	find(string iCallName) {
		return (mCallData.find(iCallName) != mCallData.end() );
	}
};

