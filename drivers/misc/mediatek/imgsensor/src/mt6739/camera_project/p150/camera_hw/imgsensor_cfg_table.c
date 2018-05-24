/*
 * Copyright (C) 2017 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "kd_imgsensor.h"

#include "regulator/regulator.h"
#include "gpio/gpio.h"
#include "mclk/mclk.h"



#include "imgsensor_cfg_table.h"

enum IMGSENSOR_RETURN (*hw_open[IMGSENSOR_HW_ID_MAX_NUM])(struct IMGSENSOR_HW_DEVICE **) = {
	imgsensor_hw_regulator_open,
	imgsensor_hw_gpio_open,
	imgsensor_hw_mclk_open
};

struct IMGSENSOR_HW_CFG imgsensor_custom_config[] = {
	{
		IMGSENSOR_SENSOR_IDX_MAIN,
		IMGSENSOR_I2C_DEV_0,
		{
			{IMGSENSOR_HW_ID_MCLK, IMGSENSOR_HW_PIN_MCLK1},
			{IMGSENSOR_HW_ID_GPIO, IMGSENSOR_HW_PIN_AVDD},
			{IMGSENSOR_HW_ID_REGULATOR, IMGSENSOR_HW_PIN_DOVDD},
			{IMGSENSOR_HW_ID_REGULATOR, IMGSENSOR_HW_PIN_DVDD},
			{IMGSENSOR_HW_ID_REGULATOR, IMGSENSOR_HW_PIN_AFVDD},
			{IMGSENSOR_HW_ID_GPIO, IMGSENSOR_HW_PIN_PDN},
			{IMGSENSOR_HW_ID_GPIO, IMGSENSOR_HW_PIN_RST},
			{IMGSENSOR_HW_ID_NONE, IMGSENSOR_HW_PIN_NONE},
		},
	},
	{
		IMGSENSOR_SENSOR_IDX_SUB,
		IMGSENSOR_I2C_DEV_1,
		{
			{IMGSENSOR_HW_ID_MCLK, IMGSENSOR_HW_PIN_MCLK2},
			{IMGSENSOR_HW_ID_GPIO, IMGSENSOR_HW_PIN_AVDD},
			{IMGSENSOR_HW_ID_REGULATOR, IMGSENSOR_HW_PIN_DOVDD},
			{IMGSENSOR_HW_ID_REGULATOR, IMGSENSOR_HW_PIN_DVDD},
			{IMGSENSOR_HW_ID_GPIO, IMGSENSOR_HW_PIN_PDN},
			{IMGSENSOR_HW_ID_GPIO, IMGSENSOR_HW_PIN_RST},
			{IMGSENSOR_HW_ID_NONE, IMGSENSOR_HW_PIN_NONE},
		},
	},

	{IMGSENSOR_SENSOR_IDX_NONE}
};

struct IMGSENSOR_HW_POWER_SEQ platform_power_sequence[] = {
#ifdef MIPI_SWITCH
	{
		IMGSENSOR_SENSOR_IDX_NAME_SUB,
		{
			{
				IMGSENSOR_HW_PIN_MIPI_SWITCH_EN,
				IMGSENSOR_HW_PIN_STATE_LEVEL_0,
				0,
				IMGSENSOR_HW_PIN_STATE_LEVEL_HIGH,
				0
			},
			{
				IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL,
				IMGSENSOR_HW_PIN_STATE_LEVEL_HIGH,
				0,
				IMGSENSOR_HW_PIN_STATE_LEVEL_0,
				0
			},
		}
	},
#endif

	{NULL}
};

/* Legacy design */
struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence[] = {
#if defined(GC5025_CXT_K600)
		{
			SENSOR_DRVNAME_GC5025_CXT_K600_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{DOVDD, Vol_1800, 0},
				{DVDD, Vol_1200, 0},
				{AVDD, Vol_2800, 0},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif

#if defined(GC5025_CXT_K600_FRONT)
		{
			SENSOR_DRVNAME_GC5025_CXT_K600_FRONT_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{DOVDD, Vol_1800, 0},
				{DVDD, Vol_1200, 0},
				{AVDD, Vol_2800, 0},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif
#if defined(S5K3L8_SUNWIN_C300_MIPI_RAW)
		{
			SENSOR_DRVNAME_S5K3L8_SUNWIN_C300_REAR_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{DVDD, Vol_1200, 0},
				{AVDD, Vol_2800, 0},
				{DOVDD, Vol_1800, 0},
			    {AFVDD, Vol_2800, 0},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif
#if defined(S5K3P3ST_SUNNY_C300_MIPI_RAW)
		{
			SENSOR_DRVNAME_S5K3P3ST_SUNNY_C300_FRONT_MIPI_RAW,
			{
				{SensorMCLK1, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{AVDD, Vol_2800, 0},
				{DVDD, Vol_1200, 0},
				{DOVDD, Vol_1800, 0},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif
#if defined(IMX135_SUNWIN_C300_MIPI_RAW)
		{
			SENSOR_DRVNAME_imx135_SUNWIN_C300_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{AVDD, Vol_2800, 0},
				{DVDD, Vol_1100, 0},
				{DOVDD, Vol_1800, 0},
				{AFVDD, Vol_2800, 0},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif

#if defined(IMX135_SUNWIN_P300_MIPI_RAW)
				{
					SENSOR_DRVNAME_IMX135_SUNWIN_P300_MIPI_RAW,
					{
						{SensorMCLK, Vol_High, 0},
						{PDN, Vol_Low, 1},
						//{RST, Vol_Low, 1},
						{AVDD, Vol_2800, 0},
						{DVDD, Vol_1100, 0},
						{DOVDD, Vol_1800, 0},
						{AFVDD, Vol_2800, 2},
						{PDN, Vol_High, 5},
						//{RST, Vol_High, 5}
					},
				},
#endif

#if defined(IMX135_SUNWIN_P130_MIPI_RAW)
						{
							SENSOR_DRVNAME_IMX135_SUNWIN_P130_MIPI_RAW,
							{
								{SensorMCLK, Vol_High, 0},
								{PDN, Vol_Low, 1},
								{AVDD, Vol_2800, 0},
								{DVDD, Vol_1100, 0},
								{DOVDD, Vol_1800, 0},
								{AFVDD, Vol_2800, 2},
								{PDN, Vol_High, 5}
							},
						},
#endif
#if defined(GC5025_HOLITECH_P130_MIPI_RAW)
						{
							SENSOR_DRVNAME_GC5025_HOLITECH_P130_MIPI_RAW,
							{
				                {SensorMCLK1, Vol_High, 0},
				                {PDN, Vol_Low, 1},
				                {RST, Vol_Low, 1},
				                {DOVDD, Vol_1800, 0},
				                {DVDD, Vol_1100, 0},
				                {AVDD, Vol_2800, 0},
				                {PDN, Vol_High, 5},
				                {RST, Vol_High, 5}

							},
						},
#endif
#if defined(OV8856_SUNWIN_P130_MIPI_RAW)
								{
									SENSOR_DRVNAME_OV8856_SUNWIN_P130_MIPI_RAW,
									{
										{SensorMCLK1, Vol_High, 0},
										{RST, Vol_Low, 1},
										{AVDD, Vol_2800, 0},
										{DVDD, Vol_1100, 0},
										{DOVDD, Vol_1800, 0},
										{RST, Vol_High, 5}
									},
								},
#endif
#if defined(S5K4H8_SUNWIN_P130_MIPI_RAW)
						{
							SENSOR_DRVNAME_S5K4H8_SUNWIN_P130_MIPI_RAW,
							{
								{SensorMCLK, Vol_High, 0},
								{RST, Vol_Low, 1},
								{AVDD, Vol_2800, 0},
								{DVDD, Vol_1100, 0},
								{DOVDD, Vol_1800, 0},
								{AFVDD, Vol_2800, 2},
								{RST, Vol_High, 5}
							},
						},
#endif
#if defined(IMX135_SUNNY_P130_MIPI_RAW)
								{
									SENSOR_DRVNAME_IMX135_SUNNY_P130_MIPI_RAW,
									{
										{SensorMCLK, Vol_High, 0},
										{RST, Vol_Low, 1},
										{AVDD, Vol_2800, 0},
										{DVDD, Vol_1100, 0},
										{DOVDD, Vol_1800, 0},
										{AFVDD, Vol_2800, 2},
										{RST, Vol_High, 5}
									},
								},
#endif

#if defined(GC5025A_HOLITECH_P150_MIPI_RAW)
								{
									SENSOR_DRVNAME_GC5025A_HOLITECH_P150_MIPI_RAW,
									{
										{SensorMCLK1, Vol_High, 0},
										{PDN, Vol_Low, 1},
										{RST, Vol_Low, 1},
										{DOVDD, Vol_1800, 0},
										{DVDD, Vol_1200, 0},
										{AVDD, Vol_2800, 0},
										{PDN, Vol_High, 5},
										{RST, Vol_High, 5}
		
									},
								},
#endif

#if defined(S5K3L8_OFILM_P150_MIPI_RAW)
				{
					SENSOR_DRVNAME_S5K3L8_OFILM_P150_REAR_MIPI_RAW,
					{
						{SensorMCLK, Vol_High, 0},
						{PDN, Vol_Low, 1},
						{RST, Vol_Low, 1},
						{DVDD, Vol_1200, 0},
						{AVDD, Vol_2800, 0},
						{DOVDD, Vol_1800, 0},
						{AFVDD, Vol_2800, 0},
						{PDN, Vol_High, 5},
						{RST, Vol_High, 5}
					},
				},
#endif

#if defined(S5K4H7YX_SUNWIN_P150_MIPI_RAW)
				{
					SENSOR_DRVNAME_S5K4H7YX_SUNWIN_P150_MIPI_RAW,
					{
						{SensorMCLK1, Vol_High, 0},
						{PDN, Vol_Low, 1},
						//{RST, Vol_Low, 1},
						{AVDD, Vol_2800, 0},
						{DOVDD, Vol_1800, 0},
						{DVDD, Vol_1200, 0},
						{PDN, Vol_High, 5},
						//{RST, Vol_High, 5}
					},
				},
#endif
#if defined(S5K3L8_SUNWIN_P150_MIPI_RAW)
						{
							SENSOR_DRVNAME_S5K3L8_SUNWIN_P150_REAR_MIPI_RAW,
							{
								{SensorMCLK, Vol_High, 0},
								{PDN, Vol_Low, 1},
								{RST, Vol_Low, 1},
								{DVDD, Vol_1200, 0},
								{AVDD, Vol_2800, 0},
								{DOVDD, Vol_1800, 0},
								{AFVDD, Vol_2800, 0},
								{PDN, Vol_High, 5},
								{RST, Vol_High, 5}
							},
						},
#endif

#if defined(GC5025A_SUNWIN_P150_MIPI_RAW)
				{
					SENSOR_DRVNAME_GC5025A_SUNWIN_P150_MIPI_RAW,
					{
						{SensorMCLK1, Vol_High, 0},
						{PDN, Vol_Low, 1},
						{RST, Vol_Low, 1},
						{DOVDD, Vol_1800, 0},
						{DVDD, Vol_1200, 0},
						{AVDD, Vol_2800, 0},
						{PDN, Vol_High, 5},
						{RST, Vol_High, 5}
				
					},
				},
#endif

#if defined(GC8034_CXT_K600)
		{
			SENSOR_DRVNAME_GC8034_CXT_K600_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{DOVDD, Vol_1800, 0},
				{DVDD, Vol_1200, 0},
				{AVDD, Vol_2800, 0},
				{AFVDD, Vol_2800, 1},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif

#if defined(GC5025_CMK_K600)
		{
			SENSOR_DRVNAME_GC5025_CMK_K600_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{DOVDD, Vol_1800, 0},
				{DVDD, Vol_1200, 0},
				{AVDD, Vol_2800, 0},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif

#if defined(GC5025_SUNNINESS_K600)
		{
			SENSOR_DRVNAME_GC5025_SUNNINESS_K600_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{DOVDD, Vol_1800, 0},
				{DVDD, Vol_1200, 0},
				{AVDD, Vol_2800, 0},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif

#if defined(GC5025_SUNNINESS_K600_FRONT)
		{
			SENSOR_DRVNAME_GC5025_SUNNINESS_K600_FRONT_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_Low, 1},
				{RST, Vol_Low, 1},
				{DOVDD, Vol_1800, 0},
				{DVDD, Vol_1200, 0},
				{AVDD, Vol_2800, 0},
				{PDN, Vol_High, 5},
				{RST, Vol_High, 5}
			},
		},
#endif

#if defined(OV13870_MIPI_RAW)
	{
		SENSOR_DRVNAME_OV13870_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 5}
		},
	},
#endif
#if defined(IMX398_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX398_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2800, 0},
			{DOVDD, Vol_1800, 0},
			{DVDD, Vol_1100, 0},
			{AFVDD, Vol_2800, 0},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 1},
		},
	},
#endif
#if defined(OV23850_MIPI_RAW)
	{
		SENSOR_DRVNAME_OV23850_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 2},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 5},
		},
	},
#endif
#if defined(IMX386_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX386_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2800, 0},
			{DOVDD, Vol_1800, 0},
			{DVDD, Vol_1100, 0},
			{AFVDD, Vol_2800, 0},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 1},
		},
	},
