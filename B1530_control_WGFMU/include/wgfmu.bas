Attribute VB_Name = "Module1"
' ---------------------------------------------------------------------------
'  Distributed by Agilent Technologies
'
'  Do not modify the contents of this file.
' ---------------------------------------------------------------------------
'
'  Title   : WGFMU.BAS
'  Date    : 10-30-2008
'  Purpose : B1530A (WGFMU) Instrument Library API and Constant Definitions
'            for Microsoft Visual Basic 6.0
'
' ---------------------------------------------------------------------------

' API Return Value - Error Code
Public Const WGFMU_NO_ERROR As Long = 0
Public Const WGFMU_PARAMETER_OUT_OF_RANGE_ERROR As Long = -1
Public Const WGFMU_ILLEGAL_STRING_ERROR As Long = -2
Public Const WGFMU_CONTEXT_ERROR As Long = -3
Public Const WGFMU_FUNCTION_NOT_SUPPORTED_ERROR As Long = -4
Public Const WGFMU_COMMUNICATION_ERROR As Long = -5
Public Const WGFMU_FW_ERROR As Long = -6
Public Const WGFMU_LIBRARY_ERROR As Long = -7
Public Const WGFMU_ERROR As Long = -8
Public Const WGFMU_CHANNEL_NOT_FOUND_ERROR As Long = -9
Public Const WGFMU_PATTERN_NOT_FOUND_ERROR As Long = -10
Public Const WGFMU_EVENT_NOT_FOUND_ERROR As Long = -11
Public Const WGFMU_PATTERN_ALREADY_EXISTS_ERROR As Long = -12
Public Const WGFMU_SEQUENCER_NOT_RUNNING_ERROR As Long = -13
Public Const WGFMU_RESULT_NOT_READY_ERROR As Long = -14
Public Const WGFMU_RESULT_OUT_OF_DATE As Long = -15

Public Const WGFMU_ERROR_CODE_MIN As Long = -9999

' WGFMU_doSelfCaliration, WGFMU_doSelfTest
Public Const WGFMU_PASS As Long = 0
Public Const WGFMU_FAIL As Long = 1

' WGFMU_treatWarningsAsErrors, WGFMU_setWarningLevel
Public Const WGFMU_WARNING_LEVEL_OFFSET As Long = 1000
Public Const WGFMU_WARNING_LEVEL_OFF As Long = WGFMU_WARNING_LEVEL_OFFSET + 0
Public Const WGFMU_WARNING_LEVEL_SEVERE As Long = WGFMU_WARNING_LEVEL_OFFSET + 1
Public Const WGFMU_WARNING_LEVEL_NORMAL As Long = WGFMU_WARNING_LEVEL_OFFSET + 2
Public Const WGFMU_WARNING_LEVEL_INFORMATION As Long = WGFMU_WARNING_LEVEL_OFFSET + 3

' WGFMU_setOperationMode
Public Const WGFMU_OPERATION_MODE_OFFSET As Long = 2000
Public Const WGFMU_OPERATION_MODE_DC As Long = WGFMU_OPERATION_MODE_OFFSET + 0
Public Const WGFMU_OPERATION_MODE_FASTIV As Long = WGFMU_OPERATION_MODE_OFFSET + 1
Public Const WGFMU_OPERATION_MODE_PG As Long = WGFMU_OPERATION_MODE_OFFSET + 2
Public Const WGFMU_OPERATION_MODE_SMU As Long = WGFMU_OPERATION_MODE_OFFSET + 3

' WGFMU_setForceVoltageRange
Public Const WGFMU_FORCE_VOLTAGE_RANGE_OFFSET As Long = 3000
Public Const WGFMU_FORCE_VOLTAGE_RANGE_AUTO As Long = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 0
Public Const WGFMU_FORCE_VOLTAGE_RANGE_3V As Long = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 1
Public Const WGFMU_FORCE_VOLTAGE_RANGE_5V As Long = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 2
Public Const WGFMU_FORCE_VOLTAGE_RANGE_10V_NEGATIVE As Long = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 3
Public Const WGFMU_FORCE_VOLTAGE_RANGE_10V_POSITIVE As Long = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 4

