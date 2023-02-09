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
#include "visa.h"
#include <math.h>
#include <time.h>
#include <direct.h>
int current_measure_range;

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
    printf("Which measurement? (simple = 1, Gvt = 4, data driven = 5, retention = 12, PRT = 13) \n");
    scanf("%d", &choice);
    if (choice == 1) {
        printf("Now executing simple multilevel programming of %d resistance states \n", nb_state);
        const char* timestamp = get_timestamp(choice, folder_path);
        int advanced;
        printf("Advance multilevel programming? (0 for yes): ");
        scanf("%lf", &advanced);
        double step_size;
        printf("Enter step for pulse amplitude: ");
        scanf("%lf", &step_size);
        double V_LTP;
        printf("Enter V_LTP: ");
        scanf("%lf", &V_LTP);
        double V_LTD;
        printf("Enter V_LTD: ");
        scanf("%lf", &V_LTD);
        programming_noise(LRS, HRS, V_LTD, V_LTP, step_size, advanced, timestamp);
    }
    else if (choice == 2) {
        printf("Now executing convergence to target \n", nb_state);
        const char* timestamp = get_timestamp(choice, folder_path);
        double R_target;
        printf("Enter R_target: ");
        scanf("%lf", &R_target);
        double V_LTP;
        printf("Enter V_LTP: ");
        scanf("%lf", &V_LTP);
        double V_LTD;
        printf("Enter V_LTD: ");
        scanf("%lf", &V_LTD);
        converge_to_target(R_target, tolerance, V_LTD, V_LTP, 0.03, timestamp, 20, 1);
    }
    else if (choice == 3) {
        printf("Now executing adaptative logarithmic multilevel programming of %d resistance states \n", nb_state);
        const char* timestamp = get_timestamp(choice, folder_path);
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
        int pulse;
        printf("Is varying pulse number: (0 for yes)\n");
        scanf("%d", &pulse);
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
        double V_LTP;
        printf("Enter V_LTP: ");
        scanf("%lf", &V_LTP);
        double V_LTD;
        printf("Enter V_LTD: ");
        scanf("%lf", &V_LTD);
        if (pulse == 0) {
            int nb_pulse_max;
            printf("Enter max pulse:");
            scanf("%d", &nb_pulse_max);
            const char* folder = get_timestamp(97, folder_path);
            const char* timestamp = get_timestamp(96, folder);
            for (int k = 0; k < repeat_Gvt; k++) {
                char ID[18]="";
                sprintf(ID, "_%d.txt", k);
                char file_name[200] = "";
                strcat(file_name, timestamp);
                strcat(file_name, ID);
                puts(file_name);
                printf("\nFOLDER NAME\n");
                puts(timestamp);
                Gvt_pulse_2(nb_pulse_max, list_pulse_amp, V_LTD, V_LTP, file_name);
            }
        }
        else if (pulse == 1) {
            const char* timestamp = get_timestamp(choice, folder_path);
            Gvt(list_time, list_pulse_amp, V_LTD, V_LTP, timestamp);
        }
       
    }
    else if (choice == 5) {
        printf("Now executing measurement for data driven model fitting \n", nb_state);
        double V_pulse_min;
        printf("Enter V_min: ");
        scanf("%lf", &V_pulse_min);
        double V_pulse_max;
        printf("Enter V_max: ");
        scanf("%lf", &V_pulse_max);
        double width_pulse;
        printf("Enter pulse width (scientific notation): ");
        scanf("%lf", &width_pulse);
        int pulse_train;
        printf("Enter desired number of pulse: ");
        scanf("%d", &pulse_train);
        for (int i = 0; i < repeat_data_driven; i++) {
            const char* timestamp = get_timestamp(choice, folder_path);
            converge_to_target(6000, tolerance, -1.0, 1.0, 0.05, timestamp, 1, 1);
            data_driven_fitting(V_pulse_min, V_pulse_max, t_pulse, pulse_train, timestamp);
        }
    }
    else if (choice == 6) {
        printf("Read variability with SMUs\n");
        DC_sweep(topChannelSMU, bottomChannelSMU);
    }

    else if (choice == 8) {
    printf("Now executing write variability measurements\n");
    double V_write_pos;
    printf("Enter V_write_pos: ");
    scanf("%lf", &V_write_pos);
    double V_write_neg;
    printf("Enter V_write_neg: ");
    scanf("%lf", &V_write_neg);
    double R_min;
    printf("Enter R_min: ");
    scanf("%lf", &R_min);
    double R_max;
    printf("Enter R_max: ");
    scanf("%lf", &R_max);
    int nb_meas;
    printf("Enter number of measurements: ");
    scanf("%d", &nb_meas);
    double resistance = R_min;
    for (int i = 0; i < nb_meas; i++) {
        const char* timestamp = get_timestamp((int)resistance, folder_path);
        write_variability_pulse_number(R_max, R_min, V_write_neg, V_write_pos, timestamp, nb_meas);
    }
    }

    else if (choice == 9) {
    printf("Now executing DC source sweep\n");
    double V_target_min;
    printf("Enter Min voltage for sweep: ");
    scanf("%lf", &V_target_min);
    double V_target_max;
    printf("Enter Max voltage for sweep: ");
    scanf("%lf", &V_target_max);
    double resolution;
    printf("Enter target voltage resolution in V: ");
    scanf("%lf", &resolution);
    const char* timestamp = get_timestamp(choice, folder_path);
    DC_source_sweep(V_target_min, V_target_max, resolution, timestamp); //change timestamp for measurement
    }

    else if (choice == 10) {
    printf("Now executing Voltage prog\n");
    double V_target;
    printf("Enter voltage target in V: ");
    scanf("%lf", &V_target);
    const char* timestamp = get_timestamp(choice, folder_path);
    DC_voltage_prog(V_target, timestamp); //change timestamp for measurement
    }
    
    else if (choice == 11) {
        double V_pulse_pos;
        printf("Enter Vp: ");
        scanf("%lf", &V_pulse_pos);
        double V_pulse_neg;
        printf("Enter Vn: ");
        scanf("%lf", &V_pulse_neg);
        double width_pulse = 2e-7;
        int pulse_train = 1;
        int nit;
        double pulse_trains[1] = { 1 }; //Keep to one depending 
        printf("Enter number of iteration: ");
        scanf("%d", &nit);
        for (int i = 0; i < 2; i++) {
            const char* timestamp = get_timestamp(choice, folder_path);
            //V_pulse_neg = pulse_matching(V_pulse_pos,width_pulse,HRS,0.01); //If you want to match the pulses so that stability stays longer
            pulse_var_studies(V_pulse_pos, V_pulse_neg, HRS, width_pulse, pulse_train, pulse_train, 1e6, timestamp);
        }
}
    else if (choice == 12) {
        printf("Now executing retention tests \n");
        const char* timestamp = get_timestamp(choice, folder_path);
        double V_LTP;
        printf("Enter V_LTP: ");
        scanf("%lf", &V_LTP);
        double V_LTD;
        printf("Enter V_LTD: ");
        scanf("%lf", &V_LTD);
        double R_target;
        printf("Enter Target resistance: ");
        scanf("%lf", &R_target);
        double total_time;
        printf("Enter Total time(h): ");
        scanf("%lf", &total_time);
        double sampling_time;
        printf("Enter sampling time(s): ");
        scanf("%lf", &sampling_time);
        if (sampling_time > total_time*3600) {
            sampling_time = 10;
        }
        retention(total_time*3600, sampling_time, R_target, V_LTD, V_LTP, timestamp);
    }

    else if (choice == 13) {
        printf("Now executing Pulsed Transient Response (PRT)\n");
        double R_target;
        printf("Enter starting resistance: ");
        scanf("%lf", &R_target);
        double Vp;
        printf("Enter Vp: ");
        scanf("%lf", &Vp);
        double Vn;
        printf("Enter Vn: ");
        scanf("%lf", &Vn);
        double nb_pulse;
        printf("Enter nb_pulse: ");
        scanf("%lf", &nb_pulse);
        const char* folder = get_timestamp(98, folder_path);
        const char* timestamp = get_timestamp(94, folder);
        for (int k = 0; k < repeat_PRT; k++) {
            char ID[18] = "";
            sprintf(ID, "_%d.txt", k);
            char file_name[200] = "";
            strcat(file_name, timestamp);
            strcat(file_name, ID);
            puts(file_name);
            printf("\nFOLDER NAME\n");
            puts(timestamp);
            converge_to_target(R_target, tolerance, -1.2, 1.0, 0.025, file_name, 5, 1);
            PRT(Vp, Vn, nb_pulse, file_name);;
        }
    }
    else if (choice == 14) {
        printf("Now executing read variability tests \n");
        const char* timestamp = get_timestamp(choice, folder_path);
        double V_LTP;
        printf("Enter V_LTP: ");
        scanf("%lf", &V_LTP);
        double V_LTD;
        printf("Enter V_LTD: ");
        scanf("%lf", &V_LTD);
        double R_target;
        printf("Enter Target resistance: ");
        scanf("%lf", &R_target);
        double total_time;
        printf("Enter Total time(s): ");
        scanf("%lf", &total_time);
        double sampling_time;
        printf("Enter sampling time (scientific notation): ");
        scanf("%lf", &sampling_time);
        if (sampling_time > total_time) {
            sampling_time = 1e-6;
        }
        read_variability(total_time, sampling_time, R_target, V_LTD, V_LTP, timestamp);
}
    else if (choice == 15) {
        const char* timestamp = get_timestamp(15, folder_path);
        double V_p;
        printf("Enter V_p: ");
        scanf("%lf", &V_p);
        double V_n;
        printf("Enter V_n: ");
        scanf("%lf", &V_n);
        double R_target;
        printf("Enter Target resistance: ");
        scanf("%lf", &R_target);
        double wait_time;
        printf("Enter wait time (scientific notation): ");
        scanf("%lf", &wait_time);
        PVS(V_p, V_n, R_target, t_pulse, wait_time, 10, timestamp);
}
    else if (choice == 16) {
        const char* timestamp = get_timestamp(1, folder_path);
        WGFMU_openSession("GPIB0::17::INSTR"); //18
        WGFMU_initialize();
        WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
        WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
        WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
        WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
        WGFMU_connect(topChannel);
        WGFMU_connect(bottomChannel);
        double pulse_amp = 0;
        double R_c = apply_pulse_new(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, timestamp, 1, 0);
        printf("Extracted resistance is %f \n", R_c);
        WGFMU_initialize();
        WGFMU_closeSession();
}
    else if (choice == 17) {
    const char* timestamp = get_timestamp(17, folder_path);
        FILE* fa;
        char buffer[64];
        fa = fopen(path_list_res, "r");
        while (fgets(buffer, 64, fa)) {
            double R_target = atof(buffer);
            converge_to_target(R_target, 0.01, -1.0, 1.0, 0.02, timestamp, 5, 0);
        }

        fclose(fa);
}
    else if (choice == 20) {
    printf("Now executing pulsed forming\n");
    const char* timestamp = get_timestamp(20, folder_path);
    forming(R_form, V_form, step_form, timestamp, 10, 0);
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
    WGFMU_setMeasureEvent("meas", "Imeas", 0.00000001, 1, tpulse-0.00000002, tpulse-0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED); // meas from 10 ns, 1 points, 0.01 ms interval, no averaging //10
    WGFMU_addSequence(bottomChannel, "meas", 1); //1 "pulse" output

    // ONLINE
    WGFMU_openSession("GPIB0::17::INSTR"); //18
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_1MA);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    double res = extract_results(topChannel, bottomChannel, 0, pulse_amp, pulse_width, file_name);
    WGFMU_initialize();
    WGFMU_closeSession();
    return res;
}

