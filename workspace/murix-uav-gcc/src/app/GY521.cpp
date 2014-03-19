

#include <Arduino.h>
#include "GY521.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

mpu6050_calibration_t::mpu6050_calibration_t(){
	 linear_speed_limit_x=100;
	 linear_speed_limit_y=100;
	 linear_speed_limit_z=100;
	 linear_position_limit_x=100;
	 linear_position_limit_y=100;
	 linear_position_limit_z=100;
	 acc_count_deadzone_limit=20;
}
	

GY521::GY521(int module){
 this->_module=module;

   //Initializations:
    currentGyroRange = 0;
    currentAcceleroRange=0;
	calibration.deadzone_ok=false;
}

void GY521::init( void ){

	tracking.imu_t_now=micros() / 1000000.0;
	tracking.imu_t_back=tracking.imu_t_now;
	
	//wake-up
	setSleepMode(false);
	//test
	if(!testConnection()){
	  Serial.println("GY521 testConnection fail");
	}
	//
	setI2CBypass(false);
	//
	setAcceleroRange(MPU6050_ACCELERO_RANGE_2G);
	//
	setGyroRange(MPU6050_GYRO_RANGE_250);
	//
	setBW(MPU6050_BW_256);
	//

    //mpu6050 settling time
    delay(30);	
}


float GY521::radian2degree(float radian){
	return radian * (180.0f / M_PI);
}

int GY521::update(){
  int error;

  
  // Read the raw values.
  // Read 14 bytes at once, 
  // containing acceleration, temperature and gyro.
  // With the default settings of the MPU-6050,
  // there is no filter enabled, and the values
  // are not very stable.
  error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &mpu6050_packet_raw, sizeof(mpu6050_packet_raw));
  if(error!=0){
	  Serial.print(F("Read accel, temp and gyro, error = "));
	  Serial.println(error,DEC);
	  return error;
  }

  // Swap all high and low bytes.
  // After this, the registers values are swapped, 
  // so the structure name like x_accel_l does no 
  // longer contain the lower byte.
  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap

  SWAP (mpu6050_packet_raw.reg.x_accel_h, mpu6050_packet_raw.reg.x_accel_l);
  SWAP (mpu6050_packet_raw.reg.y_accel_h, mpu6050_packet_raw.reg.y_accel_l);
  SWAP (mpu6050_packet_raw.reg.z_accel_h, mpu6050_packet_raw.reg.z_accel_l);
  SWAP (mpu6050_packet_raw.reg.t_h, mpu6050_packet_raw.reg.t_l);
  SWAP (mpu6050_packet_raw.reg.x_gyro_h, mpu6050_packet_raw.reg.x_gyro_l);
  SWAP (mpu6050_packet_raw.reg.y_gyro_h, mpu6050_packet_raw.reg.y_gyro_l);
  SWAP (mpu6050_packet_raw.reg.z_gyro_h, mpu6050_packet_raw.reg.z_gyro_l);  

/////////////////////////////////////////////////////////////////////

    if (currentAcceleroRange == MPU6050_ACCELERO_RANGE_2G) {
        sensors.ax=mpu6050_packet_raw.value.x_accel / 16384.0 * 9.81;
        sensors.ay=mpu6050_packet_raw.value.y_accel / 16384.0 * 9.81;
        sensors.az=mpu6050_packet_raw.value.z_accel / 16384.0 * 9.81;
        }
    if (currentAcceleroRange == MPU6050_ACCELERO_RANGE_4G){
        sensors.ax=mpu6050_packet_raw.value.x_accel / 8192.0 * 9.81;
        sensors.ay=mpu6050_packet_raw.value.y_accel / 8192.0 * 9.81;
        sensors.az=mpu6050_packet_raw.value.z_accel / 8192.0 * 9.81;
        }
    if (currentAcceleroRange == MPU6050_ACCELERO_RANGE_8G){
        sensors.ax=mpu6050_packet_raw.value.x_accel / 4096.0 * 9.81;
        sensors.ay=mpu6050_packet_raw.value.y_accel / 4096.0 * 9.81;
        sensors.az=mpu6050_packet_raw.value.z_accel / 4096.0 * 9.81;
        }
    if (currentAcceleroRange == MPU6050_ACCELERO_RANGE_16G){
        sensors.ax=mpu6050_packet_raw.value.x_accel / 2048.0 * 9.81;
        sensors.ay=mpu6050_packet_raw.value.y_accel / 2048.0 * 9.81;
        sensors.az=mpu6050_packet_raw.value.z_accel / 2048.0 * 9.81;
        }
		
