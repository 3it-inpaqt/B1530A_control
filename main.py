"""
2022-09-19
Nicolas Gariépy
Runs a PUND measurement, using WGFMU.exe on the Keysight B1500A with a B1530.
Define pulse params directly in the "program_args" dict.
"""
from datetime import datetime
from enum import Enum, auto
from subprocess import call
import json
import math
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
import shutil
import time 


# Params
# Data output location
Material= 'TiN'
#Motif= 'Pattern'
Motif= ''
#sample= "P39AE"
sample= "P474EF"
FTJ = "tr121"
print(FTJ)

class Mode(Enum): # Enum values correspond to WGFMU.exe args
    endurancePUND = 1
    singlePUND = 3
    NLS = 4
    measureOnOff = 5
    measureOnOffNoAging = 6
    LTD_LTP_voltage = 7
    LTD_LTP_pulsewidth = 8
mode = Mode.LTD_LTP_voltage

# Mixed use...
Decade_start = 0 # Start decade of "endurance type" measurements, (also the filename in "Single" measurements)
Decade_stop = 4 # Last decade of "endurance type" measurements 
LTD_LTP_Cycles = 2 # nb of times to do LTP_LTD.
singleNumber = 1 # For "Single" measurements
PROG_args = {
    "PUND_decade":Decade_stop,
    "currentrange":1, # in µA, min = 1, max = 10000
    "npoints":20, # Affects PUNDs, LTP/LTD, and "measureOnOffNoAging"
    "PUND_shape":{ # PUND
        "Vpulse": 1.2,
        "trise": 50e-6, #rise time in s
        "twidth": 0, #width of the pund pulse in s
        "tspace": 50e-6 #time between pulses in s
    },  
    "aging_shape":{ # Cycling square wave, Used for pund (ON OFF will use the write voltage and twrite for aging)
        "Vpulse": 1.2,
        "trise": 500e-9, #rise time  in s
        "twidth":50e-6, #pulse width in s
        "tspace":10e-6 #time between pulses in s
    },
    "NLS_cycle_shape":{ # "Equalization" cycle in between NLS measurements
        "Vpulse": 3, # The sign of the amplitude has no effect in NLS 
        "trise": 50e-9, #rise time  in s
        "twidth":50e-6, #pulse width in s
        "tspace":50e-6 #time between pulses in s
    },
    "NLS_PUND_read_shape":{ # Half PUND for reading in NLS
        "Vpulse": 3, # The sign of the amplitude has no effect in NLS 
        "trise": 50e-6, #rise time  in s
        "twidth":0, #pulse width in s
        "tspace":50e-6 #time between pulses in s
    },
    "Vwritepos":1.2, # For var voltage : max vwrite. For var pulse : value used (assymmetric +/-)
    "Vwriteneg":-1.2, # For var voltage : max vwrite. For var pulse : value used (assymmetric +/-)
    "Vread":0.2, # Read pulse voltage
    "twrite":50e-6, # For var voltage : value used. For var pulse : max tpulse (symmetric +/-)
    "tread": 10e-3 # Total ead time, only second half if used to get data
}

# NLS only
NLS_t_start = 50e-9
NLS_t_stop = 100e-3
NLS_voltages = [-0.4, -0.6, -0.8]
#NLS_voltages = [0.6, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.7, 2, 2.2, 2.5]
#NLS_voltages = [0.9, 1.1, 1.3, 1.4, 1.6]
NLS_voltages = np.multiply(NLS_voltages, -1)
NLS_pulse_widths = np.logspace(np.log10(NLS_t_start), np.log10(NLS_t_stop), 10)

#----------------------------------------------------------------------------------------------------------------------------------
# Classes
class pathFerro:
    def __init__(self, Material, Motif, sample, FTJ, mode):
        self.FTJ = FTJ
        if Motif=='':
            self.base_path = "C:\\Users\\ngariepy\\Documents\\UdeS\\T4\\Keysight\\"+str(Material)+'\\'+str(sample)+'\\'+str(FTJ) + '\\'
        else:
            self.base_path = "C:\\Users\\ngariepy\\Documents\\UdeS\\T4\\Keysight\\"+str(Material)+'\\'+str(Motif)+'\\'+str(sample)+'\\'+str(FTJ) + '\\'

        self.configname = "config0.json"
        self.exe_path = "./Release/WGFMU.exe"
        self.timestamp = datetime.now().strftime("%Y-%m-%d_%H%M%S")
        self.path_stamped = self.base_path + str(mode.name) + '_' + self.timestamp + '\\'
        self.configpath = self.path_stamped + self.configname
        self.excelpath = self.path_stamped + "PUND_Data.xlsx"

    def setconfig(self, configname):
        self.configname = configname
        self.configpath = self.path_stamped + self.configname

