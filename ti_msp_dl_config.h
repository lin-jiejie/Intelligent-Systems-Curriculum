/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMG6
#define PWM_0_INST_IRQHandler                                   TIMG6_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMG6_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                              1000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_21
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM46)
#define GPIO_PWM_0_C0_IOMUX_FUNC                     IOMUX_PINCM46_PF_TIMG6_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX

/* Defines for PWM_1 */
#define PWM_1_INST                                                         TIMG7
#define PWM_1_INST_IRQHandler                                   TIMG7_IRQHandler
#define PWM_1_INST_INT_IRQN                                     (TIMG7_INT_IRQn)
#define PWM_1_INST_CLK_FREQ                                              1000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_1_C0_PORT                                                 GPIOA
#define GPIO_PWM_1_C0_PIN                                         DL_GPIO_PIN_23
#define GPIO_PWM_1_C0_IOMUX                                      (IOMUX_PINCM53)
#define GPIO_PWM_1_C0_IOMUX_FUNC                     IOMUX_PINCM53_PF_TIMG7_CCP0
#define GPIO_PWM_1_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_1_C1_PORT                                                 GPIOA
#define GPIO_PWM_1_C1_PIN                                         DL_GPIO_PIN_24
#define GPIO_PWM_1_C1_IOMUX                                      (IOMUX_PINCM54)
#define GPIO_PWM_1_C1_IOMUX_FUNC                     IOMUX_PINCM54_PF_TIMG7_CCP1
#define GPIO_PWM_1_C1_IDX                                    DL_TIMER_CC_1_INDEX

/* Defines for PWM_2 */
#define PWM_2_INST                                                         TIMG8
#define PWM_2_INST_IRQHandler                                   TIMG8_IRQHandler
#define PWM_2_INST_INT_IRQN                                     (TIMG8_INT_IRQn)
#define PWM_2_INST_CLK_FREQ                                              1000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_2_C0_PORT                                                 GPIOA
#define GPIO_PWM_2_C0_PIN                                          DL_GPIO_PIN_7
#define GPIO_PWM_2_C0_IOMUX                                      (IOMUX_PINCM14)
#define GPIO_PWM_2_C0_IOMUX_FUNC                     IOMUX_PINCM14_PF_TIMG8_CCP0
#define GPIO_PWM_2_C0_IDX                                    DL_TIMER_CC_0_INDEX



/* Defines for TIMG_0 */
#define TIMG_0_INST                                                      (TIMG0)
#define TIMG_0_INST_IRQHandler                                  TIMG0_IRQHandler
#define TIMG_0_INST_INT_IRQN                                    (TIMG0_INT_IRQn)
#define TIMG_0_INST_LOAD_VALUE                                           (9999U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_0_FBRD_40_MHZ_115200_BAUD                                      (45)





/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOA)

/* Defines for LED_0: GPIOA.0 with pinCMx 1 on package pin 33 */
#define LED_LED_0_PIN                                            (DL_GPIO_PIN_0)
#define LED_LED_0_IOMUX                                           (IOMUX_PINCM1)
/* Defines for SDA: GPIOB.15 with pinCMx 32 on package pin 3 */
#define I2C_SDA_PORT                                                     (GPIOB)
#define I2C_SDA_PIN                                             (DL_GPIO_PIN_15)
#define I2C_SDA_IOMUX                                            (IOMUX_PINCM32)
/* Defines for SCL: GPIOA.17 with pinCMx 39 on package pin 10 */
#define I2C_SCL_PORT                                                     (GPIOA)
#define I2C_SCL_PIN                                             (DL_GPIO_PIN_17)
#define I2C_SCL_IOMUX                                            (IOMUX_PINCM39)
/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOB)

/* Defines for KEY_1: GPIOB.21 with pinCMx 49 on package pin 20 */
#define KEY_KEY_1_PIN                                           (DL_GPIO_PIN_21)
#define KEY_KEY_1_IOMUX                                          (IOMUX_PINCM49)
/* Defines for KEY_2: GPIOB.23 with pinCMx 51 on package pin 22 */
#define KEY_KEY_2_PIN                                           (DL_GPIO_PIN_23)
#define KEY_KEY_2_IOMUX                                          (IOMUX_PINCM51)
/* Defines for KEY_3: GPIOB.24 with pinCMx 52 on package pin 23 */
#define KEY_KEY_3_PIN                                           (DL_GPIO_PIN_24)
#define KEY_KEY_3_IOMUX                                          (IOMUX_PINCM52)
/* Port definition for Pin Group DIR */
#define DIR_PORT                                                         (GPIOA)

/* Defines for x_axle: GPIOA.13 with pinCMx 35 on package pin 6 */
#define DIR_x_axle_PIN                                          (DL_GPIO_PIN_13)
#define DIR_x_axle_IOMUX                                         (IOMUX_PINCM35)
/* Defines for y_axle: GPIOA.14 with pinCMx 36 on package pin 7 */
#define DIR_y_axle_PIN                                          (DL_GPIO_PIN_14)
#define DIR_y_axle_IOMUX                                         (IOMUX_PINCM36)
/* Defines for y_axle1: GPIOA.12 with pinCMx 34 on package pin 5 */
#define DIR_y_axle1_PIN                                         (DL_GPIO_PIN_12)
#define DIR_y_axle1_IOMUX                                        (IOMUX_PINCM34)
/* Port definition for Pin Group Relay */
#define Relay_PORT                                                       (GPIOB)

/* Defines for relay: GPIOB.13 with pinCMx 30 on package pin 1 */
#define Relay_relay_PIN                                         (DL_GPIO_PIN_13)
#define Relay_relay_IOMUX                                        (IOMUX_PINCM30)
/* Defines for z_axle: GPIOA.15 with pinCMx 37 on package pin 8 */
#define DIR_z_axle_PIN                                          (DL_GPIO_PIN_15)
#define DIR_z_axle_IOMUX                                         (IOMUX_PINCM37)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_PWM_1_init(void);
void SYSCFG_DL_PWM_2_init(void);
void SYSCFG_DL_TIMG_0_init(void);
void SYSCFG_DL_UART_0_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