' WGFMU_setMeasureMode
Public Const WGFMU_MEASURE_MODE_OFFSET As Long = 4000
Public Const WGFMU_MEASURE_MODE_VOLTAGE As Long = WGFMU_MEASURE_MODE_OFFSET + 0
Public Const WGFMU_MEASURE_MODE_CURRENT As Long = WGFMU_MEASURE_MODE_OFFSET + 1

' WGFMU_setMeasureVoltageRange
Public Const WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET As Long = 5000
Public Const WGFMU_MEASURE_VOLTAGE_RANGE_5V As Long = WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET + 1
Public Const WGFMU_MEASURE_VOLTAGE_RANGE_10V As Long = WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET + 2

' WGFMU_setMeasureCurrentRange
Public Const WGFMU_MEASURE_CURRENT_RANGE_OFFSET As Long = 6000
Public Const WGFMU_MEASURE_CURRENT_RANGE_1UA As Long = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 1
Public Const WGFMU_MEASURE_CURRENT_RANGE_10UA As Long = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 2
Public Const WGFMU_MEASURE_CURRENT_RANGE_100UA As Long = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 3
Public Const WGFMU_MEASURE_CURRENT_RANGE_1MA As Long = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 4
Public Const WGFMU_MEASURE_CURRENT_RANGE_10MA As Long = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 5

' WGFMU_setMeasureEnabled
Public Const WGFMU_MEASURE_ENABLED_OFFSET As Long = 7000
Public Const WGFMU_MEASURE_ENABLED_DISABLE As Long = WGFMU_MEASURE_ENABLED_OFFSET + 0
Public Const WGFMU_MEASURE_ENABLED_ENABLE As Long = WGFMU_MEASURE_ENABLED_OFFSET + 1

' WGFMU_setTriggerOutMode
Public Const WGFMU_TRIGGER_OUT_MODE_OFFSET As Long = 8000
Public Const WGFMU_TRIGGER_OUT_MODE_DISABLE As Long = WGFMU_TRIGGER_OUT_MODE_OFFSET + 0
Public Const WGFMU_TRIGGER_OUT_MODE_START_EXECUTION As Long = WGFMU_TRIGGER_OUT_MODE_OFFSET + 1
Public Const WGFMU_TRIGGER_OUT_MODE_START_SEQUENCE As Long = WGFMU_TRIGGER_OUT_MODE_OFFSET + 2
Public Const WGFMU_TRIGGER_OUT_MODE_START_PATTERN As Long = WGFMU_TRIGGER_OUT_MODE_OFFSET + 3
Public Const WGFMU_TRIGGER_OUT_MODE_EVENT As Long = WGFMU_TRIGGER_OUT_MODE_OFFSET + 4

Public Const WGFMU_TRIGGER_OUT_POLARITY_OFFSET As Long = 8100
Public Const WGFMU_TRIGGER_OUT_POLARITY_POSITIVE As Long = WGFMU_TRIGGER_OUT_POLARITY_OFFSET + 0
Public Const WGFMU_TRIGGER_OUT_POLARITY_NEGATIVE As Long = WGFMU_TRIGGER_OUT_POLARITY_OFFSET + 1

' WGFMU_createMergedPattern
Public Const WGFMU_AXIS_OFFSET As Long = 9000
Public Const WGFMU_AXIS_TIME As Long = WGFMU_AXIS_OFFSET + 0
Public Const WGFMU_AXIS_VOLTAGE As Long = WGFMU_AXIS_OFFSET + 1

' WGFMU_getStatus, WGFMU_getChannelStatus
Public Const WGFMU_STATUS_OFFSET As Long = 10000
Public Const WGFMU_STATUS_COMPLETED As Long = WGFMU_STATUS_OFFSET + 0
Public Const WGFMU_STATUS_DONE As Long = WGFMU_STATUS_OFFSET + 1
Public Const WGFMU_STATUS_RUNNING As Long = WGFMU_STATUS_OFFSET + 2
Public Const WGFMU_STATUS_ABORT_COMPLETED As Long = WGFMU_STATUS_OFFSET + 3
Public Const WGFMU_STATUS_ABORTED As Long = WGFMU_STATUS_OFFSET + 4
Public Const WGFMU_STATUS_RUNNING_ILLEGAL As Long = WGFMU_STATUS_OFFSET + 5
Public Const WGFMU_STATUS_IDLE As Long = WGFMU_STATUS_OFFSET + 6