void write_resistance(double Vpulse, double tpulse, int topChannel, int bottomChannel) // Square pulse voltage output
{

    // OFFLINE
    //Top electrode

    WGFMU_clear();
    WGFMU_createPattern("pulse", 0); // 0 ms, 0 V
    WGFMU_addVector("pulse", 0.00000001, Vpulse); //10 ns rise time
    WGFMU_addVector("pulse", tpulse - 0.00000002, Vpulse);
    WGFMU_addVector("pulse", 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_addSequence(topChannel, "pulse", 1); //1 pulse output

    //Bottom electrode
    WGFMU_createPattern("ground", 0);
    WGFMU_addVector("ground", 0.00000001, 0);
    WGFMU_addVector("ground", tpulse - 0.00000002, 0);
    WGFMU_addVector("ground", 0.00000001, 0);
    WGFMU_addSequence(bottomChannel, "ground", 1); //1 "pulse" output


    // ONLINE
    WGFMU_openSession("GPIB0::17::INSTR");
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    WGFMU_initialize();
    WGFMU_closeSession();
}

double apply_pulse_new(double Vpulse, double tpulse, int topChannel, int bottomChannel, double pulse_amp, double pulse_width, const char* file_name, int save, int measure) // Pulse voltage output
{

    WGFMU_clear(); // 9
    WGFMU_createPattern("pulse", 0); // 0 ms, 0 V
    WGFMU_addVector("pulse", 0.00000001, Vpulse); //10 ns rise time, 1 V
    WGFMU_addVector("pulse", tpulse - 0.00000002, Vpulse);
    WGFMU_addVector("pulse", 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_addSequence(topChannel, "pulse", 1); //1 pulse output

    //Bottom electrode
    WGFMU_createPattern("meas", 0); // 0 ms, 0 V
    WGFMU_addVector("meas", 0.00000001, 0); //10 ns rise time, 1 V
    WGFMU_addVector("meas", tpulse - 0.00000002, 0);
    WGFMU_addVector("meas", 0.00000001, 0); //10 ns fall time, 0 V
    if (measure == 0) {
        WGFMU_setMeasureEvent("meas", "Imeas", 0.00000001, 1, tpulse - 0.00000002, tpulse - 0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED); // meas from 10 ns, 1 points, 0.01 ms interval, no averaging //10
    }
  
    WGFMU_addSequence(bottomChannel, "meas", 1); //1 "pulse" output

    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    if (measure == 0) {
        int measuredSize, totalSize;
        WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
        double time, value, voltage;
        WGFMU_getMeasureValue(bottomChannel, measuredSize - 1, &time, &value); //MAYBE MEASURED_SIZE - 1
        WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
        double res;
        if (value < 0) {
            res = -voltage / value;
        }
        else {
            res = voltage / value;
        }
        if (save == 0) {
            FILE* fp = fopen(file_name, "a");
            if (fp == NULL) {
                printf("Error opening file\n");
                FILE* fp = fopen(file_name, "a");
                exit(1);
            }
            if (fp != 0) {
                if (pulse_width == 0) {
                    fprintf(fp, "%f;%f \n", pulse_amp, res);
                }
                else {
                    fprintf(fp, "%f;%e;%f \n", pulse_amp, pulse_width, res);
                }
                printf("Extracted resistance is %f \n", res);
                fclose(fp);
            }
        }
        return res;
    }
}

double extract_results(int channelId1, int channelId2, int offset, double pulse_amp, double pulse_width, const char* fileName) //extract only a single value
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

void programming_noise(double LRS, double HRS, double V_LTD, double V_LTP, double step_size, int advanced, const char* file_name)
{
    for (int i = 0; i < 10; i++) {
        simple_convergence_fast(LRS, HRS, V_LTD, V_LTP, step_size, advanced, file_name);
        simple_convergence_reverse(LRS, HRS, V_LTD, V_LTP, step_size, advanced, file_name);

    }

}

void simple_convergence_reverse(double LRS, double HRS, double V_LTD, double V_LTP, double step_size, int advanced, const char* file_name)
{
    double list_resist[8] = { 3500, 4400, 6100, 7800, 9700, 11500, 13200, 15000 };
    /*for (int i = 0;i < nb_state;i++) {
        list_resist[i] = LRS + i * (HRS - LRS) / (nb_state - 1);
    }*/
    //Multilevel programming for all resistance states
    for (int i_state = nb_state-1; i_state > -1; i_state--) {
        double R_target = list_resist[i_state];
        printf("Target resist is %f \n", R_target);
        double Rmax = R_target * (1 + tolerance);
        double Rmin = R_target * (1 - tolerance);
        converge_to_target(R_target, tolerance, V_LTD, V_LTP, step_size, file_name, stop, 0);
        /*if (advanced == 0) {
            char ID[50] = "";
            sprintf(ID, "_overlap_%.2lf.txt", R_target);
            char file_name_gaussian[200] = "";
            strcat(file_name_gaussian, file_name);
            int len = strlen(file_name_gaussian);
            file_name_gaussian[len - 4] = '\0';
            strcat(file_name_gaussian, ID);
            puts(file_name);
            printf("\nOverlap\n");
            puts(file_name_gaussian);
            char top_name[100];
            char bottom_name[100];
            char meas_event[100];
            sprintf(top_name, "pulse_%d", i_state);
            sprintf(bottom_name, "ground_%d", i_state);
            sprintf(meas_event, "Imeas_%d", i_state);
            WGFMU_createPattern(top_name, 0);
            WGFMU_createPattern(bottom_name, 0);
            //Apply reading pulse
            WGFMU_addVector(top_name, 0.0000001, 0);
            WGFMU_addVector(top_name, 0.00000001, Vr);
            WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
            WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
            WGFMU_addVector(top_name, 0.000001, 0); //10 ns fall time, 0 V

            WGFMU_addVector(bottom_name, 0.0000001, 0);
            WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time
            WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
            WGFMU_addVector(bottom_name, 0.00000001, 0);
            WGFMU_addVector(bottom_name, 0.000001, 0);

            WGFMU_setMeasureEvent(bottom_name, meas_event, 0.00000011, 1, t_read - 0.00000002, t_read - 0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED);

            //Check if measureEvent good
            WGFMU_addSequence(topChannel, top_name, 1000000);//to be tested not sure can handle 1M if not just loop
            WGFMU_addSequence(bottomChannel, bottom_name, 1000000);
            WGFMU_openSession("GPIB0::17::INSTR");
            WGFMU_initialize();
            WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
            WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
            WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
            WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
            WGFMU_connect(topChannel);
            WGFMU_connect(bottomChannel);
            WGFMU_execute();
            WGFMU_waitUntilCompleted();
            FILE* fa = fopen(file_name_gaussian, "a");
            if (fa != 0) {
                fprintf(fa, "R_target; number of points\n");
                fprintf(fa, "%.9lf; %d\n", R_target, 1000000);
                fprintf(fa, "time; voltage; current; resistance\n");
                int measuredSize, totalSize;
                WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
                for (int i = 0; i < measuredSize; i++) {
                    double time, value, voltage;
                    WGFMU_getMeasureValue(bottomChannel, i, &time, &value);
                    WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
                    fprintf(fa, "%.9lf; %.9lf; %.9lf; %.9lf\n", time, -voltage, value, -voltage / value);
                }
                fclose(fa);
            }
            WGFMU_initialize();
            WGFMU_closeSession();
        }*/
    }
    printf("End of simple multilevel programming\n");
}

void simple_convergence_fast(double LRS, double HRS, double V_LTD, double V_LTP, double step_size, int advanced, const char* file_name)
{
    double list_resist[8] = { 3500, 4400, 6100, 7800, 9700, 11500, 13200, 15000 };
    /*for (int i = 0;i < nb_state;i++) {
        list_resist[i] = LRS + i * (HRS - LRS) / (nb_state - 1);
    }*/
    //Multilevel programming for all resistance states
    for (int i_state = 0; i_state < nb_state; i_state++) {
        double R_target = list_resist[i_state];
        printf("Target resist is %f \n", R_target);
        double Rmax = R_target * (1 + tolerance);
        double Rmin = R_target * (1 - tolerance);
        converge_to_target(R_target, tolerance, V_LTD, V_LTP, step_size, file_name, stop, 0);
        /*if (advanced == 0) {
            char ID[50] = "";
            sprintf(ID, "_overlap_%.2lf.txt", R_target);
            char file_name_gaussian[200] = "";
            strcat(file_name_gaussian, file_name);
            int len = strlen(file_name_gaussian);
            file_name_gaussian[len - 4] = '\0';
            strcat(file_name_gaussian, ID);
            puts(file_name);
            printf("\nOverlap\n");
            puts(file_name_gaussian);
            char top_name[100];
            char bottom_name[100];
            char meas_event[100];
            sprintf(top_name, "pulse_%d", i_state);
            sprintf(bottom_name, "ground_%d", i_state);
            sprintf(meas_event, "Imeas_%d", i_state);
            WGFMU_createPattern(top_name, 0);
            WGFMU_createPattern(bottom_name, 0);
            //Apply reading pulse
            WGFMU_addVector(top_name, 0.0000001, 0);
            WGFMU_addVector(top_name, 0.00000001, Vr);
            WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
            WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
            WGFMU_addVector(top_name, 0.000001, 0); //10 ns fall time, 0 V

            WGFMU_addVector(bottom_name, 0.0000001, 0);
            WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time
            WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
            WGFMU_addVector(bottom_name, 0.00000001, 0);
            WGFMU_addVector(bottom_name, 0.000001, 0);

            WGFMU_setMeasureEvent(bottom_name, meas_event, 0.00000011, 1, t_read - 0.00000002, t_read - 0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED);

            //Check if measureEvent good
            WGFMU_addSequence(topChannel, top_name, 1000000);//to be tested not sure can handle 1M if not just loop
            WGFMU_addSequence(bottomChannel, bottom_name, 1000000);
            WGFMU_openSession("GPIB0::17::INSTR");
            WGFMU_initialize();
            WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
            WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
            WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
            WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
            WGFMU_connect(topChannel);
            WGFMU_connect(bottomChannel);
            WGFMU_execute();
            WGFMU_waitUntilCompleted();
            FILE* fa = fopen(file_name_gaussian, "a");
            if (fa != 0) {
                fprintf(fa, "R_target; number of points\n");
                fprintf(fa, "%.9lf; %d\n", R_target, 1000000);
                fprintf(fa, "time; voltage; current; resistance\n");
                int measuredSize, totalSize;
                WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
                for (int i = 0; i < measuredSize; i++) {
                    double time, value, voltage;
                    WGFMU_getMeasureValue(bottomChannel, i, &time, &value);
                    WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
                    fprintf(fa, "%.9lf; %.9lf; %.9lf; %.9lf\n", time, -voltage, value, -voltage / value);
                }
                fclose(fa);
            }
            WGFMU_initialize();
            WGFMU_closeSession();
        }*/
    }
    printf("End of simple multilevel programming\n");
}

void max_convergence(double LRS, double HRS, double V_LTD, double V_LTP, double step_size, int advanced, const char* file_name)
{
    int nb_state_max = 0;
    double overlap = 0;
    double R_target = LRS;
    //Multilevel programming for all resistance states
    while(R_target<=HRS*1.05){
        set_current_range(R_target); //Set the ideal current range for the measurement
        printf("Target resist is %f \n", R_target);
        double Rmax = R_target * (1 + tolerance);
        double Rmin = R_target * (1 - tolerance);
        converge_to_target(R_target, tolerance, V_LTD, V_LTP, step_size, file_name, stop, 0);
        nb_state_max++;
        char ID[50] = "";
        sprintf(ID, "_overlap_%.2lf.txt", R_target);
        char file_name_gaussian[200] = "";
        strcat(file_name_gaussian, file_name);
        int len = strlen(file_name_gaussian);
        file_name_gaussian[len - 4] = '\0';
        strcat(file_name_gaussian, ID);
        puts(file_name);
        printf("\nOverlap\n");
        puts(file_name_gaussian);
        char top_name[100];
        char bottom_name[100];
        char meas_event[100];
        sprintf(top_name, "pulse_%d", nb_state_max);
        sprintf(bottom_name, "ground_%d", nb_state_max);
        sprintf(meas_event, "Imeas_%d", nb_state_max);
        WGFMU_createPattern(top_name, 0);
        WGFMU_createPattern(bottom_name, 0);
        double read_pulse = set_read_time(R_target);
        double settling_time = set_settling_time(R_target);
        WGFMU_addVector(top_name, 0.0000001, 0);
        WGFMU_addVector(top_name, 0.00000001, Vr);
        WGFMU_addVector(top_name, settling_time + read_pulse - 0.00000002, Vr);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
        WGFMU_addVector(top_name, 0.000001, 0); //10 ns fall time, 0 V

        WGFMU_addVector(bottom_name, 0.0000001, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time
        WGFMU_addVector(bottom_name, settling_time + read_pulse - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, 0.000001, 0);

        WGFMU_setMeasureEvent(bottom_name, meas_event, settling_time + 0.00000011, 1, read_pulse - 0.00000002, read_pulse - 0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED);

        //Check if measureEvent good
        WGFMU_addSequence(topChannel, top_name, 1000000);
        WGFMU_addSequence(bottomChannel, bottom_name, 1000000);
        WGFMU_openSession("GPIB0::17::INSTR");
        WGFMU_initialize();
        WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
        WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
        WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
        WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
        WGFMU_connect(topChannel);
        WGFMU_connect(bottomChannel);
        WGFMU_execute();
        WGFMU_waitUntilCompleted();
        FILE* fa = fopen(file_name_gaussian, "a");
        if (fa != 0) {
            fprintf(fa, "R_target; number of points\n");
            fprintf(fa, "%.9lf; %d\n", R_target, 1000000);
            fprintf(fa, "time; voltage; current; resistance\n");
            int measuredSize, totalSize;
            WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
            overlap = 0;
            double mean_target = 0;
            for (int i = 0; i < measuredSize; i++) {
                double time, value, voltage;
                WGFMU_getMeasureValue(bottomChannel, i, &time, &value);
                WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
                fprintf(fa, "%.9lf; %.9lf; %.9lf; %.9lf\n", time, -voltage, value, -voltage / value);
                mean_target += -voltage / value;
                if (-voltage / value > overlap) {
                    overlap = -voltage / value;
                }
            }
            fclose(fa);
            R_target = (mean_target/1e6) + overlap;
        }
        WGFMU_initialize();
        WGFMU_closeSession();
    }
    printf("End of simple multilevel programming");
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

void converge_to_target(double R_target, double tol, double V_LTD, double V_LTP, double step_size, const char* file_name, int stop_read, int save)
{
    WGFMU_openSession("GPIB0::17::INSTR"); //18
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    printf("Target resist is %f \n", R_target);
    double Rmax = R_target * (1 + tol);
    double Rmin = R_target * (1 - tol);
    if (tol>1)
    {
        Rmax = R_target + tol;
        Rmin = R_target - tol;
    }
    printf("tolerance max %lf \n", Rmax);
    double Vp = V_LTP;
    double Vn = V_LTD;
    int c = 0;
    int pulse_number = 0;
    double pulse_amp = 0;
    double R_c = apply_pulse_new(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name, 1, 0);
    while (c < stop_read) {
        if (Rmin <= R_c && R_c <= Rmax) {
            Vp = V_LTP;
            Vn = V_LTD;
            printf("Reached target resistance \n");
            pulse_amp = 0;
            c++;
        }
        else if (R_c > Rmax) {
            apply_pulse_new(Vp, t_pulse, topChannel, bottomChannel, pulse_amp, 0, file_name, 1, 1);
            if (Vp < V_max) {
                Vp = Vp + step_size;
            }
            Vn = V_LTD;
            pulse_amp = Vp;
            printf("Apply positive pulse %f \n", Vp);
        }
        else if (R_c < Rmin) {
            apply_pulse_new(Vn, t_pulse, topChannel, bottomChannel, pulse_amp, 0, file_name, 1, 1);
            if (Vn > -V_max) {
                Vn = Vn - step_size;
            }
            Vp = V_LTP;
            pulse_amp = Vn;
            printf("Apply negative pulse %f \n", Vn);
        }
        pulse_number++;
        R_c = apply_pulse_new(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name, save, 0);
        printf("Measured resistance is %f \n", R_c);
    }
    WGFMU_initialize();
    WGFMU_closeSession();
}


void forming(double R_target, double V_form, double step_size, const char* file_name, int stop_read, int save)
{
    WGFMU_openSession("GPIB0::17::INSTR"); //18
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    //NEED TO SELECT RANGE FOR ELECTROFORMING
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_1UA);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    printf("Target resist is %f \n", R_target);
    double Vp = V_form;
    double crange = 1e-6;
    int c = 0;
    int pulse_number = 0;
    double pulse_amp = 0;
    double R_c = apply_pulse_new(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name, 1, 0);
    while (c < stop_read) {
        if (R_c <= R_target) {
            Vp = V_form;
            printf("Reached target resistance \n");
            pulse_amp = 0;
            c++;
        }
        else if (R_c > R_target) {
            apply_pulse_new(Vp, t_pulse, topChannel, bottomChannel, pulse_amp, 0, file_name, 1, 1);
            if (Vp < V_form_max) {
                Vp = Vp + step_size;
            }
            pulse_amp = Vp;
            printf("Apply forming pulse %f \n", Vp);
        }
        pulse_number++;
        R_c = apply_pulse_new(Vr, t_read, topChannel, bottomChannel, pulse_amp, 0, file_name, save, 0);
        printf("Measured resistance is %f \n", R_c);
        printf("Current range is %f \n", crange);
        if (R_c < 1800 && crange != 1e-3) {
            WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_1MA);
            crange = 1e-3;
        }
        else if (R_c < 22000 && R_c > 1800 && crange != 1e-4) {
            WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
            crange = 1e-4;
        }
        else if (R_c < 210000 && R_c > 22000 && crange != 1e-5) {
            WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_10UA);
            crange = 1e-5;
        }
        else if (R_c > 210000 && crange != 1e-6) {
            WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_1UA);
            crange = 1e-6;
        }
    }
    WGFMU_initialize();
    WGFMU_closeSession();
}