# Functions
### Get and plot data
def get_results(decade, number, paths, excelf):
    # Missing paths excelf
    # to_excel could be handled by a different function
    df = pd.read_csv(paths.path_stamped +"PUND_"+str(decade)+"_"+str(number)+".csv", sep=';')
    df.to_excel(excelf, sheet_name="PUND_"+str(decade)+"_"+str(number))
    return df 

def plt_results(ax, data, n):
    # Missing Decade_start
    if len(ax) <= n - Decade_start:
        ax.append(plt_new_axis(data,n))
    plt_update_axis(ax[n - Decade_start], data)
    
def plt_new_axis(data, n):
    fig = plt.figure(n)
    fig.suptitle("Decade "+str(n))
    ax1 = fig.add_subplot(111)
    ax1.grid(True, axis='x')
    ax1.set_xlabel(str(data.columns[0]))
    ax1.set_ylabel(str(data.columns[1]))

    ax2 = ax1.twinx()
    ax2.grid(True, axis='both')
    ax2.set_ylabel(str(data.columns[2]))
    plt.tight_layout()
    fig.show()
    fig.canvas.draw()
    fig.canvas.flush_events()
    return fig

def plt_update_axis(fig, data):
    if plt.fignum_exists(fig.number):
        #current = signal.convolve(data["Current"], fir)
        fig.axes[0].plot(data["Time"][1:], data["Voltage"][1:], "silver")
        fig.axes[1].plot(data["Time"][1:], data["Current"][1:])
        #fig.axes[1].plot(data["Time"][1:], current[lenfilter2+1:-1*lenfilter2])
        fig.canvas.draw()
        fig.canvas.flush_events()

def plt_resistances(data, paths, scale='log'):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    side1 = data.loc[data['Vwrite'] >= 0]
    side2 = data.loc[data['Vwrite'] < 0]
    ax.plot(side1.loc[side1['Vread'] >= 0].Resistance, 'o-', color='green')
    ax.plot(side1.loc[side1['Vread'] < 0].Resistance, 'o-', color='lime')
    if len(side2) != 0:
        ax.plot(side2.loc[side1['Vread'] >= 0].Resistance, 'o-', color='red')
        ax.plot(side2.loc[side1['Vread'] < 0].Resistance, 'o-', color='magenta')
    
    ax.set_xscale(scale)
    fig.suptitle("Evolution of Resistance and ON/OFF")
    ax.set_xlabel("Cycle")
    ax.set_ylabel("Resistance (Ohms)")
    ax.legend(["W+,R+", "W+,R-", "W-,R+", "W-,R-"])

    plt.tight_layout()
    fig.show()
    fig.canvas.flush_events()
    plt.savefig(paths.path_stamped + "Evolution of resistance.png", bbox_inches='tight')

def plt_LTDLTP(data, paths, mode, scale='log'):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    side1 = data.loc[data['Vwrite'] >= 0]
    side2 = data.loc[data['Vwrite'] < 0]
    ax.plot(side1.Resistance, 'o', color='green')
    ax.plot(side2.Resistance, 'o', color='red')
    
    ax.set_xscale(scale)
    fig.suptitle(str(mode))
    ax.set_xlabel("Cycle")
    ax.set_ylabel("Resistance (Ohms)")
    ax.legend(["positive", "negative"])

    plt.tight_layout()
    fig.show()
    fig.canvas.flush_events()
    plt.savefig(paths.path_stamped + str(mode) + ".png", bbox_inches='tight')

def plt_save(axis, decade, paths):
    # Warning "Decade_start" is accessed through the global scope
    axis[decade - Decade_start].get_figure().savefig(paths.path_stamped + "Chronograms of decade " + str(decade), bbox_inches='tight')

def plt_save_singles(axis, decade, paths):
    # Warning "Decade_start" is accessed through the global scope
    axis[decade - Decade_start].get_figure().savefig(paths.path_stamped + "Chronograms of singles at decade " + str(decade), bbox_inches='tight')


