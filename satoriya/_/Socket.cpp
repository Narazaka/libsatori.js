#include	"dvsWMClientSocket.h"
#include	<winsock2.h>

// WinSock��������
bool	Socket::makeup() {

	WSADATA winsock_data;
	WORD	request_winsock_version = MAKEWORD(1,1);
	if( WSAStartup(request_winsock_version, &winsock_data) != 0 ) {
		return	false;
	}

	if ( winsock_data.wVersion != request_winsock_version ) {
		WSACleanup();
		return	false;
	}

	return	true;
}

// WinSock�����ЂÂ�
void	Socket::cleanup() {
	WSACleanup();
}

// WinSock�̃G���[�ڍׂ��擾
int Socket::getLastError() {
	return WSAGetLastError();
}

// �N���X������
Socket::Socket() {
	mID = INVALID_SOCKET;
}

// �N���X���
Socket::~Socket() {
	this->disconnect();
}

// �ڑ�
bool	Socket::connect(const char* iHostName, unsigned short iPortNumber) {
	this->disconnect();

	// �\�P�b�g�쐬
	mID = socket(AF_INET, SOCK_STREAM, 0);
	if( mID==INVALID_SOCKET )
		return	false;

	// �T�[�o��IP�A�h���X���擾
	unsigned long server_ip_addres;
	server_ip_addres = inet_addr((char*)iHostName);
	if ( server_ip_addres == INADDR_NONE ) {
		// 10�i�E�h�b�g��؂�łȂ��ꍇ�̓z�X�g����ϊ����Ď擾
		struct hostent* the_hostent = gethostbyname(iHostName);
		if(the_hostent == NULL) {
			this->disconnect();
			return	false;
		}
		server_ip_addres = *((unsigned long *)(the_hostent->h_addr_list)[0]);
	}

	// �ڑ��̂��߂̍\���̂�ݒ�
	sockaddr_in server_sock_addres;
	memset((char*)(&server_sock_addres), 0, sizeof(sockaddr_in));
	server_sock_addres.sin_family = AF_INET;
	server_sock_addres.sin_addr.s_addr = server_ip_addres;
	server_sock_addres.sin_port = htons((unsigned short)iPortNumber);

	// ���ۂɐڑ�
	if( ::connect(mID, (struct sockaddr*)&server_sock_addres, sizeof(sockaddr_in))==SOCKET_ERROR ) {
		this->disconnect();
		return	false;
	}

	return	true;
}

// �ؒf
void	Socket::disconnect() {
	if ( mID == INVALID_SOCKET )
		return;
	shutdown(mID, 2);
	closesocket(mID);
	mID = INVALID_SOCKET;
}

// ���M
bool	Socket::send(const char* iBuffer, int iLength, int* oSendedBytes) {
	int r = ::send(mID, iBuffer, iLength, 0);
	if ( r == SOCKET_ERROR )
		return	false;
	if ( oSendedBytes != NULL )
		*oSendedBytes = r;
	return	true;
}

// ��M
bool	Socket::receive(char* oBuffer, int iLength, int* oReceivedBytes) {
	int	r = recv(mID, oBuffer, iLength, 0);
	if ( r == SOCKET_ERROR )
		return	false;
	if ( oReceivedBytes != NULL )
		*oReceivedBytes = r;
	return	true;
}

// �ڑ������H
bool	Socket::isConnected() {
	return	(mID != INVALID_SOCKET);
}