void log_convergence(double LRS, double HRS, double V_LTD, double V_LTP, const char* file_name)//Logarithmic multilevel programming
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

void Gvt(double list_time[], double list_pulse_amp[], double V_LTD, double V_LTP, const char* file_name) {//CHANGER WRITE RESISTANCE TO APPLY PULSE NEW AND ADD OPEN SESSION AND CLOSSE SESSION
    //int size_time = 18;
    int size_time = 11;
    FILE* fp = fopen(file_name, "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    if (fp != 0) {
        fprintf(fp, "V_min; V_max; t_min, t_max; nb_amps; nb_time\n");
        fprintf(fp, "%f;%f;%f;%f;%d;%d \n", list_pulse_amp[0], list_pulse_amp[pulse_number - 1], list_time[0], list_time[20], pulse_number, size_time);
        fprintf(fp, "pulse_amp; pulse_width; resistance\n");
        fclose(fp);
    }
    for (int i = 0; i < size_time; i++) {
        for (int j = 0; j < pulse_number; j++) {
            if (list_pulse_amp[j] < 0) {
                converge_to_target(LRS_Gvt, tolerance_write, V_LTD, V_LTP, 0.05, file_name, 1,1);
                printf("Now applying a writing pulse: %f V duration %e s \n", list_pulse_amp[j], list_time[i]);
                write_resistance(list_pulse_amp[j], list_time[i], topChannel, bottomChannel);
                //apply_pulse_new(list_pulse_amp[j], list_time[i], topChannel, bottomChannel, list_pulse_amp[i], 0, file_name, 1, 1);//write
                read_resistance(Vr, t_read, topChannel, bottomChannel, list_pulse_amp[j], list_time[i],file_name);
                //apply_pulse_new(Vr, t_read, topChannel, bottomChannel, list_pulse_amp[j], list_time[i], file_name, 0, 0);//read
                Sleep(1);
            }
            else {
                printf("Now applying a writing pulse: %f V duration %e s \n", list_pulse_amp[j], list_time[i]);
                converge_to_target(HRS_Gvt, tolerance_write, V_LTD, V_LTP, 0.05, file_name, 1, 1);
                write_resistance(list_pulse_amp[j], list_time[i], topChannel, bottomChannel);
                //apply_pulse_new(list_pulse_amp[j], list_time[i], topChannel, bottomChannel, list_pulse_amp[i], 0, file_name, 1, 1);//write
                read_resistance(Vr, t_read, topChannel, bottomChannel, list_pulse_amp[j], list_time[i], file_name);
                //apply_pulse_new(Vr, t_read, topChannel, bottomChannel, list_pulse_amp[j], list_time[i], file_name, 0, 0);//read
                Sleep(1);
            }
        }
    }
}

