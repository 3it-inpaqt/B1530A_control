' ---------------------------------------------------------------------------
'  Distributed by Agilent Technologies                                       
'                                                                            
'  Do not modify the contents of this file.                                  
' ---------------------------------------------------------------------------
'                                                                            
'  Title   : WGFMU.VB                                                        
'  Date    : 10-30-2008                                                      
'  Purpose : B1530A (WGFMU) Instrument Library API and Constant Definitions  
'            for Microsoft Visual Basic .NET                                 
'                                                                            
' ---------------------------------------------------------------------------

Module WGFMU

    ' API Return Value - Error Code
    Public Const WGFMU_NO_ERROR As Integer = 0
    Public Const WGFMU_PARAMETER_OUT_OF_RANGE_ERROR As Integer = -1
    Public Const WGFMU_ILLEGAL_STRING_ERROR As Integer = -2
    Public Const WGFMU_CONTEXT_ERROR As Integer = -3
    Public Const WGFMU_FUNCTION_NOT_SUPPORTED_ERROR As Integer = -4
    Public Const WGFMU_COMMUNICATION_ERROR As Integer = -5
    Public Const WGFMU_FW_ERROR As Integer = -6
    Public Const WGFMU_LIBRARY_ERROR As Integer = -7
    Public Const WGFMU_ERROR As Integer = -8
    Public Const WGFMU_CHANNEL_NOT_FOUND_ERROR As Integer = -9
    Public Const WGFMU_PATTERN_NOT_FOUND_ERROR As Integer = -10
    Public Const WGFMU_EVENT_NOT_FOUND_ERROR As Integer = -11
    Public Const WGFMU_PATTERN_ALREADY_EXISTS_ERROR As Integer = -12
    Public Const WGFMU_SEQUENCER_NOT_RUNNING_ERROR As Integer = -13
    Public Const WGFMU_RESULT_NOT_READY_ERROR As Integer = -14
    Public Const WGFMU_RESULT_OUT_OF_DATE As Integer = -15

    Public Const WGFMU_ERROR_CODE_MIN As Integer = -9999

    ' WGFMU_doSelfCaliration, WGFMU_doSelfTest
    Public Const WGFMU_PASS As Integer = 0
    Public Const WGFMU_FAIL As Integer = 1

    ' WGFMU_treatWarningsAsErrors, WGFMU_setWarningLevel
    Public Const WGFMU_WARNING_LEVEL_OFFSET As Integer = 1000
    Public Const WGFMU_WARNING_LEVEL_OFF As Integer = WGFMU_WARNING_LEVEL_OFFSET + 0
    Public Const WGFMU_WARNING_LEVEL_SEVERE As Integer = WGFMU_WARNING_LEVEL_OFFSET + 1
    Public Const WGFMU_WARNING_LEVEL_NORMAL As Integer = WGFMU_WARNING_LEVEL_OFFSET + 2
    Public Const WGFMU_WARNING_LEVEL_INFORMATION As Integer = WGFMU_WARNING_LEVEL_OFFSET + 3

    ' WGFMU_setOperationMode
    Public Const WGFMU_OPERATION_MODE_OFFSET As Integer = 2000
    Public Const WGFMU_OPERATION_MODE_DC As Integer = WGFMU_OPERATION_MODE_OFFSET + 0
    Public Const WGFMU_OPERATION_MODE_FASTIV As Integer = WGFMU_OPERATION_MODE_OFFSET + 1
    Public Const WGFMU_OPERATION_MODE_PG As Integer = WGFMU_OPERATION_MODE_OFFSET + 2
    Public Const WGFMU_OPERATION_MODE_SMU As Integer = WGFMU_OPERATION_MODE_OFFSET + 3

    ' WGFMU_setForceVoltageRange
    Public Const WGFMU_FORCE_VOLTAGE_RANGE_OFFSET As Integer = 3000
    Public Const WGFMU_FORCE_VOLTAGE_RANGE_AUTO As Integer = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 0
    Public Const WGFMU_FORCE_VOLTAGE_RANGE_3V As Integer = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 1
    Public Const WGFMU_FORCE_VOLTAGE_RANGE_5V As Integer = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 2
    Public Const WGFMU_FORCE_VOLTAGE_RANGE_10V_NEGATIVE As Integer = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 3
    Public Const WGFMU_FORCE_VOLTAGE_RANGE_10V_POSITIVE As Integer = WGFMU_FORCE_VOLTAGE_RANGE_OFFSET + 4

    ' WGFMU_setMeasureMode
    Public Const WGFMU_MEASURE_MODE_OFFSET As Integer = 4000
    Public Const WGFMU_MEASURE_MODE_VOLTAGE As Integer = WGFMU_MEASURE_MODE_OFFSET + 0
    Public Const WGFMU_MEASURE_MODE_CURRENT As Integer = WGFMU_MEASURE_MODE_OFFSET + 1

    ' WGFMU_setMeasureVoltageRange
    Public Const WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET As Integer = 5000
    Public Const WGFMU_MEASURE_VOLTAGE_RANGE_5V As Integer = WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET + 1
    Public Const WGFMU_MEASURE_VOLTAGE_RANGE_10V As Integer = WGFMU_MEASURE_VOLTAGE_RANGE_OFFSET + 2

    ' WGFMU_setMeasureCurrentRange
    Public Const WGFMU_MEASURE_CURRENT_RANGE_OFFSET As Integer = 6000
    Public Const WGFMU_MEASURE_CURRENT_RANGE_1UA As Integer = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 1
    Public Const WGFMU_MEASURE_CURRENT_RANGE_10UA As Integer = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 2
    Public Const WGFMU_MEASURE_CURRENT_RANGE_100UA As Integer = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 3
    Public Const WGFMU_MEASURE_CURRENT_RANGE_1MA As Integer = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 4
    Public Const WGFMU_MEASURE_CURRENT_RANGE_10MA As Integer = WGFMU_MEASURE_CURRENT_RANGE_OFFSET + 5

    ' WGFMU_setMeasureEnabled
    Public Const WGFMU_MEASURE_ENABLED_OFFSET As Integer = 7000
    Public Const WGFMU_MEASURE_ENABLED_DISABLE As Integer = WGFMU_MEASURE_ENABLED_OFFSET + 0
    Public Const WGFMU_MEASURE_ENABLED_ENABLE As Integer = WGFMU_MEASURE_ENABLED_OFFSET + 1

    ' WGFMU_setTriggerOutMode
    Public Const WGFMU_TRIGGER_OUT_MODE_OFFSET As Integer = 8000
    Public Const WGFMU_TRIGGER_OUT_MODE_DISABLE As Integer = WGFMU_TRIGGER_OUT_MODE_OFFSET + 0
    Public Const WGFMU_TRIGGER_OUT_MODE_START_EXECUTION As Integer = WGFMU_TRIGGER_OUT_MODE_OFFSET + 1
    Public Const WGFMU_TRIGGER_OUT_MODE_START_SEQUENCE As Integer = WGFMU_TRIGGER_OUT_MODE_OFFSET + 2
    Public Const WGFMU_TRIGGER_OUT_MODE_START_PATTERN As Integer = WGFMU_TRIGGER_OUT_MODE_OFFSET + 3
    Public Const WGFMU_TRIGGER_OUT_MODE_EVENT As Integer = WGFMU_TRIGGER_OUT_MODE_OFFSET + 4

    Public Const WGFMU_TRIGGER_OUT_POLARITY_OFFSET As Integer = 8100
    Public Const WGFMU_TRIGGER_OUT_POLARITY_POSITIVE As Integer = WGFMU_TRIGGER_OUT_POLARITY_OFFSET + 0
    Public Const WGFMU_TRIGGER_OUT_POLARITY_NEGATIVE As Integer = WGFMU_TRIGGER_OUT_POLARITY_OFFSET + 1

    ' WGFMU_createMergedPattern
    Public Const WGFMU_AXIS_OFFSET As Integer = 9000
    Public Const WGFMU_AXIS_TIME As Integer = WGFMU_AXIS_OFFSET + 0
    Public Const WGFMU_AXIS_VOLTAGE As Integer = WGFMU_AXIS_OFFSET + 1

    ' WGFMU_getStatus, WGFMU_getChannelStatus
    Public Const WGFMU_STATUS_OFFSET As Integer = 10000
    Public Const WGFMU_STATUS_COMPLETED As Integer = WGFMU_STATUS_OFFSET + 0
    Public Const WGFMU_STATUS_DONE As Integer = WGFMU_STATUS_OFFSET + 1
    Public Const WGFMU_STATUS_RUNNING As Integer = WGFMU_STATUS_OFFSET + 2
    Public Const WGFMU_STATUS_ABORT_COMPLETED As Integer = WGFMU_STATUS_OFFSET + 3
    Public Const WGFMU_STATUS_ABORTED As Integer = WGFMU_STATUS_OFFSET + 4
    Public Const WGFMU_STATUS_RUNNING_ILLEGAL As Integer = WGFMU_STATUS_OFFSET + 5
    Public Const WGFMU_STATUS_IDLE As Integer = WGFMU_STATUS_OFFSET + 6

    ' WGFMU_isMeasureEventCompleted
    Public Const WGFMU_MEASURE_EVENT_OFFSET As Integer = 11000
    Public Const WGFMU_MEASURE_EVENT_NOT_COMPLETED As Integer = WGFMU_MEASURE_EVENT_OFFSET + 0
    Public Const WGFMU_MEASURE_EVENT_COMPLETED As Integer = WGFMU_MEASURE_EVENT_OFFSET + 1

    ' WGFMU_setMeasureEvent
    Public Const WGFMU_MEASURE_EVENT_DATA_OFFSET As Integer = 12000
    Public Const WGFMU_MEASURE_EVENT_DATA_AVERAGED As Integer = WGFMU_MEASURE_EVENT_DATA_OFFSET + 0
    Public Const WGFMU_MEASURE_EVENT_DATA_RAW As Integer = WGFMU_MEASURE_EVENT_DATA_OFFSET + 1

    ' Common - Initialize

    Public Declare Function WGFMU_openSession Lib "wgfmu.dll" (ByVal address As String) As Integer
    Public Declare Function WGFMU_closeSession Lib "wgfmu.dll" () As Integer
    Public Declare Function WGFMU_setTimeout Lib "wgfmu.dll" (ByVal timeout As Double) As Integer
    Public Declare Function WGFMU_initialize Lib "wgfmu.dll" () As Integer
    Public Declare Function WGFMU_doSelfCalibration Lib "wgfmu.dll" (ByRef result As Integer, ByVal detail As String, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_doSelfTest Lib "wgfmu.dll" (ByRef result As Integer, ByVal detail As String, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getChannelIdSize Lib "wgfmu.dll" (ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getChannelIds Lib "wgfmu.dll" (ByVal channelIds() As Integer, ByRef size As Integer) As Integer

    ' Common - Error and Warning

    Public Declare Function WGFMU_getErrorSize Lib "wgfmu.dll" (ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getError Lib "wgfmu.dll" (ByVal errorString As String, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getErrorSummarySize Lib "wgfmu.dll" (ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getErrorSummary Lib "wgfmu.dll" (ByVal errorSummary As String, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_treatWarningsAsErrors Lib "wgfmu.dll" (ByVal warningLevel As Integer) As Integer
    Public Declare Function WGFMU_setWarningLevel Lib "wgfmu.dll" (ByVal warningLevel As Integer) As Integer
    Public Declare Function WGFMU_getWarningLevel Lib "wgfmu.dll" (ByRef warningLevel As Integer) As Integer
    Public Declare Function WGFMU_getWarningSummarySize Lib "wgfmu.dll" (ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getWarningSummary Lib "wgfmu.dll" (ByVal warningSummary As String, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_openLogFile Lib "wgfmu.dll" (ByVal fileName As String) As Integer
    Public Declare Function WGFMU_closeLogFile Lib "wgfmu.dll" () As Integer

    ' Common - Setup

    Public Declare Function WGFMU_setOperationMode Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal operationMode As Integer) As Integer
    Public Declare Function WGFMU_getOperationMode Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef operationMode As Integer) As Integer
    Public Declare Function WGFMU_setForceVoltageRange Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal forceVoltageRange As Integer) As Integer
    Public Declare Function WGFMU_getForceVoltageRange Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef forceVoltageRange As Integer) As Integer
    Public Declare Function WGFMU_setMeasureMode Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal measureMode As Integer) As Integer
    Public Declare Function WGFMU_getMeasureMode Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef measureMode As Integer) As Integer
    Public Declare Function WGFMU_setMeasureVoltageRange Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal measureVoltageRange As Integer) As Integer
    Public Declare Function WGFMU_getMeasureVoltageRange Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef measureVoltageRange As Integer) As Integer
    Public Declare Function WGFMU_setMeasureCurrentRange Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal measureCurrentRange As Integer) As Integer
    Public Declare Function WGFMU_getMeasureCurrentRange Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef measureCurrentRange As Integer) As Integer
    Public Declare Function WGFMU_setForceDelay Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal forceDelay As Double) As Integer
    Public Declare Function WGFMU_getForceDelay Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef forceDelay As Double) As Integer
    Public Declare Function WGFMU_setMeasureDelay Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal measureDelay As Double) As Integer
    Public Declare Function WGFMU_getMeasureDelay Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef measureDelay As Double) As Integer
    Public Declare Function WGFMU_setMeasureEnabled Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal measureEnabled As Integer) As Integer
    Public Declare Function WGFMU_isMeasureEnabled Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef measureEnabled As Integer) As Integer
    Public Declare Function WGFMU_setTriggerOutMode Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal triggerOutMode As Integer, ByVal polarity As Integer) As Integer
    Public Declare Function WGFMU_getTriggerOutMode Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef triggerOutMode As Integer, ByRef polarity As Integer) As Integer

    ' Common - Measurement

    Public Declare Function WGFMU_connect Lib "wgfmu.dll" (ByVal channelId As Integer) As Integer
    Public Declare Function WGFMU_disconnect Lib "wgfmu.dll" (ByVal channelId As Integer) As Integer

    ' WGFMU - Initialize

    Public Declare Function WGFMU_clear Lib "wgfmu.dll" () As Integer

    ' WGFMU - Setup - Pattern

    Public Declare Function WGFMU_createPattern Lib "wgfmu.dll" (ByVal patternName As String, ByVal initialVoltage As Double) As Integer
    Public Declare Function WGFMU_addVector Lib "wgfmu.dll" (ByVal patternName As String, ByVal deltaTime As Double, ByVal voltage As Double) As Integer
    Public Declare Function WGFMU_addVectors Lib "wgfmu.dll" (ByVal patternName As String, ByVal deltaTimes() As Double, ByVal voltages() As Double, ByVal size As Integer) As Integer
    Public Declare Function WGFMU_setVector Lib "wgfmu.dll" (ByVal patternName As String, ByVal time As Double, ByVal voltage As Double) As Integer
    Public Declare Function WGFMU_setVectors Lib "wgfmu.dll" (ByVal patternName As String, ByVal times() As Double, ByVal voltages() As Double, ByVal size As Integer) As Integer

    ' WGFMU - Setup - Pattern Opeartion

    Public Declare Function WGFMU_createMergedPattern Lib "wgfmu.dll" (ByVal patternName As String, ByVal pattern1 As String, ByVal pattern2 As String, ByVal axis As Integer) As Integer
    Public Declare Function WGFMU_createMultipliedPattern Lib "wgfmu.dll" (ByVal patternName As String, ByVal pattern1 As String, ByVal timeFactor As Double, ByVal voltageFactor As Double) As Integer
    Public Declare Function WGFMU_createOffsetPattern Lib "wgfmu.dll" (ByVal patternName As String, ByVal pattern1 As String, ByVal timeOffset As Double, ByVal voltageOffset As Double) As Integer

    ' WGFMU - Setup - Event

    Public Declare Function WGFMU_setMeasureEvent Lib "wgfmu.dll" (ByVal patternName As String, ByVal eventName As String, ByVal time As Double, ByVal measurementPoints As Integer, ByVal measurementInterval As Double, ByVal averagingTime As Double, ByVal rawData As Integer) As Integer
    Public Declare Function WGFMU_setRangeEvent Lib "wgfmu.dll" (ByVal patternName As String, ByVal eventName As String, ByVal time As Double, ByVal rangeIndex As Integer) As Integer
    Public Declare Function WGFMU_setTriggerOutEvent Lib "wgfmu.dll" (ByVal patternName As String, ByVal eventName As String, ByVal time As Double, ByVal duration As Double) As Integer

    ' WGFMU - Setup - Sequence

    Public Declare Function WGFMU_addSequence Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal patternName As String, ByVal loopCount As Double) As Integer
    Public Declare Function WGFMU_addSequences Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal patternNames() As String, ByVal loopCounts() As Double, ByVal size As Integer) As Integer

    ' WGFMU - Setup Check - Pattern

    Public Declare Function WGFMU_getPatternForceValueSize Lib "wgfmu.dll" (ByVal patternName As String, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getPatternForceValues Lib "wgfmu.dll" (ByVal patternName As String, ByVal offset As Integer, ByRef size As Integer, ByVal forceTimes() As Double, ByVal forceValues() As Double) As Integer
    Public Declare Function WGFMU_getPatternForceValue Lib "wgfmu.dll" (ByVal patternName As String, ByVal index As Integer, ByRef forceTime As Double, ByRef forceValue As Double) As Integer
    Public Declare Function WGFMU_getPatternInterpolatedForceValue Lib "wgfmu.dll" (ByVal patternName As String, ByVal time As Double, ByRef forceValue As Double) As Integer
    Public Declare Function WGFMU_getPatternMeasureTimeSize Lib "wgfmu.dll" (ByVal patternName As String, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getPatternMeasureTimes Lib "wgfmu.dll" (ByVal patternName As String, ByVal offset As Integer, ByRef size As Integer, ByVal measureTimes() As Double) As Integer
    Public Declare Function WGFMU_getPatternMeasureTime Lib "wgfmu.dll" (ByVal patternName As String, ByVal index As Integer, ByRef measureTime As Double) As Integer

    ' WGFMU - Setup Check - Sequence

    Public Declare Function WGFMU_getForceValueSize Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef size As Double) As Integer
    Public Declare Function WGFMU_getForceValues Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal offset As Double, ByRef size As Integer, ByRef forceTimes() As Double, ByVal forceValues() As Double) As Integer
    Public Declare Function WGFMU_getForceValue Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal index As Double, ByRef forceTime As Double, ByRef forceValue As Double) As Integer
    Public Declare Function WGFMU_getInterpolatedForceValue Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal time As Double, ByRef forceValue As Double) As Integer
    Public Declare Function WGFMU_getMeasureTimeSize Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getMeasureTimes Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal offset As Integer, ByRef size As Integer, ByVal measureTimes() As Double) As Integer
    Public Declare Function WGFMU_getMeasureTime Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal index As Integer, ByRef measureTime As Double) As Integer

    ' WGFMU - Setup Check - Event

    Public Declare Function WGFMU_getMeasureEventSize Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getMeasureEvents Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal offset As Integer, ByRef size As Integer, ByVal patternNames() As String, ByVal eventNames() As String, ByVal cycles() As Integer, ByVal loopIndices() As Double, ByVal counts() As Integer, ByVal offsets() As Integer, ByVal sizes() As Integer) As Integer
    Public Declare Function WGFMU_getMeasureEvent Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal index As Integer, ByVal patternName As String, ByVal eventName As String, ByRef cycle As Integer, ByRef loopIndex As Double, ByRef count As Integer, ByRef offset As Integer, ByRef size As Integer) As Integer
    Public Declare Function WGFMU_getMeasureEventAttribute Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal index As Integer, ByRef time As Double, ByRef measurementPoints As Integer, ByRef measurementInterval As Double, ByRef averagingTime As Double, ByRef rawData As Integer)

    ' WGFMU - Import / Export

    Public Declare Function WGFMU_exportAscii Lib "wgfmu.dll" (ByVal fileName As String) As Integer

    ' WGFMU - Measurement

    Public Declare Function WGFMU_update Lib "wgfmu.dll" () As Integer
    Public Declare Function WGFMU_updateChannel Lib "wgfmu.dll" (ByVal channelId As Integer) As Integer
    Public Declare Function WGFMU_execute Lib "wgfmu.dll" () As Integer
    Public Declare Function WGFMU_abort Lib "wgfmu.dll" () As Integer
    Public Declare Function WGFMU_abortChannel Lib "wgfmu.dll" (ByVal channelId As Integer) As Integer
    Public Declare Function WGFMU_getStatus Lib "wgfmu.dll" (ByRef status As Integer, ByRef elapsedTime As Double, ByRef totalTime As Double) As Integer
    Public Declare Function WGFMU_getChannelStatus Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef status As Integer, ByRef elapsedTime As Double, ByRef totalTime As Double) As Integer
    Public Declare Function WGFMU_waitUntilCompleted Lib "wgfmu.dll" () As Integer

    ' WGFMU - Data Retrieve - Measure Values

    Public Declare Function WGFMU_getMeasureValueSize Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef measuredSize As Integer, ByRef totalSize As Integer) As Integer
    Public Declare Function WGFMU_getMeasureValues Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal offset As Integer, ByRef size As Integer, ByVal measureTimes() As Double, ByVal measureValues() As Double) As Integer
    Public Declare Function WGFMU_getMeasureValue Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal index As Integer, ByRef measureTime As Double, ByRef measureValue As Double) As Integer

    ' WGFMU - Data Retrieve - Measure Event

    Public Declare Function WGFMU_getCompletedMeasureEventSize Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef measuredSize As Integer, ByRef totalSize As Integer) As Integer
    Public Declare Function WGFMU_isMeasureEventCompleted Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal patternName As String, ByVal eventName As String, ByVal cycle As Integer, ByVal loopIndex As Double, ByVal count As Integer, ByRef completed As Integer, ByRef index As Integer, ByRef offset As Integer, ByRef size As Integer) As Integer

    ' DC - Measurement

    Public Declare Function WGFMU_dcforceVoltage Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal voltage As Double) As Integer
    Public Declare Function WGFMU_dcmeasureValue Lib "wgfmu.dll" (ByVal channelId As Integer, ByRef value As Double) As Integer
    Public Declare Function WGFMU_dcmeasureAveragedValue Lib "wgfmu.dll" (ByVal channelId As Integer, ByVal count As Integer, ByVal interval As Integer, ByRef value As Double) As Integer

End Module
