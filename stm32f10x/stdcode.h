/**
  ******************************************************************************
  * @file    stdcode.h
  * @author  LHw
  * @version V1.0
  * @date    2017/07/14
  * @brief   standard function hearder, all c file need include this
  ******************************************************************************
  * @attention
	# who: lHw | when: 20170725 | content:
	1.add typedef "_pClrTimFlgAxis30s"
	2.add axisfun.c func.
	# who: lHw | when: 20170724 | content:
	1.add comment with every struct, define, enmu ...
  ******************************************************************************
  */
	
#ifndef _STDCODE_H_
#define _STDCODE_H_

/*include--------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Memory Alignment */
#pragma pack(1)

/*variable-------------------------------------------------------------------*/
/*define --------------------------------------------------------------------*/

#define		PACKSIZE			256										// Master package size define				
#define   MINPACKSIZE   7
#define   DATASIZE      PACKSIZE-MINPACKSIZE

/* magnetic define */
enum{ MAGNETICA=1,				// Magnetic steel A activation flag
			MAGNETICB=2,				// Magnetic steel B activation flag
			DETCSUCCESS=3};			// Detection success flag

/*typedef--------------------------------------------------------------------*/

/* speed detect device struct */
typedef struct{
	u8 Head;					//
	u8 Version;				
	u8 IntervalsH;		// intervals high
	u8 IntervalsL;		// intervals low
	u8 Dirc; 					// Dircetion
	u8 Illum; 				//Illumination
	u8 TempH; 				//temperature high
	u8 TempL; 				// temp. low
	u8 Count; 				// Transmit count
	u8 CheckSum;		
	u8 Tail;					//
}_CSKPack;

/* over run device struct */
typedef struct{
	u8 Head;		
	u8 IN1;				
	u8 IN2;		
	u8 IN3;	
	u8 IN4; 			
	u8 IN5; 			
	u8 IN6; 			
	u8 IN7; 		
	u8 IN8; 
	u8 IN9;
	u8 CheckSum;
	u8 Tail;				
}_CXKPack;

/* over run device struct - version b */
typedef struct{
	u8 Head;				
	u8 INline[9];				   
	u8 CheckSum;	
	u8 Tail;			
}_CXKPack_Vb;

/* slave collect device TX struct */
typedef struct{
	u8 AxesH;
  u8 AxesL;	
}_Axes_byte;
typedef union{
	u16 Axes_16b;
	_Axes_byte Axes_8b;
}_Axes;
typedef struct{
	_Axes Axes;
	u8 State;
	u8 CheckSum;
}_SCDContentTx;

/* slave collect device RX struct */
typedef struct{
	u8 CheckSum;
}_SCDContentRx;

/* master collect device Tx struct */
typedef struct{
	u8 CheckSum;
}_MCDContentTx;

/* multiple protocol union */
typedef union{
	uint8_t	Data[DATASIZE];
	_SCDContentTx ScdDataTx;
	_SCDContentRx ScdDataRx;
	_MCDContentTx McdDataTx;
}_MultipPack;

/* ji che contrl protocol */
typedef struct{
	u8 HeadA;
	u8 HeadB;
	u8 DataLen;
	u8 SourAddr;
	u8 TargAddr;	
	u8 MDataType;
	u8 SDataType;	
	_MultipPack Data;	
}_JCKZPackage;

/* SYS CONFIG STRUCT */
typedef struct{
	u8 HumbeinMode;
}_SYSConfig;

typedef struct{
	u16 IndexEnd;
	u16 IndexCur;
	u16 Loop;
}_IndexRx;
/* ALL package struct in hear */
typedef union{
	uint8_t		    Data[PACKSIZE];
	_CSKPack    	CskPack;					// son 'speed detection' package
	_CXKPack      CxkPack;          // son 'over run ' package
	_CXKPack_Vb   CxkPack_Vb;
	_JCKZPackage  CjkPack_sla;      // son 'ji che contrl ' package, slave device
	_JCKZPackage  CjkPack_mas;      // son 'ji che contrl ' package, master device
}_Package;										    // Master package 