void Gvt_pulse_2(int nb_pulse_max, double list_pulse_amp[], double V_LTD, double V_LTP, const char* file_name) {
    FILE* fp = fopen(file_name, "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    if (fp != 0) {
        fprintf(fp, "V_min; V_max; nb_pulse_max; nb_pulse\n");
        fprintf(fp, "%f;%f;%d \n", list_pulse_amp[0], list_pulse_amp[pulse_number - 1], nb_pulse_max, pulse_number);
        fprintf(fp, "pulse_amp; resistance\n");
        fclose(fp);
    }
    for (int i = 0; i < pulse_number; i++) {
        printf("%lf\n", list_pulse_amp[i]);
    }
    if (list_pulse_amp[0] < 0) {
        converge_to_target(LRS_Gvt, tolerance, V_LTD, V_LTP, 0.05, file_name, 1, 1);
    }
    if (list_pulse_amp[0] > 0) {
        converge_to_target(HRS_Gvt, tolerance, V_LTD, V_LTP, 0.05, file_name, 1, 1);
    }
    for (int i = 0; i < pulse_number; i++) {
        printf("Executing pulse %lf\n", list_pulse_amp[i]);
        char top_name[100];
        char bottom_name[100];
        char meas_event[100];
        sprintf(top_name, "pulse_%d", i);
        sprintf(bottom_name, "ground_%d", i);
        sprintf(meas_event, "Imeas_%d", i);
        WGFMU_createPattern(top_name, 0);
        WGFMU_createPattern(bottom_name, 0);
        WGFMU_addVector(top_name, 0.00000001, list_pulse_amp[i]); //10 ns rise time
        WGFMU_addVector(top_name, t_pulse - 0.00000002, list_pulse_amp[i]);
        WGFMU_addVector(top_name, 0.00000001, 0);
        WGFMU_addVector(top_name, 0.0000001, 0);

        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, t_pulse - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, 0.0000001, 0);

        //Apply reading pulse
        WGFMU_addVector(top_name, 0.00000001, Vr);
        WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
        WGFMU_addVector(top_name, 0.00000001, 0);
        WGFMU_addVector(top_name, 0.0000001, 0);

        WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
        WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, 0.0000001, 0);
        WGFMU_setMeasureEvent(bottom_name, meas_event, t_pulse + 0.00000011, 1, t_read - 0.00000002, t_read - 0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED);
        WGFMU_addSequence(topChannel, top_name, nb_pulse_max);
        WGFMU_addSequence(bottomChannel, bottom_name, nb_pulse_max);
        WGFMU_openSession("GPIB0::17::INSTR"); //18
        WGFMU_initialize();
        WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
        WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
        WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_10UA);
        WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
        WGFMU_connect(topChannel);
        WGFMU_connect(bottomChannel);
        WGFMU_execute();
        WGFMU_waitUntilCompleted();
        FILE* fp = fopen(file_name, "a");
        if (fp != 0) {
            int measuredSize, totalSize;
            WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
            for (int j = 0; j < measuredSize; j++) {
                double time, value, voltage;
                WGFMU_getMeasureValue(bottomChannel, j, &time, &value);
                WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
                fprintf(fp, "%lf;%lf \n", list_pulse_amp[i], -voltage / value);
            }
            fclose(fp);
        }
        WGFMU_initialize();
        WGFMU_closeSession();
        if (list_pulse_amp[i] < 0) {
            converge_to_target(LRS_Gvt, tolerance, V_LTD, V_LTP, 0.05, file_name, 1, 1);
        }
        if (list_pulse_amp[i] > 0) {
            converge_to_target(HRS_Gvt, tolerance, V_LTD, V_LTP, 0.05, file_name, 1, 1);
        }
    }
}

