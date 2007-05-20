#ifndef	_adf2e6ed_6521_4ee5_8122_c523e9ccd2a7_
#define	_adf2e6ed_6521_4ee5_8122_c523e9ccd2a7_


// ���p���郉�C�u����
#include <string>
#include <exception>
#include <stdexcept>
using std::string;


// TCPSocket�̃��\�b�h���g�p�����O�I�u�W�F�N�g
struct socket_error : public std::runtime_error
{
	socket_error(string i_str) : runtime_error(i_str) {}
	virtual ~socket_error() throw() {}
};


// �\�P�b�g�N���C�A���g
class TCPSocket {
public:

	TCPSocket();
	virtual ~TCPSocket();

	// �ڑ�
	void	connect(const char* iHostName, unsigned short iPortNumber);
	void	connect(string iHostName, unsigned short iPortNumber) { TCPSocket::connect(iHostName.c_str(), iPortNumber); }
	// �ؒf
	void	disconnect();
	// ���M�@�߂�l��sended octet[s]
	int		send(const char* iBuffer, int iLength);
	// ��M�@�߂�l��received octet[s]
	int		receive(char* oBuffer, int iLength);

	// �ڑ������H
	bool	is_connect();

	// �s�x�[�X�̂��Ƃ�p�BCRLF�������ŕt�^�܂��͍폜����
	void	send_line(const string& i_str);
	string	receive_line();
	
	// �z�X�g����IP�A�h���X�ւƕϊ��i���O�����j����B�ł��Ȃ��������O�𓊂���B
	static string convert_hostname_to_IPaddress(string i_hostname) throw (socket_error);

private:
	unsigned int m_id;

	#ifdef _MSC_VER
		static int sm_num_of_instance;
		static bool sm_is_can_use_winsock2;

		static void winsock2_startup();
		static void winsock2_cleanup();
	#endif // _MSC_VER
};


#endif	//	_adf2e6ed_6521_4ee5_8122_c523e9ccd2a7_
