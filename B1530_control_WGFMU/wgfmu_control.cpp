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
#include <visa.h>
#include <math.h>
#include <time.h>


int main() {
    //TO DO ==> AUTOMATIC ACQUISITION OF HRS AND LRS VALUES
    double LRS;
    printf("Enter LRS value: ");
    scanf("%lf", &LRS);
    printf("Selected LRS is %f \n", LRS);
    double HRS;
    printf("Enter HRS value: ");
    scanf("%lf", &HRS);
    printf("Selected LRS is %f \n", HRS);
    int choice;
    printf("Which measurement? (simple = 1, log = 2, adapt log = 3, Gvt = 4) \n");
    scanf("%d", &choice);
    if (choice == 1) {
        printf("Now executing simple multilevel programming of %d resistance states \n", nb_state);
        const char* timestamp = get_timestamp(choice);
        double step_size;
        printf("Enter step for pulse amplitude: ");
        scanf("%lf", &step_size);
        double V_LTP;
        printf("Enter V_LTP: ");
        scanf("%lf", &V_LTP);
        double V_LTD;
        printf("Enter V_LTD: ");
        scanf("%lf", &V_LTD);
        simple_convergence(LRS, HRS, V_LTD, V_LTP, step_size, timestamp);
    }
    else if (choice == 2) {
        printf("Now executing logarithmic multilevel programming of %d resistance states \n", nb_state);
        const char* timestamp = get_timestamp(choice);
        double V_LTP;
        printf("Enter V_LTP: ");
        scanf("%lf", &V_LTP);
        double V_LTD;
        printf("Enter V_LTD: ");
        scanf("%lf", &V_LTD);
        log_convergence(LRS, HRS, V_LTD, V_LTP, timestamp);
    }
    else if (choice == 3) {
        printf("Now executing adaptative logarithmic multilevel programming of %d resistance states \n", nb_state);
        const char* timestamp = get_timestamp(choice);
        double V_LTP;
        printf("Enter V_LTP: ");
        scanf("%lf", &V_LTP);
        double V_LTD;
        printf("Enter V_LTD: ");
        scanf("%lf", &V_LTD);
        log_convergence_2(LRS, HRS, V_LTD, V_LTP, timestamp);
    }
    else if (choice == 4) {
        printf("Now executing G(v,t) pulse scanning \n");
        const char* timestamp = get_timestamp(choice);
        double V_pulse_min;
        printf("Enter V_pulse_min: ");
        scanf("%lf", &V_pulse_min);
        double V_pulse_max;
        printf("Enter V_pulse_max: ");
        scanf("%lf", &V_pulse_max);
        double list_pulse_amp[pulse_number];
        if (V_pulse_min > V_pulse_max) {
            const double temp = V_pulse_min;
            V_pulse_min = V_pulse_max;
            V_pulse_max = temp;
        }
        for (int i = 0; i < pulse_number; i++) {
            list_pulse_amp[i] = V_pulse_min + i * (V_pulse_max - V_pulse_min) / (pulse_number - 1);
        }
        Gvt(list_time, list_pulse_amp, timestamp);
    }

    else {
        printf("Enter a defined operation type! \n You entered %d", choice);
    }

    return 0;
}

double read_resistance(double Vpulse, double tpulse, int topChannel, int bottomChannel, double pulse_amp, double pulse_width, const char* file_name) // Pulse voltage output
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
    double res = extract_results(topChannel, bottomChannel, 0, file_name, pulse_amp, pulse_width);
    WGFMU_initialize();
    WGFMU_closeSession();
    return res;
}