/* breath flag */
typedef struct{ u8 BrethFlg; }_SPEflag;
/* falling/rising trigger sign */
typedef enum{	FALLINGtRI=1,	RISINGtRI=2 }_extDetType; 

/* count axis device flag */
typedef struct{
	u16 MissMagnA;
	u16 MissMagnB;
	u16 CountAxis; 
}_JZflag;

/* call back pointer to function clear timer flag, Axis Ns */
typedef void (*_pClrTimFlgAxisNs)(void);

/* delay.c func -------------------------------------------------------------*/
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

/* nvicfg.c func ------------------------------------------------------------*/
u8 nviconfig(	uint8_t 	 IRQn, uint8_t         Prio, 
							uint8_t SubPrio, FunctionalState sta );

/* usart.c func -------------------------------------------------------------*/

/* usart1 */
#define U1RXBUFLEN    	1024								//USART1 RX cache BUFFER SIZE
#define U1TXBUFLEN    	1024								//USART1 RX cache BUFFER SIZE
#define U1RXDMABUFLEN 	1024 	      			//USART1 RX DMA BUFFER SIZE
#define U1TXDMABUFLEN 	1024								//USART1 TX DMA BUFFER SIZE
#define U1RXDMACH DMA1_Channel5						//USART1 RX DMA CHANNEL
#define U1TXDMACH DMA1_Channel4					  //USART1 TX DMA CHANNEL
#define U1PERIADDR (u32)(&USART1->DR) 		//USART1 DMA Peripheral Address
#define IS_U1TX_DATA_LEN_LEGAL(LEN) (LEN<=U1TXBUFLEN)	

/* usart2 */
#define U2RXBUFLEN    	1024								//USART2 RX cache BUFFER SIZE
#define U2TXBUFLEN    	1024								//USART2 RX cache BUFFER SIZE
#define U2RXDMABUFLEN 	1024 	      			//USART2 RX DMA BUFFER SIZE
#define U2TXDMABUFLEN 	1024								//USART2 TX DMA BUFFER SIZE
#define U2RXDMACH DMA1_Channel6						//USART2 RX DMA CHANNEL
#define U2TXDMACH DMA1_Channel7						//USART2 TX DMA CHANNEL
#define U2PERIADDR (u32)(&USART2->DR) 		//USART2 DMA Peripheral Address
#define IS_U2TX_DATA_LEN_LEGAL(LEN) (LEN<=U2TXBUFLEN)	


/* usart3 */
#define U3RXBUFLEN    	1024								//USART3 RX cache BUFFER SIZE
#define U3TXBUFLEN    	1024								//USART3 RX cache BUFFER SIZE
#define U3RXDMABUFLEN 	1024 	      			//USART3 RX DMA BUFFER SIZE
#define U3TXDMABUFLEN 	1024								//USART3 TX DMA BUFFER SIZE
#define U3RXDMACH DMA1_Channel3						//USART3 RX DMA CHANNEL
#define U3TXDMACH DMA1_Channel2						//USART3 TX DMA CHANNEL
#define U3PERIADDR (u32)(&USART3->DR) 		//USART3 DMA Peripheral Address
#define IS_U3TX_DATA_LEN_LEGAL(LEN) (LEN<=U3TXBUFLEN)	


u8 usartCfg( GPIO_TypeDef* GPIOx, USART_TypeDef* USARTx,
						uint16_t GPIO_Pin_Rx, uint16_t GPIO_Pin_Tx,
						uint32_t BaudRate, uint16_t ITdefine,
						uint16_t Flag, FunctionalState sta);
						
u8 usartDmaCfg(	DMA_Channel_TypeDef* Channel, USART_TypeDef* USARTx,
								uint32_t PeriAddr, uint16_t Request,
								uint32_t MemAddr, uint32_t DIR,
								uint32_t Buffsize, uint32_t PeriInc,
								uint32_t MemInc, uint32_t PeriDataSize,
								uint32_t MemDataSize, uint32_t Mode,
								uint32_t Prio, uint32_t M2M,
								uint32_t DMAIT, FunctionalState sta );