///////////////////////////////////////////////////////////////////////		
		
    if (currentGyroRange == MPU6050_GYRO_RANGE_250) {
        sensors.gx=mpu6050_packet_raw.value.x_gyro / 131.0;//7505.7;
        sensors.gy=mpu6050_packet_raw.value.y_gyro / 131.0;//7505.7;
        sensors.gz=mpu6050_packet_raw.value.z_gyro / 131.0;//7505.7;
        }
    if (currentGyroRange == MPU6050_GYRO_RANGE_500){
        sensors.gx=mpu6050_packet_raw.value.x_gyro / 65.5;//3752.9;
        sensors.gy=mpu6050_packet_raw.value.y_gyro / 65.5;//3752.9;
        sensors.gz=mpu6050_packet_raw.value.z_gyro / 65.5;//3752.9;
        }
    if (currentGyroRange == MPU6050_GYRO_RANGE_1000){
        sensors.gx=mpu6050_packet_raw.value.x_gyro / 32.8;//1879.3;;
        sensors.gy=mpu6050_packet_raw.value.y_gyro / 32.8;//1879.3;
        sensors.gz=mpu6050_packet_raw.value.z_gyro / 32.8;//1879.3;
        }
    if (currentGyroRange == MPU6050_GYRO_RANGE_2000){
        sensors.gx=mpu6050_packet_raw.value.x_gyro / 16.4;//939.7;
        sensors.gy=mpu6050_packet_raw.value.y_gyro / 16.4;//939.7;
        sensors.gz=mpu6050_packet_raw.value.z_gyro / 16.4;//939.7;
        }		
		
////////////////////////////////////////////////////////////////////////////////
  
  // The temperature sensor is -40 to +85 degrees Celsius.
  // It is a signed integer.
  // According to the datasheet: 
  //   340 per degrees Celsius, -512 at 35 degrees.
  // At 0 degrees: -512 - (340 * 35) = -12412
  sensors.temp =  ((double) mpu6050_packet_raw.value.temperature+521.0)/340.0+35.0;
  
////////////////////////////////////////////////////////////////////////////////

	//extract pitch and roll from accelerometer (no drift, but very noise)
	tracking.acc_pitch =  radian2degree(  atan2( -sensors.ax,sensors.az)  ) ;
	tracking.acc_roll =   radian2degree(  atan2(sensors.ay,sensors.az)  ) ;

////////////////////////////////////////////////////////////////////////////////

     //remove offset using calibration data
	sensors.gx -= calibration.gx_avg;
	sensors.gy -= calibration.gy_avg;
	sensors.gz -= calibration.gz_avg;
    tracking.acc_pitch -= calibration.acc_pitch_avg;
    tracking.acc_roll  -=  calibration.acc_roll_avg;

