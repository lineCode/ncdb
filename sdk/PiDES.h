#pragma once
#include "PiTypeDef.h"
/************************************************************************
	fun:des底层加密逻辑
	remark:加密后的数据长度跟原文一致
************************************************************************/

class CPiDES {  
public:  
    BYTE bOriMsg[8] ;               // 初始消息 ( 64 bit )  
    BYTE LMsg[17][4], RMsg[17][4] ; // 中间变量L0-->L16,R0-->R16 ( 32 bit )  
    BYTE bKey[8] ;                  // 保存密钥 ( 64 bit )  
    BYTE bSubKey[17][6] ;           // 保存子密钥K1-->K16 ( 48 bit )  
    BYTE bCryptedMsg[8] ;           // 密文  
public:  
    void DES_Encrypt ( ) ;  
    void DES_Decrypt ( ) ;  
	bool DES_Encrypt ( LPBYTE lpSour, LPBYTE lpDest, UINT uLen, LPBYTE lpKey ) ;  
	/************************************************************************
		fun:	加密, 加密后的数据所需内存在内部申请
		param:lpDest:out, 返回加密后数据的内存地址, 需要外部释放. uLen: 原文长度, key:8个字节
		ret:	加密后数据的大小, 字节数
	************************************************************************/
	UNINT DES_EncryptEx ( LPBYTE lpSour, LPBYTE& lpDest, UNINT uLen, LPBYTE lpKey ) ;
    UNINT DES_EncryptEx ( LPBYTE lpSour, LPBYTE& lpDest, UNINT uLen, const char* lpKey ) ;

	bool DES_Decrypt ( LPBYTE lpSour, LPBYTE lpDest, UINT uLen, LPBYTE lpKey ) ;  
	UNINT DES_DecryptEx ( LPBYTE lpSour, LPBYTE& lpDest, UINT uLen, LPBYTE lpKey ) ;  
	UNINT DES_DecryptEx ( LPBYTE lpSour, LPBYTE& lpDest, UINT uLen, const char* lpKey ) ;  
    UNINT DES_DecryptEx ( LPBYTE lpSour, string& lpDest, UINT uLen, const char* lpKey ) ;  
public:  
    // 初始置换  
    void DESInitSwap ( ) ;  
    // 初始逆置换  
    void DESInitReSwap () ;  
    // 产生子密钥  
    void DESGenSubKey ( ) ;  
    // DES的单轮加密过程  
    void DESSingleTurn ( BYTE nTurnIndex, bool bMode ) ;  
    // DES的F函数  
    void DES_f ( BYTE bTurnIndex, bool bMode ) ;  
public:  
    // 初始化  
    void DES_SysInit () ;   
    // 设置明文  
    void DES_SetOriMsg ( PBYTE pMsg, UINT uLen ) ;  
    // 设置密钥  
    void DES_SetKey ( PBYTE pKey, UINT uLen ) ;  
} ; 