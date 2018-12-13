#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#include "Config.h"
//#include "ace/Asynch_IO.h"
//#include "ace/Message_Block.h"
//#include "ace/SString.h"
#include <string>

class CRingBuffer
{
public:
	friend class CMsgParser;
	enum{BUFFER_SIZE = 1024*4};
	CRingBuffer(int iSize = BUFFER_SIZE):m_size(iSize),m_buffer(NULL),m_readPtr(0),m_writePtr(0)
	{
		m_buffer = new  char[m_size];
		memset(m_buffer,0,m_size);
	}
	virtual ~CRingBuffer()
	{
		delete[] m_buffer;}

	int readBuffer(char *buf,int iLen);
	int writeBuffer(char *buf,int iLen);
	unsigned char checkSum(int iLen);
	unsigned char checkSum(int iOffset, int iLen);
	//////////////////////////////////////////////////////////////////////////
	//iOffset��ǰλ�ú����ƫ�ƣ�Ϊ1���ʾ��һ���ַ� 
	//���أ�0 �ɹ� ��1ʧ��
	int getAt(int iOffset,unsigned char& cByte)
	{
		if (iOffset >= length()) return -1;
		cByte = m_buffer[(m_readPtr+iOffset)%m_size];
		return 0;
	}
	int readNextByte(unsigned char& cByte)
	{
		if ( length() < 1) return -1;
		cByte = m_buffer[(m_readPtr++)%m_size];
		return 0;
	}

public:
	int length()
	{
		return (m_writePtr - m_readPtr >= 0 ) ? (m_writePtr-m_readPtr) : (m_writePtr-m_readPtr+m_size);
	}
	char * m_buffer;
	int m_readPtr;
	int m_writePtr;
	int m_size;
};

#endif