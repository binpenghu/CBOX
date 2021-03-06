
/****************************************************************************
// Copyright (c) 2017
//
// @Module        
// @Filename      
// @Project       
// @Author        HBP
//----------------------------------------------------------------------------
// @Controller    STM8S
//
// @Compiler      IAR
//
// @Description   
//
//----------------------------------------------------------------------------
// @Date          2017
// @Version       V1.0
****************************************************************************/

#ifndef _KEY_H_
#define _KEY_H_
//****************************************************************************
// @Project Includes
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************

//****************************************************************************
// @Typedefs
//****************************************************************************
 typedef enum
{
	V_KEY = 1,
	A_KEY = 2,
	AH_KEY = 3,
	PER_KEY = 4,
	MAX_KEY =5,
    WAIT_KEY
}KEY_VALUE_E;
//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************

//****************************************************************************
// @Prototypes Of Static Functions
//****************************************************************************

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************
extern KEY_VALUE_E key_task(void);
//****************************************************************************
// @Interrupt Vectors
//****************************************************************************


#endif
/*
 *######################################################################
 *                           End of File
 *######################################################################
 */





