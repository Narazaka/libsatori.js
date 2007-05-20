#ifndef	_adf2e6ed_6521_4ee5_8122_c523e9ccd2a7_
#define	_adf2e6ed_6521_4ee5_8122_c523e9ccd2a7_

#include	<vector>
using std::vector;

// �\�P�b�g�N���C�A���g�B
// �Ԓlfalse����getLastError�ŏڍׂ��擾�ł���B
// �S�C���X�^���X�̎g�p�O��makeup, �S�C���X�^���X�̎g�p���cleanup���邱�ƁB
// �vws2_32.lib�B
class Socket {
public:
	static	bool makeup();
	static	void cleanup();
	static	int	getLastError();

	Socket();
	virtual ~Socket();

	// �ڑ�
	bool	connect(const char* iHostName, unsigned short iPortNumber);
	// �ؒf
	void	disconnect();
	// ���M
	bool	send(const char* iBuffer, int iLength, int* oSendedBytes=NULL);
	// ��M
	bool	receive(char* oBuffer, int iLength, int* oReceivedBytes);

	// �ڑ������H
	bool	isConnected();

private:
	unsigned int mID;
};


#endif	//	_adf2e6ed_6521_4ee5_8122_c523e9ccd2a7_
