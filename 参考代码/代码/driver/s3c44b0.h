#define REGBASE		0x01c00000
#define REGL(addr)	(*(volatile unsigned int *)(REGBASE+addr))
#define REGW(addr)	(*(volatile unsigned short *)(REGBASE+addr))
#define REGB(addr)	(*(volatile unsigned char *)(REGBASE+addr))

#define rINTMSK		REGL(0x20000c)

/*********************/
/* UART Registers    */
/*********************/

#define rULCON0		REGL(0x100000)
#define rULCON1		REGL(0x104000)
#define rUCON0		REGL(0x100004)
#define rUCON1		REGL(0x104004)
#define rUFCON0		REGL(0x100008)
#define rUFCON1		REGL(0x104008)
#define rUMCON0		REGL(0x10000c)
#define rUMCON1		REGL(0x10400c)
#define rUTRSTAT0	REGL(0x100010)
#define rUTRSTAT1	REGL(0x104010)
#define rUERSTAT0	REGL(0x100014)
#define rUERSTAT1	REGL(0x104014)
#define rUFSTAT0		REGL(0x100018)
#define rUFSTAT1		REGL(0x104018)
#define rUMSTAT0		REGL(0x10001c)
#define rUMSTAT1		REGL(0x10401c)
#define rUTXH0		REGB(0x100020)
#define rUTXH1		REGB(0x104020)
#define rURXH0		REGB(0x100024)
#define rURXH1		REGB(0x104024)
#define rUBRDIV0		REGL(0x100028)
#define rUBRDIV1		REGL(0x104028)
