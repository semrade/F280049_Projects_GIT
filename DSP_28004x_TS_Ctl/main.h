/*
 * main.h
 *
 *  Created on: 7 janv. 2021
 *      Author: STuser
 */

#ifndef MAIN_H_
#define MAIN_H_


typedef enum
{
    CMD_MODE_INIT_STATE,
    CMD_MODE_WAIT_STATE,
    CMD_MODE_CLOSE_LOOP_STATE,
    CMD_MODE_OPEN_LOOP_STATE,
    CMD_MODE_COM_STATE

}eSystemState;

typedef enum
{
    CMD_MODE_INIT_EVENT,
    CMD_MODE_WAIT_EVENT,
    CMD_MODE_CLOSE_LOOP_EVENT,
    CMD_MODE_OPEN_LOOP_EVENT,
    CMD_MODE_COM_EVENT,
    CMD_MODE_NON_EVENT

}eSystemEvent;


#endif /* MAIN_H_ */