#endif

#if defined(IMX338_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX338_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2500, 0},
			{DOVDD, Vol_1800, 0},
			{DVDD, Vol_1100, 0},
			{AFVDD, Vol_2800, 0},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 1}
		},
	},
#endif
#if defined(S5K4E6_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K4E6_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 1},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2900, 0},
			{DVDD, Vol_1200, 2},
			{AFVDD, Vol_2800, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(S5K3P8SP_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K3P8SP_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1000, 0},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 4},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 1},
			{RST, Vol_High, 0},
		},
	},
#endif
#if defined(S5K3M2_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K3M2_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 4},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 1},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(S5K3P3SX_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K3P3SX_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 4},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 1},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(S5K5E2YA_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K5E2YA_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 4},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 1},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(S5K4ECGX_MIPI_YUV)
	{
		SENSOR_DRVNAME_S5K4ECGX_MIPI_YUV,
		{
			{DVDD, Vol_1200, 1},
			{AVDD, Vol_2800, 1},
			{DOVDD, Vol_1800, 1},
			{AFVDD, Vol_2800, 0},
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 1},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 1},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(OV16880_MIPI_RAW)
	{
		SENSOR_DRVNAME_OV16880_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 1},
			{AVDD, Vol_2800, 1},
			{DVDD, Vol_1200, 5},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 1},
			{RST, Vol_High, 2}
		},
	},
