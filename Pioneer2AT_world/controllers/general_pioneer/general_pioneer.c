//Funciones tipicas de C y las correspondientes de Webots
#include <webots/distance_sensor.h>
#include <webots/robot.h>
#include <webots/position_sensor.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <webots/lidar.h>
#include <webots/motor.h>


#define TIME_STEP 64
#define max_vel 2.5   
#define Factor_reductor 0.85


int i = 0;
int j = 0;
double back_left_speed = 0.0, back_right_speed = 0.0;
double front_left_speed = 0.0, front_right_speed = 0.0;
//De acuerdo a la cinematica del robot, de tipo skid steering, la velocidad de las ruedas de ambos lados es la misma en direccion x
double val_ps_wheels[4] = {0.0, 0.0, 0.0, 0.0};

//Defincion de obstaculos
double derecha = 0.0;
double izquierda = 0.0;


//Funcion Gaussiana
double gaussian(double x, double mu, double sigma) {
return (1.0 / (sigma * sqrt(2.0 * M_PI))) * exp(-((x - mu) * (x - mu)) / (2 * sigma * sigma));
}


int main(int argc, char **argv) {
  wb_robot_init();

 
  
// Definicion de los Dispositivos: Actuadores y sensores
  
  //Actuadores
  WbDeviceTag wheels[4];
  char wheels_names[4][10] = {"rueda_id", "rueda_dd", "rueda_it", "rueda_dt"};
  for (i = 0; i < 4; i++) {
    wheels[i] = wb_robot_get_device(wheels_names[i]);
   }
   
  //Sonar delantero y trasero
  int i;
  bool avoid_obstacle_counter = 0;
  WbDeviceTag ds[16];
  char ds_names[16][10] = {"Sonar_0", "Sonar_1","Sonar_2","Sonar_3","Sonar_4","Sonar_5","Sonar_6","Sonar_7","Sonar_8","Sonar_9",
"Sonar_10","Sonar_11","Sonar_12","Sonar_13","Sonar_14","Sonar_15"  };
  for (i = 0; i < 16; i++) {
    ds[i] = wb_robot_get_device(ds_names[i]);
    wb_distance_sensor_enable(ds[i], TIME_STEP);
  }
    
    //Lidar
 WbDeviceTag lidar = wb_robot_get_device("Sick LMS 291");
 wb_lidar_enable(lidar, TIME_STEP);
 const int lidar_ancho = wb_lidar_get_horizontal_resolution(lidar); //Resolucion configurada a 180
 const int mitad_ancho = lidar_ancho / 2;
 const int max_rango = wb_lidar_get_max_range(lidar); //Rango maximo posible de alcance
 const double rango_considerado = max_rango / 25.0; //Rango umbral considerado de vision
 const float *lidar_valores = NULL;
 
 
    // Sensores ruedas
    WbDeviceTag ps_wheels[4];
  char ps_wheels_names[4][20] = {"rueda_id_sensor", "rueda_dd_sensor", "rueda_it_sensor", "rueda_dt_sensor"};
  for (i = 0; i < 4; i++) {
    ps_wheels[i] = wb_robot_get_device(ps_wheels_names[i]);
    wb_position_sensor_enable(ps_wheels[i], TIME_STEP);
    }
    
  // inicialciacion de los motores
    wb_motor_set_position(wheels[0], INFINITY);
    wb_motor_set_position(wheels[1], INFINITY);
    wb_motor_set_position(wheels[2], INFINITY);
    wb_motor_set_position(wheels[3], INFINITY);

    wb_motor_set_velocity(wheels[0], 0.0);
    wb_motor_set_velocity(wheels[1], 0.0);
    wb_motor_set_velocity(wheels[2], 0.0);
    wb_motor_set_velocity(wheels[3], 0.0);
 
  // Coeficientes del Lidar
  double *const coef= (double *)malloc(sizeof(double) * lidar_ancho);
  for (i = 0; i < lidar_ancho; ++i){  //A lo largo de la resolucion configurada en el lidar
  coef[i] = gaussian(i, mitad_ancho, lidar_ancho / 5); //Media centrada en el LIDAR (segundo parametro) y varianza arbitraria (tercer parametro)
   }    

  /* Bucle principal del controlador */
  
  while (wb_robot_step(TIME_STEP) != -1) {
    /*
     * Read the sensors : */
     for(i=0;i<4; i++) {
       val_ps_wheels[i] = wb_position_sensor_get_value(ps_wheels[i]);
      }
      /*
     printf("El valor de la rueda 1 es %f\n", val_ps_wheels[0]);
     printf("El valor de la rueda 3 es %f\n", val_ps_wheels[2]);
     */
     
    /* Procesamiento de los datos del LIDAR*/
    //Lectura datos del Lidar
      lidar_valores = wb_lidar_get_range_image(lidar);
      //Deteccion obstaculo izquierda
      for (i = 0; i < mitad_ancho; ++i) {
      if (lidar_valores[i] < rango_considerado)  //Recorre mitad derecha
        izquierda += coef[i] * (1.0 - lidar_valores[i] / max_rango);
      // Deteccion obstaculo derecha
      j = lidar_ancho - i - 1;
      if (lidar_valores[j] < rango_considerado) //Recorre mitad izquierda
        derecha += coef[i] * (1.0 - lidar_valores[j] / max_rango);  //Se multiplica por el coef igual que en la izq debido a la simetria de la funcion gaussiana.
    }
    
    const double obstaculo = izquierda + derecha;
    
    if (obstaculo > 0.1) {  //Valor umbral
      const double vel = (1.0 - Factor_reductor * obstaculo) * max_vel / obstaculo;
      front_left_speed = vel * izquierda;
      front_right_speed = vel * derecha;
      back_left_speed =  0.85 * front_left_speed;
      back_right_speed =  0.85 * front_right_speed;
    } else {
    back_left_speed = 2.5;
    back_right_speed = 2.5;
    front_left_speed = 2.5;
    front_right_speed = 2.5;
    }
    
 
      
    wb_motor_set_velocity(wheels[0], front_left_speed);
    wb_motor_set_velocity(wheels[1], front_right_speed);
    wb_motor_set_velocity(wheels[2], back_left_speed);
    wb_motor_set_velocity(wheels[3], back_right_speed);
    
    izquierda = 0.0;
    derecha = 0.0;
    
  }

  /* Enter your cleanup code here */

  free(coef);
  wb_robot_cleanup();

  return 0;
}