' WGFMU_isMeasureEventCompleted
Public Const WGFMU_MEASURE_EVENT_OFFSET As Long = 11000
Public Const WGFMU_MEASURE_EVENT_NOT_COMPLETED As Long = WGFMU_MEASURE_EVENT_OFFSET + 0
Public Const WGFMU_MEASURE_EVENT_COMPLETED As Long = WGFMU_MEASURE_EVENT_OFFSET + 1
    
' WGFMU_setMeasureEvent
Public Const WGFMU_MEASURE_EVENT_DATA_OFFSET As Long = 12000
Public Const WGFMU_MEASURE_EVENT_DATA_AVERAGED As Long = WGFMU_MEASURE_EVENT_DATA_OFFSET + 0
Public Const WGFMU_MEASURE_EVENT_DATA_RAW As Long = WGFMU_MEASURE_EVENT_DATA_OFFSET + 1

' Common - Initialize

Public Declare Function WGFMU_openSession Lib "wgfmu.dll" (ByVal address As String) As Long
Public Declare Function WGFMU_closeSession Lib "wgfmu.dll" () As Long
Public Declare Function WGFMU_setTimeout Lib "wgfmu.dll" (ByVal timeout As Double) As Long
Public Declare Function WGFMU_initialize Lib "wgfmu.dll" () As Long
Public Declare Function WGFMU_doSelfCalibration Lib "wgfmu.dll" (ByRef result As Long, ByVal detail As String, ByRef size As Long) As Long
Public Declare Function WGFMU_doSelfTest Lib "wgfmu.dll" (ByRef result As Long, ByVal detail As String, ByRef size As Long) As Long
Public Declare Function WGFMU_getChannelIdSize Lib "wgfmu.dll" (ByRef size As Long) As Long
Public Declare Function WGFMU_getChannelIds Lib "wgfmu.dll" (ByRef channelIds As Long, ByRef size As Long) As Long

' Common - Error and Warning

Public Declare Function WGFMU_getErrorSize Lib "wgfmu.dll" (ByRef size As Long) As Long
Public Declare Function WGFMU_getError Lib "wgfmu.dll" (ByVal errorString As String, ByRef size As Long) As Long
Public Declare Function WGFMU_getErrorSummarySize Lib "wgfmu.dll" (ByRef size As Long) As Long
Public Declare Function WGFMU_getErrorSummary Lib "wgfmu.dll" (ByVal errorSummary As String, ByRef size As Long) As Long
Public Declare Function WGFMU_treatWarningsAsErrors Lib "wgfmu.dll" (ByVal warningLevel As Long) As Long
Public Declare Function WGFMU_setWarningLevel Lib "wgfmu.dll" (ByVal warningLevel As Long) As Long
Public Declare Function WGFMU_getWarningLevel Lib "wgfmu.dll" (ByRef warningLevel As Long) As Long
Public Declare Function WGFMU_getWarningSummarySize Lib "wgfmu.dll" (ByRef size As Long) As Long
Public Declare Function WGFMU_getWarningSummary Lib "wgfmu.dll" (ByVal warningSummary As String, ByRef size As Long) As Long
Public Declare Function WGFMU_openLogFile Lib "wgfmu.dll" (ByVal fileName As String) As Long
Public Declare Function WGFMU_closeLogFile Lib "wgfmu.dll" () As Long

' Common - Setup

