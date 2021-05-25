/*---------------------------------------------------------------------------*/
/* Distributed by Agilent Technologies                                       */
/*                                                                           */
/* Do not modify the contents of this file.                                  */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Title   : WGFMU.H                                                         */
/* Date    : 10-30-2008                                                      */
/* Purpose : B1530A (WGFMU) Instrument Library API and Constant Definitions  */
/*           for Microsoft Visual C++ .NET                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef WGFMU_INSTRUMENTLIBRARY_H
#define WGFMU_INSTRUMENTLIBRARY_H

#define WGFMUAPI _stdcall

// API Return Value - Error Code
static const int WGFMU_NO_ERROR							= 0;
static const int WGFMU_PARAMETER_OUT_OF_RANGE_ERROR		= -1;
static const int WGFMU_ILLEGAL_STRING_ERROR				= -2;
static const int WGFMU_CONTEXT_ERROR					= -3;
static const int WGFMU_FUNCTION_NOT_SUPPORTED_ERROR		= -4;
static const int WGFMU_COMMUNICATION_ERROR				= -5;
static const int WGFMU_FW_ERROR							= -6;
static const int WGFMU_LIBRARY_ERROR					= -7;
static const int WGFMU_ERROR							= -8;
static const int WGFMU_CHANNEL_NOT_FOUND_ERROR			= -9;
static const int WGFMU_PATTERN_NOT_FOUND_ERROR			= -10;
static const int WGFMU_EVENT_NOT_FOUND_ERROR			= -11;
static const int WGFMU_PATTERN_ALREADY_EXISTS_ERROR		= -12;
static const int WGFMU_SEQUENCER_NOT_RUNNING_ERROR		= -13;
static const int WGFMU_RESULT_NOT_READY_ERROR			= -14;
static const int WGFMU_RESULT_OUT_OF_DATE				= -15;

static const int WGFMU_ERROR_CODE_MIN					= -9999;

// WGFMU_doSelfCaliration, WGFMU_doSelfTest
static const int WGFMU_PASS = 0;
static const int WGFMU_FAIL = 1;

// WGFMU_treatWarningsAsErrors, WGFMU_setWarningLevel
static const int WGFMU_WARNING_LEVEL_OFFSET				= 1000;
static const int WGFMU_WARNING_LEVEL_OFF				= WGFMU_WARNING_LEVEL_OFFSET + 0;
static const int WGFMU_WARNING_LEVEL_SEVERE				= WGFMU_WARNING_LEVEL_OFFSET + 1;
static const int WGFMU_WARNING_LEVEL_NORMAL				= WGFMU_WARNING_LEVEL_OFFSET + 2;
static const int WGFMU_WARNING_LEVEL_INFORMATION		= WGFMU_WARNING_LEVEL_OFFSET + 3;

// WGFMU_setOperationMode
static const int WGFMU_OPERATION_MODE_OFFSET			= 2000;
static const int WGFMU_OPERATION_MODE_DC				= WGFMU_OPERATION_MODE_OFFSET + 0;
static const int WGFMU_OPERATION_MODE_FASTIV			= WGFMU_OPERATION_MODE_OFFSET + 1;
static const int WGFMU_OPERATION_MODE_PG				= WGFMU_OPERATION_MODE_OFFSET + 2;
static const int WGFMU_OPERATION_MODE_SMU				= WGFMU_OPERATION_MODE_OFFSET + 3;

// WGFMU_setForceVoltageRange
static const int WGFMU_FORCE_VOLTAGE_RANGE_OFFSET		= 3000;
static const int WGFMU_FORCE_VOLTAGE_RANGE_AUTO			= WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 0;
static const int WGFMU_FORCE_VOLTAGE_RANGE_3V			= WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 1;
static const int WGFMU_FORCE_VOLTAGE_RANGE_5V			= WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 2;
static const int WGFMU_FORCE_VOLTAGE_RANGE_10V_NEGATIVE	= WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 3;
static const int WGFMU_FORCE_VOLTAGE_RANGE_10V_POSITIVE	= WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 4;

// WGFMU_setMeasureMode
static const int WGFMU_MEASURE_MODE_OFFSET				= 4000;
static const int WGFMU_MEASURE_MODE_VOLTAGE				= WGFMU_MEASURE_MODE_OFFSET + 0;
static const int WGFMU_MEASURE_MODE_CURRENT				= WGFMU_MEASURE_MODE_OFFSET + 1;

// WGFMU_setMeasureVoltageRange
static const int WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET		= 5000;
static const int WGFMU_MEASURE_VOLTAGE_RANGE_5V			= WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET + 1;
static const int WGFMU_MEASURE_VOLTAGE_RANGE_10V		= WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET + 2;

// WGFMU_setMeasureCurrentRange
static const int WGFMU_MEASURE_CURRENT_RANGE_OFFSET		= 6000;
static const int WGFMU_MEASURE_CURRENT_RANGE_1UA		= WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 1;
static const int WGFMU_MEASURE_CURRENT_RANGE_10UA		= WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 2;
static const int WGFMU_MEASURE_CURRENT_RANGE_100UA		= WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 3;
static const int WGFMU_MEASURE_CURRENT_RANGE_1MA		= WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 4;
static const int WGFMU_MEASURE_CURRENT_RANGE_10MA		= WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 5;

// WGFMU_setMeasureEnabled
static const int WGFMU_MEASURE_ENABLED_OFFSET			= 7000;
static const int WGFMU_MEASURE_ENABLED_DISABLE			= WGFMU_MEASURE_ENABLED_OFFSET + 0;
static const int WGFMU_MEASURE_ENABLED_ENABLE			= WGFMU_MEASURE_ENABLED_OFFSET + 1;

// WGFMU_setTriggerOutMode
static const int WGFMU_TRIGGER_OUT_MODE_OFFSET			= 8000;
static const int WGFMU_TRIGGER_OUT_MODE_DISABLE			= WGFMU_TRIGGER_OUT_MODE_OFFSET + 0;
static const int WGFMU_TRIGGER_OUT_MODE_START_EXECUTION = WGFMU_TRIGGER_OUT_MODE_OFFSET + 1;
static const int WGFMU_TRIGGER_OUT_MODE_START_SEQUENCE	= WGFMU_TRIGGER_OUT_MODE_OFFSET + 2;
static const int WGFMU_TRIGGER_OUT_MODE_START_PATTERN	= WGFMU_TRIGGER_OUT_MODE_OFFSET + 3;
static const int WGFMU_TRIGGER_OUT_MODE_EVENT			= WGFMU_TRIGGER_OUT_MODE_OFFSET + 4;

static const int WGFMU_TRIGGER_OUT_POLARITY_OFFSET		= 8100;
static const int WGFMU_TRIGGER_OUT_POLARITY_POSITIVE	= WGFMU_TRIGGER_OUT_POLARITY_OFFSET+ 0;
static const int WGFMU_TRIGGER_OUT_POLARITY_NEGATIVE	= WGFMU_TRIGGER_OUT_POLARITY_OFFSET+ 1;

// WGFMU_createMergedPattern
static const int WGFMU_AXIS_OFFSET						= 9000;
static const int WGFMU_AXIS_TIME						= WGFMU_AXIS_OFFSET + 0;
static const int WGFMU_AXIS_VOLTAGE						= WGFMU_AXIS_OFFSET + 1;

// WGFMU_getStatus, WGFMU_getChannelStatus
static const int WGFMU_STATUS_OFFSET					= 10000;
static const int WGFMU_STATUS_COMPLETED					= WGFMU_STATUS_OFFSET + 0;
static const int WGFMU_STATUS_DONE						= WGFMU_STATUS_OFFSET + 1;
static const int WGFMU_STATUS_RUNNING					= WGFMU_STATUS_OFFSET + 2;
static const int WGFMU_STATUS_ABORT_COMPLETED			= WGFMU_STATUS_OFFSET + 3;
static const int WGFMU_STATUS_ABORTED					= WGFMU_STATUS_OFFSET + 4;
static const int WGFMU_STATUS_RUNNING_ILLEGAL			= WGFMU_STATUS_OFFSET + 5;
static const int WGFMU_STATUS_IDLE						= WGFMU_STATUS_OFFSET + 6;

// WGFMU_isMeasureEventCompleted
static const int WGFMU_MEASURE_EVENT_OFFSET				= 11000;
static const int WGFMU_MEASURE_EVENT_NOT_COMPLETED		= WGFMU_MEASURE_EVENT_OFFSET + 0;
static const int WGFMU_MEASURE_EVENT_COMPLETED			= WGFMU_MEASURE_EVENT_OFFSET + 1;

// WGFMU_setMeasureEvent
static const int WGFMU_MEASURE_EVENT_DATA_OFFSET		= 12000;
static const int WGFMU_MEASURE_EVENT_DATA_AVERAGED		= WGFMU_MEASURE_EVENT_DATA_OFFSET + 0;
static const int WGFMU_MEASURE_EVENT_DATA_RAW			= WGFMU_MEASURE_EVENT_DATA_OFFSET + 1;

// Common - Initialize

int WGFMUAPI WGFMU_openSession(const char* address);
int WGFMUAPI WGFMU_closeSession();
int WGFMUAPI WGFMU_initialize();
int WGFMUAPI WGFMU_setTimeout(double timeout);
int WGFMUAPI WGFMU_doSelfCalibration(int* result, char* detail, int* size);
int WGFMUAPI WGFMU_doSelfTest(int* result, char* detail, int* size);
int WGFMUAPI WGFMU_getChannelIdSize(int* size);
int WGFMUAPI WGFMU_getChannelIds(int* channelIds, int* size);

// Common - Error and Warning

int WGFMUAPI WGFMU_getErrorSize(int* size);
int WGFMUAPI WGFMU_getError(char* error, int* size);
int WGFMUAPI WGFMU_getErrorSummarySize(int* size);
int WGFMUAPI WGFMU_getErrorSummary(char* errorSummary, int* size);
int WGFMUAPI WGFMU_treatWarningsAsErrors(int warningLevel);
int WGFMUAPI WGFMU_setWarningLevel(int warningLevel);
int WGFMUAPI WGFMU_getWarningLevel(int* warningLevel);
int WGFMUAPI WGFMU_getWarningSummarySize(int* size);
int WGFMUAPI WGFMU_getWarningSummary(char* warningSummary, int* size);
int WGFMUAPI WGFMU_openLogFile(const char* fileName);
int WGFMUAPI WGFMU_closeLogFile();

// Common - Setup

int WGFMUAPI WGFMU_setOperationMode(int channelId,int operationMode);
int WGFMUAPI WGFMU_getOperationMode(int channelId, int* operationMode);
int WGFMUAPI WGFMU_setForceVoltageRange(int channelId, int forceVoltageRange);
int WGFMUAPI WGFMU_getForceVoltageRange(int channelId, int* forceVoltageRange);
int WGFMUAPI WGFMU_setMeasureMode(int channelId, int measureMode);
int WGFMUAPI WGFMU_getMeasureMode(int channelId, int* measureMode);
int WGFMUAPI WGFMU_setMeasureVoltageRange(int channelId, int measureVoltageRange);
int WGFMUAPI WGFMU_getMeasureVoltageRange(int channelId, int* measureVoltageRange);
int WGFMUAPI WGFMU_setMeasureCurrentRange(int channelId, int measureCurrentRange);
int WGFMUAPI WGFMU_getMeasureCurrentRange(int channelId, int* measureCurrentRange);
int WGFMUAPI WGFMU_setForceDelay(int channelId, double forceDelay);
int WGFMUAPI WGFMU_getForceDelay(int channelId, double* forceDelay);
int WGFMUAPI WGFMU_setMeasureDelay(int channelId, double measureDelay);
int WGFMUAPI WGFMU_getMeasureDelay(int channelId, double* measureDelay);
int WGFMUAPI WGFMU_setMeasureEnabled(int channelId, int measureEnabled);
int WGFMUAPI WGFMU_isMeasureEnabled(int channelId, int* measureEnabled);
int WGFMUAPI WGFMU_setTriggerOutMode(int channelId, int triggerOutMode, int polarity);
int WGFMUAPI WGFMU_getTriggerOutMode(int channelId, int* triggerOutMode, int* polarity);

// Common - Measurement

int WGFMUAPI WGFMU_connect(int channelId);
int WGFMUAPI WGFMU_disconnect(int channelId);

// WGFMU - Initialize

int WGFMUAPI WGFMU_clear();

// WGFMU - Setup - Pattern

int WGFMUAPI WGFMU_createPattern(const char* patternName, double initialVoltage);
int WGFMUAPI WGFMU_addVector(const char* patternName, double deltaTime, double voltage);
int WGFMUAPI WGFMU_addVectors(const char* patternName, double* deltaTimes, double* voltages, int size);
int WGFMUAPI WGFMU_setVector(const char* patternName, double time, double voltage);
int WGFMUAPI WGFMU_setVectors(const char* patternName, double* times, double* voltages, int size);

// WGFMU - Setup - Pattern Opeartion

int WGFMUAPI WGFMU_createMergedPattern(const char* patternName, const char* pattern1, const char* pattern2, int axis);
int WGFMUAPI WGFMU_createMultipliedPattern(const char* patternName, const char* pattern1, double timeFactor, double voltageFactor);
int WGFMUAPI WGFMU_createOffsetPattern(const char* patternName, const char* pattern1, double timeOffset, double voltageOffset);

// WGFMU - Setup - Event
	
int WGFMUAPI WGFMU_setMeasureEvent(const char* patternName, const char* eventName, double time, int measurementPoints, double measurementInterval, double averagingTime, int rawData);
int WGFMUAPI WGFMU_setRangeEvent(const char* patternName, const char* eventName, double time, int rangeIndex);
int WGFMUAPI WGFMU_setTriggerOutEvent(const char* patternName, const char* eventName, double time, double duration);

// WGFMU - Setup - Sequence

int WGFMUAPI WGFMU_addSequence(int channelId, const char* patternName, double loopCount);
int WGFMUAPI WGFMU_addSequences(int channelId, const char** patternNames, double* loopCounts, int size);

// WGFMU - Setup Check - Pattern

int WGFMUAPI WGFMU_getPatternForceValueSize(const char* patternName, int* size);
int WGFMUAPI WGFMU_getPatternForceValues(const char* patternName, int offset, int* size, double* forceTimes, double* forceValues);
int WGFMUAPI WGFMU_getPatternForceValue(const char* patternName, int index, double* forceTime, double* forceValue);
int WGFMUAPI WGFMU_getPatternInterpolatedForceValue(const char* patternName, double time, double* forceValue);
int WGFMUAPI WGFMU_getPatternMeasureTimeSize(const char* patternName, int* size);
int WGFMUAPI WGFMU_getPatternMeasureTimes(const char* patternName, int offset, int* size, double* measureTimes);
int WGFMUAPI WGFMU_getPatternMeasureTime(const char* patternName, int index, double* measureTime);

// WGFMU - Setup Check - Sequence

int WGFMUAPI WGFMU_getForceValueSize(int channelId, double* size);
int WGFMUAPI WGFMU_getForceValues(int channelId, double offset, int* size, double* forceTimes, double* forceValues);
int WGFMUAPI WGFMU_getForceValue(int channelId, double index, double* forceTime, double* forceValue);
int WGFMUAPI WGFMU_getInterpolatedForceValue(int channelId, double time, double* forceValue);
int WGFMUAPI WGFMU_getMeasureTimeSize(int channelId, int* size);
int WGFMUAPI WGFMU_getMeasureTimes(int channelId, int offset, int* size, double* measureTimes);
int WGFMUAPI WGFMU_getMeasureTime(int channelId, int index, double* measureTime);

// WGFMU - Setup Check - Event

int WGFMUAPI WGFMU_getMeasureEventSize(int channelId, int* size);
int WGFMUAPI WGFMU_getMeasureEvents(int channelId, int offset, int* size, char** patternNames, char** eventNames, int* cycles, double* loops, int* counts, int* offsets, int* sizes);
int WGFMUAPI WGFMU_getMeasureEvent(int channelId, int index, char* patternName, char* eventName, int* cycle, double* loop, int* count, int* offset, int* size);
int WGFMUAPI WGFMU_getMeasureEventAttribute(int channelId, int index, double* time, int* measurementPoints, double* measurementInterval, double* averagingTime, int* rawData);

// WGFMU - Import / Export

int WGFMUAPI WGFMU_exportAscii(const char* fileName);

// WGFMU - Measurement

int WGFMUAPI WGFMU_update();
int WGFMUAPI WGFMU_updateChannel(int channelId);
int WGFMUAPI WGFMU_execute();
int WGFMUAPI WGFMU_abort();
int WGFMUAPI WGFMU_abortChannel(int channelId);
int WGFMUAPI WGFMU_getStatus(int* status, double* elapsedTime, double* totalTime);
int WGFMUAPI WGFMU_getChannelStatus(int channelId, int* status, double* elapsedTime, double* totalTime);
int WGFMUAPI WGFMU_waitUntilCompleted();

// WGFMU - Data Retrieve - Measure Value

int WGFMUAPI WGFMU_getMeasureValueSize(int channelId, int* measuredSize, int* totalSize);
int WGFMUAPI WGFMU_getMeasureValues(int channelId, int offset, int* size, double* measureTimes, double* measureValues);
int WGFMUAPI WGFMU_getMeasureValue(int channelId, int index, double* measureTime, double* measureValue);

// WGFMU - Data Retrieve - Measure Event

int WGFMUAPI WGFMU_getCompletedMeasureEventSize(int channelId, int* measuredSize, int* totalSize);
int WGFMUAPI WGFMU_isMeasureEventCompleted(int channelId, const char* patternName, const char* eventName, int cycle, double loop, int count, int* completed, int* index, int* offset, int* size);

// DC - Measure

int WGFMUAPI WGFMU_dcforceVoltage(int channelId, double voltage);
int WGFMUAPI WGFMU_dcmeasureValue(int channelId, double* value);
int WGFMUAPI WGFMU_dcmeasureAveragedValue(int channelId, int count, int interval, double* value);

#endif//WGFMU_INSTRUMENTLIBRARY_H
