/*
 * CAN_main.h
 *
 *  Created on: 11 nov. 2020
 *      Author: STuser
 */

#ifndef TS_280049_MAIN_PROJECTS_01_TS_28004X_CAN_TS_SYSMNG_INC_CAN_MAIN_H_
#define TS_280049_MAIN_PROJECTS_01_TS_28004X_CAN_TS_SYSMNG_INC_CAN_MAIN_H_


//
// Defines
//

__interrupt void canaISR(void);

#define TXCOUNT  1000
#define MSG_DATA_LENGTH    8
#define TX_MSG_OBJ_ID    1
#define RX_MSG_OBJ_ID    2


#endif /* TS_280049_MAIN_PROJECTS_01_TS_28004X_CAN_TS_SYSMNG_INC_CAN_MAIN_H_ */