#endif
#if defined(S5K2P7_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K2P7_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1000, 0},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 4},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 1},
			{RST, Vol_High, 0},
		},
	},
#endif
#if defined(S5K2P8_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K2P8_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 4},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 1},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(IMX258_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX258_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(IMX258_MIPI_MONO)
	{
		SENSOR_DRVNAME_IMX258_MIPI_MONO,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(IMX377_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX377_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(OV8858_MIPI_RAW)
	{
		SENSOR_DRVNAME_OV8858_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 1},
			{AVDD, Vol_2800, 1},
			{DVDD, Vol_1200, 5},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 1},
			{RST, Vol_High, 2}
		},
	},
#endif
#if defined(OV8856_MIPI_RAW)
	{SENSOR_DRVNAME_OV8856_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 2},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 5},
		},
	},
#endif
#if defined(S5K2X8_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K2X8_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(IMX214_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX214_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2800, 0},
			{DOVDD, Vol_1800, 0},
			{DVDD, Vol_1000, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 1}
		},
	},
#endif
#if defined(IMX214_MIPI_MONO)
	{
		SENSOR_DRVNAME_IMX214_MIPI_MONO,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2800, 0},
			{DOVDD, Vol_1800, 0},
			{DVDD, Vol_1000, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 1}
		},
	},
