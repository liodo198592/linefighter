/****************************************************************************
  Copyright(C)    2012,  易司拓电力科技股份有限公司.
  File name :     EST_RingBuf.h
  Author :        ZHOUYU
  Version:        1.0
  Date:           2012.04.17
  Description:    环形缓冲数据结构，用以缓冲数据报文
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
    int mRdptr; //读指针
    int mWtptr; //写指针
}SRingBuf;

/*!    @brief
 *     @funcName: RBInit  初始化环形缓冲
    @param[in] pRBBuf  环形缓冲对象
 *    @return    无
 */
void RBInit(SRingBuf * pRBBuf);


/*!    @brief
 *     @funcName: RBLenth 获取RingBuffer中数据的长度
    @param[in] pRBBuf  环形缓冲对象
 *    @return    返回数据长度
 */
int RBLenth(SRingBuf * pRBBuf);


/*!    @brief
 *     @funcName: RBRead  从RINGBUF中读取相应的字节，如果缓冲区中数据不够返回-1
    @param[in] pRBBuf  环形缓冲对象
    @param[in/out] buf 读取缓冲
    @param[in/out] iLen需要读取的字节数
 *    @return    返回数据长度
 */
int RBRead( unsigned char * buf, int iLen ,SRingBuf * pRBBuf);

/*!    @brief
 *     @funcName: RBRead  从RINGBUF中copy相应的字节,预读后数据还在缓冲区内,如果缓冲区中数据不够返回-1
    @param[in] pRBBuf  环形缓冲对象
    @param[in/out] buf 读取缓冲
    @param[in/out] iLen需要读取的字节数
 *    @return    返回数据长度
 */
int RBReadPerFetch( unsigned char * buf, int iLen ,SRingBuf * pRBBuf);

/*!    @brief
 *     @funcName: RBWrite  
    @param[in] pRBBuf  向环形缓冲对象中写入报文，缓冲区不够的情况下返回失败
    @param[in] buf 写入缓冲
    @param[in] iLen写入读取的字节数
 *    @return    成功返回RTN_SUCCESS ， 失败返回 -1
 */
int RBWrite( unsigned char * buf, int iLen ,SRingBuf * pRBBuf);

#endif /* HRJ_RINGBUF_H_ */
