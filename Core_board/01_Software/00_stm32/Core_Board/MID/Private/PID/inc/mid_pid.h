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
#define DEBUG


//******************************** Defines **********************************//

//******************************** Defines **********************************//


//******************************** Declaring ********************************//

typedef struct {
    float Kp;               /* proportion   coefficient */
    float Ki;               /* Integral     coefficient */
    float Kd;               /* Differential coefficient */

    float integral_limit;   /* Integral limit           */
    float output_limit;     /* Output limit             */

    float integral_sum;     /* Sum of integral terms    */   
    float prev_error;       /* Previous error           */   
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
pid_controller_t* creat_pid_controller(float             Kp,
                                       float             Ki,
                                       float             Kd,
                                       float integral_limit,
                                       float   output_limit,
                                       float   integral_sum,
                                       float     prev_error);   
									   
/**
 * @brief compute the pid's output
 * 
 * @param[in] pid_controller_t* : the point step motor object.
 * @param[in] int32_t           : the error value
 * 
 * @return    float             : the pid's output
 * */									   
float pid_compute(pid_controller_t* pid_inst, int32_t error);



//******************************** Declaring ********************************//

#endif // End of __MID_PID_H__
