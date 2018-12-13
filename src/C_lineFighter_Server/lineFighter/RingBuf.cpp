#include "RingBuf.h"

void RBInit(SRingBuf * pRBBuf)
{
    memset(pRBBuf->mBuf, 0 , HOST_FRAM_MAX_LEN);
    pRBBuf->mRdptr = 0;
    pRBBuf->mWtptr = 0;
}

int RBLenth(SRingBuf * pRBBuf)
{
    return (pRBBuf->mWtptr >= pRBBuf->mRdptr) ? (pRBBuf->mWtptr-pRBBuf->mRdptr)
            : (HOST_FRAM_MAX_LEN + pRBBuf->mWtptr - pRBBuf->mRdptr);
}

int RBRead( unsigned char * buf, int iLen ,SRingBuf * pRBBuf)
{
    int i = 0;
    if ( buf == 0 || pRBBuf == 0 ||iLen <= 0 )
    {
        return -1;
    }
    if (iLen > RBLenth(pRBBuf))
    {
        return -1;
    }

    for ( i = 0; i < iLen;)
    {
        buf[i++] = pRBBuf->mBuf[(pRBBuf->mRdptr++)%HOST_FRAM_MAX_LEN];
    }
    return iLen;
}

int RBReadPerFetch( unsigned char * buf, int iLen ,SRingBuf * pRBBuf)
{
    int i = 0;    
    if ( buf == 0 || pRBBuf == 0 ||iLen <= 0 )
    {
        return -1;
    }
    if (iLen > RBLenth(pRBBuf))
    {
        return -1;
    }

    
    for ( i = 0; i < iLen; ++i)
    {
        buf[i] = pRBBuf->mBuf[(pRBBuf->mRdptr+i)%HOST_FRAM_MAX_LEN];
    }
    return iLen;
}

int RBWrite( unsigned char * buf, int iLen ,SRingBuf * pRBBuf)
{
    int i = 0;
    if (pRBBuf == 0  || iLen > (HOST_FRAM_MAX_LEN-1-RBLenth(pRBBuf)))
    {
        return -1;
    }

    for (i = 0; i < iLen;)
    {
        pRBBuf->mBuf[(pRBBuf->mWtptr++)%HOST_FRAM_MAX_LEN] = buf[i++];
    }

    return 0;
}
