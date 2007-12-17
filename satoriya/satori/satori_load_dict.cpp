#include	"satori.h"

#include	<fstream>
#include	<cassert>
#include <algorithm>

#include	"../_/Utilities.h"
#include	"satori_load_dict.h"

#ifdef POSIX
#  include <iostream>
#  include "stltool.h"
#endif

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

struct satori_unit
{
	string typemark;
	string name;
	string condition;
	strvec body;
};
#ifdef _DEBUG
ostream& operator<<(ostream& o, const satori_unit& su)
{
	o << su.typemark << "/" << su.name << "/" << su.condition << endl;
	o << su.body;
	return o;
}
#endif

static void lines_to_units(
	const vector<string>& i_lines,
	const vector<string>& i_typemarks,
	const string& i_name_cond_delimiter,
	vector<satori_unit>& o_units)
{
	vector<string>::const_iterator line_it = i_lines.begin();
	for ( ; line_it != i_lines.end() ; ++line_it)
	{
		//cout << *line_it << endl;

		// �s����typemarks�̂����ꂩ���o�����Ă��邩�T��
		vector<string>::const_iterator mark_it = i_typemarks.begin();
		for ( ; mark_it != i_typemarks.end() ; ++mark_it) 
		{
			if ( line_it->compare(0, mark_it->size(), mark_it->c_str()) ==0 )
			{
				break;
			}
		}
		
		// �o�����Ă���w�b�_�A�����Ȃ��΃{�f�B
		if ( mark_it != i_typemarks.end() )
		{
			satori_unit unit;
			unit.typemark = *mark_it;
			
			const char* name = line_it->c_str() + mark_it->size();
			const char* delimiter = strstr_hz(name, i_name_cond_delimiter.c_str());
			if ( delimiter==NULL )
			{
				unit.name.assign(name);
			}
			else
			{
				unit.name.assign(name, delimiter);
				unit.condition.assign(delimiter + i_name_cond_delimiter.size());
			}

			o_units.push_back(unit);
		}
		else
		{
			if ( o_units.empty() ) 
			{
				// o_units����̂Ƃ��i�ŏ���typemarks�o���O�j�̓R�����g�ƌ��􂵁A�������Ȃ��B
			}
			else
			{
				// �Ō��o_units�Ƀ~��ǉ�
				o_units.back().body.push_back(*line_it);
			}
		}
	}

	// �eunit�����̋�s�����
	for ( vector<satori_unit>::iterator i = o_units.begin() ; i != o_units.end() ; ++i )
	{
		while (true)
		{
			strvec::iterator j = i->body.end();
			if ( j == i->body.begin() || (--j)->length()>0 )
			{
				break;
			}
			i->body.pop_back();
		}

		//sender << *i << endl;
	}
}


// �v���v���Z�X�I�ȏ����B
// ���s�L�����Z���K�p�A�R�����g�폜�Abefore_replace�̓K�p
static bool pre_process(
	const strvec& in,
	strvec& out,

	escaper& io_escaper,
	strmap& io_replace_dic
	
	)
{
	int	kakko_nest_count=0;	// "�i" �̃l�X�g���B1�ȏ�̏ꍇ�͉��s�𖳌�������B
	string	accumulater="";	// �s������ނ�[��
	int	line_number=1;
	for ( strvec::const_iterator fi=in.begin() ; fi!=in.end() ; ++fi, ++line_number )
	{
		const char* p=fi->c_str();
		bool	escape = false;

		// �J�b�R���̏ꍇ�A�s���̃^�u�͖�������B
		if ( kakko_nest_count>0 )
			while ( *p=='\t' )
				++p;

		// ��s�i�����s�j�ɑ΂��鏈��
		while ( *p!='\0' )
		{
			string	c=get_a_chr(p);	// �S�p���p��킸�ꕶ���擾�B

			if ( escape ) 
			{
				accumulater += (c=="��") ? c : io_escaper.insert(c);
				escape = false;
			}
			else 
			{
				if ( c=="��" ) 
				{
					escape = true;
					continue;
				}
				if ( c=="��" )
					break;	// �s�I��

				if ( c=="�i" )
					++kakko_nest_count;
				else if (  c=="�j" && kakko_nest_count>0 )
					--kakko_nest_count;

				accumulater += c;
			}
		}

		if ( escape )
		{
			continue;
		}

		if ( kakko_nest_count==0 )
		{
			// �u�����������K�p
			for ( strmap::iterator di=io_replace_dic.begin() ; di!=io_replace_dic.end() ; ++di )
			{
				replace(accumulater, di->first, di->second);
			}

			// ��s�ǉ�
			out.push_back(accumulater);
			//sender << line_number << " [" << accumulater << "]" << endl;
			accumulater="";
		}
		else if ( line_number == in.size() ) 
		{
			// �G���[
			return false;
		}
	}
	return true;
}

