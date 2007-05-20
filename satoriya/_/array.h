#ifndef	ARRAY_H
#define	ARRAY_H

#if 0  // --------------------------------------------------------

	�Œ蒷�z��R���e�i array<class T>

	2000/08�@����͂�
	2001/02�@�����Q�͂�

#endif // --------------------------------------------------------

#include	<assert.h>

template<class T>
class array {
public:
	// �v�f�����w�肵�Ĕz����쐬
	array(int iSize=0);
	// �z���j��
	~array();
	// �R�s�[�R���X�g���N�^
	array( const array& iArray );
	// ������Z�q
	array&	operator = ( const array& iArray );	
	// �z���L�k
	void	resize(int);
	
	// �z�񂪐������m�ۂ��ꂽ���H
	operator bool() const { return (mArray != NULL); }

	// �v�f�̎Q��
	T&	operator[]( int iPosition ) const {
		assert(iPosition >= 0);
		assert(iPosition < size());
		return	mArray[iPosition];
	}
	// �v�f�������A�Y������Ԃ��B������Ȃ���� -1�B
	int	find( const T& iT, int iStartPosition=0 );

	// �v�f�����擾
	int		size() const { return mSize; }
	operator int() const { return size(); }
	bool 	empty() const { return size() == 0; }

	// �z����N���A
	void	clear() { resize(0); }

	// �e�v�f�Ɉ����l����
	void	fill( const T& iT ) {
		for ( int i=0 ; i<mSize ; i++ )
			(*this)[i] = iT;
	}

	// �e�v�f���֐��ɓn���B
	// �֐��� Function( T& ); �ŌĂяo�����B
	// �Ԓl�͗��p����Ȃ��B
	template<class Function>
	void	each( Function iFunction ) {
		for ( int i=0 ; i<mSize ; i++ )
			iFunction( (*this)[i] );
	}

private:
	T*	mArray;
	int	mSize;
};

#ifdef	_OSTREAM_
template<class T>
inline ostream& operator<<( ostream& stream, const array<T>& ary ) {
	for ( int i=0 ; i<ary.size() ; i++ ) {
		stream << (ary[i]);
		if ( i < ary.size()-1 )
			stream << ", ";
	}
	return	stream;
}
#endif

// --------------------------------------------------------

// �v�f�����w�肵�Ĕz����쐬
template<class T>
array<T>::array<T>( int iSize ) : mSize(iSize) {
	assert(iSize >= 0);
	mArray = new T[mSize];
}

// �z���j��
template<class T>
array<T>::~array() {
	delete [] mArray;
}

// �R�s�[�R���X�g���N�^
template<class T>
array<T>::array<T>( const array<T>& iArray ) : mSize(iArray.mSize) {
	mArray = new T[mSize];
	for ( int i=0 ; i<mSize ; i++ )
		mArray[i] = iArray[i];
}

// ������Z�q
template<class T>
array<T>&	array<T>::operator = ( const array<T>& iArray ) {
	if ( &iArray == this )
		return	*this;
	mSize = iArray.mSize;
	delete [] mArray;
	mArray = new T[mSize];
	for ( int i=0 ; i<mSize ; i++ )
		mArray[i] = iArray[i];
	return	*this;
}

// �z�񒷂̐L�k
template<class T>
void	array<T>::resize( int iSize ) {
	T*	thearray = new T[iSize];
	int	theCopyLen = ( mSize > iSize ) ? iSize : mSize;
	for ( int i=0 ; i<theCopyLen ; i++ )
		thearray[i] = mArray[i];
	delete [] mArray;
	mArray = thearray;
	mSize = iSize;
}

// �v�f���������A�����ƍŏ��Ɉ�v�������_�ł̓Y������Ԃ��B������Ȃ���� -1�B
template<class T>
int	array<T>::find( const T& iT, int iStartPosition ) {
	assert(iStartPosition < mSize);
	for ( int i=iStartPosition ; i<mSize ; i++ )
		if ( mArray[i] == iT )
			return	i;
	return	-1;
}


#endif	//	ARRAY_H
