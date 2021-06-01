//wgfmu_control.h
//Author: Pierre-Antoine Mouny
//Date: 25/05/2021
//Universite de Sherbrooke (3IT)
double read_resistance(double, double, int, int, double, const char*);
void write_resistance(double, double, int, int);
double extract_results(int, int, int, const char*, double);
void simple_convergence(double, double, double, double, double, const char*);
void log_convergence(double, double, double, double, const char*);
void log_convergence_2(double, double, double, double, const char*);
const char* get_timestamp(int);
static const double min_shift = 0.005; //used for log convergence less than a 0.5% shift with respect to HRS-LRS
static const double max_shift = 0.2; //not more than a 20% shift with respect to HRS-LRS
static const double a = 0.3; //Coefficient determining increase speed of pulse amplitude for log convergence
static const int increment = 3; //Argument for log convergence
static const double t_pulse = 2E-7; //Pulse width of writing pulses
static const double Vr = 2E-1; //Amplitude of reading pulses
static const double t_read = 1E-5; //Pulse width of reading pulses
static const double V_max = 2.8;
static const int stop = 20;
static const int nb_state = 16; //Number of distinct resistance states targeted (they are equally spaced in resistance)
static const double tolerance = 0.015;
static const char* folder_path = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\";
static const char* path_csv = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\multilevel_prog.txt";//The path to your CSV;
static const int topChannel = 101;//physical channel of WGFMU used by the probe tip placed on the top electrode
static const int bottomChannel = 102;//physical channel of WGFMU used by the probe tip placed on the bottom electrode