void Gvt_pulse(int nb_pulse_max, double list_pulse_amp[], double V_LTD, double V_LTP, const char* file_name) {
    FILE* fp = fopen(file_name, "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    if (fp != 0) {
        fprintf(fp, "V_min; V_max; nb_pulse_max; nb_pulse\n");
        fprintf(fp, "%f;%f;%d \n", list_pulse_amp[0], list_pulse_amp[pulse_number - 1], nb_pulse_max, pulse_number);
        fprintf(fp, "pulse_amp; resistance\n");
        fclose(fp);
    }
    for (int i = 0; i < pulse_number; i++) {
        printf("%lf\n", list_pulse_amp[i]);
    }
    if (list_pulse_amp[0] < 0) {
        converge_to_target(LRS_Gvt, tolerance, V_LTD, V_LTP, 0.05, file_name, 1, 1);
    }
    if (list_pulse_amp[0] > 0) {
        converge_to_target(HRS_Gvt, tolerance, V_LTD, V_LTP, 0.05, file_name, 1, 1);
    }
    for (int i = 0; i < pulse_number; i++) {
        WGFMU_openSession("GPIB0::17::INSTR"); //18
        WGFMU_initialize();
        WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
        WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
        WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_1MA);
        WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
        WGFMU_connect(topChannel);
        WGFMU_connect(bottomChannel);
        for (int j = 0; j < nb_pulse_max; j++) {
            apply_pulse_new(Vr, t_read, topChannel, bottomChannel, list_pulse_amp[i], 0, file_name, 0, 0);//read
            apply_pulse_new(list_pulse_amp[i], t_pulse, topChannel, bottomChannel, list_pulse_amp[i], 0, file_name, 1, 1);//write
        }
        WGFMU_initialize();
        WGFMU_closeSession();
        if (list_pulse_amp[i] < 0){
            converge_to_target(LRS_Gvt, tolerance, V_LTD, V_LTP, 0.05, file_name, 1, 1);
        }
        if (list_pulse_amp[i] > 0) {
            converge_to_target(HRS_Gvt, tolerance, V_LTD, V_LTP, 0.05, file_name, 1, 1);
        }
    }
}

void retention(double total_time, double sampling_time, double R_target, double V_LTD, double V_LTP, const char* file_name) {
    int nb_pulses = total_time/2e-3;
    converge_to_target(R_target, tolerance, V_LTD, V_LTP, 0.05, file_name, 20, 1);
    FILE* fp = fopen(file_name, "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    if (fp != 0) {
        fprintf(fp, "R_target;total time(s); sampling time(s); total pulses\n");
        fprintf(fp, "%f;%f;%f;%d \n", R_target, total_time, sampling_time, nb_pulses);
        fprintf(fp, "time(s); resistance\n");
        fclose(fp);
    }
    for (int i = 0; i < nb_pulses+1; i++) {
        read_resistance(Vr, 1e-3, topChannel, bottomChannel, sampling_time*i,0, file_name);
        printf("Time remaining: %lfs\nApplied: %d pulses\n", (total_time - 1e-3 * (i + 1)), i+1);
        Sleep(1e-3);
    }
}

void read_variability(double total_time, double sampling_time, double R_target, double V_LTD, double V_LTP, const char* file_name) {
    int pulse_train = 1E6;
    double pulse_width = 1E-5;
    /*if (pulse_train > 1099511627775) {//Max pulses B1530A can handle
        pulse_train = 1099511627770;
        printf("Too much pulses (either too long measurement time or too small sampling time).\n Taking max possible pulses as default (1e12 pulses).\n");
    }
    if (pulse_width < 4e-8) {
        pulse_width = 4e-8;
        printf("Sampling time too small. Picking 8e-8 instead. \n");
    }*/
    converge_to_target(R_target, tolerance, V_LTD, V_LTP, 0.05, file_name, 20, 1);
    char top_name[100] = "top";
    char bottom_name[100] = "bot";
    char meas_event[100] = "meas";
    WGFMU_createPattern(top_name, 0);
    WGFMU_createPattern(bottom_name, 0);

    //Apply reading pulse
    WGFMU_addVector(top_name, 0.00000001, Vr);
    WGFMU_addVector(top_name, pulse_width - 0.00000002, Vr);
    WGFMU_addVector(top_name, 0.00000001, 0);
    WGFMU_addVector(top_name, pulse_width, 0);

    WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
    WGFMU_addVector(bottom_name, pulse_width - 0.00000002, 0);
    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_addVector(bottom_name, pulse_width, 0);
    WGFMU_setMeasureEvent(bottom_name, meas_event, 0.00000001, 1, pulse_width - 0.00000002, 1e-6, WGFMU_MEASURE_EVENT_DATA_AVERAGED);
    WGFMU_addSequence(topChannel, top_name, pulse_train);
    WGFMU_addSequence(bottomChannel, bottom_name, pulse_train);
    WGFMU_openSession("GPIB0::17::INSTR"); //18
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    FILE* fp = fopen(file_name, "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    if (fp != 0) {
        fprintf(fp, "R_target;total time(s); sampling time(s); pulse_width; total pulses\n");
        fprintf(fp, "%f;%f;%f;%f;%d \n", R_target, total_time, sampling_time, pulse_width, pulse_train);
        fprintf(fp, "time(s); resistance\n");
        int measuredSize, totalSize;
        WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
        printf("Measured size: %d", measuredSize);
        for (int j = 0; j < measuredSize; j++) {
            double time, value, voltage;
            WGFMU_getMeasureValue(bottomChannel, j, &time, &value);
            WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
            fprintf(fp, "%lf;%lf \n", time, -voltage / value);
        }
        fclose(fp);
    }
    WGFMU_initialize();
    WGFMU_closeSession();

}

void DC_sweep(int topChannel, int bottomChannel) { //Need to implement compliance
    //If single=1 doing a single if single=3 doing a double (from V_min to V_max back to V_min)
    time_t rawtime;
    struct tm* timeinfo;
    static char buffer[18];
    static char file_name[200];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strcpy(file_name, folder_path);
    strftime(buffer, 16, "%G%m%d%H%M%S", timeinfo);
    strcat(file_name, buffer);
    strcat(file_name, "_reaad_varia_SMU.txt");
    char str[128];
    ViSession defaultRM;
    ViSession vi;
    viOpenDefaultRM(&defaultRM);
    viOpen(defaultRM, "GPIB0::17::INSTR", VI_NULL, VI_NULL, &vi); //Connect to B1500 201 et 301 3IT 301 et 401 IQ
    viPrintf(vi, "CN 201\n"); //Enable SMU of channel 201
    viPrintf(vi, "CN 301\n"); //Enable SMU of channel 202
    //viPrintf(vi, "RI 2,-17\n");//Compliance current of 1mA
    viPrintf(vi, "DV 301,0,0.2,0.001\n");//Force 0V with compliance 1mA
    viPrintf(vi, "FMT 1, 1\n");
    viPrintf(vi, "MM 1,%d\n", 201);
    viPrintf(vi, "MM 1,%d\n", 301);
        
    for (int i = 0; i < 1e4; i++) {
        viPrintf(vi, "CMM %d,1\n", 301);
        viPrintf(vi, "XE\n");
        while (viScanf(vi, "%T", str) == VI_ERROR_TMO) { //wait for the response
        }
        //printf("%s\n", str);
        FILE* fp = fopen(file_name, "a");
        if (fp != 0) {
            fprintf(fp, "%s", str);
            }
            fclose(fp);
            if (i % 50 == 0) {
                printf("%d/10000\n", i);
        }
        }
    //viPrintf(vi, "WM 2,1\n");
    /*static char staircase[100];
    sprintf(staircase, "WV 201,%d,0,%lf,%lf,%d,%lf\n", single, V_min, V_max, nb_points, compliance);//Casting the string for the staircase
    ViConstString Vistaircase = staircase;
    viPrintf(vi, Vistaircase);
    //viPrintf(vi, "WV 201,3,0,0,-1.5,400,0.01\n"); //Staircase measurement*/
    Sleep(sleep_time);
    viPrintf(vi, "CL 201\n"); //Disable SMU of channel 201
    viPrintf(vi, "CL 301\n"); //Disable SMU of channel 202
    viClose(vi);
    viClose(defaultRM);
    
}

