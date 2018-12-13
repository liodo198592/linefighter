/****************************************************************************
  Copyright(C)    2012,  ��˾�ص����Ƽ��ɷ����޹�˾.
  File name :     EST_RingBuf.h
  Author :        ZHOUYU
  Version:        1.0
  Date:           2012.04.17
  Description:    ���λ������ݽṹ�����Ի������ݱ���
  Other:
  Mode History:
          <author>        <time>      <version>   <desc>
          zouliuhua     2011-04-17     V1.0.0.1
****************************************************************************/

#ifndef HRJ_RINGBUF_H_
#define HRJ_RINGBUF_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

#define HOST_FRAM_MAX_LEN 2000*100

typedef struct
{
    unsigned char mBuf[HOST_FRAM_MAX_LEN];
    int mRdptr; //��ָ��
    int mWtptr; //дָ��
}SRingBuf;

/*!    @brief
 *     @funcName: RBInit  ��ʼ�����λ���
    @param[in] pRBBuf  ���λ������
 *    @return    ��
 */
void RBInit(SRingBuf * pRBBuf);


/*!    @brief
 *     @funcName: RBLenth ��ȡRingBuffer�����ݵĳ���
    @param[in] pRBBuf  ���λ������
 *    @return    �������ݳ���
 */
int RBLenth(SRingBuf * pRBBuf);


/*!    @brief
 *     @funcName: RBRead  ��RINGBUF�ж�ȡ��Ӧ���ֽڣ���������������ݲ�������-1
    @param[in] pRBBuf  ���λ������
    @param[in/out] buf ��ȡ����
    @param[in/out] iLen��Ҫ��ȡ���ֽ���
 *    @return    �������ݳ���
 */
int RBRead( unsigned char * buf, int iLen ,SRingBuf * pRBBuf);

/*!    @brief
 *     @funcName: RBRead  ��RINGBUF��copy��Ӧ���ֽ�,Ԥ�������ݻ��ڻ�������,��������������ݲ�������-1
    @param[in] pRBBuf  ���λ������
    @param[in/out] buf ��ȡ����
    @param[in/out] iLen��Ҫ��ȡ���ֽ���
 *    @return    �������ݳ���
 */
int RBReadPerFetch( unsigned char * buf, int iLen ,SRingBuf * pRBBuf);

/*!    @brief
 *     @funcName: RBWrite  
    @param[in] pRBBuf  ���λ��������д�뱨�ģ�����������������·���ʧ��
    @param[in] buf д�뻺��
    @param[in] iLenд���ȡ���ֽ���
 *    @return    �ɹ�����RTN_SUCCESS �� ʧ�ܷ��� -1
 */
int RBWrite( unsigned char * buf, int iLen ,SRingBuf * pRBBuf);

#endif /* HRJ_RINGBUF_H_ */
