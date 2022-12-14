#include <vector>
#include "B1530_control_WGFMU/wgfmu_control.h"

int source_PUND_pulse2(pulseshape shape, int npoints, bool half) {
	std::string shapename = "PUND";
	if (half) shapename = "PU";

	WGFMU_createPattern("P", 0);
	WGFMU_addVector("P", shape.tspace, 0); // Delay is prepended
	WGFMU_addVector("P", shape.trise, shape.Vpulse); // Rise to Vpulse
	if (shape.twidth != 0) WGFMU_addVector("P", shape.twidth, shape.Vpulse); // Hold at VPULSE
	WGFMU_addVector("P", shape.trise, 0); // Return to 0
	WGFMU_createMergedPattern("PU", "P", "P", WGFMU_AXIS_TIME);
	WGFMU_createMultipliedPattern("ND", "PU", 1, -1);
	WGFMU_createMergedPattern("PUND", "PU", "ND", WGFMU_AXIS_TIME);

	// Create a measure pulse of the same lenght, where all voltages are 0
	WGFMU_createMultipliedPattern("meas", shapename.c_str(), 1, -1); // Can't multiplty voltage by 0
	WGFMU_createMergedPattern("meas", "meas", shapename.c_str(), WGFMU_AXIS_VOLTAGE); // So add the opposite

	// Determine sample time for desired npoints
	double ttotal = 2 * (shape.tspace + 2 * shape.trise + shape.twidth);
	if (!half) ttotal *= 2;
	double t_sample = ttotal / npoints;

	// Evaluate if it's possible to use this sample time...
	if (t_sample < WGFMU_t_min_measure) t_sample = WGFMU_t_min_measure; // 5 ns is the minimal sampling time

	// Snap sample time to grid, (WGFMU does this auto, but makes measurement event too long)
	double t_sample_q = round(t_sample / WGFMU_t_min_measure) * WGFMU_t_min_measure;
	int npoints_q = ttotal / t_sample_q - 1; // without the "-1", the event is still sometimes too long

	double taveraging = (t_sample_q < WGFMU_t_min_average) ? WGFMU_t_min_average : t_sample_q; // 10 ns is the minimal averaging time

	WGFMU_setMeasureEvent(shapename.c_str(), "Vmeas", 0, npoints_q, t_sample_q, taveraging, WGFMU_MEASURE_EVENT_DATA_AVERAGED);
	WGFMU_setMeasureEvent("meas", "Imeas", 0, npoints_q, t_sample_q, taveraging, WGFMU_MEASURE_EVENT_DATA_AVERAGED);

	WGFMU_addSequence(topChannel, shapename.c_str(), 1);
	WGFMU_addSequence(bottomChannel, "meas", 1);
	
	return npoints_q;
}

void source_save_splitPUND(const getAllresult& res, int npoints, int decade, std::string basepath) {
	std::string filename;

	int nb = res.time.size()/npoints;

	for (int i = 0; i < nb; i++) {
		filename = basepath + "\\PUND_" + std::to_string(decade) + '_' + std::to_string(i) + ".csv";
		ofstreamhighpres of(filename);
		of.stream << "Time;Voltage;Current" << std::endl;
		for (int j = i*npoints; j < (i+1)*npoints; j++) {
			of.stream << res.time[j] << ";" << res.voltage[j] << ";" << res.current[j] << std::endl;
		}
	}

};

void source_better_NLS(PROG_args args) {
	getAllresult res;
	std::vector<double> pulsewidths; // get this
	std::vector<double> voltages; // get this
	for (int i = 0; i < voltages.size(); i++) {
		init_session(args.currentrange, WGFMU_OPERATION_MODE_PG);
		args.aging_shape.Vpulse = voltages[i];

		for (int j = 0; j < pulsewidths.size(); j++) {
			args.aging_shape.twidth = pulsewidths[j];
			aging_pulse(NLS_cycle, "NLS_cycle", 2, false); // Standard pulse, using const params
			
			// Not aging, just the specified NLS
			aging_pulse(args.aging_shape, "NLS_params", 1, true); // "The NLS", using params from args
			
			source_PUND_pulse2(NLS_halfPUND, args.npoints, true);
		}

		execute_getAll(&res);
		save_splitPUND(res, args.npoints, i, args.path);
		//saydone();

		WGFMU_clear();
		res.time.clear();
		res.voltage.clear();
		res.current.clear();
	}
	//alldone();
}