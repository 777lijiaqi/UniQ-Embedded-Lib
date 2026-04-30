/**
 * @file        uniq_xxx.h
 * @author      Li Jiaqi (李家琦)
 * @version     V1.0.0
 * @date        2024-05-01
 * @brief       UniQ Core - [此处填写模块名]
 *
 * @copyright   Copyright (c) 2024 Li Jiaqi. All rights reserved.
 *
 * @license     Dual-Licensed: GPLv3 or Commercial Waiver.
 *              Details: https://github.com/777lijiaqi/UniQ-Embedded-Lib#sponsorship
 *
 * @legal       免责声明 (DISCLAIMER):
 *              1. 本软件按“原样”提供，不提供任何形式的明示或暗示担保。
 *              2. 作者不对因使用本软件导致的任何硬件损坏、数据丢失或利润损失承担责任。
 *              3. 使用者需自行承担在特定硬件环境下测试和验证代码安全性的风险。
 *              
 *              1. THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *              2. THE AUTHOR SHALL NOT BE LIABLE FOR ANY DAMAGES, INCLUDING BUT NOT 
 *                 LIMITED TO HARDWARE DAMAGE, DATA LOSS, OR PROFITS LOSS.
 *              3. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE IS WITH YOU.
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef __UNIQ_XXX_H
#define __UNIQ_XXX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>

/** @addtogroup UNIQ_XXX_MODULE
 * @{
 */

/* Exported Types ------------------------------------------------------------*/
/** @defgroup UNIQ_XXX_Exported_Types
 * @{
 */
typedef struct {
    void *hw_handle;    /* 硬件接口句柄 */
    /* 添加更多成员... */
} Uniq_XXX_Handle_t;
/**
 * @}
 */

/* Exported Functions Prototypes ---------------------------------------------*/
/** @defgroup UNIQ_XXX_Exported_Functions
 * @{
 */

/**
 * @brief       [函数名]: Uniq_XXX_Init
 * @description: 初始化模块
 * @param       handle: [Uniq_XXX_Handle_t*] 模块句柄
 * @retval      [int8_t] 0:成功, -1:失败
 */
int8_t Uniq_XXX_Init(Uniq_XXX_Handle_t *handle);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __UNIQ_XXX_H */

/************************ (C) COPYRIGHT Li Jiaqi *****END OF FILE****/
