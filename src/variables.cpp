//
// Created by lazaroofarrill on 9/14/21.
//

#include "Logger/variables.h"

#include <Arduino.h>


String Trama = "";           //Guarda temporalmente la repuesta Colectada
const int time_check = 100;   //Establece el número de milisegundos que se debe esperar para que haya datos disponibles en el puerto serie
int Canal = 0;
float Rango_Medicion_Min = 0, Rango_Medicion_Max = 0;


String Station::Elabora_la_Pregunta() {
    String Trama_Pregunta = "&_28673_M_00";
    return Trama_Pregunta;
}

void Station::Pregunta(String Pregunta_Trama) {
    Serial.println(Pregunta_Trama + Canal);
    Serial.flush();
}

void Station::Colecta_la_Respuesta() {
    Trama = Serial.readStringUntil('\n');
//   Serial.println(Trama);
    Serial.flush();
}

float Station::Respuesta_Procesada() {
    int Extracion = 0;
    float Ecuacion = 0;
    Extracion = Trama.substring(Trama.lastIndexOf(" ") + 1, Trama.length() - 1).toInt();
    Serial.println(Extracion);
    Ecuacion = (Rango_Medicion_Max - Rango_Medicion_Min) / 65520.0f * Extracion + (Rango_Medicion_Min);


    return Ecuacion;
}

void Salva_Respuesta(float Respalda) {
    //Serial.println(Respalda);
}

void Station::Proceso_Completo() {

    Pregunta(Elabora_la_Pregunta());
    Colecta_la_Respuesta();
    Salva_Respuesta(Respuesta_Procesada());

}


void Station::Temp_Grados_C() {    //100
    Canal = 100;
    Rango_Medicion_Min = -50;
    Rango_Medicion_Max = 60;

    Proceso_Completo();
}

void Station::Temp_Grados_F() {    //105
    Canal = 105;
    Rango_Medicion_Min = -58;
    Rango_Medicion_Max = 140;

    Proceso_Completo();
}

void Station::Temp_Dewpoint_C() {   //110
    Canal = 110;
    Rango_Medicion_Min = -50;
    Rango_Medicion_Max = 60;

    Proceso_Completo();

}

void Station::Temp_Dewpoint_F() {   //115
    Canal = 115;
    Rango_Medicion_Min = -58;
    Rango_Medicion_Max = 140;
    Proceso_Completo();
}

void Station::wind_chill_Temp_C() {  //111
    Canal = 111;
    Rango_Medicion_Min = -60;
    Rango_Medicion_Max = 70;

    Proceso_Completo();
}

void Station::wind_chill_temp_F() {  //116
    Canal = 116;
    Rango_Medicion_Min = -76;
    Rango_Medicion_Max = 158;

    Proceso_Completo();
}

void Station::wet_bulb_Temp_C() {  //114
    Canal = 114;
    Rango_Medicion_Min = -50;
    Rango_Medicion_Max = 60;

    Proceso_Completo();
}

void Station::wet_bulb_Temp_F() {  //119
    Canal = 119;
    Rango_Medicion_Min = -58;
    Rango_Medicion_Max = 140;

    Proceso_Completo();
}

void Station::wind_heater_Temp_C() {  //112
    Canal = 112;
    Rango_Medicion_Min = -50;
    Rango_Medicion_Max = 150;

    Proceso_Completo();
}

void Station::wind_heater_Temp_F() {  //117
    Canal = 117;
    Rango_Medicion_Min = -58;
    Rango_Medicion_Max = 302;
}


//++ Humedad

void Station::Humd_Relativa_Porciento() {  //200
    Canal = 200;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 100;

    Proceso_Completo();

}

void Station::Humd_Absoluta_g_m3() {  //205
    Canal = 205;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 1000;

    Proceso_Completo();
}

void Station::Mixing_Ratio_g_kg() {  //210
    Canal = 210;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 1000;

    Proceso_Completo();
}

//++ Enthalpy

void Station::specific_enthalpy_KJ_Kg() {  //215
    Canal = 215;
    Rango_Medicion_Min = -100;
    Rango_Medicion_Max = 1000;

    Proceso_Completo();
}

//++ Presion

void Station::abs_air_pressure_Hpa() {  //300
    Canal = 300;
    Rango_Medicion_Min = 300;
    Rango_Medicion_Max = 1200;

    Proceso_Completo();
}

void Station::rel_air_pressure_Hpa() {  //305
    Canal = 305;
    Rango_Medicion_Min = 300;
    Rango_Medicion_Max = 1200;

    Proceso_Completo();
}

void Station::air_density_Kg_m3() {  //310
    Canal = 310;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 3;

    Proceso_Completo();
}

//++ Velocidad del Viento

void Station::wind_speed_mXs() {  //400
    Canal = 400;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 75;

    Proceso_Completo();
}

void Station::wind_speed_KxH() {  //405
    Canal = 405;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 270;

    Proceso_Completo();
}

