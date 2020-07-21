#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <webots/robot.h>

#define TIME_STEP 64

int main(int argc, char **argv) {
  wb_robot_init();
  int i;
  bool avoid_obstacle_counter = 0;
  WbDeviceTag ds[16];
  char ds_names[16][10] = {"Sonar_0", "Sonar_1","Sonar_2","Sonar_3","Sonar_4","Sonar_5","Sonar_6","Sonar_7","Sonar_8","Sonar_9",
"Sonar_10","Sonar_11","Sonar_12","Sonar_13","Sonar_14","Sonar_15"  };
  for (i = 0; i < 16; i++) {
    ds[i] = wb_robot_get_device(ds_names[i]);
    wb_distance_sensor_enable(ds[i], TIME_STEP);
  }
  WbDeviceTag wheels[4];
  char wheels_names[4][10] = {"rueda_id", "rueda_dd", "rueda_it", "rueda_dt"};
  for (i = 0; i < 4; i++) {
    wheels[i] = wb_robot_get_device(wheels_names[i]);
    wb_motor_set_position(wheels[i], INFINITY);
  }
  while (wb_robot_step(TIME_STEP) != -1) {
    double left_speed = 1.5;
    double right_speed = 1.5;
    if (avoid_obstacle_counter > 0) {
      avoid_obstacle_counter--;
      left_speed = 1.0;
      right_speed = -1.0;
    } else { // read sensors
      double ds_values[16];
      for (i = 0; i < 16; i++)
        ds_values[i] = wb_distance_sensor_get_value(ds[i]);
      if (ds_values[0] < 950.0 || ds_values[1] < 950.0 || ds_values[2] < 950.0 || ds_values[3] < 950.0 || ds_values[4] < 950.0 || ds_values[5] < 950.0 || ds_values[6] < 950.0 || ds_values[7] < 950.0
      || ds_values[8] < 950.0 || ds_values[9] < 950.0|| ds_values[10] < 950.0 || ds_values[11] < 950.0|| ds_values[12] < 950.0 || ds_values[13] < 950.0|| ds_values[14] < 950.0 || ds_values[15] < 950.0)
        avoid_obstacle_counter = 100;
    }
    wb_motor_set_velocity(wheels[0], left_speed);
    wb_motor_set_velocity(wheels[1], right_speed);
    wb_motor_set_velocity(wheels[2], left_speed);
    wb_motor_set_velocity(wheels[3], right_speed);
  }
  wb_robot_cleanup();
  return 0;  // EXIT_SUCCESS
}