/////////////////////////////////////////////////////////////////////////////////
    
	//calculate delta time
	tracking.imu_t_back=tracking.imu_t_now;
	tracking.imu_t_now=micros() / 1000000.0;
	tracking.imu_dt=tracking.imu_t_now-tracking.imu_t_back;
	if(tracking.imu_dt>0.1) return 0;
	
	
	//calculate delta s
	tracking.gyro_x_dt_back=tracking.gyro_x_dt;
	tracking.gyro_y_dt_back=tracking.gyro_y_dt;
	tracking.gyro_z_dt_back=tracking.gyro_z_dt;
	tracking.gyro_x_dt=sensors.gx*tracking.imu_dt;
	tracking.gyro_y_dt=sensors.gy*tracking.imu_dt;
	tracking.gyro_z_dt=sensors.gz*tracking.imu_dt;
	
	//integrate gyros to obtain pitch, roll and yaw (drift when run long time)
	tracking.gyro_x_integrate_back=tracking.gyro_x_integrate;
	tracking.gyro_y_integrate_back=tracking.gyro_y_integrate;
	tracking.gyro_z_integrate_back=tracking.gyro_z_integrate;	
    //integrate using Trapezoidal Method (reduce integrative error)
	tracking.gyro_x_integrate=tracking.gyro_x_integrate_back + tracking.gyro_x_dt_back + (tracking.gyro_x_dt-tracking.gyro_x_dt_back)/2.0;
	tracking.gyro_y_integrate=tracking.gyro_y_integrate_back + tracking.gyro_y_dt_back + (tracking.gyro_y_dt-tracking.gyro_y_dt_back)/2.0;
	tracking.gyro_z_integrate=tracking.gyro_z_integrate_back + tracking.gyro_z_dt_back + (tracking.gyro_z_dt-tracking.gyro_z_dt_back)/2.0;
	
	//prevent to reach overflow (prevent windup like PIDs)
	tracking.gyro_x_integrate=constrain(tracking.gyro_x_integrate,-180,180);
	tracking.gyro_y_integrate=constrain(tracking.gyro_y_integrate,-180,180);
	tracking.gyro_z_integrate=constrain(tracking.gyro_z_integrate,-180,180);

	//////////////////////////////////////////////////////////////////////////////
	//complementary filter - fusion accelerometer and gyroscope data - pitch and roll
	tracking.fusion_pitch= 0.98*(tracking.fusion_pitch+tracking.gyro_x_dt)+(0.02*tracking.acc_pitch);
	tracking.fusion_roll = 0.98*(tracking.fusion_roll+tracking.gyro_y_dt)+(0.02*tracking.acc_roll);
	//fusion_yaw=constrain(gyro_z_integrate,-180,180);

	//prevent to reach overflow (prevent windup like PIDs)
	tracking.fusion_pitch=constrain(tracking.fusion_pitch,-180,180);
	tracking.fusion_roll=constrain(tracking.fusion_roll,-180,180);
	//fusion_yaw=constrain(gyro_z_integrate,-180,180);
  
 
  /////////////////////////////////////////////////////////////////////////////////////
  
    // alpha is calculated as t / (t + dT) with t, the low-pass filter's time-constant and dT, the event delivery rate
	float low_pass_filter_timer_constant = 1.0 / 256.0; // 256 Hz low-pass filter
	float alpha= low_pass_filter_timer_constant / (low_pass_filter_timer_constant + tracking.imu_dt);
	
	
	// Isolate the force of gravity with the low-pass filter.
	tracking.gravity_x = alpha * tracking.gravity_x + (1-alpha) * sensors.ax;
	tracking.gravity_y = alpha * tracking.gravity_y + (1-alpha) * sensors.ay;
	tracking.gravity_z = alpha * tracking.gravity_z + (1-alpha) * sensors.az;
	
	// Remove the gravity contribution with the high-pass filter.
	tracking.acc_linear_x[0] = tracking.acc_linear_x[1];
	tracking.acc_linear_y[0] = tracking.acc_linear_y[1];
	tracking.acc_linear_z[0] = tracking.acc_linear_z[1];
	tracking.acc_linear_x[1] = sensors.ax - tracking.gravity_x;
	tracking.acc_linear_y[1] = sensors.ay - tracking.gravity_y;
	tracking.acc_linear_z[1] = sensors.az - tracking.gravity_z;
  
	//linear acceleration deadzone (central region where acceleration must be zero)
	
	if(tracking.acc_linear_x[1]>calibration.linear_ax_min&&tracking.acc_linear_x[1]<calibration.linear_ax_max) tracking.acc_linear_x[1]=0;
	if(tracking.acc_linear_y[1]>calibration.linear_ay_min&&tracking.acc_linear_y[1]<calibration.linear_ay_max) tracking.acc_linear_y[1]=0;
	if(tracking.acc_linear_z[1]>calibration.linear_az_min&&tracking.acc_linear_z[1]<calibration.linear_az_max) tracking.acc_linear_z[1]=0;
	
	  
	//linear acceleration stagnation counter (how many times linear acceleration is null)
	if(tracking.acc_linear_x[1]==0){ tracking.acc_count_deadzone_x++; } else { tracking.acc_count_deadzone_x=0; }
	if(tracking.acc_linear_y[1]==0){ tracking.acc_count_deadzone_y++; } else { tracking.acc_count_deadzone_y=0; }
	if(tracking.acc_linear_z[1]==0){ tracking.acc_count_deadzone_z++; } else { tracking.acc_count_deadzone_z=0; }
  
    //integral linear accel -> linear speed [using Trapezoidal Method (reduce integrative error)]
	tracking.acc_linear_x_speed[0]=	tracking.acc_linear_x_speed[1];
	tracking.acc_linear_y_speed[0]=	tracking.acc_linear_y_speed[1];
	tracking.acc_linear_z_speed[0]=	tracking.acc_linear_z_speed[1];
	tracking.acc_linear_x_speed[1] = tracking.acc_linear_x_speed[0] + tracking.acc_linear_x[0] + ((tracking.acc_linear_x[1]-tracking.acc_linear_x[0])/2.0);
	tracking.acc_linear_y_speed[1] = tracking.acc_linear_y_speed[0] + tracking.acc_linear_y[0] + ((tracking.acc_linear_y[1]-tracking.acc_linear_y[0])/2.0);
	tracking.acc_linear_z_speed[1] = tracking.acc_linear_z_speed[0] + tracking.acc_linear_z[0] + ((tracking.acc_linear_z[1]-tracking.acc_linear_z[0])/2.0);
  
    //linear speed clamp (prevent windup like PIDs)
	tracking.acc_linear_x_speed[1]=constrain(tracking.acc_linear_x_speed[1],-calibration.linear_speed_limit_x,calibration.linear_speed_limit_x);
	tracking.acc_linear_y_speed[1]=constrain(tracking.acc_linear_y_speed[1],-calibration.linear_speed_limit_y,calibration.linear_speed_limit_y);
	tracking.acc_linear_z_speed[1]=constrain(tracking.acc_linear_z_speed[1],-calibration.linear_speed_limit_z,calibration.linear_speed_limit_z);
  
    //linear speed reset if not acceleration for long time (take account dissipative forces)
	
	if(tracking.acc_count_deadzone_x > calibration.acc_count_deadzone_limit ) tracking.acc_linear_x_speed[1]=0;
	if(tracking.acc_count_deadzone_y > calibration.acc_count_deadzone_limit ) tracking.acc_linear_y_speed[1]=0;
	if(tracking.acc_count_deadzone_z > calibration.acc_count_deadzone_limit ) tracking.acc_linear_z_speed[1]=0;
	
  
    //integral linear speed -> position [using Trapezoidal Method (reduce integrative error)]
	tracking.acc_linear_x_position[0] = tracking.acc_linear_x_position[1];
	tracking.acc_linear_y_position[0] = tracking.acc_linear_y_position[1];
	tracking.acc_linear_z_position[0] = tracking.acc_linear_z_position[1];
	tracking.acc_linear_x_position[1] = tracking.acc_linear_x_position[0]+tracking.acc_linear_x_speed[0]+((tracking.acc_linear_x_speed[1]-tracking.acc_linear_x_speed[0])/2.0);
	tracking.acc_linear_y_position[1] = tracking.acc_linear_y_position[0]+tracking.acc_linear_y_speed[0]+((tracking.acc_linear_y_speed[1]-tracking.acc_linear_y_speed[0])/2.0);
	tracking.acc_linear_z_position[1] = tracking.acc_linear_z_position[0]+tracking.acc_linear_z_speed[0]+((tracking.acc_linear_z_speed[1]-tracking.acc_linear_z_speed[0])/2.0);
  
    //clamp position to 3d world box (prevent windup like PIDs)
	tracking.acc_linear_x_position[1]=constrain(tracking.acc_linear_x_position[1],-calibration.linear_position_limit_x,calibration.linear_position_limit_x); 
	tracking.acc_linear_y_position[1]=constrain(tracking.acc_linear_y_position[1],-calibration.linear_position_limit_y,calibration.linear_position_limit_y); 
	tracking.acc_linear_z_position[1]=constrain(tracking.acc_linear_z_position[1],-calibration.linear_position_limit_z,calibration.linear_position_limit_z); 
  
    /////////////////////////////////////////////////////////////////////////////////////

  return 0;
}