Public Declare Function WGFMU_setOperationMode Lib "wgfmu.dll" (ByVal channelId As Long, ByVal operationMode As Long) As Long
Public Declare Function WGFMU_getOperationMode Lib "wgfmu.dll" (ByVal channelId As Long, ByRef operationMode As Long) As Long
Public Declare Function WGFMU_setForceVoltageRange Lib "wgfmu.dll" (ByVal channelId As Long, ByVal forceVoltageRange As Long) As Long
Public Declare Function WGFMU_getForceVoltageRange Lib "wgfmu.dll" (ByVal channelId As Long, ByRef forceVoltageRange As Long) As Long
Public Declare Function WGFMU_setMeasureMode Lib "wgfmu.dll" (ByVal channelId As Long, ByVal measureMode As Long) As Long
Public Declare Function WGFMU_getMeasureMode Lib "wgfmu.dll" (ByVal channelId As Long, ByRef measureMode As Long) As Long
Public Declare Function WGFMU_setMeasureVoltageRange Lib "wgfmu.dll" (ByVal channelId As Long, ByVal measureVoltageRange As Long) As Long
Public Declare Function WGFMU_getMeasureVoltageRange Lib "wgfmu.dll" (ByVal channelId As Long, ByRef measureVoltageRange As Long) As Long
Public Declare Function WGFMU_setMeasureCurrentRange Lib "wgfmu.dll" (ByVal channelId As Long, ByVal measureCurrentRange As Long) As Long
Public Declare Function WGFMU_getMeasureCurrentRange Lib "wgfmu.dll" (ByVal channelId As Long, ByRef measureCurrentRange As Long) As Long
Public Declare Function WGFMU_setForceDelay Lib "wgfmu.dll" (ByVal channelId As Long, ByVal forceDelay As Double) As Long
Public Declare Function WGFMU_getForceDelay Lib "wgfmu.dll" (ByVal channelId As Long, ByRef forceDelay As Double) As Long
Public Declare Function WGFMU_setMeasureDelay Lib "wgfmu.dll" (ByVal channelId As Long, ByVal measureDelay As Double) As Long
Public Declare Function WGFMU_getMeasureDelay Lib "wgfmu.dll" (ByVal channelId As Long, ByRef measureDelay As Double) As Long
Public Declare Function WGFMU_setMeasureEnabled Lib "wgfmu.dll" (ByVal channelId As Long, ByVal measureEnabled As Long) As Long
Public Declare Function WGFMU_isMeasureEnabled Lib "wgfmu.dll" (ByVal channelId As Long, ByRef measureEnabled As Long) As Long
Public Declare Function WGFMU_setTriggerOutMode Lib "wgfmu.dll" (ByVal channelId As Long, ByVal triggerOutMode As Long, ByVal polarity As Long) As Long
Public Declare Function WGFMU_getTriggerOutMode Lib "wgfmu.dll" (ByVal channelId As Long, ByRef triggerOutMode As Long, ByRef polarity As Long) As Long

' Common - Measurement

Public Declare Function WGFMU_connect Lib "wgfmu.dll" (ByVal channelId As Long) As Long
Public Declare Function WGFMU_disconnect Lib "wgfmu.dll" (ByVal channelId As Long) As Long

' WGFMU - Initialize

Public Declare Function WGFMU_clear Lib "wgfmu.dll" () As Long

' WGFMU - Setup - Pattern

Public Declare Function WGFMU_createPattern Lib "wgfmu.dll" (ByVal patternName As String, ByVal initialVoltage As Double) As Long
Public Declare Function WGFMU_addVector Lib "wgfmu.dll" (ByVal patternName As String, ByVal deltaTime As Double, ByVal voltage As Double) As Long
Public Declare Function WGFMU_addVectors Lib "wgfmu.dll" (ByVal patternName As String, ByRef deltaTimes As Double, ByRef voltages As Double, ByVal size As Long) As Long
Public Declare Function WGFMU_setVector Lib "wgfmu.dll" (ByVal patternName As String, ByVal time As Double, ByVal voltage As Double) As Long
Public Declare Function WGFMU_setVectors Lib "wgfmu.dll" (ByVal patternName As String, ByRef times As Double, ByRef voltages As Double, ByVal size As Long) As Long

' WGFMU - Setup - Pattern Opeartion

Public Declare Function WGFMU_createMergedPattern Lib "wgfmu.dll" (ByVal patternName As String, ByVal pattern1 As String, ByVal pattern2 As String, ByVal axis As Long) As Long
Public Declare Function WGFMU_createMultipliedPattern Lib "wgfmu.dll" (ByVal patternName As String, ByVal pattern1 As String, ByVal timeFactor As Double, ByVal voltageFactor As Double) As Long
Public Declare Function WGFMU_createOffsetPattern Lib "wgfmu.dll" (ByVal patternName As String, ByVal pattern1 As String, ByVal timeOffset As Double, ByVal voltageOffset As Double) As Long

' WGFMU - Setup - Event