u8 usartMaintain(DMA_Channel_TypeDef* DMAy_Cx_Tx, DMA_Channel_TypeDef* DMAy_Cx_Rx,
													uint16_t* TxLen, FlagStatus* DMATxFinFlag);
u8 clearUsartSRDR(USART_TypeDef* USARTx);
u8 usart_send( u8* TarBuf, u8* SrcBuf, u16 Len, u16* indexTxcb, u16 txBuflen );

u8 usart_printf( void* SrcBuf ); // realization in app files
u8 maintainDataFlow(void);       // realization in app files
/* timer.c func -------------------------------------------------------------*/

typedef struct {
	TIM_TypeDef * const TIMx; // TIMx
	u8 Lock;					// Detection process sign
	u8 Dirc;  				// Direction
	u8 Pair;        	// A->B or B->A make a Pair; A->A or B->B can't make a Pair
	u8 Counter;				// Detection counter
	u16 InterCarry;     // Intervals carry
	u16 Intervals;		// Final Intervals
	u16 IntervalsAB; 	// A->B Intervals
	u16 IntervalsBA; 	// B->A Intervals
}_TimrValus;

typedef struct {
	u16 Counter;
	u8  Set;
}_TimrFLags;		// Timer set a flag

/**/
typedef struct{	_TimrValus* pTimVal; u8 MutexSign; }_CheckGroup;

u8 genTimCfg( TIM_TypeDef *TIMx, uint16_t Period, 
							uint16_t 		 Pres, uint16_t Div, 
							uint16_t 		CMode, uint16_t Flag,
							uint16_t ITsource, FunctionalState sta);

u8 genTimCaMpCfg(	TIM_TypeDef 	 *TIMx, uint16_t Channel,
									uint16_t 		Polarity, uint16_t Selec,
									uint16_t 				Pres, uint16_t Filter,
									GPIO_TypeDef 	*GPIOx, uint16_t GPIO_Pin,
									uint16_t 		ITsource, GPIOMode_TypeDef GPIO_Mode,
									uint32_t  GPIO_Remap, FunctionalState sta );
									
u8 genTimCaMpDmaCfg(	DMA_Channel_TypeDef* Channel, TIM_TypeDef* TIMx,
									uint32_t PeriAddr, uint16_t Request,
									uint32_t MemAddr, uint32_t DIR,
									uint32_t Buffsize, uint32_t PeriInc,
									uint32_t MemInc, uint32_t PeriDataSize,
									uint32_t MemDataSize, uint32_t Mode,
									uint32_t Prio, uint32_t M2M,
									uint32_t DMAIT, FunctionalState sta );


/* gpio.c func ---------------------------------------------------------*/

#define LEDBUTTON(gpio,pin,sta) sta ? GPIO_SetBits(gpio, pin) \
																		: GPIO_ResetBits(gpio, pin)
																		
#define SETGPIO(gpio,pin,sta) sta ? GPIO_SetBits(gpio, pin) \
																		: GPIO_ResetBits(gpio, pin)

u8 gpioCfg(	GPIO_TypeDef* GPIOx,uint16_t Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef Speed,FlagStatus sta);
/* extiline.c func ----------------------------------------------------------*/
u8 extiCfg(	GPIO_TypeDef* GPIOx, uint16_t Pin, GPIOMode_TypeDef GMode,
						uint8_t PortSource, uint8_t PinSource, uint32_t Line, 
						EXTIMode_TypeDef EMode,	EXTITrigger_TypeDef Trigger, 
						FunctionalState sta);
/* wdg.c func --------------------------------------------------------------*/
void initWWDG(void);
void initIWDG(void);
/* axisfun.c func. ----------------------------------------------------------*/
u8   checkSum(u8* srcdata, u8 len);
u8 reCheckSum(u8* srcdata, u8 len);
u8   saveAxesCap( _pClrTimFlgAxisNs sysENtim, 
								 _TimrValus* timAVal, _TimrValus* timBVal);

#endif /*_STDCODE_H_ */