void GY521::flat_trim(int n_samples){
	//
	calibration.gx_sum=0;
	calibration.gy_sum=0;
	calibration.gz_sum=0;
	calibration.acc_pitch_sum=0;
	calibration.acc_roll_sum=0;

	//
	calibration.gx_avg=0;
	calibration.gy_avg=0;
	calibration.gz_avg=0;
	calibration.acc_pitch_avg=0;
	calibration.acc_roll_avg=0;
	
	//
	calibration.deadzone_ok=false;
	calibration.gx_min=0;
	calibration.gy_min=0;
	calibration.gz_min=0;
	calibration.gx_max=0;
	calibration.gy_max=0;
	calibration.gz_max=0;	
	
	//
	update();
	
	//init accel min and max
	calibration.ax_min=sensors.ax;
	calibration.ax_max=sensors.ax;
	calibration.ay_min=sensors.ay;
	calibration.ay_max=sensors.ay;
	calibration.az_min=sensors.az;
	calibration.az_max=sensors.az;	
	//init gyro min and max
	calibration.gx_min=sensors.gx;
	calibration.gx_max=sensors.gx;
	calibration.gy_min=sensors.gy;
	calibration.gy_max=sensors.gy;
	calibration.gz_min=sensors.gz;
	calibration.gz_max=sensors.gz;
	//------------------------------
	calibration.linear_ax_min=tracking.acc_linear_x[1];
	calibration.linear_ax_max=tracking.acc_linear_x[1];
	calibration.linear_ay_min=tracking.acc_linear_y[1];
	calibration.linear_ay_max=tracking.acc_linear_y[1];
	calibration.linear_az_min=tracking.acc_linear_z[1];
	calibration.linear_az_max=tracking.acc_linear_z[1];
	
	for(int i=0;i<n_samples;i++){
		update();
		calibration.gx_sum+=sensors.gx;
		calibration.gy_sum+=sensors.gy;
		calibration.gz_sum+=sensors.gz;
		calibration.acc_pitch_sum+=tracking.acc_pitch;
		calibration.acc_roll_sum+=tracking.acc_roll;
		
		//
		if(sensors.ax<calibration.ax_min)calibration.ax_min=sensors.ax;
		if(sensors.ax>calibration.ax_max)calibration.ax_max=sensors.ax;
		
		if(sensors.ay<calibration.ay_min)calibration.ay_min=sensors.ay;
		if(sensors.ay>calibration.ay_max)calibration.ay_max=sensors.ay;
		
		if(sensors.az<calibration.az_min)calibration.az_min=sensors.az;
		if(sensors.az>calibration.az_max)calibration.az_max=sensors.az;		

		//
		if(sensors.gx<calibration.gx_min)calibration.gx_min=sensors.gx;
		if(sensors.gx>calibration.gx_max)calibration.gx_max=sensors.gx;
		
		if(sensors.gy<calibration.gy_min)calibration.gy_min=sensors.gy;
		if(sensors.gy>calibration.gy_max)calibration.gy_max=sensors.gy;
		
		if(sensors.gz<calibration.gz_min)calibration.gz_min=sensors.gz;
		if(sensors.gz>calibration.gz_max)calibration.gz_max=sensors.gz;
		
		
		//
		if(tracking.acc_linear_x[1]<calibration.linear_ax_min)calibration.linear_ax_min=tracking.acc_linear_x[1];
		if(tracking.acc_linear_x[1]>calibration.linear_ax_max)calibration.linear_ax_max=tracking.acc_linear_x[1];
		
		if(tracking.acc_linear_y[1]<calibration.linear_ay_min)calibration.linear_ay_min=tracking.acc_linear_y[1];
		if(tracking.acc_linear_y[1]>calibration.linear_ay_max)calibration.linear_ay_max=tracking.acc_linear_y[1];
		
		if(tracking.acc_linear_z[1]<calibration.linear_az_min)calibration.linear_az_min=tracking.acc_linear_z[1];
		if(tracking.acc_linear_z[1]>calibration.linear_az_max)calibration.linear_az_max=tracking.acc_linear_z[1];
	}
	//
	calibration.deadzone_ok=true;
	
	//
	calibration.gx_avg=calibration.gx_sum/(double) n_samples;
	calibration.gy_avg=calibration.gy_sum/(double) n_samples;
	calibration.gz_avg=calibration.gz_sum/(double) n_samples;
	calibration.acc_pitch_avg=calibration.acc_pitch_sum/(double) n_samples;
	calibration.acc_roll_avg=calibration.acc_roll_sum/(double) n_samples;	
	
	//reset integrals after calibration
	tracking.gyro_x_integrate=0;
	tracking.gyro_y_integrate=0;
	tracking.gyro_z_integrate=0;
}