string	Satori::SentenceToSakuraScriptExec_with_PreProcess(const strvec& i_vec)
{
	strvec vec;
	pre_process(i_vec, vec, m_escaper, replace_before_dic);
	return SentenceToSakuraScriptExec(vec);
}

// .txt��.sat�̗���������̂ŁA�V������������ǂݍ��ށB
static bool select_dict_and_load_to_vector(const string& iFileName, strvec& oFileBody, bool warnFileName)
{
	string txtfile = set_extention(iFileName, "txt");
	string satfile = set_extention(iFileName, "sat");

	string realext = get_extention(iFileName);

	bool FileExist(const string& f);
	bool decodeMe = false;
	string file;

	//SAT / TXT
	if ( realext == "sat" ) {
		if ( FileExist(satfile.c_str()) ) {
			file = satfile;
			decodeMe = true;
		}
		else {
			if ( warnFileName ) {
				sender << "  " << satfile << "is not exist.";
			}
			file = txtfile;
		}
	}
	else {
		if ( FileExist(txtfile.c_str()) ) {
			file = txtfile;
		}
		else {
			if ( warnFileName ) {
				sender << "  " << txtfile << "is not exist.";
			}
			file = satfile;
			decodeMe = true;
		}
	}

	sender << "  loading " << get_file_name(file);
	if ( !strvec_from_file(oFileBody, file) )
	{
		sender << "... failed." << endl;
		return	false;
	}

	if ( decodeMe ) {
		// �Í���������
		for ( strvec::iterator it=oFileBody.begin() ; it!=oFileBody.end() ; ++it )
		{
			*it = decode( decode(*it) );
		}
	}

	return true;
}

static bool satori_anchor_compare(const string &lhs,const string &rhs)
{
	return lhs.size() > rhs.size();
}

