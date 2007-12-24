#include "Family.h"
#include "random.h"


// �v�f�͒P��܂��̓g�[�N�B

// �������O�����v�f�̏W����Family�B

// Families�͖��O�ɂ����肳���Family�̏W���B
// �ق� map< string, Family<T> > ����public�p���͂����A�C���^�t�F�[�X�����肷��

template<typename T>
class Families
{
    typedef typename map< string, Family<T> >::iterator iterator;
    typedef typename map< string, Family<T> >::const_iterator const_iterator;
	
	set<string> m_clearOC_at_talk_end;
	map< string, Family<T> > m_elements;
	
public:
	//Families() { cout << "Families()" << endl; }
	//~Families() { cout << "~Families()" << endl; }
	
	// �v�f�̓o�^
	const T& add_element(const string& i_name, const T& i_t, const Condition& i_condition = Condition())
	{
		std::pair<iterator,bool> found = m_elements.insert(map< string, Family<T> >::value_type(i_name,Family<T>()));
		if ( found.second ) {
			found.first->second.set_namevec(i_name);
		}
		return found.first->second.add_element(i_t, i_condition);
	}
	
	// �ߋ��݊��̒�
	const map< string, Family<T> >& compatible() const
	{
		return m_elements;
	}
	
	// ���O����Family���擾
	Family<T>* get_family(string i_name)
	{
		iterator i = m_elements.find(i_name);
		return ( i == m_elements.end() ) ? NULL : &(i->second);
	}
	
	// ���O�̑��݂��m�F
	bool is_exist(const string& i_name) const
	{
		return m_elements.find(i_name) != m_elements.end();
	}
	
	// T���P�I�����A���̃|�C���^��Ԃ�
	const T* select(const string& i_name, Evalcator& i_evalcator)
	{
		iterator it = m_elements.find(i_name);
		if ( it == m_elements.end() ) {
			return NULL;
		}
		return it->second.select(i_evalcator);
	}
	
	// �폜
	void erase(const string& i_name)
	{
		m_elements.erase(i_name);
		m_clearOC_at_talk_end.erase(i_name);
	}
	
	// �g�[�N�̏I����ʒm�B�d��������Ԃ��u�g�[�N���v�ł���Family�̏d����𐧌���N���A����
	void handle_talk_end()
	{
		for ( set<string>::iterator it = m_clearOC_at_talk_end.begin() ; it != m_clearOC_at_talk_end.end() ; ++it )
		{
			get_family(*it)->clear_OC();
		}
	}
	
	// family��
	int size_of_family() const
	{
		return m_elements.size();
	}
	
	// �SFamily�̑S�v�f�����v�Z
	int size_of_element() const
	{
		int r = 0;
		for ( const_iterator it = m_elements.begin() ; it != m_elements.end() ; ++it )
		{
			r += it->second.size_of_element();
		}
		return r;
	}
	
	// �S�N���A
	void clear()
	{
		m_elements.clear();
		m_clearOC_at_talk_end.clear();
	}
	
	// �d����𐧌��I������B�����̓^�C�v�A����
	void setOC(string i_name, string i_value)
	{
		iterator st, ed;
		if ( i_name == "��" )
		{
			st = m_elements.begin();
			ed = m_elements.end();
		}
		else
		{
			st = m_elements.find(i_name);
			if ( st == m_elements.end() )
			{
				sender << "'" << i_name << "' �͑��݂��܂���B" << endl;
				return;
			}
			++(ed = st);
		}
		
		strvec argv;
		const int n = split(i_value, "�A,", argv);
		const string method = (n>=1) ? argv[0] : "����";
		const string span = (n>=2) ? argv[1] : "�N����";
		
		for ( iterator it = st; it != ed ; ++it )
		{
			Family<T>& family = it->second;
			if ( family.empty() )
			{
				continue;
			}
			
			if (0)
				NULL;
			else if ( method=="���O" )
				family.set_OC(new OC_NonDual<const T*>);
			else if ( method=="�~��" || method=="����" )
				family.set_OC(new OC_Sequential<const T*>);
			else if ( method=="����" || method=="�t��" )
				family.set_OC(new OC_SequentialDesc<const T*>);
			else if ( method=="�L��" || method=="���S" )
				family.set_OC(new OC_NonOverlap<const T*>);
			else if ( method=="����" )
				family.set_OC(new OC_Random<const T*>);
			else
				sender << "�d����𐧌�̕��@'" << method << "' �͒�`����Ă��܂���B" << endl;
			
			if ( span == "�g�[�N��" )
				m_clearOC_at_talk_end.insert(it->first);
			else if ( span == "�N����")
				NULL;
			else
				sender << "�d������̊���'" << method << "' �͒�`����Ă��܂���B" << endl;
			
		}
	}
	
