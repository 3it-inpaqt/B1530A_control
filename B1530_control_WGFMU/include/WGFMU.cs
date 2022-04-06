/*---------------------------------------------------------------------------*/
/* Distributed by Agilent Technologies                                       */
/*                                                                           */
/* Do not modify the contents of this file.                                  */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Title   : WGFMU.CS                                                        */
/* Date    : 10-30-2008                                                      */
/* Purpose : B1530A (WGFMU) Instrument Library API and Constant Definitions  */
/*           for Microsoft Visual C# .NET                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/

using System;

using System.Runtime.InteropServices;
using System.Text;

public class WGFMU
{
	// API Return Value - Error Code
	public const int NO_ERROR							= 0;
	public const int PARAMETER_OUT_OF_RANGE_ERROR		= -1;
	public const int ILLEGAL_STRING_ERROR				= -2;
	public const int CONTEXT_ERROR						= -3;
	public const int FUNCTION_NOT_SUPPORTED_ERROR		= -4;
	public const int COMMUNICATION_ERROR				= -5;
	public const int FW_ERROR							= -6;
	public const int LIBRARY_ERROR						= -7;
	public const int ERROR								= -8;
	public const int CHANNEL_NOT_FOUND_ERROR			= -9;
	public const int PATTERN_NOT_FOUND_ERROR			= -10;
	public const int EVENT_NOT_FOUND_ERROR				= -11;
	public const int PATTERN_ALREADY_EXISTS_ERROR		= -12;
	public const int SEQUENCER_NOT_RUNNING_ERROR		= -13;
	public const int RESULT_NOT_READY_ERROR				= -14;
	public const int RESULT_OUT_OF_DATE					= -15;

	public const int ERROR_CODE_MIN						= -9999;

	// WGFMU.doSelfCaliration, WGFMU.doSelfTest
	public const int PASS = 0;
	public const int FAIL = 1;

	// WGFMU.treatWarningsAsErrors, WGFMU.setWarningLevel
	public const int WARNING_LEVEL_OFFSET				= 1000;
	public const int WARNING_LEVEL_OFF					= WARNING_LEVEL_OFFSET + 0;
	public const int WARNING_LEVEL_SEVERE				= WARNING_LEVEL_OFFSET + 1;
	public const int WARNING_LEVEL_NORMAL				= WARNING_LEVEL_OFFSET + 2;
	public const int WARNING_LEVEL_INFORMATION			= WARNING_LEVEL_OFFSET + 3;

	// WGFMU.setOperationMode
	public const int OPERATION_MODE_OFFSET				= 2000;
	public const int OPERATION_MODE_DC					= OPERATION_MODE_OFFSET + 0;
	public const int OPERATION_MODE_FASTIV				= OPERATION_MODE_OFFSET + 1;
	public const int OPERATION_MODE_PG					= OPERATION_MODE_OFFSET + 2;
	public const int OPERATION_MODE_SMU					= OPERATION_MODE_OFFSET + 3;

	// WGFMU.setForceVoltageRange
	public const int FORCE_VOLTAGE_RANGE_OFFSET			= 3000;
	public const int FORCE_VOLTAGE_RANGE_AUTO			= FORCE_VOLTAGE_RANGE_OFFSET + 0;
	public const int FORCE_VOLTAGE_RANGE_3V				= FORCE_VOLTAGE_RANGE_OFFSET + 1;
	public const int FORCE_VOLTAGE_RANGE_5V				= FORCE_VOLTAGE_RANGE_OFFSET + 2;
	public const int FORCE_VOLTAGE_RANGE_10V_NEGATIVE	= FORCE_VOLTAGE_RANGE_OFFSET + 3;
	public const int FORCE_VOLTAGE_RANGE_10V_POSITIVE	= FORCE_VOLTAGE_RANGE_OFFSET + 4;

	// WGFMU.setMeasureMode
	public const int MEASURE_MODE_OFFSET				= 4000;
	public const int MEASURE_MODE_VOLTAGE				= MEASURE_MODE_OFFSET + 0;
	public const int MEASURE_MODE_CURRENT				= MEASURE_MODE_OFFSET + 1;

	// WGFMU.setMeasureVoltageRange
	public const int MEASURE_VOLTAGE_RANGE_OFFSET		= 5000;
	public const int MEASURE_VOLTAGE_RANGE_5V			= MEASURE_VOLTAGE_RANGE_OFFSET + 1;
	public const int MEASURE_VOLTAGE_RANGE_10V			= MEASURE_VOLTAGE_RANGE_OFFSET + 2;

	// WGFMU.setMeasureCurrentRange
	public const int MEASURE_CURRENT_RANGE_OFFSET		= 6000;
	public const int MEASURE_CURRENT_RANGE_1UA			= MEASURE_CURRENT_RANGE_OFFSET + 1;
	public const int MEASURE_CURRENT_RANGE_10UA			= MEASURE_CURRENT_RANGE_OFFSET + 2;
	public const int MEASURE_CURRENT_RANGE_100UA		= MEASURE_CURRENT_RANGE_OFFSET + 3;
	public const int MEASURE_CURRENT_RANGE_1MA			= MEASURE_CURRENT_RANGE_OFFSET + 4;
	public const int MEASURE_CURRENT_RANGE_10MA			= MEASURE_CURRENT_RANGE_OFFSET + 5;

	// WGFMU.setMeasureEnabled
	public const int MEASURE_ENABLED_OFFSET				= 7000;
	public const int MEASURE_ENABLED_DISABLE			= MEASURE_ENABLED_OFFSET + 0;
	public const int MEASURE_ENABLED_ENABLE				= MEASURE_ENABLED_OFFSET + 1;

	// WGFMU.setTriggerOutMode
	public const int TRIGGER_OUT_MODE_OFFSET			= 8000;
	public const int TRIGGER_OUT_MODE_DISABLE			= TRIGGER_OUT_MODE_OFFSET + 0;
	public const int TRIGGER_OUT_MODE_START_EXECUTION	= TRIGGER_OUT_MODE_OFFSET + 1;
	public const int TRIGGER_OUT_MODE_START_SEQUENCE	= TRIGGER_OUT_MODE_OFFSET + 2;
	public const int TRIGGER_OUT_MODE_START_PATTERN		= TRIGGER_OUT_MODE_OFFSET + 3;
	public const int TRIGGER_OUT_MODE_EVENT				= TRIGGER_OUT_MODE_OFFSET + 4;

	public const int TRIGGER_OUT_POLARITY_OFFSET		= 8100;
	public const int TRIGGER_OUT_POLARITY_POSITIVE		= TRIGGER_OUT_POLARITY_OFFSET + 0;
	public const int TRIGGER_OUT_POLARITY_NEGATIVE		= TRIGGER_OUT_POLARITY_OFFSET + 1;

	// WGFMU.createMergedPattern
	public const int AXIS_OFFSET						= 9000;
	public const int AXIS_TIME							= AXIS_OFFSET + 0;
	public const int AXIS_VOLTAGE						= AXIS_OFFSET + 1;

	// WGFMU.getStatus, WGFMU_getChannelStatus
	public const int STATUS_OFFSET						= 10000;
	public const int STATUS_COMPLETED					= STATUS_OFFSET + 0;
	public const int STATUS_DONE						= STATUS_OFFSET + 1;
	public const int STATUS_RUNNING						= STATUS_OFFSET + 2;
	public const int STATUS_ABORT_COMPLETED				= STATUS_OFFSET + 3;
	public const int STATUS_ABORTED						= STATUS_OFFSET + 4;
	public const int STATUS_RUNNING_ILLEGAL				= STATUS_OFFSET + 5;
	public const int STATUS_IDLE						= STATUS_OFFSET + 6;

	// WGFMU.isMeasureEventCompleted
	public const int MEASURE_EVENT_OFFSET				= 11000;
	public const int MEASURE_EVENT_NOT_COMPLETED		= MEASURE_EVENT_OFFSET + 0;
	public const int MEASURE_EVENT_COMPLETED			= MEASURE_EVENT_OFFSET + 1;

	// WGFMU.setMeasureEvent
	public const int MEASURE_EVENT_DATA_OFFSET			= 12000;
	public const int MEASURE_EVENT_DATA_AVERAGED		= MEASURE_EVENT_DATA_OFFSET + 0;
	public const int MEASURE_EVENT_DATA_RAW				= MEASURE_EVENT_DATA_OFFSET + 1;

	// Common - Initialize

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_openSession(String address);
	public static int openSession(String address)
	{
		return WGFMU_openSession(address);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_closeSession();
	public static int closeSession()
	{
		return WGFMU_closeSession();
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setTimeout(double timeout);
	public static int setTimeout(double timeout)
	{
		return WGFMU_setTimeout(timeout);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_initialize();
	public static int initialize()
	{
		return WGFMU_initialize();
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_doSelfCalibration(ref int result, StringBuilder detail, ref int size);
	public static int doSelfCalibration(ref int result, StringBuilder detail, ref int size)
	{
		return WGFMU_doSelfCalibration(ref result, detail, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_doSelfTest(ref int result, StringBuilder detail, ref int size);
	public static int doSelfTest(ref int result, StringBuilder detail, ref int size)
	{
		return WGFMU_doSelfTest(ref result, detail, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getChannelIdSize(ref int size);
	public static int getChannelIdSize(ref int size)
	{
		return WGFMU_getChannelIdSize(ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getChannelIds(int[] channelIds, ref int size);
	public static int getChannelIds(int[] channelIds, ref int size)
	{
		return WGFMU_getChannelIds(channelIds, ref size);
	}

	// Common - Error and Warning

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getErrorSize(ref int size);
	public static int getErrorSize(ref int size)
	{
		return WGFMU_getErrorSize(ref size);
		
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getError(StringBuilder error, ref int size);
	public static int getError(StringBuilder error, ref int size)
	{
		return WGFMU_getError(error, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getErrorSummarySize(ref int size);
	public static int getErrorSummarySize(ref int size)
	{
		return WGFMU_getErrorSummarySize(ref size);
		
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getErrorSummary(StringBuilder errorSummary, ref int size);
	public static int getErrorSummary(StringBuilder errorSummary, ref int size)
	{
		return WGFMU_getErrorSummary(errorSummary, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_treatWarningsAsErrors(int warningLevel);
	public static int treatWarningsAsErrors(int warningLevel)
	{
		return WGFMU_treatWarningsAsErrors(warningLevel);
		
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setWarningLevel(int warningLevel);
	public static int setWarningLevel(int warningLevel)
	{
		return WGFMU_setWarningLevel(warningLevel);
		
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getWarningLevel(ref int warningLevel);
	public static int getWarningLevel(ref int warningLevel)
	{
		return WGFMU_setWarningLevel(warningLevel);
		
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getWarningSummarySize(ref int size);
	public static int getWarningSummarySize(ref int size)
	{
		return WGFMU_getWarningSummarySize(ref size);
		
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getWarningSummary(StringBuilder warningSummary, ref int size);
	public static int getWarningSummary(StringBuilder errorSummary, ref int size)
	{
		return WGFMU_getWarningSummary(errorSummary, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_openLogFile(String logFile);
	public static int openLogFile(String logFile)
	{
		return WGFMU_openLogFile(logFile);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_closeLogFile();
	public static int closeLogFile()
	{
		return WGFMU_closeLogFile();
	}

	// Common - Setup

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setOperationMode(int channelId, int operationMode);
	public static int setOperationMode(int channelId, int operationMode)
	{
		return WGFMU_setOperationMode(channelId, operationMode);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getOperationMode(int channelId, ref int operationMode);
	public static int getOperationMode(int channelId, ref int operationMode)
	{
		return WGFMU_getOperationMode(channelId, ref operationMode);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setForceVoltageRange(int channelId, int forceVoltageRange);
	public static int setForceVoltageRange(int channelId, int forceVoltageRange)
	{
		return WGFMU_setForceVoltageRange(channelId, forceVoltageRange);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getForceVoltageRange(int channelId, ref int forceVoltageRange);
	public static int getForceVoltageRange(int channelId, ref int forceVoltageRange)
	{
		return WGFMU_getForceVoltageRange(channelId, ref forceVoltageRange);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setMeasureMode(int channelId, int measureMode);
	public static int setMeasureMode(int channelId, int measureMode)
	{
		return WGFMU_setMeasureMode(channelId, measureMode);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureMode(int channelId, ref int measureMode);
	public static int getMeasureMode(int channelId, ref int measureMode)
	{
		return WGFMU_getMeasureMode(channelId, ref measureMode);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setMeasureVoltageRange(int channelId, int measureVoltageRange);
	public static int setMeasureVoltageRange(int channelId, int measureVoltageRange)
	{
		return WGFMU_setMeasureVoltageRange(channelId, measureVoltageRange);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureVoltageRange(int channelId, ref int measureVoltageRange);
	public static int getMeasureVoltageRange(int channelId, ref int measureVoltageRange)
	{
		return WGFMU_getMeasureVoltageRange(channelId, ref measureVoltageRange);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setMeasureCurrentRange(int channelId, int measureCurrentRange);
	public static int setMeasureCurrentRange(int channelId, int measureCurrentRange)
	{
		return WGFMU_setMeasureCurrentRange(channelId, measureCurrentRange);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureCurrentRange(int channelId, ref int measureCurrentRange);
	public static int getMeasureCurrentRange(int channelId, ref int measureCurrentRange)
	{
		return WGFMU_getMeasureCurrentRange(channelId, ref measureCurrentRange);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setForceDelay(int channelId, double forceDelay);
	public static int setForceDelay(int channelId, double forceDelay)
	{
		return WGFMU_setForceDelay(channelId, forceDelay);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getForceDelay(int channelId, ref double forceDelay);
	public static int getForceDelay(int channelId, ref double forceDelay)
	{
		return WGFMU_getForceDelay(channelId, ref forceDelay);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setMeasureDelay(int channelId, double measureDelay);
	public static int setMeasureDelay(int channelId, double measureDelay)
	{
		return WGFMU_setMeasureDelay(channelId, measureDelay);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureDelay(int channelId, ref double measureDelay);
	public static int getMeasureDelay(int channelId, ref double measureDelay)
	{
		return WGFMU_getMeasureDelay(channelId, ref measureDelay);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setMeasureEnabled(int channelId, int measureEnabled);
	public static int setMeasureEnabled(int channelId, int measureEnabled)
	{
		return WGFMU_setMeasureEnabled(channelId, measureEnabled);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_isMeasureEnabled(int channelId, ref int measureEnabled);
	public static int isMeasureEnabled(int channelId, ref int measureEnabled)
	{
		return WGFMU_isMeasureEnabled(channelId, ref measureEnabled);
	}
	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setTriggerOutMode(int channelId, int triggerOutMode, int polarity);
	public static int setTriggerOutMode(int channelId, int triggerOutMode, int polarity)
	{
		return WGFMU_setTriggerOutMode(channelId, triggerOutMode, polarity);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getTriggerOutMode(int channelId, ref int triggerOutMode, ref int polarity);
	public static int getTriggerOutMode(int channelId, ref int triggerOutMode, ref int polarity)
	{
		return WGFMU_getTriggerOutMode(channelId, ref triggerOutMode, ref polarity);
	}

	// Common - Measurement

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_connect(int channelId);
	public static int connect(int channelId)
	{
		return WGFMU_connect(channelId);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_disconnect(int channelId);
	public static int disconnect(int channelId)
	{
		return WGFMU_disconnect(channelId);
	}

	// WGFMU - Initialize

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_clear();
	public static int clear()
	{
		return WGFMU_clear();
	}

	// WGFMU - Setup - Pattern

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_createPattern(String patternName, double initialVoltage);
	public static int createPattern(String patternName, double initialVoltage)
	{
		return WGFMU_createPattern(patternName, initialVoltage);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_addVector(String patternName, double delataTime, double voltage);
	public static int addVector(String patternName, double delataTime, double voltage)
	{
		return WGFMU_addVector(patternName, delataTime, voltage);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_addVectors(String patternName, double[] delataTimes, double[] voltages, int size);
	public static int addVectors(String patternName, double[] delataTimes, double[] voltages, int size)
	{
		return WGFMU_addVectors(patternName, delataTimes, voltages, size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setVector(String patternName, double time, double voltage);
	public static int setVector(String patternName, double time, double voltage)
	{
		return WGFMU_setVector(patternName, time, voltage);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setVectors(String patternName, double[] times, double[] voltages, int size);
	public static int setVectors(String patternName, double[] times, double[] voltages, int size)
	{
		return WGFMU_setVectors(patternName, times, voltages, size);
	}

	// WGFMU - Setup - Pattern Opeartion

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_createMergedPattern(String patternName, String pattern1, String pattern2, int axis);
	public static int createMergedPattern(String patternName, String pattern1, String pattern2, int axis)
	{
		return WGFMU_createMergedPattern(patternName, pattern1, pattern2, axis);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_createMultipliedPattern(String patternName, String pattern1, double timeFactor, double voltageFactor);
	public static int createMultipliedPattern(String patternName, String pattern1, double timeFactor, double voltageFactor)
	{
		return WGFMU_createMultipliedPattern(patternName, pattern1, timeFactor, voltageFactor);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_createOffsetPattern(String patternName, String pattern1, double timeOffset, double voltageOffset);
	public static int createOffsetPattern(String patternName, String pattern1, double timeOffset, double voltageOffset)
	{
		return WGFMU_createOffsetPattern(patternName, pattern1, timeOffset, voltageOffset);
	}

	// WGFMU - Setup - Event

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setMeasureEvent(String patternName, String eventName, double time, int measurementPoints, double measurementInterval, double averagingTime, int rawData);
	public static int setMeasureEvent(String patternName, String eventName, double time, int measurementPoints, double measurementInterval, double averagingTime, int rawData)
	{
		return WGFMU_setMeasureEvent(patternName, eventName, time, measurementPoints, measurementInterval, averagingTime, rawData);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setRangeEvent(String patternName, String eventName, double time, int rangeIndex);
	public static int setRangeEvent(String patternName, String eventName, double time, int rangeIndex)
	{
		return WGFMU_setRangeEvent(patternName, eventName, time, rangeIndex);
	}
 
	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_setTriggerOutEvent(String patternName, String eventName, double time, double duration);
	public static int setTriggerOutEvent(String patternName, String eventName, double time, double duration)
	{
		return WGFMU_setTriggerOutEvent(patternName, eventName, time, duration);
	}

	// WGFMU - Setup - Sequence

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_addSequence(int channelId, String patternName, double loopCount);
	public static int addSequence(int channelId, String patternName, double loopCount)
	{
		return WGFMU_addSequence(channelId, patternName, loopCount);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_addSequences(int channelId, String[] patternNames, double[] loopCounts, int size);
	public static int addSequences(int channelId, String[] patternNames, double[] loopCounts, int size)
	{
		return WGFMU_addSequences(channelId, patternNames, loopCounts, size);
	}

	// WGFMU - Setup Check - Pattern

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getPatternForceValueSize(String patternName, ref int size);
	public static int getPatternForceValueSize(String patternName, ref int size)
	{
		return WGFMU_getPatternForceValueSize(patternName, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getPatternForceValues(String patternName, int offset, ref int size, double[] forceTimes, double[] forceValues);
	public static int getPatternForceValues(String patternName, int offset, ref int size, double[] forceTimes, double[] forceValues)
	{
		return WGFMU_getPatternForceValues(patternName, offset, ref size, forceTimes, forceValues);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getPatternForceValue(String patternName, int index, ref double forceTime, ref double forceValue);
	public static int getPatternForceValue(String patternName, int index, ref double forceTime, ref double forceValue)
	{
		return WGFMU_getPatternForceValue(patternName, index, ref forceTime, ref forceValue);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getPatternInterpolatedForceValue(String patternName, double time, ref double forceValue);
	public static int getPatternInterpolatedForceValue(String patternName, double time, ref double forceValue)
	{
		return WGFMU_getPatternInterpolatedForceValue(patternName, time, ref forceValue);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getPatternMeasureTimeSize(String patternName, ref int size);
	public static int getPatternMeasureTimeSize(String patternName, ref int size)
	{
		return WGFMU_getPatternMeasureTimeSize(patternName, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getPatternMeasureTimes(String patternName, int offset, ref int size, double[] measureTimes);
	public static int getPatternMeasureTimes(String patternName, int offset, ref int size, double[] measureTimes)
	{
		return WGFMU_getPatternMeasureTimes(patternName, offset, ref size, measureTimes);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getPatternMeasureTime(String patternName, int index, ref double measureTime);
	public static int getPatternMeasureTime(String patternName, int index, ref double measureTime)
	{
		return WGFMU_getPatternMeasureTime(patternName, index, ref measureTime);
	}

	// WGFMU - Setup Check - Sequence

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getForceValueSize(int channelId, ref double size);
	public static int getForceValueSize(int channelId, ref double size)
	{
		return WGFMU_getForceValueSize(channelId, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getForceValues(int channelId, double offset, ref int size, double[] forceTimes, double[] forceValues);
	public static int getForceValues(int channelId, double offset, ref int size, double[] forceTimes, double[] forceValues)
	{
		return WGFMU_getForceValues(channelId, offset, ref size, forceTimes, forceValues);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getForceValue(int channelId, double index, ref double forceTime, ref double forceValue);
	public static int getForceValue(int channelId, double index, ref double forceTime, ref double forceValue)
	{
		return WGFMU_getForceValue(channelId, index, ref forceTime, ref forceValue);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getInterpolatedForceValue(int channelId, double time, ref double forceValue);
	public static int getInterpolatedForceValue(int channelId, double time, ref double forceValue)
	{
		return WGFMU_getInterpolatedForceValue(channelId, time, ref forceValue);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureTimeSize(int channelId, ref int size);
	public static int getMeasureTimeSize(int channelId, ref int size)
	{
		return WGFMU_getMeasureTimeSize(channelId, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureTimes(int channelId, int offset, ref int size, double[] measureTimes);
	public static int getMeasureTimes(int channelId, int offset, ref int size, double[] measureTimes)
	{
		return WGFMU_getMeasureTimes(channelId, offset, ref size, measureTimes);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureTime(int channelId, int index, ref double measureTime);
	public static int getMeasureTime(int channelId, int index, ref double measureTime)
	{
		return WGFMU_getMeasureTime(channelId, index, ref measureTime);
	}

	// WGFMU - Setup Check - Event

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureEventSize(int channelId, ref int size);
	public static int getMeasureEventSize(int channelId, ref int size)
	{
		return WGFMU_getMeasureEventSize(channelId, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureEvents(int channelId, int offset, ref int size, [In, Out] String[] patternNames, [In, Out] String[] eventNames, int[] cycles, double[] loops, int[] counts, int[] offsets, int[] sizes);
	public static int getMeasureEvents(int channelId, int offset, ref int size, String[] patternNames, String[] eventNames, int[] cycles, double[] loops, int[] counts, int[] offsets, int[] sizes)
	{
		return WGFMU_getMeasureEvents(channelId, offset, ref size, patternNames, eventNames, cycles, loops, counts, offsets, sizes);

	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureEvent(int channelId, int index, StringBuilder patternName, StringBuilder eventName, ref int cycle, ref double loop, ref int count, ref int offset, ref int size);
	public static int getMeasureEvent(int channelId, int index, StringBuilder patternName, StringBuilder eventName, ref int cycle, ref double loop, ref int count, ref int offset, ref int size)
	{
		return WGFMU_getMeasureEvent(channelId, index, patternName, eventName, ref cycle, ref loop, ref count, ref offset, ref size);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureEventAttribute(int channelId, int index, ref double time, ref int measurementPoints, ref double measurementInterval, ref double averagingTime, ref int rawData);
	public static int getMeasureEventAttribute(int channelId, int index, ref double time, ref int measurementPoints, ref double measurementInterval, ref double averagingTime, ref int rawData)
	{
		return WGFMU_getMeasureEventAttribute(channelId, index, ref time, ref measurementPoints, ref measurementInterval, ref averagingTime, ref rawData);
	}

	// WGFMU - Import / Export

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_exportAscii(String fileName);
	public static int exportAscii(String fileName)
	{
		return WGFMU_exportAscii(fileName);
	}

	// WGFMU - Measurement

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_update();
	public static int update()
	{
		return WGFMU_update();
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_updateChannel(int channelId);
	public static int updateChannel(int channelId)
	{
		return WGFMU_updateChannel(channelId);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_execute();
	public static int execute()
	{
		return WGFMU_execute();
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_abort();
	public static int abort()
	{
		return WGFMU_abort();
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_abortChannel(int channelId);
	public static int abortChannel(int channelId)
	{
		return WGFMU_abortChannel(channelId);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getStatus(ref int status, ref double elapsedTime, ref double totalTime);
	public static int getStatus(ref int status, ref double elapsedTime, ref double totalTime)
	{
		return WGFMU_getStatus(ref status, ref elapsedTime, ref totalTime);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getChannelStatus(int channelId, ref int status, ref double elapsedTime, ref double totalTime);
	public static int getChannelStatus(int channelId, ref int status, ref double elapsedTime, ref double totalTime)
	{
		return WGFMU_getChannelStatus(channelId, ref status, ref elapsedTime, ref totalTime);
	}
	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_waitUntilCompleted();
	public static int waitUntilCompleted()
	{
		return WGFMU_waitUntilCompleted();
	}

	// WGFMU - Data Retrieve - Measure Value

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureValueSize(int channelId, ref int measuredSize, ref int totalSize);
	public static int getMeasureValueSize(int channelId, ref int measuredSize, ref int totalSize)
	{
		return WGFMU_getMeasureValueSize(channelId, ref measuredSize, ref totalSize);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureValues(int channelId, int offset, ref int size, double[] measureTimes, double[] measureValues);
	public static int getMeasureValues(int channelId, int offset, ref int size, double[] measureTimes, double[] measureValues)
	{
		return WGFMU_getMeasureValues(channelId, offset, ref size, measureTimes, measureValues);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getMeasureValue(int channelId, int index, ref double measureTime, ref double measureValue);
	public static int getMeasureValue(int channelId, int index, ref double measureTime, ref double measureValue)
	{
		return WGFMU_getMeasureValue(channelId, index, ref measureTime, ref measureValue);
	}

	// WGFMU - Data Retrieve - Measure Event

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_getCompletedMeasureEventSize(int channelId, ref int measuredSize, ref int totalSize);
	public static int getCompletedMeasureEventSize(int channelId, ref int measuredSize, ref int totalSize)
	{
		return WGFMU_getCompletedMeasureEventSize(channelId, ref measuredSize, ref totalSize);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_isMeasureEventCompleted(int channelId, String patternName, String eventName, int cycle, double loop, int count, ref int completed, ref int index, ref int offset, ref int size);
	public static int isMeasureEventCompleted(int channelId, String patternName, String eventName, int cycle, double loop, int count, ref int completed, ref int index, ref int offset, ref int size)
	{
		return WGFMU_isMeasureEventCompleted(channelId, patternName, eventName, cycle, loop, count, ref completed, ref index, ref offset, ref size);
	}

	// DC - Measurement

	[DllImport("wgfmu.dll")]
	private static extern int  WGFMU_dcforceVoltage(int channelId, double voltage);
	public static int dcforceVoltage(int channelId, double voltage)
	{
		return WGFMU_dcforceVoltage(channelId, voltage);
	}

	[DllImport("wgfmu.dll")]
	private static extern int  WGFMU_dcmeasureValue(int channelId, ref double value);
	public static int dcmeasureValue(int channelId, ref double value)
	{
		return WGFMU_dcmeasureValue(channelId, ref value);
	}

	[DllImport("wgfmu.dll")]
	private static extern int WGFMU_dcmeasureAveragedValue(int channelId, int count, int interval, ref double value);
	public static int dcmeasureAveragedValue(int channelId, int count, int interval, ref double value)
	{
		return WGFMU_dcmeasureAveragedValue(channelId, count, interval, ref value);
	}
}
