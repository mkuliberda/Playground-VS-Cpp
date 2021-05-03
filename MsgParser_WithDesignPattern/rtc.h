#pragma once
/**
  * @brief  RTC Time structure definition
  */



/** @defgroup RTC_WeekDay_Definitions RTC WeekDay Definitions
* @{
 */
#define RTC_WEEKDAY_MONDAY             ((uint8_t)0x01)
#define RTC_WEEKDAY_TUESDAY            ((uint8_t)0x02)
#define RTC_WEEKDAY_WEDNESDAY          ((uint8_t)0x03)
#define RTC_WEEKDAY_THURSDAY           ((uint8_t)0x04)
#define RTC_WEEKDAY_FRIDAY             ((uint8_t)0x05)
#define RTC_WEEKDAY_SATURDAY           ((uint8_t)0x06)
#define RTC_WEEKDAY_SUNDAY             ((uint8_t)0x07)

/**
 * @}
*/

typedef struct
{
	uint8_t Hours;            /*!< Specifies the RTC Time Hour.
								   This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
								   This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected  */

	uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
								   This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

	uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
								   This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

	uint8_t TimeFormat;       /*!< Specifies the RTC AM/PM Time.
								   This parameter can be a value of @ref RTC_AM_PM_Definitions */

	uint32_t SubSeconds;     /*!< Specifies the RTC_SSR RTC Sub Second register content.
								   This parameter corresponds to a time unit range between [0-1] Second
								   with [1 Sec / SecondFraction +1] granularity */

	uint32_t SecondFraction;  /*!< Specifies the range or granularity of Sub Second register content
								   corresponding to Synchronous pre-scaler factor value (PREDIV_S)
								   This parameter corresponds to a time unit range between [0-1] Second
								   with [1 Sec / SecondFraction +1] granularity.
								   This field will be used only by HAL_RTC_GetTime function */

	uint32_t DayLightSaving;  /*!< Specifies DayLight Save Operation.
								   This parameter can be a value of @ref RTC_DayLightSaving_Definitions */

	uint32_t StoreOperation;  /*!< Specifies RTC_StoreOperation value to be written in the BCK bit
								   in CR register to store the operation.
								   This parameter can be a value of @ref RTC_StoreOperation_Definitions */
}RTC_TimeTypeDef;

/**
  * @brief  RTC Date structure definition
  */
typedef struct
{
	uint8_t WeekDay;  /*!< Specifies the RTC Date WeekDay.
						   This parameter can be a value of @ref RTC_WeekDay_Definitions */

	uint8_t Month;    /*!< Specifies the RTC Date Month (in BCD format).
						   This parameter can be a value of @ref RTC_Month_Date_Definitions */

	uint8_t Date;     /*!< Specifies the RTC Date.
						   This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

	uint8_t Year;     /*!< Specifies the RTC Date Year.
						   This parameter must be a number between Min_Data = 0 and Max_Data = 99 */

}RTC_DateTypeDef;