	const Talk* communicate_search(const string& iSentence, bool iAndMode) const
	{
		sender << "�����̌������J�n" << endl;
		sender << "�@�Ώە�����: " << iSentence << endl;
		sender << "�@�S�P���v���[�h: " << (iAndMode?"true":"false") << endl;

		vector<const_iterator> elem_vector;

		bool isComNameMode = (iSentence.find("�u") != string::npos);
		if ( isComNameMode ) {
			sender << "�@�u�����A���O���胂�[�h�Ɉڍs" << endl;
			for ( const_iterator it = m_elements.begin() ; it != m_elements.end() ; ++it )
			{
				if ( it->second.is_comname() ) {
					string comName = it->second.get_comname();
					if ( comName.length() ) {
						if ( iSentence.compare(0,comName.size(),comName) == 0 ) {
							elem_vector.push_back(it);
						}
					}
				}
			}
		}
		else {
			sender << "�@�u�Ȃ��A�ʏ�R�~���T�����[�h�Ɉڍs" << endl;
			for ( const_iterator it = m_elements.begin() ; it != m_elements.end() ; ++it )
			{
				if ( ! it->second.is_comname() ) {
					elem_vector.push_back(it);
				}
			}
		}

		if ( elem_vector.size() <= 0 ) {
			sender << "����: �Y���Ȃ��i�����������Ȃ��j" << endl;
			return	NULL;
		}
		
		vector<const Talk*>	result;
		int	max_hit_point=0;
		for ( vector<const_iterator>::const_iterator it = elem_vector.begin() ; it != elem_vector.end() ; ++it )
		{
			// ��Q��S�p�X�y�[�X�ŋ�؂�
			const strvec &words = (**it).second.get_namevec();
			
			// �����̒P�ꂪ�q�b�g�������B�P��P��10�Ă�A�����P������1�Ă�
			int	hit_point=0;
			strvec::const_iterator wds_it=words.begin();
			if ( isComNameMode ) { wds_it += 1; }; //�ЂƂ߂͖��O�i���łɒ��o�ρj

			for ( ; wds_it!=words.end() ; ++wds_it )
			{
				if ( iSentence.find(*wds_it) != string::npos )
				{
					if ( (!isComNameMode) && compare_tail(*wds_it, "�u") )	// ������ �u �ł�����̂����̏ꍇ�̓q�b�g�ƌ��Ȃ��Ȃ��悤�ɁB
						hit_point += 4;
					else
						hit_point += 10+(wds_it->size()/4);	// ��v�����P��B
				}
				else
				{
					hit_point -= (iAndMode ? 999 : 1);	// ��v���Ȃ������A������Ȃ������P��
				}
			}
			if ( hit_point<=4 )
			{
				continue;	// ����������v���Ȃ��ꍇ
			}
			
			sender << "'" << (**it).first << "' : " << hit_point << "pt ,";
			
			if ( hit_point<max_hit_point) {
				sender << "�p��" << endl;
				continue;
			} else if ( hit_point == max_hit_point ) {
				sender << "���Ƃ��Ēǉ�" << endl;
			} else {
				max_hit_point = hit_point;
				sender << "�P�Ƃō̗p" << endl;
				result.clear();
			}
			
			
			(**it).second.get_elements_pointers(result);
		}
		
		sender << "����: ";
		if ( result.size() <= 0 ) {
			sender << "�Y���Ȃ��i������₠��A�P�ꌟ�����s�j" << endl;
			return	NULL;
		}
		
		return result[ random(result.size()) ];
	}
};


