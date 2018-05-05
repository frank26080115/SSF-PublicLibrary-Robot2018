#ifndef _BOOKWORMIMU_H_
#define _BOOKWORMIMU_H_

// include order matters here
#define MPU6050_INCLUDE_DMP_MOTIONAPPS20
#include <helper_3dmath.h>
#include "MPU6050.h"

extern MPU6050 mpu;

class cBookWormIMU
{
	public:
		bool init(bool forceCalib);
		void calibrate(bool force);
		double getAngle(uint8_t idx);
};

extern cBookWormIMU BookWormIMU;

#endif