// --------------------------------------------------------
// MPU6050_read
//
// This is a common function to read multiple bytes 
// from an I2C device.
//
// It uses the boolean parameter for Wire.endTransMission()
// to be able to hold or release the I2C-bus. 
// This is implemented in Arduino 1.0.1.
//
// Only this function is used to read. 
// There is no function for a single byte.
//
int GY521::MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n;

  // stellaris/tiva-c-series : I2C Port
  Wire.setModule(this->_module);
  
  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);

  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // return : no error
}


// --------------------------------------------------------
// MPU6050_write_reg
//
// An extra function to write a single register.
// It is just a wrapper around the MPU_6050_write()
// function, and it is only a convenient function
// to make it easier to write a single register.
//
int GY521::MPU6050_write_reg(int reg, uint8_t data)
{
  // stellaris/tiva-c-series : I2C Port
  Wire.setModule(this->_module);


  int error;

  error = MPU6050_write(reg, &data, 1);

  return (error);
}


// --------------------------------------------------------
// MPU6050_write
//
// This is a common function to write multiple bytes to an I2C device.
//
// If only a single register is written,
// use the function MPU_6050_write_reg().
//
// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.
//
// If only a single register is written, a pointer
// to the data has to be used, and the size is
// a single byte:
//   int data = 0;        // the data to write
//   MPU6050_write (MPU6050_PWR_MGMT_1, &c, 1);
//
int GY521::MPU6050_write(int start, const uint8_t *pData, int size)
{
  // stellaris/tiva-c-series : I2C Port
  Wire.setModule(this->_module);


  int n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);

  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
    return (error);

  return (0);         // return : no error
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
 
