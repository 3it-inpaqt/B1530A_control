//wgfmu_control.h
//Author: Pierre-Antoine Mouny
//Date: 25/05/2021
//Universite de Sherbrooke (3IT)
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "wgfmu.h"

/// <param name="Vpulse"> Max voltage of the pulse. </param>
/// <param name="trise"> Time to raise to Vpulse and fall to 0V. </param>
/// <param name="twidth"> Time spent at Vpulse, after trise. </param>
/// <param name="tspace"> Time spent at 0V before each pulse. </param>
class pulseshape {
	public:
		double Vpulse;
		double trise;
		double twidth;
		double tspace;
		double t_singlepusle(){
			return 2 * trise + twidth + tspace;
		}
};

/// <param name="PUND_decade"> Number of cycle done before this PUND measure. 10^decade = Qty of aging pulses </param>
/// <param name="PUND_number"> Number of the pusle WITHIN the decade (0 to 9) </param>
/// <param name="PUND_maxdecade"> Used as stopping point for PUND_decade if it's incremented within c++ instead of python. </param>
/// <param name="aging_shape"> Shape of the aging cycles. </param>
/// <param name="PUND_shape"> Shape of the PUND pulses. </param>
/// <param name="npoints"> How many points to capture in the current measurement. </param>
/// <param name="currentrange"> Current range to be used, in µA. </param>
/// <param name="path"> Folder where all the .csv data will be output. </param>
struct PROG_args {
	// PUND
	double PUND_decade;
	double PUND_number;
	double PUND_maxdecade; 
	pulseshape aging_shape;
	pulseshape PUND_shape;
	
	// LTP, LTD
	double Vwritepos;
	double Vwriteneg;
	double Vread;
	double twrite;

	// Generic
	int npoints;
	double currentrange;
	std::string path;
};

struct premadesequence {
	std::string topsequence;
	std::string botsequence;
	double t_singlepusle;
};

class ofstreamhighpres {
	public:
		ofstreamhighpres(std::string filename) : stream(filename, std::ofstream::app) {
			stream << std::setprecision(std::numeric_limits<double>::digits10 + 1);
			//stream << std::scientific;
		}
		~ofstreamhighpres() {
			stream.flush();
			stream.close();
		}
		std::ofstream stream;
};

struct getAllresult {
	std::vector<double> time;
	std::vector<double> voltage;
	std::vector<double> current;
};

// Methods
double read_resistance(double, double, int, int, double, double, const char*);
void write_resistance(double, double, int, int);
void write_resistance_triangle(double, double, int, int);
double apply_pulse_new(double, double, int, int, double, double, const char*, int, int);
double extract_results(int, int, int, double, double, const char*);
void simple_convergence(double, double, double, double, double, const char*);
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

// Ferro / Pund specific funcitons
void parseargs(int argc, char* argv[], PROG_args* args);
bool checkargs(const PROG_args& args);
int getAgingCycles(int decade, bool* initialize);
int getCycle(int decade, int number);

void enduranceTest(PROG_args args);
void singlePUND(PROG_args args, bool init);
void partialSwitching(PROG_args args);
void measureOnOff(PROG_args args);
void measureOnOffNoAging(PROG_args args);
void LTD_LTP_voltage(PROG_args args);
void LTD_LTP_puslewidth(PROG_args args);
void LTP_LTD_newversion(PROG_args args, bool varPulsewidth);
void addLTP_LTD_fastrangechange(const std::vector<double>& params, PROG_args args, bool varPulsewidth);

void add_square_pulse(pulseshape shape, std::string name, double count, bool invertAmplitude, bool oppositePulses);
void executePulse(pulseshape shape, const char* name, int count, bool invertAmplitude, bool oppositePulses);
void maxaveragePulse(double Vpulse, std::string name, int npoints);
premadesequence maxaveragePulse2(double Vpulse); // Similar to above, always 1 point, doesn't add the sequence immediatly
void addPremadeSequence(premadesequence seq, int count);
void PUND_pulse(pulseshape shape, int npoints, bool half);
void init_session(double range, int mode);

void execute_and_save(std::string filename);
double execute_getRes(double* voltage_out, double* current_out);
void execute_getAll(getAllresult* res);
void writeLTPLTDresults(ofstreamhighpres& ofs, int CycleID,double writevoltage, double writetime, double readvoltage, double voltage, double current);


premadesequence PUND_pulse2(pulseshape shape, int npoints, bool half, int* npointsreal);
void save_splitPUND(const getAllresult& res, int npoints, int decade, std::string basepath);
void better_NLS(PROG_args args);