Public Declare Function WGFMU_setMeasureEvent Lib "wgfmu.dll" (ByVal patternName As String, ByVal eventName As String, ByVal time As Double, ByVal measurementPoints As Long, ByVal measurementInterval As Double, ByVal averagingTime As Double, ByVal rawData As Long) As Long
Public Declare Function WGFMU_setRangeEvent Lib "wgfmu.dll" (ByVal patternName As String, ByVal eventName As String, ByVal time As Double, ByVal rangeIndex As Long) As Long
Public Declare Function WGFMU_setTriggerOutEvent Lib "wgfmu.dll" (ByVal patternName As String, ByVal eventName As String, ByVal time As Double, ByVal duration As Double) As Long

' WGFMU - Setup - Sequence

Public Declare Function WGFMU_addSequence Lib "wgfmu.dll" (ByVal channelId As Long, ByVal patternName As String, ByVal loopCount As Double) As Long
Private Declare Function WGFMU_addSequences2 Lib "wgfmu.dll" (ByVal channelId As Long, ByVal patternNames As Long, ByRef loopCounts As Double, ByVal size As Long) As Long

' WGFMU - Setup Check - Pattern

Public Declare Function WGFMU_getPatternForceValueSize Lib "wgfmu.dll" (ByVal patternName As String, ByRef size As Long) As Long
Public Declare Function WGFMU_getPatternForceValues Lib "wgfmu.dll" (ByVal patternName As String, ByVal offset As Long, ByRef size As Long, ByRef forceTimes As Double, ByRef forceValues As Double) As Long
Public Declare Function WGFMU_getPatternForceValue Lib "wgfmu.dll" (ByVal patternName As String, ByVal index As Long, ByRef forceTime As Double, ByRef forceValue As Double) As Long
Public Declare Function WGFMU_getPatternInterpolatedForceValue Lib "wgfmu.dll" (ByVal patternName As String, ByVal time As Double, ByRef forceValue As Double) As Long
Public Declare Function WGFMU_getPatternMeasureTimeSize Lib "wgfmu.dll" (ByVal patternName As String, ByRef size As Long) As Long
Public Declare Function WGFMU_getPatternMeasureTimes Lib "wgfmu.dll" (ByVal patternName As String, ByVal offset As Long, ByRef size As Long, ByRef measureTimes As Double) As Long
Public Declare Function WGFMU_getPatternMeasureTime Lib "wgfmu.dll" (ByVal patternName As String, ByVal index As Long, ByRef measureTime As Double) As Long

' WGFMU - Setup Check - Sequence

Public Declare Function WGFMU_getForceValueSize Lib "wgfmu.dll" (ByVal channelId As Long, ByRef size As Double) As Long
Public Declare Function WGFMU_getForceValues Lib "wgfmu.dll" (ByVal channelId As Long, ByVal offset As Double, ByRef size As Long, ByRef forceTimes As Double, ByRef forceValues As Double) As Long
Public Declare Function WGFMU_getForceValue Lib "wgfmu.dll" (ByVal channelId As Long, ByVal index As Double, ByRef forceTime As Double, ByRef forceValue As Double) As Long
Public Declare Function WGFMU_getInterpolatedForceValue Lib "wgfmu.dll" (ByVal channelId As Long, ByVal time As Double, ByRef forceValue As Double) As Long
Public Declare Function WGFMU_getMeasureTimeSize Lib "wgfmu.dll" (ByVal channelId As Long, ByRef size As Long) As Long
Public Declare Function WGFMU_getMeasureTimes Lib "wgfmu.dll" (ByVal channelId As Long, ByVal offset As Long, ByRef size As Long, ByRef measureTimes As Double) As Long
Public Declare Function WGFMU_getMeasureTime Lib "wgfmu.dll" (ByVal channelId As Long, ByVal index As Long, ByRef measureTime As Double) As Long

' WGFMU - Setup Check - Event

Public Declare Function WGFMU_getMeasureEventSize Lib "wgfmu.dll" (ByVal channelId As Long, ByRef size As Long) As Long
Private Declare Function WGFMU_getMeasureEvents2 Lib "wgfmu.dll" (ByVal channelId As Long, ByVal offset As Long, ByRef size As Long, ByVal patternNames As Long, ByVal eventNames As Long, ByRef cycles As Long, ByRef loopIndices As Double, ByRef counts As Long, ByRef offsets As Long, ByRef sizes As Long) As Long
Public Declare Function WGFMU_getMeasureEvent Lib "wgfmu.dll" (ByVal channelId As Long, ByVal index As Long, ByVal patternName As String, ByVal eventName As String, ByRef cycle As Long, ByRef loopIndex As Double, ByRef count As Long, ByRef offset As Long, ByRef size As Long) As Long
Public Declare Function WGFMU_getMeasureEventAttribute Lib "wgfmu.dll" (ByVal channelId As Long, ByVal index As Long, ByRef time As Double, ByRef measurementPoints As Long, ByRef measurementInterval As Double, ByRef averagingTime As Double, ByRef rawData As Long)