#endif
#if defined(IMX230_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX230_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2800, 10},
			{DOVDD, Vol_1800, 10},
			{DVDD, Vol_1200, 10},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(S5K3L8_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K3L8_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1200, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(IMX362_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX362_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2800, 10},
			{DOVDD, Vol_1800, 10},
			{DVDD, Vol_1200, 10},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(S5K2L7_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K2L7_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1000, 0},
			{AFVDD, Vol_2800, 3},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 5}
		},
	},
#endif
#if defined(IMX318_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX318_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2800, 10},
			{DOVDD, Vol_1800, 10},
			{DVDD, Vol_1200, 10},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(OV8865_MIPI_RAW)
	{
		SENSOR_DRVNAME_OV8865_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 5},
			{RST, Vol_Low, 5},
			{DOVDD, Vol_1800, 5},
			{AVDD, Vol_2800, 5},
			{DVDD, Vol_1200, 5},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_High, 5},
			{RST, Vol_High, 5}
		},
	},
#endif
#if defined(IMX219_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX219_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{AVDD, Vol_2800, 10},
			{DOVDD, Vol_1800, 10},
			{DVDD, Vol_1000, 10},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_Low, 0},
			{PDN, Vol_High, 0},
			{RST, Vol_Low, 0},
			{RST, Vol_High, 0}
		},
	},
