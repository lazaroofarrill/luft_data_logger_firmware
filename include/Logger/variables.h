//
// Created by lazaroofarrill on 9/14/21.
//

#ifndef BACKEND_VARIABLES_H
#define BACKEND_VARIABLES_H

#include "Arduino.h"

class Station {
    String Elabora_la_Pregunta();

    void Pregunta(String Pregunta_Trama);

    void Colecta_la_Respuesta();

    float Respuesta_Procesada();

    void Salva_Respuesta(float Respalda);

    void Proceso_Completo();


public:
//++ Temperatura

    void Temp_Grados_C();

    void Temp_Grados_F();

    void Temp_Dewpoint_C();

    void Temp_Dewpoint_F();

    void wind_chill_Temp_C();

    void wind_chill_temp_F();

    void wet_bulb_Temp_C();

    void wet_bulb_Temp_F();

    void wind_heater_Temp_C();

    void wind_heater_Temp_F();


//++ Humedad

    void Humd_Relativa_Porciento();

    void Humd_Absoluta_g_m3();

    void Mixing_Ratio_g_kg();

//++ Enthalpy

    void specific_enthalpy_KJ_Kg();

//++ Presion

    void abs_air_pressure_Hpa();

    void rel_air_pressure_Hpa();

    void air_density_Kg_m3();

//++ Velocidad del Viento

    void wind_speed_mXs();

    void wind_speed_KxH();

    void wind_speed_mpH();

    void wind_speed_Kts();

    void wind_speed_fast_mXs();

    void wind_speed_fast_KxH();

    void wind_speed_fast_mpH();

    void wind_speed_fast_kts();

    void wind_speed_standard_deviation_mXs();

    void wind_speed_standard_deviation_mpH();

//++ Direccion del Viento

    void wind_direction_Grados();

    void wind_direction_Fast_Grados();

    void wind_direction_Corr_Grados();

    void wind_direction_standard_deviation_Grados();

    void wind_value_quality_Porciento();

    void wind_value_quality_fast();

    void compass_heading_Grados();

//++ Precipitacion Cantidad

    void Precipitation_Quantity_Absolute_Litros_m2();

    void Precipitation_Quantity_Absolute_mm();

    void Precipitation_Quantity_Absolute_inches();

    void Precipitation_Quantity_Absolute_mil();

    void Precipitation_Quantity_Diferencial_Litros_m2();

    void Precipitation_Quantity_Diferencial_mm();

    void Precipitation_Quantity_Diferencial_inches();

    void Precipitation_Quantity_Diferencial_mil();

//++ Precipitacion Intensidad

    void Precipitation_Intensity_lm2H();

    void Precipitation_Intensity_mmH();

    void Precipitation_Intensity_inH();

    void Precipitation_Intensity_milH();

//++Radiacion

    void Global_Radiation_Wm2();


    float temp_prueba();

    float humedad_prueba();

    float velocidad_viento_prueba();

    String dir_viento_prueba();

    float presion_prueba();

    float pira_prueba();

    float epavo_prueba();
};

#endif //BACKEND_VARIABLES_H