void Station::wind_speed_mpH() {  //410
    Canal = 410;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 167.8;

    Proceso_Completo();
}

void Station::wind_speed_Kts() {  //415
    Canal = 415;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 145.8;

    Proceso_Completo();
}

void Station::wind_speed_fast_mXs() {  //401
    Canal = 401;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 75;

    Proceso_Completo();
}

void Station::wind_speed_fast_KxH() {  //406
    Canal = 406;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 270;

    Proceso_Completo();
}

void Station::wind_speed_fast_mpH() {  //411
    Canal = 411;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 167.8;

    Proceso_Completo();
}

void Station::wind_speed_fast_kts() {  //416
    Canal = 416;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 145.8;

    Proceso_Completo();
}

void Station::wind_speed_standard_deviation_mXs() {  //403
    Canal = 403;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 75;

    Proceso_Completo();
}

void Station::wind_speed_standard_deviation_mpH() {  //413
    Canal = 413;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 167.8;

    Proceso_Completo();
}

//++ Direccion del Viento

void Station::wind_direction_Grados() {  //500
    Canal = 500;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 359.9;

    Proceso_Completo();
}

void Station::wind_direction_Fast_Grados() {  //501
    Canal = 501;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 359.9;

    Proceso_Completo();
}

void Station::wind_direction_Corr_Grados() {  //502
    Canal = 502;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 359.9;

    Proceso_Completo();
}

void Station::wind_direction_standard_deviation_Grados() {  //503
    Canal = 503;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 359.9;

    Proceso_Completo();
}

void Station::wind_value_quality_Porciento() {  //805
    Canal = 805;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 100;

    Proceso_Completo();
}

void Station::wind_value_quality_fast() {  //806
    Canal = 806;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 100;

    Proceso_Completo();
}

void Station::compass_heading_Grados() {  //510
    Canal = 510;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 359;

    Proceso_Completo();
}

//++ Precipitacion Cantidad

void Station::Precipitation_Quantity_Absolute_Litros_m2() {  //600
    Canal = 600;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 100000;

    Proceso_Completo();
}

void Station::Precipitation_Quantity_Absolute_mm() {  //620
    Canal = 620;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 100000;

    Proceso_Completo();
}

void Station::Precipitation_Quantity_Absolute_inches() {  //640
    Canal = 640;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 3937;

    Proceso_Completo();
}

void Station::Precipitation_Quantity_Absolute_mil() {  //660
    Canal = 660;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 3937008;

    Proceso_Completo();
}

void Station::Precipitation_Quantity_Diferencial_Litros_m2() {  //605
    Canal = 605;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 100000;

    Proceso_Completo();
}

void Station::Precipitation_Quantity_Diferencial_mm() {  //625
    Canal = 625;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 100000;

    Proceso_Completo();
}

void Station::Precipitation_Quantity_Diferencial_inches() { //645
    Canal = 645;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 3937;

    Proceso_Completo();
}

void Station::Precipitation_Quantity_Diferencial_mil() {  //665
    Canal = 665;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 3937008;

    Proceso_Completo();
}

//++ Precipitacion Intensidad

void Station::Precipitation_Intensity_lm2H() {  //800
    Canal = 800;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 200;

    Proceso_Completo();
}

void Station::Precipitation_Intensity_mmH() {  //820
    Canal = 820;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 200;

    Proceso_Completo();
}

void Station::Precipitation_Intensity_inH() {  //840
    Canal = 840;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 7.8740;

    Proceso_Completo();
}

void Station::Precipitation_Intensity_milH() {  //860
    Canal = 860;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 7874;

    Proceso_Completo();
}

//++Radiacion

void Station::Global_Radiation_Wm2() {  //900
    Canal = 900;
    Rango_Medicion_Min = 0;
    Rango_Medicion_Max = 1400;

    Proceso_Completo();
}


float Station::temp_prueba() {

    return (random(40, 70) / 2.8);
}

float Station::humedad_prueba() {

    return (random(40, 70) / 2.8);
}

float Station::velocidad_viento_prueba() {

    return (random(15, 70) / 2.8);
}

String Station::dir_viento_prueba() {

    switch (random(1, 9)) {
        case 1:
            return ("norte");
            break;
        case 2:
            return ("sur");
            break;
        case 3:
            return ("este");
            break;
        case 4:
            return ("oeste");
            break;
        case 5:
            return ("noreste");
            break;
        case 6:
            return ("noroeste");
            break;
        case 7:
            return ("sureste");
            break;
        case 8:
            return ("suroeste");
            break;


        default:
            return ("norte");
            break;
    }
}

float Station::presion_prueba() {

    return (random(80, 150) / 2.8);
}

float Station::pira_prueba() {

    return (random(30, 70) / 2.8);
}

float Station::evapo_prueba() {

    return (random(0, 70) / 2.8);
}

float Station::precipitationMM_prueba() {
    return (random(0, 70) / 2.8);
}
