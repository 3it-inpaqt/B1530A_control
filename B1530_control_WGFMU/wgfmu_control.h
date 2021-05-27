//wgfmu_control.h
//Author: Pierre-Antoine Mouny
//Date: 25/05/2021
//Universite de Sherbrooke (3IT)
double read_resistance(double, double, int, int, double);
void write_resistance(double, double, int, int);
double extract_results(int, int, int, const char*, double);
static const double t_pulse = 2E-7; //Pulse width of writing pulses
static const double Vr = 2E-1; //Amplitude of reading pulses
static const double t_read = 1E-6; //Pulse width of reading pulses
static const double V_max = 3;
static const int stop = 20;
static const int nb_state = 3; //Number of distinct resistance states targeted (they are equally spaced in resistance)
static const double tolerance = 0.02;
static const char* path_csv = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\multilevel_prog.txt";//The path to your CSV;
static const int topChannel = 101;//physical channel of WGFMU used by the probe tip placed on the top electrode
static const int bottomChannel = 102;//physical channel of WGFMU used by the probe tip placed on the bottom electrode