/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/SmartPointer/SmartPointer.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Smart-Pointers & Garbage collection using reference counting.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// _GCInfo implementation
template<typename T>
_GCInfo<T>::_GCInfo(T * pAddress, UInt iArraySize)
{
	m_pAddress = pAddress;
	m_iRefCount = 1;
	m_iArraySize = iArraySize;
	m_bIsArray = ( m_iArraySize > 0 );
	m_pNext = NULL;
    m_pPrev = NULL;
}
template<typename T>
_GCInfo<T>::~_GCInfo()
{
    // nothing to do
}

template<typename T>
inline Bool _GCInfo<T>::operator==(const _GCInfo<T> & rhs) const {
    return (m_pAddress == rhs.m_pAddress);
}

///////////////////////////////////////////////////////////////////////////////////
// _GCList implementation
template<typename T>
_GCList<T>::_GCList()
{
    m_pHead = NULL;
    m_iSize = 0;
    m_iMemoryUsed = 0;
}
template<typename T>
_GCList<T>::~_GCList()
{
    // nothing to do
}

template<typename T>
inline _GCInfo<T> * _GCList<T>::Begin() const {
    return m_pHead;
}
template<typename T>
inline UInt _GCList<T>::GetSize() const {
    return m_iSize;
}
template<typename T>
inline UInt _GCList<T>::GetMemoryUsage() const {
    return m_iMemoryUsed;
}