void write_variability_pulse_number(double resistance_up, double resistance_down, double V_write_neg, double V_write_pos, const char* file_name, int nb_meas) {
    //ADD INITIAL CONVERGENCE TO BOTTOM STATE
    converge_to_target(resistance_down, tolerance_write+0.01, V_write_neg, V_write_pos, step_varia, file_name, 5, 1);
    FILE* fp = fopen(file_name, "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    if (fp != 0) {
        fprintf(fp, "bottom resistance;top resistance;V_p;V_n, nb_meas \n");
        fprintf(fp, "%f;%f;%f;%f;%d \n", resistance_down, resistance_up, V_write_neg, V_write_pos, nb_meas);
        fclose(fp);
    }
    for (int i = 0; i < nb_meas; i++) {
        fp = fopen(file_name, "a");
        if (fp != 0) {
            fprintf(fp, "s%dn \n", i+1);
            printf("Now starting %d/%d loop (Increase) \n", i+1, nb_meas);
            fclose(fp);
        }
        converge_to_target(resistance_up, 75, V_write_neg, V_write_pos, 0, file_name, 1, 0);
        fp = fopen(file_name, "a");
        if (fp != 0) {
            fprintf(fp, "s%dp \n", i + 1);
            printf("Now starting %d/%d loop (Decrease) \n", i + 1, nb_meas);
            fclose(fp);
        }
        converge_to_target(resistance_down, 75, V_write_neg, V_write_pos, 0, file_name, 1, 0);
        Sleep(10);
    }
}

void WGFMU_DC_sweep(int topChannel, int bottomChannel, double V_min, double V_max, int single, double meas_time, double compliance) { //Need to implement compliance
    //If single=1 doing a single if single=3 doing a double (from V_min to V_max back to V_min)
    double step_length = meas_time / nb_points;
    double Voltage = V_min;
    //create pattern
    WGFMU_clear();
    WGFMU_createPattern("staircase", 0);
    WGFMU_createPattern("ground", 0);

    //Initial 0V
    WGFMU_addVector("staircase", 0.00000001, Voltage);
    WGFMU_addVector("staircase", step_length - 0.00000001, Voltage);

    WGFMU_addVector("ground", 0.00000001, 0);
    WGFMU_addVector("ground", step_length - 0.00000002, 0);

    if (single == 1) {
        for (int i = 0; i < nb_points; i++) {
            Voltage = V_min + i*(V_max-V_min)/(nb_points-1);
            WGFMU_addVector("pulse", 0.00000001, Voltage);
            WGFMU_addVector("pulse", step_length - 0.00000001, Voltage);

            WGFMU_addVector("ground", 0.00000001, 0);
            WGFMU_addVector("ground", step_length - 0.00000001, 0);
        }
    }
    if (single == 3) {
        double polarity = 1;
        for (int i = 0; i < nb_points; i++) {
            Voltage = V_min + polarity* i * (V_max - V_min) / (nb_points - 1);
            WGFMU_addVector("pulse", 0.00000001, Voltage);
            WGFMU_addVector("pulse", step_length - 0.00000001, Voltage);

            WGFMU_addVector("ground", 0.00000001, 0);
            WGFMU_addVector("ground", step_length - 0.00000001, 0);
            if (polarity == 1 && i == nb_points - 1) {
                polarity = -1;
                i = 0;
            }
        }
    }
    else {
        printf("out");
        exit(0);
    }
    WGFMU_addSequence(topChannel, "staircase", 1);
    WGFMU_addSequence(bottomChannel, "ground", 1);
    WGFMU_setMeasureCurrentRange(topChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
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

void data_driven_fitting(double V_pulse_min, double V_pulse_max, double t_pulse, int pulse_train, const char* file_name) {
    //create pattern
    WGFMU_clear();
    double polarity = 1;
    for (int i = 0; i < 2*pulse_number; i++) {
        char top_name[100];
        char bottom_name[100];
        char meas_event[100];
        sprintf(top_name, "pulse_%d", i);
        sprintf(bottom_name, "ground_%d", i);
        sprintf(meas_event, "Imeas_%d", i);
        WGFMU_createPattern(top_name, 0);
        WGFMU_createPattern(bottom_name, 0);
        if (i % 2 == 0) {
            polarity = 1;
        }
        else {
            polarity = -1;
        }
        int coeff = i / 2;
        double Vpulse = polarity * (V_pulse_min + coeff * ((V_pulse_max - V_pulse_min) / (pulse_number-1)));
        printf("%lf \n", Vpulse);
        //Apply writing pulse
        WGFMU_addVector(top_name, 0.00000001, Vpulse); //10 ns rise time
        WGFMU_addVector(top_name, t_pulse - 0.00000002, Vpulse);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V

        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, t_pulse - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);
            
        //Apply reading pulse
        WGFMU_addVector(top_name, 0.00000001, Vr);
        WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V

        WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
        WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_setMeasureEvent(bottom_name, meas_event, t_pulse + 0.00000001, 1, t_read - 0.00000002, t_read - 0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED);

        //Check if measureEvent good
        WGFMU_addSequence(topChannel, top_name, pulse_train);
        WGFMU_addSequence(bottomChannel, bottom_name, pulse_train);
    }

    WGFMU_openSession("GPIB0::17::INSTR"); 
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    FILE* fp = fopen(file_name, "w");
    if (fp != 0) {
        fprintf(fp, "V_min; V_max; number of amplitudes; number of pulses; pulse width\n");
        fprintf(fp, "%.9lf; %.9lf; %d; %d; %.9lf\n",V_pulse_min, V_pulse_max, pulse_number, pulse_train, t_pulse);
        fprintf(fp, "time; voltage; current; resistance\n");
        int measuredSize, totalSize;
        WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
        for (int i = 0; i < measuredSize; i++) {
            double time, value, voltage;
            WGFMU_getMeasureValue(bottomChannel, i, &time, &value);
            WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
            fprintf(fp, "%.9lf; %.9lf; %.9lf; %.9lf\n", time, -voltage, value, -voltage/value);
        }
        fclose(fp);
    }
    WGFMU_initialize();
    WGFMU_closeSession();
}

void PRT(double Vp, double Vn, double nb_pulse, const char* file_name) {
    //create pattern
    double Vpulse;
    WGFMU_clear();
    for (int i = 0; i < 2 * repeat_data_driven; i++) {
        char top_name[100];
        char bottom_name[100];
        char meas_event[100];
        sprintf(top_name, "pulse_%d", i);
        sprintf(bottom_name, "ground_%d", i);
        sprintf(meas_event, "Imeas_%d", i);
        WGFMU_createPattern(top_name, 0);
        WGFMU_createPattern(bottom_name, 0);
        int coeff = i % 2;
        if (coeff == 0) {
            Vpulse = Vp;
        }
        else {
            Vpulse = Vn;
        }
        printf("%lf \n", Vpulse);
        WGFMU_addVector(top_name, 0.00000001, Vpulse); //10 ns rise time
        WGFMU_addVector(top_name, t_pulse - 0.00000002, Vpulse);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
        WGFMU_addVector(top_name, 0.0000001, 0);//Adding 100ns buffer

        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, t_pulse - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, 0.0000001, 0);//Adding 100ns buffer

        //Apply reading pulse
        WGFMU_addVector(top_name, 0.00000001, Vr);
        WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
        WGFMU_addVector(top_name, 0.0000001, 0);//Adding 100ns buffer

        WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
        WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, 0.0000001, 0); //Adding 100ns buffer
        WGFMU_setMeasureEvent(bottom_name, meas_event, t_pulse + 0.00000011, 1, t_read - 0.00000002, t_read - 0.00000002, WGFMU_MEASURE_EVENT_DATA_AVERAGED);//Adding 100ns buffer

        //Check if measureEvent good
        WGFMU_addSequence(topChannel, top_name, nb_pulse);
        WGFMU_addSequence(bottomChannel, bottom_name, nb_pulse);
    }

    WGFMU_openSession("GPIB0::17::INSTR");
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    FILE* fp = fopen(file_name, "w");
    if (fp != 0) {
        fprintf(fp, "Vp; Vn; number of pulses; repeat; pulse width\n");
        fprintf(fp, "%.9lf; %.9lf; %d; %d; %.9lf\n", Vp, Vn, repeat_data_driven, nb_pulse, t_pulse);
        fprintf(fp, "time; voltage; current; resistance\n");
        int measuredSize, totalSize;
        WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
        for (int i = 0; i < measuredSize; i++) {
            double time, value, voltage;
            WGFMU_getMeasureValue(bottomChannel, i, &time, &value);
            WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
            fprintf(fp, "%.9lf; %.9lf; %.9lf; %.9lf\n", time, -voltage, value, -voltage / value);
        }
        fclose(fp);
    }
    WGFMU_initialize();
    WGFMU_closeSession();
}