//Global parameters
static const char* folder_path = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\";
static const char* folder_path_Gvt = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\5\\";
static const char* path_csv = "C:\\Users\\moup2702\\Desktop\\UdeS\\Measurements\\Multilevel_programming\\multilevel_prog.txt";//The path to your CSV
static const char* path_list_res = "C:\\Users\\moup2702\\Desktop\\UdeS\\Simulation\\Image\\Feynman_list_res_grayscale.txt";
static const int topChannel = 101;//physical channel of WGFMU used by the probe tip placed on the top electrode
static const int bottomChannel = 102;//physical channel of WGFMU used by the probe tip placed on the bottom electrode
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
static const double V_max = 2.8;
static const int stop = 20;
static const int nb_state = 8; //Number of distinct resistance states targeted (they are equally spaced in resistance)
static const double tolerance = 0.025;

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
static double list_time[18] = {3E-8, 5E-8, 7E-8, 3E-7, 5E-7, 7E-7, 3E-6, 5E-6, 7E-6, 3E-5, 5E-5, 7E-5, 3E-4, 5E-4, 7E-4, 3E-3, 5E-3, 7E-3}; // list pulse width for G(V,t) measurements
static const double HRS_Gvt = 100000;
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

//WGFMU ALWG specs
static const double WGFMU_t_min_segment = 5e-09; // Minimum lenght of any vector in a waveform pattern
static const double WGFMU_t_min_measure = 10e-09; // Minimum sample time in a measure event (datasheet says 5ns, but gets quantized to 10ns automatically)
static const double WGFMU_t_min_average = 10e-09; // Minimum averaging time (can also be 0, where there is no averaging)
static const double WGFMU_t_max_average = 20e-3; // Maximum averaging time (strictly speaking its 20.971512e-6)
static const double WGFMU_t_min_rise_PG = 30e-09; // Minimum rise time in PG mode
static const std::map<int, double> WGFMU_bandwith_PG = { // key = current range in µA, val = bandwith in Hz
	{WGFMU_MEASURE_CURRENT_RANGE_1UA, 80e3},
	{WGFMU_MEASURE_CURRENT_RANGE_10UA,600e3},
	{WGFMU_MEASURE_CURRENT_RANGE_100UA,2.4e6},
	{WGFMU_MEASURE_CURRENT_RANGE_1MA,8e6},
	{WGFMU_MEASURE_CURRENT_RANGE_10MA,16e6} }; 
static const std::map<int, double> WGFMU_t_min_rise_IV = { // key = current range in µA, val = t in s, specs for 0 to 5V
	{WGFMU_MEASURE_CURRENT_RANGE_1UA, 2e-6},
	{WGFMU_MEASURE_CURRENT_RANGE_10UA,4.5e-6},
	{WGFMU_MEASURE_CURRENT_RANGE_100UA,600e-9},
	{WGFMU_MEASURE_CURRENT_RANGE_1MA,250e-9},
	{WGFMU_MEASURE_CURRENT_RANGE_10MA,80e-9} };
static const std::map<int, double> WGFMU_t_min_pulsewidth_IV = { // key = current range in µA, val = t in s
	{WGFMU_MEASURE_CURRENT_RANGE_1UA, 115e-6},
	{WGFMU_MEASURE_CURRENT_RANGE_10UA,14.5e-6},
	{WGFMU_MEASURE_CURRENT_RANGE_100UA,1.6e-6},
	{WGFMU_MEASURE_CURRENT_RANGE_1MA,500e-9},
	{WGFMU_MEASURE_CURRENT_RANGE_10MA,180e-9} };
static const std::map<int, double> WGFMU_t_settle_IV = { // key = current range in µA, val = t in s
	{WGFMU_MEASURE_CURRENT_RANGE_1UA, 80e-6},
	{WGFMU_MEASURE_CURRENT_RANGE_10UA,10e-6},
	{WGFMU_MEASURE_CURRENT_RANGE_100UA,1e-6},
	{WGFMU_MEASURE_CURRENT_RANGE_1MA,250e-9},
	{WGFMU_MEASURE_CURRENT_RANGE_10MA,100e-9} };

//NLS parameters
const struct pulseshape NLS_cycle= { 3, 50e-09, 50e-6, 50e-6 };
const struct pulseshape NLS_halfPUND = { 3, 50e-6, 0, 50e-6 };

//Ferro resistance and on-off params
const double t_measure_blank = 20e-3; // 20 ms before starting to measure current in a resistance test, to let the transiants pass