def callWGFMU(type, decade, number, paths, excelf):
    start_t = time.time()
    retval = call([paths.exe_path, str(type.value), str(decade), str(number), paths.configpath])
    end_t = time.time()
    print("Execution time (s) : " + str(end_t - start_t))
    if retval == 1:
        print("Invalid time parameters. Rise time or fall time is too small")
        exit()
    if retval != 0:
        print("WGFMU.exe finished execution with error code:" + str(retval))
        print("Most likely an error with the python script, the exe will run \"correctly\" even if not connected to the machine")
        exit()
    # LOG HERE
    with open(paths.base_path + "Allmeasurements.log", 'a') as logfile:
        logfile.write(paths.timestamp + ';' + str(type) + ';' + str(decade) + ';' + str(number) + ';' + paths.configpath + '\n')
    return get_results(decade, number, paths, excelf)
    #return fake_data(fakedf, decade)

#----------------------------------------------------
def main():
    paths = pathFerro(Material, Motif, sample, FTJ, mode)
    PROG_args["path"] = paths.path_stamped
    
    # Write params to .json file, so it can be passed to the cpp .exe
    os.makedirs(paths.path_stamped, exist_ok=True) 
    dumpargs(PROG_args,paths)

    if mode == Mode.endurancePUND:
        endurancePUND(Decade_start, Decade_stop, paths, PROG_args)
    elif mode == Mode.singlePUND:
        singlePUND(singleNumber, Decade_start, paths)
    elif mode == Mode.NLS:
        #NLS2(NLS_pulse_widths, NLS_voltages, paths)
        NLS(NLS_pulse_widths, NLS_voltages, paths)
    elif mode == Mode.measureOnOff:
        measureOnOff(paths, True)
    elif mode == Mode.measureOnOffNoAging:
        measureOnOff(paths, False)
    elif (mode == Mode.LTD_LTP_voltage) or (mode == Mode.LTD_LTP_pulsewidth):
        PROG_args["PUND_decade"] = LTD_LTP_Cycles
        dumpargs(PROG_args, paths)
        LTD_LTP(paths)
    else :
        print("Invalid test name.")
        exit()
    input("Done press enter to exit")
        

def endurancePUND(Decade_start, Decade_stop, paths, program_args):
    ax = []
    excelf = pd.ExcelWriter(paths.excelpath)
    stop = False
    for PUND_decade in range(Decade_start, Decade_stop+1):
        # PUND_number : 9 mesures, de 2 à 10 (11 est exclu du range). 10x10^(n-1) = 1x10^(n)
        for PUND_number in range(2,11): # [2x10^(n-1), 3x10^(n-1), 4x10^(n-1), ... 9x10^(n-1), 10x10^(n-1)]
            if stop:
                break

            if PUND_decade == 0:
                # Do PUND00 (special case, one measurement, no aging, asks for current range adjust)¸
                df = callWGFMU(mode, PUND_decade, 0, paths, excelf)
                plt_results(ax, df, PUND_decade)

                # Ask for current range adjust
                while Decade_stop != 0:
                    choice = input("Change current range (default = keep same): ")
                    if choice == "":
                        break # Keep same config, nothing to do

                    choice = float(choice)
                    if(choice>=1 and math.log10(choice).is_integer() and math.log10(choice) < 5):
                        program_args["currentrange"] = choice # new current range
                        program_args["PUND_decade"] = PUND_decade # new current range
                        paths.setconfig("config1.json")
                        dumpargs(program_args,paths) # Create new config with new current range
                        break 
                    else:
                        print("Invalid current range.")
                break # Decade 0 only has 1 PUND in it, correspoding to cycle number 1

            if PUND_decade == 1:
                df = callWGFMU(mode, PUND_decade, 0, paths, excelf)
                plt_results(ax, df, PUND_decade)
                break # Decade 1 only has 1 PUND in it, correspoding to cycle number 10

            df = callWGFMU(mode, PUND_decade, PUND_number, paths, excelf)
            plt_results(ax, df, PUND_decade)
            """
            if PUND_decade > 6:
                while True:
                    ans = input("Continue ? (Y/n)")
                    if ans == "" or ans == "Y":
                        break
                    elif ans == "n":
                        stop = True
                        break
            """

        plt_save(ax, PUND_decade, paths) # Save fig of each decade as .png


    excelf.close()
    shutil.copy2(paths.excelpath, paths.base_path)
    shutil.copy2(paths.configpath, paths.base_path)

