#ifndef SAKURACS_H
#define SAKURACS_H

#include "../_/stltool.h"

// ���ʂ̊��N���X
class SakuraCS
{
};



// io_target����i_delimiter�̒��O�܂ł�؂�o���B
// i_delimiter��������Ȃ���΁A�c��S�Ă�؂�o���B
string cut_token(string& io_target, const string& i_delimiter);
// ����B������������Ȃ������ꍇ��false��Ԃ��B
bool cut_token(string& io_target, const string& i_delimiter, string& o_token);


#endif // SAKURACS_H