template<typename T>
Void _GCList<T>::PushFront( _GCInfo<T> * pNewNode )
{
	pNewNode->m_pPrev = NULL;
	pNewNode->m_pNext = m_pHead;
	if (m_pHead != NULL)
		m_pHead->m_pPrev = pNewNode;
	m_pHead = pNewNode;
	++m_iSize;
    if (pNewNode->m_bIsArray)
        m_iMemoryUsed += pNewNode->m_iArraySize * sizeof(T);
    else
        m_iMemoryUsed += sizeof(T);
}
template<typename T>
Void _GCList<T>::Remove( _GCInfo<T> * pNode )
{
	if (pNode == NULL || m_iSize == 0)
		return;
	if (pNode == m_pHead) {
		m_pHead = m_pHead->m_pNext;
		if (m_pHead != NULL)
			m_pHead->m_pPrev = NULL;
	} else {
		pNode->m_pPrev->m_pNext = pNode->m_pNext;
		if (pNode->m_pNext != NULL)
			pNode->m_pNext->m_pPrev = pNode->m_pPrev;
	}
	pNode->m_pPrev = NULL;
	pNode->m_pNext = NULL;
	--m_iSize;
    if (pNode->m_bIsArray)
        m_iMemoryUsed -= pNode->m_iArraySize * sizeof(T);
    else
        m_iMemoryUsed -= sizeof(T);
}
template<typename T>
_GCInfo<T> * _GCList<T>::Find( T * pAddress )
{
	_GCInfo<T> * pRes = m_pHead;
	while(pRes != NULL) {
		if (pRes->m_pAddress == pAddress)
			return pRes;
		pRes = pRes->m_pNext;
	}
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////////
// GCPointer implementation
template<typename T, UInt ARRAYSIZE>
GCPointer<T,ARRAYSIZE>::GCPointer(T * pAddress)
{
	_GCInfo<T> * pFound = sm_listGC.Find(pAddress);
	if ( pFound == NULL )
		sm_listGC.PushFront( new _GCInfo<T>(pAddress, ARRAYSIZE) );
	else
		++(pFound->m_iRefCount);

	m_pAddress = pAddress;
	m_iArraySize = ARRAYSIZE;
	m_bIsArray = ( m_iArraySize > 0 );
}
template<typename T, UInt ARRAYSIZE>
GCPointer<T,ARRAYSIZE>::GCPointer(const GCPointer<T,ARRAYSIZE> & rhs)
{
	_GCInfo<T> * pFound = sm_listGC.Find(rhs.m_pAddress);
	++(pFound->m_iRefCount);

	m_pAddress = rhs.m_pAddress;
	m_iArraySize = rhs.m_iArraySize;
	m_bIsArray = rhs.m_bIsArray;
}
template<typename T, UInt ARRAYSIZE>
GCPointer<T,ARRAYSIZE>::~GCPointer()
{
	_GCInfo<T> * pFound = sm_listGC.Find(m_pAddress);
	if ( pFound->m_iRefCount > 0 )
		--(pFound->m_iRefCount);
	if (pFound->m_iRefCount == 0)
		++sm_iGCUnrefCount;
	
	if ( sm_iGCMode == GCMODE_PARANOID )
		CollectGarbage();
	else if ( sm_iGCMode == GCMODE_STRICT ) {
		if ( pFound->m_iRefCount == 0 ) {
			sm_listGC.Remove(pFound);
			if ( pFound->m_pAddress != NULL ) {
				if ( pFound->m_bIsArray )
					delete[] (pFound->m_pAddress);
				else
					delete (pFound->m_pAddress);
			}
			delete pFound;
			--sm_iGCUnrefCount;
		}
	} else {
		if ( sm_iGCMode & GCMODE_LISTSIZE ) {
			if ( sm_listGC.GetSize() >= sm_iGCMaxListSize ) {
				CollectGarbage();
				return;
			}
		}
		if ( sm_iGCMode & GCMODE_UNREFCOUNT ) {
			if ( sm_iGCUnrefCount >= sm_iGCMaxUnref ) {
				CollectGarbage();
				return;
			}
		}
		if ( sm_iGCMode & GCMODE_MEMORYUSAGE ) {
			if ( sm_listGC.GetMemoryUsage() >= sm_iGCMaxMemoryUse ) {
				CollectGarbage();
				return;
			}
		}
	}
}

template<typename T, UInt ARRAYSIZE>
T * GCPointer<T,ARRAYSIZE>::operator=(T * rhs)
{
	_GCInfo<T> * pFound = sm_listGC.Find(m_pAddress);
	--(pFound->m_iRefCount);
	if ( pFound->m_iRefCount == 0 )
		++sm_iGCUnrefCount;
	if ( sm_iGCMode == GCMODE_STRICT ) {
		if ( pFound->m_iRefCount == 0 ) {
			sm_listGC.Remove(pFound);
			if ( pFound->m_pAddress != NULL ) {
				if ( pFound->m_bIsArray )
					delete[] (pFound->m_pAddress);
				else
					delete (pFound->m_pAddress);
			}
			delete pFound;
			--sm_iGCUnrefCount;
		}
	}

	pFound = sm_listGC.Find(rhs);
	if ( pFound == NULL )
		sm_listGC.PushFront( new _GCInfo<T>(rhs, ARRAYSIZE) );
	else
		++(pFound->m_iRefCount);

	m_pAddress = rhs;
	return rhs;
}
template<typename T, UInt ARRAYSIZE>
GCPointer<T,ARRAYSIZE> & GCPointer<T,ARRAYSIZE>::operator=(const GCPointer<T,ARRAYSIZE> & rhs)
{
	_GCInfo<T> * pFound = sm_listGC.Find(m_pAddress);
	--(pFound->m_iRefCount);
	if ( pFound->m_iRefCount == 0 )
		++sm_iGCUnrefCount;
	if ( sm_iGCMode == GCMODE_STRICT ) {
		if ( pFound->m_iRefCount == 0 ) {
			sm_listGC.Remove(pFound);
			if ( pFound->m_pAddress != NULL ) {
				if ( pFound->m_bIsArray )
					delete[] (pFound->m_pAddress);
				else
					delete (pFound->m_pAddress);
			}
			delete pFound;
			--sm_iGCUnrefCount;
		}
	}

	pFound = sm_listGC.Find(rhs.m_pAddress);
	++(pFound->m_iRefCount);

	m_pAddress = rhs.m_pAddress;
	return (*this);
}

template<typename T, UInt ARRAYSIZE>
inline T & GCPointer<T,ARRAYSIZE>::operator*() const {
    return *m_pAddress;
}
template<typename T, UInt ARRAYSIZE>
inline T * GCPointer<T,ARRAYSIZE>::operator->() const {
    return m_pAddress;
}
template<typename T, UInt ARRAYSIZE>
inline T & GCPointer<T,ARRAYSIZE>::operator[](UInt i) const {
    return m_pAddress[i];
}
template<typename T, UInt ARRAYSIZE>
inline GCPointer<T,ARRAYSIZE>::operator T*() const {
    return m_pAddress;
}

template<typename T, UInt ARRAYSIZE>
inline GarbageCollectorMode GCPointer<T,ARRAYSIZE>::GetMode() {
    return sm_iGCMode;
}
template<typename T, UInt ARRAYSIZE>
inline UInt GCPointer<T,ARRAYSIZE>::GetMaxListSize() {
    return sm_iGCMaxListSize;
}
template<typename T, UInt ARRAYSIZE>
inline UInt GCPointer<T,ARRAYSIZE>::GetMaxUnrefs() {
    return sm_iGCMaxUnref;
}
template<typename T, UInt ARRAYSIZE>
inline UInt GCPointer<T,ARRAYSIZE>::GetMaxMemoryUse() {
    return sm_iGCMaxMemoryUse;
}
template<typename T, UInt ARRAYSIZE>
inline Void GCPointer<T,ARRAYSIZE>::SetMode(GarbageCollectorMode iMode) {
    sm_iGCMode = iMode;
}
template<typename T, UInt ARRAYSIZE>
inline Void GCPointer<T,ARRAYSIZE>::SetMaxListSize(UInt iMaxListSize) {
    sm_iGCMaxListSize = iMaxListSize;
}
template<typename T, UInt ARRAYSIZE>
inline Void GCPointer<T,ARRAYSIZE>::SetMaxUnrefs(UInt iMaxUnrefs) {
    sm_iGCMaxUnref = iMaxUnrefs;
}
template<typename T, UInt ARRAYSIZE>
inline Void GCPointer<T,ARRAYSIZE>::SetMaxMemoryUse(UInt iMaxMemoryUse) {
    sm_iGCMaxMemoryUse = iMaxMemoryUse;
}

template<typename T, UInt ARRAYSIZE>
Bool GCPointer<T,ARRAYSIZE>::CollectGarbage()
{
	Bool bMemFreed = false;
	
	_GCInfo<T> * pCurrent;
	do {
		pCurrent = sm_listGC.Begin();
		while( pCurrent != NULL ) {
			if ( pCurrent->m_iRefCount == 0 ) {
				sm_listGC.Remove(pCurrent);
				if ( pCurrent->m_pAddress != NULL ) {
					if( pCurrent->m_bIsArray )
						delete[] (pCurrent->m_pAddress);
					else
						delete (pCurrent->m_pAddress);
				}
				delete pCurrent;
				--sm_iGCUnrefCount;
				bMemFreed = true;
				break;
			}

			pCurrent = pCurrent->m_pNext;
		}
	} while( pCurrent != NULL );
	
	return bMemFreed;
}
template<typename T, UInt ARRAYSIZE>
Void GCPointer<T,ARRAYSIZE>::CollectRemaining()
{
	if ( sm_listGC.GetSize() == 0 )
		return;

	_GCInfo<T> * pCurrent = NULL;
	while( sm_listGC.GetSize() > 0 ) {
		pCurrent = sm_listGC.Begin();
		sm_listGC.Remove(pCurrent);
		if ( pCurrent->m_pAddress != NULL ) {
			if( pCurrent->m_bIsArray )
				delete[] (pCurrent->m_pAddress);
			else
				delete (pCurrent->m_pAddress);
		}
		delete pCurrent;
	}
	sm_iGCUnrefCount = 0;
}

