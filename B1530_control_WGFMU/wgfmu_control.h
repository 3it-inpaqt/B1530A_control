//wgfmu_control.h
//Author: Pierre-Antoine Mouny
//Date: 25/05/2021
//Universite de Sherbrooke (3IT)
double read_resistance(double, double, int, int, double, double, const char*);
void write_resistance(double, double, int, int);
double extract_results(int, int, int, double, double, const char*);
void simple_convergence(double, double, double, double, double, const char*);
void log_convergence(double, double, double, double, const char*);
void log_convergence_2(double, double, double, double, const char*);
void Gvt(double[], double[], const char*);
void DC_sweep(int, int, double, double, int, double, double);
void data_driven_fitting(double, double, double, int, const char*);
const char* get_timestamp(int);
static const double min_shift = 0.005; //used for log convergence less than a 0.5% shift with respect to HRS-LRS
static const double max_shift = 0.3; //not more than a 20% shift with respect to HRS-LRS
static const double a = 0.20; //Coefficient determining increase speed of pulse amplitude for log convergence
static const int increment = 5; //Argument for log convergence
static const double t_pulse = 2E-7; //Pulse width of writing pulses
static const double Vr = 2E-1; //Amplitude of reading pulses
static const double t_read = 1E-5; //Pulse width of reading pulses
static const double V_max = 3.0;
static const int stop = 20;
static const int nb_state = 2; //Number of distinct resistance states targeted (they are equally spaced in resistance)
static const double tolerance = 0.01;
static const char* folder_path = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\";
static const char* path_csv = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\multilevel_prog.txt";//The path to your CSV;
static const int topChannel = 101;//physical channel of WGFMU used by the probe tip placed on the top electrode
static const int bottomChannel = 102;//physical channel of WGFMU used by the probe tip placed on the bottom electrode
static const int nb_points = 250; //Should be an even number
static const int sleep_time = 20000; //Time between two DC sweep (waiting for the measurement to end) in ms
static const double V_set = 1.7; // Set voltage (DC sweep)
static const double V_reset = -1.8; //Reset voltage (DC sweep)
static const double compliance_set = 0.001; //Compliance current used in DC sweeps in set operations
static const double compliance_reset = 0.01; //Compliance current used in DC sweeps in reset operations not necessary but used as a security
static const double meas_time = 10; //Duration in second of the DC sweep
static const int pulse_number = 6; //Number of pulse amplitudes to test for G(V,t) measurements or data driven model
static double list_time[21] = {3E-8, 5E-8, 7E-8, 3E-7, 5E-7, 7E-7, 3E-6, 5E-6, 7E-6, 3E-5, 5E-5, 7E-5, 3E-4, 5E-4, 7E-4, 3E-3, 5E-3, 7E-3, 3E-2, 5E-2, 7E-2 }; // list pulse width for G(V,t) measurements