void LTD_LTP_measurement(double V_pulse_min, double V_pulse_max, double t_pulse_min, double t_pulse_max, int pulse_train, const char* file_name) {
    //create pattern
    WGFMU_clear();
    double polarity = 1;
    for (int i = 0; i < 2 * pulse_number_LTD_LTP; i++) {
        char top_name[100];
        char bottom_name[100];
        char meas_event[100];
        sprintf(top_name, "pulse_%d", i);
        sprintf(bottom_name, "ground_%d", i);
        sprintf(meas_event, "Imeas_%d", i);
        WGFMU_createPattern(top_name, 0);
        WGFMU_createPattern(bottom_name, 0);
        if (i % 2 == 0) {
            polarity = 1;
        }
        else {
            polarity = -1;
        }
        int coeff = i / 2;
        double Vpulse = polarity * (V_pulse_min + coeff * ((V_pulse_max - V_pulse_min) / (pulse_number_LTD_LTP - 1)));
        printf("%lf \n", Vpulse);
        //Apply writing pulse
        WGFMU_addVector(top_name, 0.00000001, Vpulse); //10 ns rise time
        WGFMU_addVector(top_name, t_pulse - 0.00000002, Vpulse);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V

        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, t_pulse - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);

        //Apply reading pulse
        WGFMU_addVector(top_name, 0.00000001, Vr);
        WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V

        WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
        WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_setMeasureEvent(bottom_name, meas_event, t_pulse + 0.00000001, 1, t_read - 0.00000004, t_read - 0.00000004, WGFMU_MEASURE_EVENT_DATA_AVERAGED);

        //Check if measureEvent good
        WGFMU_addSequence(topChannel, top_name, pulse_train);
        WGFMU_addSequence(bottomChannel, bottom_name, pulse_train);
    }

    WGFMU_openSession("GPIB0::17::INSTR");
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    FILE* fp = fopen(file_name, "w");
    if (fp != 0) {
        fprintf(fp, "V_min; V_max; number of amplitudes; number of pulses; pulse width min, pulse width max, number of pulse width\n");
        fprintf(fp, "%.9lf; %.9lf; %d; %d; %.9lf; %.9lf; %d\n", V_pulse_min, V_pulse_max, pulse_number_LTD_LTP, pulse_train, t_pulse_min, t_pulse_max, t_pulse_number_LTD_LTP);
        fprintf(fp, "time; voltage; current; resistance\n");
        int measuredSize, totalSize;
        WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
        for (int i = 0; i < measuredSize; i++) {
            double time, value, voltage;
            WGFMU_getMeasureValue(bottomChannel, i, &time, &value);
            WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
            fprintf(fp, "%.9lf; %.9lf; %.9lf; %.9lf\n", time, -voltage, value, -voltage / value);
        }
        fclose(fp);
    }
    WGFMU_initialize();
    WGFMU_closeSession();
}

void DC_voltage_prog(double V_target, const char* file_name) {
    if (nb_memristor == 1) {
        double R_target = V_target / (V_in * R_L);
        converge_to_target(R_target, tolerance, V_p_min, V_n_min, step_DC, file_name, 1, 0);
        double R_meas = apply_pulse_new(Vr, t_read, topChannel, bottomChannel, 0.2, 0, file_name, 1, 0);
        double V_meas = R_meas * (V_in * R_L);
        double V_diff = (V_target-V_meas);
        printf("Measured output voltage: %.6lfV\nV_target was: %.6lfV\nVoltage difference: %.3lfuV", V_meas, V_target, V_diff* 1000000);
        FILE* fp = fopen(file_name, "w");
        if (fp != 0) {
            fprintf(fp, "%.9lf; %.9lf; %.9lf; %.9lf\n", V_target, V_meas, V_diff, R_meas);
            fclose(fp);
        }
    }
    else//first version all memristors same value second version should implement the methods for python simulator
    {
        double list_resist_meas[nb_memristor];
        double R_target = V_target / (V_in * R_L*nb_memristor);
        double G_tot = 0;
        double buffer;
        for (int i = 0; i < nb_memristor; i++) {
            converge_to_target(R_target, tolerance, V_p_min, V_n_min, step_DC, file_name, 1, 0);
            double R_meas = apply_pulse_new(Vr, t_read, topChannel, bottomChannel, 0.2, 0, file_name, 1, 0);
            list_resist_meas[i] = R_meas;
            G_tot = G_tot + 1 / R_meas;
            printf("Measured memristor %d: %.6lfV\n", i, R_meas);
            printf("Change probe TE to memristor %d and press Enter:\n", i+2);
            scanf("%lf", &buffer);
        }
        double V_meas = (1/G_tot) * (V_in * R_L);
        double V_diff = (V_target - V_meas);
        printf("Measured output voltage before balance: %.6lfV\nV_target was: %.6lfV\nVoltage difference: %.3lfuV", V_meas, V_target, V_diff * 1000000);
        printf("Moveback probe TE to memristor 1 and press Enter:\n");
        scanf("%lf", &buffer);
        //Balance
        double R_new_target = R_target + (R_target/nb_memristor-(1/G_tot));
        converge_to_target(R_new_target, tolerance, V_p_min, V_n_min, step_DC, file_name, 1, 0);
        double R_meas = apply_pulse_new(Vr, t_read, topChannel, bottomChannel, 0.2, 0, file_name, 1, 0);
        list_resist_meas[0] = R_meas;
        G_tot = 0;
        for (int i = 0; i < nb_memristor; i++) {
            G_tot = G_tot + 1 / list_resist_meas[i];
        }
        V_meas = (1 / G_tot) * (V_in * R_L);
        V_diff = (V_target - V_meas);
        printf("Measured after balance output voltage: %.6lfV\nV_target was: %.6lfV\nVoltage difference: %.3lfuV", V_meas, V_target, V_diff * 1000000);
        FILE* fp = fopen(file_name, "w");
        if (fp != 0) {
            fprintf(fp, "%.9lf; %.9lf; %.9lf; %.9lf\n", V_target, V_meas, V_diff, 1/G_tot);
            fclose(fp);
        }
    }
}

void DC_source_sweep(double V_target_min, double V_target_max, double resolution, const char* file_name)
{
    FILE* fp = fopen(file_name, "w");
    if (fp != 0) {
        fprintf(fp, "V_min; V_max; resolution; LRS; HRS, V_p, V_n\n");
        fprintf(fp, "%.9lf; %.9lf; %.9lf; %.9lf; %.9lf; %.9lf\n", V_target_min, V_target_max, resolution, R_min, R_max, V_p_min, V_n_min);
        fprintf(fp, "V_target; V_meas; V_diff; R config\n");
        fclose(fp);
    }
    int nb_voltage = (V_target_max - V_target_max) / resolution;
    //Sweep over all voltage
    for (int i = 0; i < nb_voltage; i++) {
        double V_target = V_target_min + i * resolution;
        DC_voltage_prog(V_target, file_name);
        Sleep(5000);
    }
    printf("End of voltage sweep");
}



void pulse_var_studies(double V_pulse_pos, double V_pulse_neg, double Res, double t_pulse, int pulses_per_train_pos, int pulses_per_train_neg, int pulse_train, const char* file_name) {
    //create pattern
    WGFMU_clear();
    double polarity = 1;
    double V_pulse_neg_sent = V_pulse_neg;
    double V_pulse_pos_sent = V_pulse_pos;
    //Next lines are simply so that there is quicker convergence to target since variability ping pong strategy is often in between 0.2 and 1.0 V 
    //(e.g. avoid having to wait for 0.02V increment to program the device from 0.2V)
    if (V_pulse_neg > -1) {
        V_pulse_neg_sent = -1.0;
    }
    if (V_pulse_pos < 1) {
        V_pulse_pos_sent = 1.0;
    }
    converge_to_target(Res, tolerance_write, V_pulse_neg_sent, V_pulse_pos_sent, 0.02, file_name, 5, 1);
    char top_name[100] = "top";
    char bottom_name[100] = "bottom";
    char meas_event1[100] = "meas1";
    char meas_event2[100] = "meas2";
    WGFMU_createPattern(top_name, 0);
    WGFMU_createPattern(bottom_name, 0);

    for (int i = 0; i < pulses_per_train_pos; i++) {
        //Apply writing pulse
        WGFMU_addVector(top_name, 0.00000001, V_pulse_pos); //10 ns rise time
        WGFMU_addVector(top_name, t_pulse - 0.00000002, V_pulse_pos);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V

        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, t_pulse - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);

    }
    //Apply reading pulse
    WGFMU_addVector(top_name, 0.00000001, Vr);
    WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
    WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V

    WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
    WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_setMeasureEvent(bottom_name, meas_event1, (pulses_per_train_pos)*t_pulse + 0.00000001, 1, t_read - 0.00000004, t_read - 0.00000004, WGFMU_MEASURE_EVENT_DATA_AVERAGED);

    for (int i = 0; i < pulses_per_train_neg; i++) {
        //Apply writing pulse
        WGFMU_addVector(top_name, 0.00000001, V_pulse_neg); //10 ns rise time
        WGFMU_addVector(top_name, t_pulse - 0.00000002, V_pulse_neg);
        WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V

        WGFMU_addVector(bottom_name, 0.00000001, 0);
        WGFMU_addVector(bottom_name, t_pulse - 0.00000002, 0);
        WGFMU_addVector(bottom_name, 0.00000001, 0);

    }

    //Apply reading pulse
    WGFMU_addVector(top_name, 0.00000001, Vr);
    WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
    WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V

    WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
    WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_setMeasureEvent(bottom_name, meas_event2, (pulses_per_train_pos + pulses_per_train_neg) * t_pulse + 0.00000001 + t_read, 1, t_read - 0.00000004, t_read - 0.00000004, WGFMU_MEASURE_EVENT_DATA_AVERAGED);

    //Check if measureEvent good
    WGFMU_addSequence(topChannel, top_name, 1000000);
    WGFMU_addSequence(bottomChannel, bottom_name, 1000000);

    WGFMU_openSession("GPIB0::17::INSTR");
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    //TODO: change current range accoridngly 
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    FILE* fp = fopen(file_name, "a");
    if (fp != 0) {
        fprintf(fp, "V_min; V_max; number of amplitudes; number of pulses; pulse width\n");
        fprintf(fp, "%.9lf; %.9lf; %d; %d; %d; %.9lf\n", V_pulse_pos, V_pulse_neg, pulses_per_train_pos, pulses_per_train_neg, pulse_train, t_pulse);
        fprintf(fp, "time; voltage; current; resistance\n");
        int measuredSize, totalSize;
        WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
        for (int i = 0; i < measuredSize; i++) {
            double time, value, voltage;
            WGFMU_getMeasureValue(bottomChannel, i, &time, &value);
            WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
            fprintf(fp, "%.9lf; %.9lf; %.9lf; %.9lf\n", time, -voltage, value, -voltage / value);
        }
        fclose(fp);
    }
    WGFMU_initialize();
    WGFMU_closeSession();
}

