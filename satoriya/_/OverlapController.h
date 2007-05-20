/*
	�u�w�肳�ꂽ�ő�l�͈̔͂Ő�����I������v���߂̂��낢��B

  �@�I����@�͂��낢��B
  �@�����_���Ƃ��d������Ƃ����ԂƂ��B

	�u�d������󋵁v�͕ۑ�/���A���\�B
*/

#include	<iostream>	// �V���A���C�Y�p
#include	<cassert>

int	random(); // 32bit�̓K���ȗ����l��Ԃ��Ƃ��Ă��������B
string	stringf(const char* iFormat, ...); // printf�݊��ŕ�����𐶐����Astring�^�ŕԂ��B

// �����I���N���X�̃x�[�X�N���X�B
class OverlapController {

private:
	int	mSize; // �ő吔�B0�`���̐�-1���I�����ƂȂ�B
	int	mChoiceCount; // �I����
	
	//int	mTotalChoiceCount;	// �I��
	//int	mResetCount;

protected:
	// �u�P�I���v�̎����B
	virtual int choice_impl()=0;
	// �T�C�Y�ύX�̎����B����Ă�ł�Ԃ�size()�͌��̒l��Ԃ��B
	// �d������󋵂��ێ�����Ă��邩�ǂ�����Ԃ��B
	virtual bool resize_impl(int) { return true; }
	// �d������󋵃N���A�̎����B
	virtual void clear_impl() {}

	virtual string	serialize_impl() const =0;
	virtual void	unserialize_impl(string)=0;
	
public:
	OverlapController(int iSize) : mSize(iSize), mChoiceCount(0) { assert(iSize>0); }
	virtual ~OverlapController() { clear(); }

	// �P�I�����ĕԂ�
	int choice() {
		++mChoiceCount;
		return	choice_impl();
	}

	// �d������󋵂̃N���A
	void clear() {
		clear_impl();
		mChoiceCount = 0;
	}

	// �ő�l�̕ύX�B
	bool resize(int iSize) {
		assert(iSize > 0);
		bool r = resize_impl(iSize);
		if (r)
			mChoiceCount = 0;
		mSize = iSize;
		return r;
	}

	// �ő�l�̎擾
	int size() const { assert(mSize>0); return mSize; }
	// �Ō�ɏd������󋵂����Z�b�g����Ă���Achoice���Ă΂ꂽ�񐔂̎擾
	int	choice_count() const { return mChoiceCount; }

	// �ۑ��ƕ��A�B
	// ���̃N���X�̃C���X�^���X��serialize�������̂��A����N���X�̃C���X�^���X��unserialize�����ہA
	// �d������󋵂��Č������΂���ŗǂ��B
	friend  ostream&	operator<<(ostream& out, const OverlapController& oc);
	friend  istream&	operator>>(istream& in, OverlapController& oc);
};
inline ostream&	operator<<(ostream& out, const OverlapController& oc) {
	out << "1 " << oc.mSize << " ";
	return out;
}
inline istream&	operator>>(istream& in, OverlapController& oc) {
	int	version;
	in >> version;
	in >> oc.mSize;
	return in;
}


// �����_���ȑI��
class OC_Random : public OverlapController {

	// �P�I�����ĕԂ�
	virtual	int	choice_impl() {
		return	((unsigned)random()) % size();
	}

public:
	OC_Random(int iSize) : OverlapController(iSize) {}
	virtual ~OC_Random() {}
};

// ���ԂɑI��
class OC_Sequential : public OverlapController {

	int	mIndex;

	// �P�I�����ĕԂ�
	virtual int choice_impl() {
		if ( mIndex >= size() )
			mIndex = 0;
		return	mIndex++;
	}
	
	// �ő�l�̕ύX�B
	virtual bool resize_impl(int iSize) {
		if ( iSize > mIndex )
			return	true;
		mIndex = 0;
		return	false;
	}

	// �d������󋵂̃N���A
	virtual void clear_impl() {
		mIndex = 0;
	}
	
public:
	OC_Sequential(int iSize) : OverlapController(iSize), mIndex(0) {}
	virtual ~OC_Sequential() {}
};

// �t���ɑI��
class OC_SequentialDesc : public OverlapController {

	int	mIndex;

	// �P�I�����ĕԂ�
	virtual int choice_impl() {
		if ( --mIndex < 0 )
			mIndex = size()-1;
		return	mIndex;
	}
	
	// �ő�l�̕ύX�B
	virtual bool resize_impl(int iSize) {
		// �����̗]�n�͂��邪�A��index���s���ɂȂ����ꍇ�����N���A�Ƃ���B���������͌ォ��I�ԁB
		// ��index�ȏ�̏ꍇ�͊֌W�Ȃ��B
		if ( iSize > mIndex )
			return	true;
		mIndex = 0;
		return	false;
	}

	// �d������󋵂̃N���A
	virtual void clear_impl() {
		mIndex = 0;
	}
	

public:
	OC_SequentialDesc(int iSize) : OverlapController(iSize), mIndex(0) {}
	virtual ~OC_SequentialDesc() {}
};

// ���O�̂��̂����d�����
class OC_NonDual : public OverlapController {

	int	mLast;

	// �P�I�����ĕԂ�
	virtual int choice_impl() {
		if ( size()<2 )
			return	0; // ��Ƀ[��

		int	theSelect;
		do {
			theSelect = ((unsigned)random()) % size();
		} while ( theSelect == mLast ); // ���O�Ɠ����ł����Ȃ���΍\��Ȃ�

		mLast = theSelect;
		return	theSelect;
	}

	// �ő�l�̕ύX�B
	virtual bool resize_impl(int iSize) {
		if ( mLast < iSize ) // ����������ĂȂ����
			return	true;	 // �󋵂ɕω��͂Ȃ�
		mLast = -1;
		return	false;
	}