#endif
#if defined(S5K3M3_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K3M3_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 0},
			{RST, Vol_Low, 0},
			{DOVDD, Vol_1800, 0},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1000, 0},
			{AFVDD, Vol_2800, 1},
			{PDN, Vol_High, 0},
			{RST, Vol_High, 2}
		},
	},
#endif
#if defined(OV5670_MIPI_RAW)
	{
		SENSOR_DRVNAME_OV5670_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 5},
			{RST, Vol_Low, 5},
			{DOVDD, Vol_1800, 5},
			{AVDD, Vol_2800, 5},
			{DVDD, Vol_1200, 5},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_High, 5},
			{RST, Vol_High, 5}
		},
	},
#endif
#if defined(OV5670_MIPI_RAW_2)
	{
		SENSOR_DRVNAME_OV5670_MIPI_RAW_2,
		{
			{SensorMCLK, Vol_High, 0},
			{PDN, Vol_Low, 5},
			{RST, Vol_Low, 5},
			{DOVDD, Vol_1800, 5},
			{AVDD, Vol_2800, 5},
			{DVDD, Vol_1200, 5},
			{AFVDD, Vol_2800, 5},
			{PDN, Vol_High, 5},
			{RST, Vol_High, 5}
		},
	},
#endif
#if defined(OV20880_MIPI_RAW)
	{
		SENSOR_DRVNAME_OV20880_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{RST, Vol_Low, 1},
			{AVDD, Vol_2800, 1},
			{DOVDD, Vol_1800, 1},
			{DVDD, Vol_1100, 1},
			{RST, Vol_High, 5}
		},
	},
#endif
#if defined(GC2365_MIPI_RAW)
		{
			SENSOR_DRVNAME_GC2365_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_High, 1},
				{RST, Vol_Low, 10},
				{DOVDD, Vol_1800, 5},
				{DVDD, Vol_1200, 5},
				{AVDD, Vol_2800, 5},
				{PDN, Vol_Low, 5},
				{RST, Vol_High, 5}
			},
		},
#endif
#if defined(GC2366_MIPI_RAW)
		{
			SENSOR_DRVNAME_GC2366_MIPI_RAW,
			{
				{SensorMCLK, Vol_High, 0},
				{PDN, Vol_High, 1},
				{RST, Vol_Low, 10},
				{DOVDD, Vol_1800, 5},
				{DVDD, Vol_1200, 5},
				{AVDD, Vol_2800, 5},
				{PDN, Vol_Low, 5},
				{RST, Vol_High, 5}
			},
		},
#endif
	/* add new sensor before this line */
	{NULL,},
};