void PVS(double V_pulse_pos, double V_pulse_neg, double Res, double t_pulse, double wait_time, int cycle_number, const char* file_name) {
    //create pattern
    WGFMU_clear();
    double polarity = 1;
    double V_pulse_neg_sent = V_pulse_neg;
    double V_pulse_pos_sent = V_pulse_pos;
    if (V_pulse_neg < -1) {
        V_pulse_neg_sent = -1.0;
    }
    if (V_pulse_pos > 1) {
        V_pulse_pos_sent = 1.0;
    }
    converge_to_target(Res, tolerance_write, V_pulse_neg_sent, V_pulse_pos_sent, 0.02, file_name, 5, 1);
    char top_name[100] = "top";
    char bottom_name[100] = "bottom";
    char meas_event1[100] = "meas1";
    WGFMU_createPattern(top_name, 0);
    WGFMU_createPattern(bottom_name, 0);

    WGFMU_addVector(top_name, 0.00000001, V_pulse_pos); //10 ns rise time
    WGFMU_addVector(top_name, t_pulse - 0.00000002, V_pulse_pos);
    WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_addVector(top_name, wait_time, 0);

    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_addVector(bottom_name, t_pulse - 0.00000002, 0);
    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_addVector(bottom_name, wait_time, 0);

    //Apply reading pulse
    WGFMU_addVector(top_name, 0.00000001, Vr);
    WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
    WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_addVector(top_name, wait_time, 0);

    WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
    WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_addVector(bottom_name, wait_time, 0);
    
    WGFMU_addVector(top_name, 0.00000001, V_pulse_neg); //10 ns rise time
    WGFMU_addVector(top_name, t_pulse - 0.00000002, V_pulse_neg);
    WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_addVector(top_name, wait_time, 0);

    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_addVector(bottom_name, t_pulse - 0.00000002, 0);
    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_addVector(bottom_name, wait_time, 0);

    WGFMU_addVector(top_name, 0.00000001, Vr);
    WGFMU_addVector(top_name, t_read - 0.00000002, Vr);
    WGFMU_addVector(top_name, 0.00000001, 0); //10 ns fall time, 0 V
    WGFMU_addVector(top_name, wait_time, 0);

    WGFMU_addVector(bottom_name, 0.00000001, 0); //10 ns rise time, 1 V
    WGFMU_addVector(bottom_name, t_read - 0.00000002, 0);
    WGFMU_addVector(bottom_name, 0.00000001, 0);
    WGFMU_addVector(bottom_name, wait_time, 0);
    int nb_points = (2 * (t_read + t_pulse) + 4 * wait_time) / 1e-8;
    WGFMU_setMeasureEvent(bottom_name, meas_event1, 1e-8, nb_points-1, 1e-8, 1e-8, WGFMU_MEASURE_EVENT_DATA_RAW);

    //Check if measureEvent good
    WGFMU_addSequence(topChannel, top_name, cycle_number);
    WGFMU_addSequence(bottomChannel, bottom_name, cycle_number);

    WGFMU_openSession("GPIB0::17::INSTR");
    WGFMU_initialize();
    WGFMU_setOperationMode(topChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setOperationMode(bottomChannel, WGFMU_OPERATION_MODE_FASTIV);
    WGFMU_setMeasureMode(bottomChannel, WGFMU_MEASURE_MODE_CURRENT);
    //TODO: change current range accoridngly 
    WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    WGFMU_connect(topChannel);
    WGFMU_connect(bottomChannel);
    WGFMU_execute();
    WGFMU_waitUntilCompleted();
    FILE* fp = fopen(file_name, "a");
    if (fp != 0) {
        fprintf(fp, "V_p; V_n; nb_points; cycle_number; wait_time\n");
        fprintf(fp, "%.9lf; %.9lf; %d; %d; %.9lf; %.9lf\n", V_pulse_pos, V_pulse_neg, nb_points, cycle_number, wait_time, Res);
        fprintf(fp, "time; voltage; current; resistance\n");
        int measuredSize, totalSize;
        WGFMU_getMeasureValueSize(bottomChannel, &measuredSize, &totalSize);
        for (int i = 0; i < measuredSize; i++) {
            double time, value, voltage;
            WGFMU_getMeasureValue(bottomChannel, i, &time, &value);
            WGFMU_getInterpolatedForceValue(topChannel, time, &voltage);
            fprintf(fp, "%.9lf; %.9lf; %.9lf; %.9lf\n", time, -voltage, value, -voltage / value);
        }
        fclose(fp);
    }
    WGFMU_initialize();
    WGFMU_closeSession();
}

const char* get_timestamp(int choice, const char* folder) {
    time_t rawtime;
    struct tm* timeinfo;
    static char buffer[18];
    static char file_name[200];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strcpy(file_name, folder);
    puts(folder);
    strftime(buffer, 16, "%G%m%d%H%M%S", timeinfo);
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
    else if (choice == 5) {
        strcat(file_name, "_data_driven_fitting.txt");
    }
    else if (choice == 7) {
        strcat(file_name, "_triangle_convergence.txt");
    }
    else if (choice == 9) {
        strcat(file_name, "_DC_sweep.txt");
    }
    else if (choice == 10) {
        strcat(file_name, "_DC_prog.txt");
    }
    else if (choice == 11) {
        strcat(file_name, "_write_variability_ddvariant.txt");
    }
    else if (choice == 12) {
        strcat(file_name, "_retention.txt");
    }
    else if (choice == 13) {
        strcat(file_name, "_PRT.txt");
    }
    else if (choice == 14) {
        strcat(file_name, "_read_varia.txt");
    }
    else if (choice == 15) {
        strcat(file_name, "_PVS.txt");
    }
    else if (choice == 17) {
        strcat(file_name, "_res_list.txt");
    }
    else if (choice == 20) {
        strcat(file_name, "_pulsed_forming.txt");
    }
    else if (choice == 94) {
        strcat(file_name, "_PRT");
    }
    else if (choice == 95) {
        strcat(file_name, "_prog_overlap.txt");
    }
    else if (choice == 96) {
        strcat(file_name, "_Gvt_pulse");
    }
    else if (choice == 97) {
        strcat(file_name, "_Gvt_pulse_folder\\");
        _mkdir(file_name);
    }
    else if (choice == 98) {
        strcat(file_name, "_PRT_folder\\");
        _mkdir(file_name);
    }
    else {
        static char name[100];
        sprintf(name, "_write_variability_%dOhm.txt", choice);
        strcat(file_name, name);
    }
    printf("The measurement will be saved as: ");
    puts(file_name);
    return file_name;
}

void set_current_range(double res) {
    if (res < 1800) {
        WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_1MA);
    }
    else if (res < 22000) {
        WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_100UA);
    }
    else if (res < 210000) {
        WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_10UA);
    }
    else if (res > 210000) {
        WGFMU_setMeasureCurrentRange(bottomChannel, WGFMU_MEASURE_CURRENT_RANGE_1UA);
    }
}

double set_settling_time(double res){
    if (res < 2000) {
        return(200e-9);
    }
    else if (res < 20000) {
        return(820e-9);
    }
    else if (res < 200000) {
        return(5.8e-6);
    }
    else if (res > 200000) {
        return(37e-6);
    }
}

double set_read_time(double res) {
    if (res < 2000) {
        return(200e-9);
    }
    else if (res < 20000) {
        return(200e-9);
    }
    else if (res < 200000) {
        return(1e-6);
    }
    else if (res > 200000) {
        return(5e-6);
    }
}
