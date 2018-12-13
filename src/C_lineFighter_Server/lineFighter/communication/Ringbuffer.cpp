#include "RingBuffer.h"


int CRingBuffer::readBuffer(char *buf,int iLen)
{
	if ( m_buffer == NULL || buf == NULL || iLen <= 0 ) return -1;
	if (iLen > length()) return -1;

	for	(int i = 0; i < iLen;)
		buf[i++] = m_buffer[(m_readPtr++)%m_size];

	return iLen;
}


int CRingBuffer::writeBuffer(char *buf,int iLen)
{
	if (iLen > (m_size-1-length())) return -1;

	for	(int i = 0; i < iLen;)
		m_buffer[(m_writePtr++)%m_size] = buf[i++];
	return 0;
}

unsigned char CRingBuffer::checkSum(int iLen)
{
	unsigned char temp = 0;
	for (int i = 0; i < iLen; i++)
		temp += m_buffer[(m_readPtr+i)%m_size];
	return temp;
}

unsigned char CRingBuffer::checkSum(int iOffset, int iLen)
{
	int i = 0;
	i += iOffset;
	unsigned char temp = 0;
	for (; i < iLen + iOffset; i++)
		temp += m_buffer[(m_readPtr+i)%m_size];
	return temp;
}