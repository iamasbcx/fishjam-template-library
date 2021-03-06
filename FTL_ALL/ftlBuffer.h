#ifndef FTL_BUFFER_H
#define FTL_BUFFER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlBuffer.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
	* 
	*****************************************************************************************************/

	//网络的读写缓冲基类 -- 参照 CInternetFile::Read/Write 改写而来(原有代码有Bug，进行了更改)
    //模版实现子类必须实现 ReadReal 和 WriteReal 方法, 从而提供真正的读写函数(本地文件、网络socket等)
	template <typename T>
	class CFRWBufferT 
	{
	public:
		//T* pT = static_cast<T*>(this);
		FTLINLINE CFRWBufferT();
		FTLINLINE virtual ~CFRWBufferT();
        
        FTLINLINE BOOL Attach(PBYTE pReadBuffer, LONG nReadBufferSize, PBYTE pWriteBuffer, LONG nWriteBufferSize);
        FTLINLINE BOOL Detach(PBYTE* ppReadBuffer, LONG* pReadBufferSize, PBYTE* ppWriteBuffer, LONG* pWriteBufferSize); 
        	
		FTLINLINE BOOL SetReadBufferSize(LONG nReadSize);
		FTLINLINE BOOL SetWriteBufferSize(LONG nWriteSize);

		//为了防止名称冲突，更改了名字，不使用太常见的 Read/Write/Flush 等
		FTLINLINE BOOL ReadFromBuffer(PBYTE pBuffer, LONG nCount, LONG* pRead);
		FTLINLINE BOOL WriteToBuffer(const PBYTE pBuffer, LONG nCount, LONG* pWrite);
		FTLINLINE BOOL FlushFromBuffer(LONG* pWrite);
	protected:
		//BOOL ReadReal(PBYTE pBuffer, LONG nCount, LONG* pWrite) = 0;
		//BOOL WriteReal(const PBYTE pBuffer, LONG nCount, LONG* pWrite) = 0;
	protected:
		//说明：Read 比 Write 多一个 m_nReadBufferBytes(个人理解是指 Buffer 中有效数据的结尾) -- 因此实现逻辑和Write不一致, MS 在这里有Bug
		LPBYTE	m_pbWriteBuffer;
		BOOL    m_bWriteBufferIsAttached;
		LONG	m_nWriteBufferSize;
		LONG	m_nWriteBufferPos;

		LPBYTE	m_pbReadBuffer;
		BOOL    m_bReadBufferIsAttached;
		LONG	m_nReadBufferSize;
		LONG	m_nReadBufferPos;
		LONG	m_nReadBufferBytes;

	};
}

#endif //FTL_BUFFER_H

#ifndef USE_EXPORT
#  include "ftlBuffer.hpp"
#endif