// ������ǂݍ��ށB
bool Satori::LoadDictionary(const string& iFileName,bool warnFileName) 
{
	// �t�@�C������vector�֓ǂݍ��ށB
	// ���̍ہA���t�@�C�����Ŋg���q��.txt��.sat�̃t�@�C���̓��t���r���A�V�������������̗p����B
	strvec	file_vec;
	if ( !select_dict_and_load_to_vector(iFileName, file_vec, warnFileName) )
	{
		return false;
	}

	bool	is_for_anchor = compare_head(get_file_name(iFileName), "dicAnchor");

	strvec preprocessed_vec;
	if ( false == pre_process(file_vec, preprocessed_vec, m_escaper, replace_before_dic) )
	{
#ifdef POSIX
	        // MessageBox�Ȃ�Ė����I
	        std::cerr <<
		    "syntax error - SATORI : " << iFileName << std::endl <<
		    std::endl <<
		    "There are some mismatched parenthesis." << std::endl <<
		    "The dictionary is not loaded correctly." << std::endl <<
		    std::endl <<
		    "If you want to display parenthesis independently," << std::endl <<
		    "use \"phi\" symbol to escape it." << std::endl;
#else
		::MessageBox(NULL, 
			(string() + iFileName + "\n\n"
			"\n"
			"�J�b�R�̑Ή��֌W���������Ȃ�����������܂��B" "\n"
			"�����͐������ǂݍ��܂�Ă��܂���B" "\n"
			"\n"
			"�J�b�R��P�Ƃŕ\������ꍇ�́@�Ӂi�@�ƋL�q���Ă��������B").c_str(),
			"syntax error - SATORI", MB_OK|MB_SYSTEMMODAL);
#endif
	}

	static vector<string> typemarks;
	if ( typemarks.empty() )
	{
		typemarks.push_back("��");
		typemarks.push_back("��");
	}

	vector<satori_unit> units;
	lines_to_units(preprocessed_vec, typemarks, "\t", units); // �P��Q��/�g�[�N���ƍ̗p�������̋�؂�


	for ( vector<satori_unit>::iterator i=units.begin() ; i!=units.end() ; ++i)
	{
		// �����̋�s���폜
		//while ( i->body.size()>0 && i->body.size()==0 )
		//{
		//	i->body.pop_back();
		//}
	        while (i->body.size() > 0 && i->body[i->body.size()-1].length() == 0) {
		        i->body.pop_back();
		}
		
		if ( i->typemark == "��" )
		{
			// �g�[�N�̏ꍇ
			if ( is_for_anchor ) { anchors.push_back(i->name); }
			talks.add_element(i->name, i->body, i->condition);
		}
		else
		{
			// �P��Q�̏ꍇ
			const strvec& v = i->body;
			for ( strvec::const_iterator j=v.begin() ; j!=v.end() ; ++j)
			{
				words.add_element(i->name, *j, i->condition);
			}
		}

	}

	if ( is_for_anchor ) {
		sort(anchors.begin(),anchors.end(),satori_anchor_compare);
	}

	//sender << "�@�@�@talk:" << talks.count_all() << ", word:" << words.count_all() << endl;
	sender << "... ok." << endl;
	return	true;
}

#ifdef POSIX
#  include <sys/types.h>
#  include <dirent.h>
#endif

void list_files(string i_path, vector<string>& o_files)
{
	unify_dir_char(i_path); // \\��/�����ɉ����ēK�؂ȕ��ɓ���
#ifdef POSIX

	DIR* dh = opendir(i_path.c_str());
	if (dh == NULL)
	{
	    sender << "file not found." << endl;
	}
	while (1) {
	    struct dirent* ent = readdir(dh);
	    if (ent == NULL) {
		break;
	    }
#if defined(__WINDOWS__) || defined(__CYGWIN__)
	    string fname(ent->d_name);
#else
//	    string fname(ent->d_name, ent->d_namlen);
	    string fname(ent->d_name);
#endif
		o_files.push_back(fname);
	}
	closedir(dh);
#else /* POSIX */
	HANDLE			hFIND;	// �����n���h��
	WIN32_FIND_DATA	fdFOUND;// ���������t�@�C���̏��
	hFIND = ::FindFirstFile((i_path+"*.*").c_str(), &fdFOUND);
	if ( hFIND == INVALID_HANDLE_VALUE )
	{
		sender << "file not found." << endl;
	}

	do
	{
		o_files.push_back(fdFOUND.cFileName);
	} while ( ::FindNextFile(hFIND,&fdFOUND) );
	::FindClose(hFIND);
#endif /* POSIX */

}




int Satori::LoadDicFolder(const string& i_base_folder)
{
	sender << "LoadDicFolder(" << i_base_folder << ")" << endl;
	vector<string> files;
	list_files(i_base_folder, files);

	int count = 0;
	
	for (vector<string>::const_iterator it=files.begin() ; it!=files.end() ; ++it)
	{
		const int len = it->size();
		if ( len < 7 ) { continue; } // dic.txt���ŒZ�t�@�C����
		if ( it->compare(0,3,"dic") != 0 ) { continue; }
		if ( it->compare(len-4,4,".txt") != 0 && it->compare(len-4,4,".sat") != 0 ) { continue; }

		if ( LoadDictionary(i_base_folder + *it) ) {
			++count;
		}
	}

	sender << "ok." << endl;
	return count;
}