	// �d������󋵂̃N���A
	virtual void clear_impl() {
		mLast = -1;
	}

public:
	OC_NonDual(int iSize) : mLast(-1), OverlapController(iSize) {}
	virtual ~OC_NonDual() {}

};

// ���S�Ɉꏄ����܂ŏd�����Ȃ�
#include	<deque>
class OC_NonOverlap : public OverlapController {

	std::deque<int>	mArray; // �I���ς݂𔻒肷��z��
	int	mRestCount;				// �܂��I�����Ă��Ȃ��c�萔

	// �P�I�����ĕԂ�
	virtual int choice_impl() {
		if ( mArray.empty() )
			init(size());

		assert(mRestCount > 0);
		int select = ((unsigned)random()) % mRestCount; // ���ݎc���Ă钆�̂ǂꂩ��I�ԁB

		// �{�[����~���̎�O�Ǝ��ւ���
		int	ball = mArray[select];
		mArray[select] = mArray[mRestCount-1];
		mArray[mRestCount-1] = ball;

		// �~�������炷�i�c�������炷�j�B�Ȃ��Ȃ�����S�����g�p�����B
		if ( --mRestCount==0 )
			mRestCount = size();

		return	ball;	// �I�񂾃{�[����Ԃ�
	}
	
	// �ő�l�̕ύX�B
	virtual bool resize_impl(int iSize) {
		if ( iSize < size() ) { 
			// �������Ȃ����Ȃ��蒼��
			init(iSize);
			return	false;
		}
		else {
			// �傫���Ȃ����Ȃ炻�̃T�C�Y�܂Ń{�[����ǉ��B
			for (int i=size(); i<iSize ; ++i )
				mArray.push_front(i);
			mRestCount += iSize - size();
			return	true; // �d�������Ԃ͈ێ������
		}
	}
	
	// �d������󋵂̃N���A
	virtual void clear_impl() {
		mRestCount = size();
	}
	
	// mArray�̏������B�K�R�I��mRestCount���������B
	void	init(int iSize) {

		// ���Ԃ͂ǂ��ł��悭�āA�P�Ɉ�ӂɃ{�[�����l�߂�
		mArray.resize(iSize);
		for (int i=0 ; i<iSize ; ++i) 
			mArray[i] = i;

		mRestCount = iSize;
	}
	
public:
	OC_NonOverlap(int iSize) : OverlapController(iSize) {}
	virtual ~OC_NonOverlap() {}
};



#include	<string>
using std::string;
#include	<vector>
using std::vector;


// ���ꂪ�����Ȃ̂͂ǂ����������H
// �T�C�Y���[���̎��Ƃ����B���[��[����Choice�����Ⴂ���Ȃ��킯���B
template<typename T>
class OverlapControllableVector : public vector<T> {
public:
	enum Mode { MIN_DUMMY=-1, RANDOM=0, SEQUENTIAL, SEQUENTIAL_DESC, NONDUAL, NONOVERLAP, MAX_DUMMY };

public:

	OverlapControllableVector() : mOC(NULL), mMode(MIN_DUMMY), vector<T>() {
	}
	
	~OverlapControllableVector() {
		if ( mOC!=NULL )
			delete mOC;
	}

	// �P�擾
	const T&		choice() { // OC�g���ėv�f���P�I��
		assert( !empty() );	 // ��̂Ƃ��ɂ͂��������Ă�ł͂����Ȃ�

		if ( mOC==NULL )
			selectOC(RANDOM);

		if ( mOC->size() != size() )// OC�ƃT�C�Y���Ⴄ�Ȃ�A
			mOC->resize(size());	// OC�����T�C�Y�ύX����B���������ꂤ��B

		const_iterator i=begin();
		advance(i, mOC->choice());
		return	*i;
	}

	// �d�����ݒ��������
	void	clearOC() {
		if ( mOC!=NULL )
			mOC->clear();
	}
	
	// ���[�h�؂�ւ��B�d�����ݒ�͏����������B
	void	selectOC(Mode iMode) {
		assert( iMode>MIN_DUMMY && iMode<MAX_DUMMY ); // ���[�h�͔͈͓��ł���K�v������
		assert( !empty() );
		if ( mMode == iMode )
			return; // �ω������B

		if ( mOC != NULL )
			delete mOC; // ��������Ȃ�����Ă���

		mMode = iMode;
		switch ( mMode ) {
		case RANDOM: mOC = new OC_Random(size()); break;
		case SEQUENTIAL: mOC = new OC_Sequential(size()); break;
		case SEQUENTIAL_DESC: mOC = new OC_SequentialDesc(size()); break;
		case NONDUAL: mOC = new OC_NonDual(size()); break;
		case NONOVERLAP: mOC = new OC_NonOverlap(size()); break;
		default: assert(0); break;
		}
	}

	// OC�̕ۑ��ƕ��A
	string	serializeOC() {
		//stringf("mode=%x,"
	}
	void	deserializeOC(const string&) {
		//sscanf
	}


private:
	OverlapController*	mOC;
	Mode	mMode;
};


/*
// ���[�h�؂�ւ�
void	overlap_control::setMode(Mode iMode) {
	if ( iMode == mMode )
		return;
	mMode = iMode;
	if ( iMode == RANDOM )
		
}

string	overlap_control::serialize() {
	if ( mMode == NONOVERLAP ) {
		string	str = "2:";
		
	}
	else if ( mMode == SEQUENTIAL ) {
		char	buf[256];
		sprintf("1:%d:", mIndex);
	}
	else 
		return	"0:";
}
void	overlap_control::deserialize(const string&) {

}
*/