void GY521::setSleepMode(bool state) {

    char temp=0;

	//
	MPU6050_read(MPU6050_PWR_MGMT_1_REG,(uint8_t*)&temp,1);
	
    if (state == true)
        temp |= 1<<MPU6050_SLP_BIT;
    if (state == false)
        temp &= ~(1<<MPU6050_SLP_BIT);
	
	//
    MPU6050_write_reg(MPU6050_PWR_MGMT_1_REG, temp);
}
 
bool GY521::testConnection( void ) {

    char temp=0;
	
	//
	MPU6050_read(MPU6050_WHO_AM_I_REG,(uint8_t*)&temp,1);
	
    return (temp == (MPU6050_ADDRESS & 0xFE));
}
 
void GY521::setBW(char BW) {

    char temp;
    BW=BW & 0x07;
    
	MPU6050_read(MPU6050_CONFIG_REG,(uint8_t*)&temp,1);
	
    temp &= 0xF8;
    temp = temp + BW;
	
    MPU6050_write_reg(MPU6050_CONFIG_REG, temp);
}
 
void GY521::setI2CBypass(bool state) {

    char temp;
	
	MPU6050_read(MPU6050_INT_PIN_CFG,(uint8_t*)&temp,1);
	
    if (state == true)
        temp |= 1<<MPU6050_BYPASS_BIT;
    if (state == false)
        temp &= ~(1<<MPU6050_BYPASS_BIT);
		
    MPU6050_write_reg(MPU6050_INT_PIN_CFG, temp);
}
 
//--------------------------------------------------
//----------------Accelerometer---------------------
//--------------------------------------------------
 
void GY521::setAcceleroRange( char range ) {
    char temp;
    range = range & 0x03;
    currentAcceleroRange = range;
	
	MPU6050_read(MPU6050_ACCELERO_CONFIG_REG,(uint8_t*)&temp,1);
	
    temp &= ~(3<<3);
    temp = temp + (range<<3);
	
    MPU6050_write_reg(MPU6050_ACCELERO_CONFIG_REG, temp);
}
 
//--------------------------------------------------
//------------------Gyroscope-----------------------
//--------------------------------------------------
void GY521::setGyroRange( char range ) {
    char temp;
    currentGyroRange = range;
    range = range & 0x03;
	
	MPU6050_read(MPU6050_GYRO_CONFIG_REG,(uint8_t*)&temp,1);
	
    temp &= ~(3<<3);
    temp = temp + (range<<3);
	
    MPU6050_write_reg(MPU6050_GYRO_CONFIG_REG, temp);
}
 
