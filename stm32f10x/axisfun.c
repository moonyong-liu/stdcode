/**
  *****************************************************************************
  * @file    axisfun.c
  * @author  LHw
  * @version V1.0
  * @date    2017/07/24
  * @brief   about all axis function, count, speed, director...
  *****************************************************************************
  * @attention
	* @updateRecord
	# who: lhw | when: 20170724 | content:
	1.create update record
  *****************************************************************************
  */
/**
  * @brief  Speed detection master func. main() call
	* @param  None
  * @retval SUCCESS/ERROR
  */
#include "stdcode.h"

/* Global Variables ---------------------------------------------------------*/
static _JZflag JzFlag;															   // ji zhou ka flag 
static _CheckGroup timNvalues;
/* PRIVATE DEFINITION -------------------------------------------------------*/
static u8   speedDetc(_TimrValus* values);
static void speedDetcStep1(_TimrValus* values,u8 magnetic);

/* PUBLIC DECLARATION -------------------------------------------------------*/

/* PUBLIC DEFINITION --------------------------------------------------------*/
/**
  * @brief  Check sum func. a1+a2...+aN = sum(8bit)
	* @param  *srcdata 	# source data address
	* @param 	len				#	length of data
  * @retval sum				# result
  */
u8 checkSum(u8* srcdata, u8 len)
{
	u8 sum = RESET;
	while(len--){
		sum += *srcdata++;
	}
	return sum;
}
/**
  * @brief  reCheck sum func. a1+a2...+(aN-1) - aN(sum) = 0(8bit)
	* @param  *srcdata 	# source data address
	* @param 	len				#	length of data
  * @retval sum				# result
  */
u8 reCheckSum(u8* srcdata, u8 len)
{
	u8 sum = RESET;
	while(--len){
		sum += *srcdata++;
	}
	if( sum == *srcdata ){
		return SUCCESS;
	}
	else{
		return ERROR;
	}
}
/**
  * @brief  Saving Axes(capture mode) information, 
						sign the axes detect successful or not
	* @param  *data	   # applicatioin call _Package*
  * @param  sysENtim # exit axsi detec system delay
  * @param  timAVal  # one timer(capture magnetic trigger) for count axes
	* @param  timBVal  # aother timer(capture magnetic trigger) for count axes
  * @retval SUCCESS/ERROR
  */
u8 saveAxesCap( _pClrTimFlgAxisNs sysENtim,
								 _TimrValus* timAVal, _TimrValus* timBVal )
{
	/* check input group */
	if( timNvalues.MutexSign == RESET ){
			/* check enter */
		if( timAVal->Pair != RESET ){
			/* Enter speed detection mode */
			sysENtim();
			speedDetc(timAVal);
			/* add count axis information at here */
			//axisInfo();
			/* set first activate input */
			timNvalues.pTimVal = timAVal;
			/* mutex sign */
			timNvalues.MutexSign = SET;
		}
		else if( timBVal->Pair != RESET ){
			/* Enter speed detection mode */
			sysENtim();
			speedDetc(timBVal);
			/* add count axis information at here */
			//axisInfo();
			/* set first activate input */
			timNvalues.pTimVal = timBVal;
			/* mutex sign */
			timNvalues.MutexSign = SET;
		}
	}
	else{
			/* check enter */
		if( timNvalues.pTimVal->Pair != RESET ){
			/* Enter speed detection mode */
			sysENtim();
			speedDetc(timNvalues.pTimVal);
			/* add count axis information at here */
			//axisInfo();
		}
		/* According Lock status collection write data */
		if( timNvalues.pTimVal->Lock == DETCSUCCESS){
			return SUCCESS;
		}
		else if(0/*tim1Values.Lock == ?*/){			
		}
	}
	return ERROR;
}

/* PRIVATE DEFINITION -------------------------------------------------------*/
/**
  * @brief  speed detcetion algorithm
	* @param  values # timer about struction
  * @retval SUCCESS/ERROR
  */
static u8 speedDetc(_TimrValus* values)
{
	/* Capture magnetica steel A activation */
	if( values->Pair == MAGNETICA ){
		if( values->Lock != MAGNETICB ){      // if detect Failed
			values->Counter = RESET;
			JzFlag.MissMagnA++;              // Miss magnetic A 
		}
		/* Lock, gettim, counter+1, clear */
		speedDetcStep1(values,MAGNETICA);
	}
	/* Capture magnetica steel B activation */
	else if( values->Pair == MAGNETICB ){
		if( values->Lock != MAGNETICA ){			// if detect Failed
			values->Counter = RESET;
			JzFlag.MissMagnB++;							// Miss magnetic B
		}
		/* Lock, gettim, counter+1, clear */
		speedDetcStep1(values,MAGNETICB);
	}
	else{		
	}
	/* Detection SUCCESS */
	if( values->Counter >= DETCSUCCESS ){
		/* shortAB langBA DIR=A */
		if(values->IntervalsAB < values->IntervalsBA){
			values->Dirc = 'A';
			values->Intervals = values->IntervalsAB;
		}
		/* shortBA langAB DIR=B */
		else if( values->IntervalsBA < values->IntervalsAB ){
			values->Dirc = 'B';
			values->Intervals = values->IntervalsBA;
		}
		else{
			values->Intervals = 0xffff;           // MAX intervals
		}
		values->Lock = DETCSUCCESS;
	}
	/* Detectioin lost axes */
	/* Single axis speed detec */
	else if(0){
		
	}
	return SUCCESS;
}

/**
  * @brief  
	* @param  
  * @retval SUCCESS/ERROR
  */
static void speedDetcStep1(_TimrValus* values,u8 magnetic)
{
	u16 tmpinter=RESET;
	tmpinter = tmpinter;										// Shielding warning

	/* Lock, getTim, counter+1, clear */
	values->Lock = magnetic;
	tmpinter  = TIM_GetCounter(values->TIMx);
	tmpinter |= (values->InterCarry) ? 0xffff : 0;
	if( magnetic == MAGNETICA ){
		values->IntervalsBA =  tmpinter;
	}
	else if( magnetic == MAGNETICB ){
		values->IntervalsAB =  tmpinter;
	}
	
	values->Counter++;
	TIM_SetCounter(values->TIMx,RESET);	
	values->Pair = RESET;
	values->InterCarry = RESET;
	JzFlag.CountAxis++;              // Statistics Axes quantity
	
}



