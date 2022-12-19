//wgfmu_control.h
//Author: Pierre-Antoine Mouny
//Date: 25/05/2021
//Universite de Sherbrooke (3IT)


// Methods
double read_resistance(double, double, int, int, double, double, const char*);
void write_resistance(double, double, int, int);
void write_resistance_triangle(double, double, int, int);
double apply_pulse_new(double, double, int, int, double, double, const char*, int, int);
double extract_results(int, int, int, double, double, const char*);
void simple_convergence(double, double, double, double, double, const char*);
void programming_noise(double, double, double, double, double, int, const char*);
void simple_convergence_reverse(double, double, double, double, double, int, const char*);
void simple_convergence_fast(double, double, double, double, double, int, const char*);
void converge_to_target(double, double, double, double, double, const char*, int, int);
void simple_triangle_convergence(double, double, double, double, double, const char*);
void log_convergence(double, double, double, double, const char*);
void log_convergence_2(double, double, double, double, const char*);
void Gvt(double[], double[], double, double, const char*);
void Gvt_pulse(int, double[], double, double, const char*);
void Gvt_pulse_2(int, double[], double, double, const char*);
void retention(double, double, double, double, double, const char*);
void read_variability(double, double, double, double, double, const char*);
void PRT(double, double, double, const char*);
void write_variability_pulse_number(double, double, double, double, const char*, int);
void DC_sweep(int, int);
void data_driven_fitting(double, double, double, int, const char*);
void LTD_LTP_measurement(double, double, double, double, int, const char*);
void DC_source_sweep(double, double, double, const char*);
void DC_voltage_prog(double, const char*);
void pulse_var_studies(double, double, double, double, int, int, int, const char*);
void PVS(double , double , double , double , double , int , const char*);
const char* get_timestamp(int, const char*);
void set_current_range(double);
double set_settling_time(double);
double set_read_time(double);

//Global parameters
static const char* folder_path = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\";
static const char* folder_path_Gvt = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\5\\";
static const char* path_csv = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\multilevel_prog.txt";//The path to your CSV
static const char* path_list_res = "C:\\Users\\moup2702\\Desktop\\UdeS\\Simulation\\Image\\Feynman_list_res_grayscale.txt";
static const int topChannel = 201;//physical channel of WGFMU used by the probe tip placed on the top electrode
static const int bottomChannel = 202;//physical channel of WGFMU used by the probe tip placed on the bottom electrode
static const int topChannelSMU = 201;
static const int bottomChannelSMU = 301;
static const double V_set = 1.4; // Set voltage (DC sweep)
static const double V_reset = -1.5; //Reset voltage (DC sweep)
static const double meas_time = 10; //Duration in second of the DC sweep
static const double compliance_set = 0.001; //Compliance current used in DC sweeps in set operations
static const double compliance_reset = 0.01; //Compliance current used in DC sweeps in reset operations not necessary but used as a security
static const double t_pulse = 2E-7; //Pulse width of writing pulses
static const double Vr = 2E-1; //Amplitude of reading pulses
static const double t_read = 1E-5; //Pulse width of reading pulses Warning should not be set below 1e-5 seems to lead to bad measurements otherwise

//Multilevel programming parameters
static const double min_shift = 0.005; //used for log convergence less than a 0.5% shift with respect to HRS-LRS
static const double max_shift = 0.3; //not more than a 30% shift with respect to HRS-LRS
static const double a = 0.20; //Coefficient determining increase speed of pulse amplitude for log convergence
static const int increment = 5; //Argument for log convergence
static const double V_max = 3.8;
static const int stop = 20;
static const int nb_state = 8; //Number of distinct resistance states targeted (they are equally spaced in resistance)
static const double tolerance = 0.015;

//Write variability parameters
static const int write_varia_loop = 100000;
static const int pulse_number_loop = 1;
static const double tolerance_write = 0.01;
static const double step_varia = 0.02;

//G(V,t) parameters
static const int nb_points = 250; //Should be an even number
static const int sleep_time = 20000; //Time between two DC sweep (waiting for the measurement to end) in ms
static const int repeat_data_driven = 8;
static const int repeat_Gvt = 5;
static const int pulse_number = 8; //Number of pulse amplitudes to test for G(V,t) measurements or data driven model
static double list_time[11] = {5E-8, 1E-7, 2E-7, 4E-7, 8E-7, 1E-6, 2E-6, 4E-6, 8E-6, 1E-5, 2E-5}; // list pulse width for G(V,t) measurements
//static double list_time[18] = {3E-8, 5E-8, 7E-8, 3E-7, 5E-7, 7E-7, 3E-6, 5E-6, 7E-6, 3E-5, 5E-5, 7E-5, 3E-4, 5E-4, 7E-4, 3E-3, 5E-3, 7E-3}; // list pulse width for G(V,t) measurements
static const double HRS_Gvt = 200000;
static const double LRS_Gvt = 20000;
static const int repeat_PRT = 10;


//LTD LTP measurements for ferro team
static const int pulse_number_LTD_LTP = 6; //Number of different (absolute) pulse amplitude to be used in LTD LTP measurements
static const int t_pulse_number_LTD_LTP = 6; //Number of different pulse width to be used in LTD LTP measurements

//DC source measurements
static const double V_p_min = 0.8;
static const double V_n_min = -0.8;
static const double step_DC = 0.02;
static const double R_min = 4000;
static const double R_max = 16000;
static const int nb_memristor = 1;
static const double V_in = 0.001;
static const double R_L = 1;