void write_resistance(double Vpulse, double tpulse, int topChannel, int bottomChannel) // Pulse voltage output
{

    // OFFLINE
    //Top electrode

    WGFMU_clear();
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
    WGFMU_openSession("GPIB0::17::INSTR"); 
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
double extract_results(int channelId1, int channelId2, int offset,  const char* fileName, double pulse_amp, double pulse_width) //extract only a single value
{   int measuredSize, totalSize;
    // For CSV output
    FILE* fp = fopen(fileName, "a");
    if(fp == NULL){
        printf("Error opening file\n");
        FILE* fp = fopen(fileName, "a");
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
        if (pulse_width == 0) {
            fprintf(fp, "%f;%f \n", pulse_amp, res);
        }
        else {
            fprintf(fp, "%f;%e;%f \n", pulse_amp, pulse_width, res);
        }
        printf("Extracted resistance is %f \n", res);
        fclose(fp);
        return(res);
    }
}

void simple_convergence(double LRS, double HRS, double V_LTD, double V_LTP, double step_size, const char* file_name)
{
    double list_resist[nb_state];
    for (int i = 0;i < nb_state;i++) {
        list_resist[i] = LRS + i * (HRS - LRS) / (nb_state - 1);
    }
    //Multilevel programming for all resistance states
    for (int i_state = 0; i_state < nb_state; i_state++) {
        double R_target = list_resist[i_state];
        printf("Target resist is %f \n", R_target);
        double Rmax = R_target * (1 + tolerance);
        double Rmin = R_target * (1 - tolerance);
        double Vp = V_LTP;
        double Vn = V_LTD;
        int c = 0;
        int pulse_number = 0;
        double pulse_amp = 0;
        double R_c = read_resistance(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name);
        while (c < stop) {
            if (Rmin <= R_c && R_c <= Rmax) {
                Vp = V_LTP;
                Vn = V_LTD;
                printf("Reached target resistance \n");
                pulse_amp = 0;
                c++;
            }
            else if (R_c > Rmax) {
                write_resistance(Vp, t_pulse, topChannel, bottomChannel);
                if (Vp < V_max) {
                    Vp = Vp + step_size;
                }
                Vn = V_LTD;
                pulse_amp = Vp;
                printf("Apply positive pulse %f \n", Vp);
                //c=0; //Could cause issue
            }
            else if (R_c < Rmin) {
                write_resistance(Vn, t_pulse, topChannel, bottomChannel);
                if (Vn > -V_max) {
                    Vn = Vn - step_size;
                }
                Vp = V_LTP;
                pulse_amp = Vn;
                printf("Apply negative pulse %f \n", Vn);
                //c=0; //Could cause issue
            }
            pulse_number++;
            R_c = read_resistance(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name);
            printf("Measured resistance is %f \n", R_c);
        }
    }
    printf("End of simple multilevel programming");
}

void log_convergence(double LRS, double HRS, double V_LTD, double V_LTP, const char* file_name)//Logarithmic multilevel programming
{
    /*int nb_state;
printf("Enter number of resistance levels: ");
scanf("%d", &nb_state);*/
    double list_resist[nb_state];
    for (int i = 0;i < nb_state;i++) {
        list_resist[i] = LRS + i * (HRS - LRS) / (nb_state - 1);
    }
    //Multilevel programming for all resistance states
    for (int i_state = 0; i_state < nb_state; i_state++) {
        double R_target = list_resist[i_state];
        printf("Target resist is %f \n", R_target);
        double Rmax = R_target * (1 + tolerance);
        double Rmin = R_target * (1 - tolerance);
        double Vp = V_LTP;
        double Vn = V_LTD;
        int c = 0;
        int count_n = 0;
        int count_p = 0;
        int pulse_number = 0;
        double pulse_amp = 0;
        double R_shift = 0;
        double R_c = read_resistance(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name);
        while (c < stop) {
            if (Rmin <= R_c && R_c <= Rmax) {
                Vp = V_LTP;
                Vn = V_LTD;
                count_n = 0;
                count_p = 0;
                printf("Reached target resistance \n");
                pulse_amp = 0;
                c++;
            }
            else if (R_c > Rmax) {
                if (count_p >= increment && (R_shift < min_shift * (HRS - LRS))) {
                    Vp = Vp+a*log10((R_c - R_target)/R_shift); //Maybe issues 
                    count_p = 0;
                }
                else if(R_shift>max_shift*(HRS-LRS)){
                    Vp = V_LTP;
                }
                if (Vp > V_max) {
                    Vp = V_max;
                }
                write_resistance(Vp, t_pulse, topChannel, bottomChannel);
                count_p++;
                pulse_amp = Vp;
                printf("Apply positive pulse %f \n", Vp);
            }
            else if (R_c < Rmin) {
                if (count_n >= increment && R_shift < min_shift * (HRS - LRS)) {
                    Vn = Vn - a * log10((R_target - R_c) / R_shift); //Maybe issues 
                    count_n = 0;
                }
                else if (R_shift > max_shift * (HRS - LRS)) {
                    Vn = V_LTD;
                }
                if (Vn < -V_max) {
                    Vn = -V_max;
                }
                write_resistance(Vn, t_pulse, topChannel, bottomChannel);
                count_n++;
                pulse_amp = Vn;
                printf("Apply negative pulse %f \n", Vn);
            }
            pulse_number++;
            R_shift = R_c;
            R_c = read_resistance(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name);
            R_shift = R_shift - R_c;
            if (R_shift < 0) {
                R_shift = -R_shift;
            }
            printf("Measured resistance is %f \n", R_c);
        }
    }
    printf("End of logarithmic multilevel programming");
}

void log_convergence_2(double LRS, double HRS, double V_LTD, double V_LTP, const char* file_name)//Logarithmic multilevel programming
{
    /*int nb_state;
printf("Enter number of resistance levels: ");
scanf("%d", &nb_state);*/
    double list_resist[nb_state];
    for (int i = 0;i < nb_state;i++) {
        list_resist[i] = LRS + i * (HRS - LRS) / (nb_state - 1);
    }
    //Multilevel programming for all resistance states
    for (int i_state = 0; i_state < nb_state; i_state++) {
        double R_target = list_resist[i_state];
        printf("Target resist is %f \n", R_target);
        double Rmax = R_target * (1 + tolerance);
        double Rmin = R_target * (1 - tolerance);
        double Vp = V_LTP;
        double Vn = V_LTD;
        int c = 0;
        int count_n = 0;
        int count_p = 0;
        int pulse_number = 0;
        double pulse_amp = 0;
        double R_shift = 0;
        double R_c = read_resistance(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name);//put input there
        while (c < stop) {
            if (Rmin <= R_c && R_c <= Rmax) {
                Vp = V_LTP;
                Vn = V_LTD;
                count_n = 0;
                count_p = 0;
                printf("Reached target resistance \n");
                pulse_amp = 0;
                c++;
            }
            else if (R_c > Rmax) {
                if (count_p >= increment && R_shift > -min_shift * (HRS - LRS)) {
                       if (R_shift < 0) {
                            R_shift = -R_shift;
                       }
                    Vp = Vp + a * (R_c/HRS) * log10((R_c - R_target) / R_shift); 
                    //Vp = Vp + a * log10((R_c - R_target) / R_shift);
                    count_p = 0;
                }
                else if (R_shift > max_shift * (HRS - LRS)) {
                    Vp = V_LTP;
                }
                if (Vp > V_max) {
                    Vp = V_max;
                }
                write_resistance(Vp, t_pulse, topChannel, bottomChannel);
                count_p++;
                pulse_amp = Vp;
                printf("Apply positive pulse %f \n", Vp);
            }
            else if (R_c < Rmin) {
                if (count_n >= increment && R_shift < min_shift * (HRS - LRS)) {
                    if (R_shift < 0) {
                        R_shift = -R_shift;
                    }
                    Vn = Vn - a * (R_c / HRS) * log10((R_target - R_c) / R_shift); 
                    //Vn = Vn - a * log10((R_target - R_c) / R_shift);
                    count_n = 0;
                }
                else if (R_shift > max_shift * (HRS - LRS)) {
                    Vn = V_LTD;
                }
                if (Vn < -V_max) {
                    Vn = -V_max;
                }
                write_resistance(Vn, t_pulse, topChannel, bottomChannel);
                count_n++;
                pulse_amp = Vn;
                printf("Apply negative pulse %f \n", Vn);
            }
            pulse_number++;
            R_shift = R_c;
            R_c = read_resistance(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name);
            R_shift = R_shift - R_c;
            //if (R_shift < 0) {
              //  R_shift = -R_shift;
            //}
            printf("Measured resistance is %f \n", R_c);
        }
    }
    printf("End of logarithmic multilevel programming");
}

void Gvt(double list_time[], double list_pulse_amp[], const char* file_name) {
    int size_time = 21;
    //int size_pulse_amp = sizeof(list_pulse_amp) / sizeof(list_pulse_amp[0]);
    for (int i = 0; i < size_time; i++) {
        for (int j = 0; j < pulse_number; j++) {
            if (list_pulse_amp[j] < 0) {
                DC_sweep(topChannel, bottomChannel, 0, V_set, 3, meas_time, compliance_set); //Set/Reset/Set cycle before applying the pulse
                //Sleep(sleep_time);
                DC_sweep(topChannel, bottomChannel, 0, V_reset, 3, meas_time, compliance_reset);
                //Sleep(sleep_time);
                DC_sweep(topChannel, bottomChannel, 0, V_set, 3, meas_time, compliance_set);
                //Sleep(sleep_time);
                printf("Now applying a writing pulse: %f V duration %e s \n", list_pulse_amp[j], list_time[i]);
                write_resistance(list_pulse_amp[j], list_time[i], topChannel, bottomChannel);
                Sleep(1);
                read_resistance(Vr, t_read, topChannel, bottomChannel, list_pulse_amp[j], list_time[i],file_name);
                Sleep(1);
            }
            else {
                DC_sweep(topChannel, bottomChannel, 0, V_reset, 3, meas_time, compliance_reset); //Reset/Set/Reset cycle before applying the pulse
                //Sleep(sleep_time);
                DC_sweep(topChannel, bottomChannel, 0, V_reset, 3, meas_time, compliance_set);
                //Sleep(sleep_time);
                DC_sweep(topChannel, bottomChannel, 0, V_reset, 3, meas_time, compliance_reset);
                //Sleep(sleep_time);
                printf("Now applying a writing pulse: %f V duration %e s \n", list_pulse_amp[j], list_time[i]);
                write_resistance(list_pulse_amp[j], list_time[i], topChannel, bottomChannel);
                Sleep(1);
                read_resistance(Vr, t_read, topChannel, bottomChannel, list_pulse_amp[j], list_time[i], file_name);
                Sleep(1);
            }
        }
    }
}

void DC_sweep(int topChannel, int bottomChannel, double V_min,double V_max, int single, double meas_time, double compliance) { //Need to implement compliance
    //If single=1 doing a single if single=3 doing a double (from V_min to V_max back to V_min)
    if (single == 1 || single == 3) {
        ViSession defaultRM;
        ViSession vi;
        viOpenDefaultRM(&defaultRM);
        viOpen(defaultRM, "GPIB0::17::INSTR", VI_NULL, VI_NULL, &vi); //Connect to B1500
        viPrintf(vi, "CN 2\n"); //Enable SMU of channel 201
        viPrintf(vi, "CN 3\n"); //Enable SMU of channel 202
        //viPrintf(vi, "MM 16,201,202\n"); //Mulit-channel sweep
        viPrintf(vi, "RI 2,-17\n");//Compliance current of 1mA
        viPrintf(vi, "DV 3,0,0,0.001\n");//Force 0V with compliance 1mA
        viPrintf(vi, "MM 2,2\n"); //Operation mode to staircase sweep measurements
        //viPrintf(vi, "RI 2,-17\n");//Compliance current of 1mA
        viPrintf(vi, "WM 2,1\n");
        static char staircase[100];
        sprintf(staircase, "WV 2,%d,0,%lf,%lf,%d,%lf\n", single, V_min, V_max, nb_points, compliance);//Casting the string for the staircase
        ViConstString Vistaircase = staircase;
        viPrintf(vi, Vistaircase); //Staircase measurement
        Sleep(sleep_time);
        viPrintf(vi, "CL 2\n"); //Disable SMU of channel 201
        viPrintf(vi, "CL 3\n"); //Disable SMU of channel 202
        viClose(vi);
        viClose(defaultRM);
    }
    else {
        printf("out");
        exit(0);
    }
}

const char* get_timestamp(int choice) {
    time_t rawtime;
    struct tm* timeinfo;
    static char buffer[16];
    static char file_name[100];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strcpy(file_name, folder_path);
    strftime(buffer, 16, "%G%m%d%H%M", timeinfo);
    strcat(file_name, buffer);
    if (choice == 1) {
        strcat(file_name, "_simple_convergence.txt");
    }
    else if (choice == 2) {
        strcat(file_name, "_logarithmic_convergence.txt");
    }
    else if (choice == 3) {
        strcat(file_name, "_adapt_logarithmic_convergence.txt");
    }
    else if (choice == 4) {
        strcat(file_name, "_Gvt_pulsing.txt");
    }
    else {
        strcat(file_name, "measurement.txt");
    }
    printf("The measurement will be saved as: ");
    puts(file_name);
    return file_name;
}
