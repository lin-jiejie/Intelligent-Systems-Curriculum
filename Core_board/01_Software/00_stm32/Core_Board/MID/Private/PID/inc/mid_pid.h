/******************************************************************************
 * Copyright (C) 2025 jiejielin, inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file mid_pid.h
 *
 * @par dependencies
 * - stdio.h
 * - stdint.h
 * - stdlib.h
 *
 * @author jiejielin
 *
 * @brief Provide the HAL APIs of Key and corresponding opetions.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2025-5-20
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/
 
#ifndef __MID_PID_H__
#define __MID_PID_H__

//******************************** Includes *********************************//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//******************************** Includes *********************************//
//#define DEBUG


//******************************** Defines **********************************//

//******************************** Defines **********************************//


//******************************** Declaring ********************************//

typedef struct {
    int32_t Kp;               /* proportion   coefficient */
    int32_t Ki;               /* Integral     coefficient */
    int32_t Kd;               /* Differential coefficient */

    int32_t integral_limit;   /* Integral limit           */
    int32_t output_limit;     /* Output limit             */

    int32_t integral_sum;     /* Sum of integral terms    */   
    int32_t prev_error;       /* Previous error           */   
} pid_controller_t;         /* */

/**
 * @brief Creating a PID controller object
 * 
 * @param[in] Kp             : proportion   coefficient
 * @param[in] Ki             : Integral     coefficient
 * @param[in] Kd             : Differential coefficient
 * @param[in] integral_limit :           Integral limit
 * @param[in] output_limit   :             Output limit
 * @param[in] integral_sum   :    Sum of integral terms
 * @param[in] prev_error     :           Previous error
 * 
 * @return pid_controller_t* : The created PID controller object pointer
 */
pid_controller_t* creat_pid_controller(int32_t             Kp,
                                       int32_t             Ki,
                                       int32_t             Kd,
                                       int32_t integral_limit,
                                       int32_t   output_limit,
                                       int32_t   integral_sum,
                                       int32_t     prev_error);   
									   
/**
 * @brief compute the pid's output
 * 
 * @param[in] pid_controller_t* : the point step motor object.
 * @param[in] int32_t           : the error value
 * 
 * @return    float             : the pid's output
 * */									   
int32_t pid_compute(pid_controller_t* pid_inst, int32_t error);



//******************************** Declaring ********************************//

#endif // End of __MID_PID_H__
