

/**
 * main.c
 */

#include "main.h"
#include "close_loop_x.h"
#include "open_loop_x.h"
#include "wait_x.h"
#include "init_x.h"
#include "com_x.h"

eSystemState (*ptr_init)(void);
eSystemState (*ptr_wait)(void);
eSystemState (*ptr_close_loop)(void);
eSystemState (*ptr_open_loop)(void);
eSystemState (*ptr_com)(void);

int main(void)
{
    eSystemState eNextState = CMD_MODE_INIT_STATE;
    eSystemEvent eNewEvent  = CMD_MODE_INIT_EVENT;

    /* Pointer to function submodules */
    ptr_close_loop  = &mode_close_loop_mng;
    ptr_init        = &mode_init_mng;
    ptr_wait        = &mode_wait_mng;
    ptr_open_loop   = &mode_open_loop_mng;
    ptr_com         = &mode_com_mng;

    while(1)
    {
        // read event
        //eNewEvent = ReadEvent();

        switch (eNextState)
        {
            case CMD_MODE_INIT_STATE:
            {
                if (CMD_MODE_INIT_EVENT == eNewEvent)
                {
                    eNextState = (*ptr_init)();
                }
            }
            break;
            case CMD_MODE_WAIT_STATE:
            {
                if (CMD_MODE_WAIT_EVENT == eNewEvent)
                {
                    eNextState = (*ptr_wait)();
                }
            }
            break;
            case CMD_MODE_CLOSE_LOOP_STATE:
            {
                if (CMD_MODE_CLOSE_LOOP_EVENT == eNewEvent)
                {
                    eNextState = (*ptr_close_loop)();
                }
            }
            break;
            case CMD_MODE_OPEN_LOOP_STATE:
            {
                if (CMD_MODE_OPEN_LOOP_EVENT == eNewEvent)
                {
                    eNextState = (*ptr_open_loop)();
                }
            }
            break;
            case CMD_MODE_COM_STATE:
            {
                if (CMD_MODE_COM_EVENT == eNewEvent)
                {
                    eNextState = (*ptr_com)();
                }
            }
            break;
        default:
            break;

        }

    }
	return 0;
}
