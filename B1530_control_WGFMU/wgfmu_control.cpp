//wgfmu_control.cpp
//Author: Pierre-Antoine Mouny
//Date: 25/05/2021
//Universite De Sherbrooke (3IT)
// Description: Implements multilevel programming for memristors using WGFMU remotely.
// Core functions of the script (read_resistance, write_resistance and extract_results) can 
// be reused for other pulsed measurements on memristors.
//#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "wgfmu.h"
#include "wgfmu_control.h"


int main(){
    //TO DO ==> AUTOMATIC ACQUISITION OF HRS AND LRS VALUES
    double LRS;
    printf("Enter LRS value: ");
    scanf("%lf", &LRS);
    printf("Selected LRS is %f \n", LRS);
    double HRS;
    printf("Enter HRS value: ");
    scanf("%lf", &HRS);
    printf("Selected LRS is %f \n", HRS);
    double step_size;
    printf("Enter step for pulse amplitude: ");
    scanf("%lf", &step_size);
    double V_LTP;
    printf("Enter V_LTP: ");
    scanf("%lf", &V_LTP);
    double V_LTD;
    printf("Enter V_LTD: ");
    scanf("%lf", &V_LTD);
    while(1){
        /*int nb_state;
        printf("Enter number of resistance levels: ");
        scanf("%d", &nb_state);*/
        double list_resist[nb_state];
        for(int i=0;i<nb_state;i++){
            list_resist[i] = LRS + i*(HRS-LRS)/(nb_state-1);
        }
        //Multilevel programming for all resistance states
        for(int i_state=0; i_state<nb_state; i_state++){
            double R_target = list_resist[i_state];
            printf("Target resist is %f \n", R_target);
            double Rmax = R_target*(1+tolerance);
            double Rmin = R_target*(1-tolerance);
            double Vp = V_LTP;
            double Vn = V_LTD;
            int c = 0;
            int pulse_number = 0;
            double pulse_amp = 0;
            double R_c = read_resistance(Vr, t_read, topChannel, bottomChannel, pulse_amp);//put input there
            while(c<stop){
                if(Rmin<=R_c && R_c<=Rmax){
                    Vp = V_LTP;
                    Vn = V_LTD;
                    printf("Reached target resistance \n");
                    pulse_amp = 0;
                    c++;
                }
                else if(R_c>Rmax){
                    write_resistance(Vp, t_pulse, topChannel, bottomChannel);//put input there
                    if (Vp < V_max) {
                        Vp = Vp + step_size;
                    }
                    Vn=V_LTD;
                    pulse_amp = Vp;
                    printf("Apply positive pulse %f \n", Vp);
                    //c=0; //Could cause issue
                }
                else if(R_c<Rmin){
                    write_resistance(Vn, t_pulse, topChannel, bottomChannel);//put input
                    if (Vn > -V_max) {
                        Vn = Vn - step_size;
                    }
                    Vp=V_LTP;
                    pulse_amp = Vn;
                    printf("Apply negative pulse %f \n", Vn);
                    //c=0; //Could cause issue
                }
                pulse_number++;
                R_c = read_resistance(Vr, t_read, topChannel, bottomChannel, pulse_amp);//put input
                printf("Measured resistance is %f \n", R_c);
            } 
        }      
    }
    return 0;
}

double read_resistance(double Vpulse, double tpulse, int topChannel, int bottomChannel, double pulse_amp) // Pulse voltage output
{
    // OFFLINE
    //Top electrode

    WGFMU_clear(); // 9
    WGFMU_createPattern("pulse", 0); // 0 ms, 0 V
    WGFMU_addVector("pulse", 0.00000001, Vpulse); //10 ns rise time, 1 V
    WGFMU_addVector("pulse", tpulse-0.00000002, Vpulse);
    WGFMU_addVector("pulse", 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_addSequence(topChannel, "pulse", 1); //1 pulse output

    //Bottom electrode
    WGFMU_createPattern("meas", 0); // 0 ms, 0 V
    WGFMU_addVector("meas", 0.00000001, 0); //10 ns rise time, 1 V
    WGFMU_addVector("meas", tpulse-0.00000002, 0);
    WGFMU_addVector("meas", 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_setMeasureEvent("meas", "Imeas", 0.00000001, 1, tpulse-0.00000002, tpulse-0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED); // meas from 10 ns, 100 points, 0.01 ms interval, no averaging //10
    WGFMU_addSequence(bottomChannel, "meas", 1); //1 "pulse" output

    // ONLINE
    WGFMU_openSession("GPIB0::17::INSTR"); //18
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    double res = extract_results(topChannel, bottomChannel, 0, path_csv, pulse_amp);
    WGFMU_initialize();
    WGFMU_closeSession();

    return res;
}


void write_resistance(double Vpulse, double tpulse, int topChannel, int bottomChannel) // Pulse voltage output
{

    // OFFLINE
    //Top electrode

    WGFMU_clear(); // 9
    WGFMU_createPattern("pulse", 0); // 0 ms, 0 V
    WGFMU_addVector("pulse", 0.00000001, Vpulse); //10 ns rise time
    WGFMU_addVector("pulse", tpulse-0.00000002, Vpulse);
    WGFMU_addVector("pulse", 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_addSequence(topChannel, "pulse", 1); //1 pulse output

    //Bottom electrode
    WGFMU_createPattern("ground", 0);
    WGFMU_addVector("ground", 0.00000001, 0);
    WGFMU_addVector("ground", tpulse-0.00000002, 0);
    WGFMU_addVector("ground", 0.00000001, 0);
    WGFMU_addSequence(bottomChannel, "ground", 1); //1 "pulse" output


    // ONLINE
    WGFMU_openSession("GPIB0::17::INSTR"); //18
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    WGFMU_initialize();
    WGFMU_closeSession();
}

//EXPORT A SINGLE VALUE
//CAREFULL WITH TIME VALUES
double extract_results(int channelId1, int channelId2, int offset,  const char* fileName, double pulse_amp) //extract only a single value
{   int measuredSize, totalSize;
    // For CSV output
    FILE* fp = fopen(fileName, "a");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
        }
    if(fp != 0) {
        WGFMU_getMeasureValueSize(channelId2, &measuredSize, &totalSize);
        double time, value, voltage;
        WGFMU_getMeasureValue(channelId2, offset, &time, &value);
        WGFMU_getInterpolatedForceValue(channelId1, time, &voltage);
        double res;
        if (value < 0) {
            res = -voltage / value;
        }
        else {
            res = voltage / value;
        }
        fprintf(fp, "%f;%f \n", pulse_amp, res);
        printf("Extracted resistance is %f \n", res);
        return(res);
    }
    fclose(fp);
}

//TO DO add limits for max amplitude
//Implement better convergence
//Measurement on BB3
//Need to save pulse amplitude as well