' WGFMU - Import / Export

Public Declare Function WGFMU_exportAscii Lib "wgfmu.dll" (ByVal fileName As String) As Long

' WGFMU - Measurement

Public Declare Function WGFMU_update Lib "wgfmu.dll" () As Long
Public Declare Function WGFMU_updateChannel Lib "wgfmu.dll" (ByVal channelId As Long) As Long
Public Declare Function WGFMU_execute Lib "wgfmu.dll" () As Long
Public Declare Function WGFMU_abort Lib "wgfmu.dll" () As Long
Public Declare Function WGFMU_abortChannel Lib "wgfmu.dll" (ByVal channelId As Long) As Long
Public Declare Function WGFMU_getStatus Lib "wgfmu.dll" (ByRef status As Long, ByRef elapsedTime As Double, ByRef totalTime As Double) As Long
Public Declare Function WGFMU_getChannelStatus Lib "wgfmu.dll" (ByVal channelId As Long, ByRef status As Long, ByRef elapsedTime As Double, ByRef totalTime As Double) As Long
Public Declare Function WGFMU_waitUntilCompleted Lib "wgfmu.dll" () As Long

' WGFMU - Data Retrieve - Measure Value

Public Declare Function WGFMU_getMeasureValueSize Lib "wgfmu.dll" (ByVal channelId As Long, ByRef measuredSize As Long, ByRef totalSize As Long) As Long
Public Declare Function WGFMU_getMeasureValues Lib "wgfmu.dll" (ByVal channelId As Long, ByVal offset As Long, ByRef size As Long, ByRef measureTimes As Double, ByRef measureValues As Double) As Long
Public Declare Function WGFMU_getMeasureValue Lib "wgfmu.dll" (ByVal channelId As Long, ByVal index As Long, ByRef measureTime As Double, ByRef measureValue As Double) As Long

' WGFMU - Data Retrieve - Measure Event

Public Declare Function WGFMU_getCompletedMeasureEventSize Lib "wgfmu.dll" (ByVal channelId As Long, ByRef measuredSize As Long, ByRef totalSize As Long) As Long
Public Declare Function WGFMU_isMeasureEventCompleted Lib "wgfmu.dll" (ByVal channelId As Long, ByVal patternName As String, ByVal eventName As String, ByVal cycle As Long, ByVal loopIndex As Double, ByVal count As Long, ByRef completed As Long, ByRef index As Long, ByRef offset As Long, ByRef size As Long) As Long

' DC - Measurement

Public Declare Function WGFMU_dcforceVoltage Lib "wgfmu.dll" (ByVal channelId As Long, ByVal voltage As Double) As Long
Public Declare Function WGFMU_dcmeasureValue Lib "wgfmu.dll" (ByVal channelId As Long, ByRef value As Double) As Long
Public Declare Function WGFMU_dcmeasureAveragedValue Lib "wgfmu.dll" (ByVal channelId As Long, ByVal count As Long, ByVal interval As Long, ByRef value As Double) As Long

' Wrappers to support String Array (VarPtrStringArray) Argumnts

Public Function WGFMU_addSequences(ByVal channelId As Long, ByVal patternNames As Long, ByRef loopCounts As Double, ByVal size As Long) As Long
    WGFMU_addSequences = WGFMU_addSequences2(channelId, patternNames, loopCounts, size)
End Function

Public Function WGFMU_getMeasureEvents(ByVal channelId As Long, ByVal offset As Long, ByRef size As Long, ByVal patternNames As Long, ByVal eventNames As Long, ByRef cycles As Long, ByRef loopIndices As Double, ByRef counts As Long, ByRef offsets As Long, ByRef sizes As Long) As Long
    WGFMU_getMeasureEvents = WGFMU_getMeasureEvents2(channelId, offset, size, patternNames, eventNames, cycles, loopIndices, counts, offsets, sizes)
End Function
