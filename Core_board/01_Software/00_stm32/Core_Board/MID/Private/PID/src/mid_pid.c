/******************************************************************************
 * Copyright (C) 2025 jiejielin, src.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file mid_pid.c
 *
 * @par dependencies
 * - mid_pid.h
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
 
#include "mid_pid.h"

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
                                       float     prev_error)
{
    if (integral_limit < 0 || output_limit < 0) {
#ifdef DEBUG
        printf("Invalid limit value in creat_pid_controller (must >=0)\r\n");
#endif
        return NULL;
    }

    pid_controller_t *p_pid_instance = (pid_controller_t *)malloc(sizeof(pid_controller_t));
    if (NULL == p_pid_instance) {
#ifdef DEBUG
        printf("Malloc failed in creat_pid_controller\r\n");
#endif
        return NULL;
    }

    /* Initialization parameters */
    p_pid_instance->Kp = Kp;
    p_pid_instance->Ki = Ki;
    p_pid_instance->Kd = Kd;
    
    /* Setting limit values */
    p_pid_instance->integral_limit = integral_limit;
    p_pid_instance->output_limit   =   output_limit;
    
    /* Initialize state variables */
    p_pid_instance->integral_sum   =   integral_sum;
    p_pid_instance->prev_error     =     prev_error;

    return p_pid_instance;
}

/**
 * @brief compute the pid's output
 * 
 * @param[in] pid_controller_t* : the point step motor object.
 * @param[in] int32_t           : the error value
 * 
 * @return    float             : the pid's output
 * */
float pid_compute(pid_controller_t* pid_inst, int32_t error) 
{
    /* Calculation of integral sum */
    pid_inst->integral_sum += error;
    
    /* Integral limit */
	if (pid_inst->integral_sum > pid_inst->integral_limit) 
	{
		pid_inst->integral_sum = pid_inst->integral_limit;
	} 
	else if (pid_inst->integral_sum < -pid_inst->integral_limit) 
	{
		pid_inst->integral_sum = -pid_inst->integral_limit;
	}

	/* Calculation of Differential now */
    float d_term = error - pid_inst->prev_error;

    /* Calculation of the output of pid */
    float output = (pid_inst->Kp * error) 
                 + (pid_inst->Ki * pid_inst->integral_sum) 
                 + (pid_inst->Kd * d_term);

    /* output limit */
    if (output > pid_inst->output_limit) 
    {
        output = pid_inst->output_limit;
    } 
    else if (output < -pid_inst->output_limit) 
    {
        output = -pid_inst->output_limit;
    }

    /* Updating state variables */
    pid_inst->prev_error = error;

    return output;
}