def singlePUND(singleNumber, Decade, paths):
    ax = []
    excelf = pd.ExcelWriter(paths.excelpath)

    for PUND_number in range(singleNumber):
        #df = callWGFMU(2, Decade, PUND_number, paths, excelf)
        df = callWGFMU(mode, Decade, PUND_number, paths, excelf)
        plt_results(ax, df, Decade)

    plt_save_singles(ax, Decade, paths) # Save fig of each decade as .png
    excelf.close()
    shutil.copy2(paths.excelpath, paths.base_path)
    shutil.copy2(paths.configpath, paths.base_path)

def NLS(pulse_widths, voltages, paths):
    ax = []
    #ax2 = []
    excelf = pd.ExcelWriter(paths.excelpath)
    NLS_dict = {"pulse_widths":list(pulse_widths), "voltages":list(voltages)}
    with open(paths.path_stamped + "NLS_indexes.json", "w", encoding="utf_8") as indexes:
        json.dump(NLS_dict, indexes, indent=4, sort_keys=False)

    # The aging shape needs to be overwritten because it's used as the shape for 
    # the writting pulse. To change the cycling applied between NLS pulse, adjust
    # the "NLS_cycle_shape" instead.
    PROG_args["aging_shape"]["trise"] = 50e-9 # 50e-9 = standard, 30e-9 = Minimum possible en mode PG
    PROG_args["aging_shape"]["tspace"] = 50e-6
    PROG_args["aging_shape"]["twidth"] = 50e-6
    PROG_args["aging_shape"]["Vpulse"] = 0
    
    for i, voltage in enumerate(voltages):
        PROG_args["aging_shape"]["Vpulse"] = voltage
        for j, pulse_width in enumerate(pulse_widths):
            PROG_args["aging_shape"]["twidth"] = pulse_width
            dumpargs(PROG_args, paths)
            df = callWGFMU(mode, j, i, paths, excelf)
            #df2 = get_results(j + 50, i, paths, excelf)
            plt_results(ax, df, i)
            #ax.getaxes[1].set_ylim(bottom = -500e-09, top = 500e-09)
    
            #plt_results(ax2, df2, j+50)
            time.sleep(1) # Actual pause of 1 second, to make 100% sure the sample "settles"
        plt.savefig(paths.path_stamped + "Chronograms_at_" +  str(voltage) + "_V" + ".png")
        #plt.close()

    excelf.close()
    shutil.copy2(paths.excelpath, paths.base_path)
    shutil.copy2(paths.configpath, paths.base_path)
    shutil.copy2(paths.path_stamped + "NLS_indexes.json", paths.base_path)

def NLS2(pulse_widths, voltages, paths):
    excelf = pd.ExcelWriter(paths.excelpath)
    df = callWGFMU(mode, 0, 0, paths, excelf)
    ax = []
    #ax2 = []
    plt_results(ax, df, 0)

def measureOnOff(paths, aging):
    retval = call([paths.exe_path, str(mode.value), str(Decade_start), str(0), paths.configpath])
    shutil.copy2(paths.path_stamped + "Resistances.csv", paths.base_path)

    df = pd.read_csv(paths.path_stamped + "Resistances.csv", sep=';', index_col='CycleID')
    if aging:
        plt_resistances(df, paths, 'log')
    else:
        plt_resistances(df, paths, 'linear')

def LTD_LTP(paths): # works for both var voltage and var pulsewidth
    start_t = time.time()
    retval = call([paths.exe_path, str(mode.value), str(0), str(0), paths.configpath])
    end_t = time.time()
    print("Execution time (s) : " + str(end_t - start_t))
    result_filename = paths.path_stamped + "LTD_LTP" + ".csv"
    shutil.copy2(result_filename, paths.base_path)
    df = pd.read_csv(result_filename, sep=';', index_col="CycleID")
    plt_LTDLTP(df,paths, mode, scale='linear')
    
def dumpargs(args,paths):
    with open(paths.configpath, "w", encoding="utf8") as outfile:
        json.dump(args, outfile, indent=4, sort_keys=False)
main()