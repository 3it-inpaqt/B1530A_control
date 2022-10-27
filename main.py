"""
2022-09-19
Nicolas Gariépy
Runs a PUND measurement, using WGFMU.exe on the Keysight B1500A with a B1530.`
Define pulse params directly in the "program_args" dict.
"""
from datetime import datetime
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
Material= 'W'
Motif= ''
sample= "Q294G"
FTJ = "br73"
print(FTJ)

# modes : "Standard", "Single", "NLS"
mode = "NLS"
Decade_start = 0 # For "Standard" measurements, also the filename in "Single" measurements
Decade_stop = 0 # For "Standard" measurements
singleNumber = 1 # For "Single" measurements
program_args = {
    "PUND_decade":0,
    "currentrange":1000, # in µA
    "npoints":800,
    "PUND_shape":{ # PUND
        "Vpulse": 1,
        "trise": 50e-6, #rise time in s
        "twidth":0, #width of the pund pulse in s
        "tspace": 10e-6 #time between pulses in s
    },
    "aging_shape":{ # Cycling square wave
        "Vpulse": 1,
        "trise":500e-9, #rise time  in s
        "twidth":5e-6, #pulse width in s
        "tspace":10e-6 #time between pulses in s
    }
}

#NLS
NLS_t_start = 30e-9
NLS_t_stop = 30e-3
NLS_voltages = [1, 1.2, 1.5, 1.7, 2, 2.2, 2.5, 2.7, 3]#np.linspace(1.5, 3, 7) #1.5 3 7
NLS_pulse_widths = np.logspace(np.log10(NLS_t_start), np.log10(NLS_t_stop), 25)
#----------------------------------------------------------------------------------------------------------------------------------
# Classes
class pathFerro:
    def __init__(self, Material, Motif, sample, FTJ):
        self.FTJ = FTJ
        if Motif=='':
            self.base_path = "C:\\Users\\ngariepy\\Documents\\UdeS\\T4\\Keysight\\"+str(Material)+'\\'+str(sample)+'\\'+str(FTJ) + '\\'
        else:
            self.base_path = "C:\\Users\\ngariepy\\Documents\\UdeS\\T4\\Keysight\\"+str(Material)+'\\'+str(Motif)+'\\'+str(sample)+'\\'+str(FTJ) + '\\'

        self.configname = "config0.json"
        self.exe_path = "./Release/WGFMU.exe"
        self.timestamp = datetime.now().strftime("%Y-%m-%d_%H%M%S")
        self.path_stamped = self.base_path + "Data_" + self.timestamp + '\\'
        self.configpath = self.path_stamped + self.configname
        self.excelpath = self.path_stamped + "PUND_Data.xlsx"

    def setconfig(self, configname):
        self.configname = configname
        self.configpath = self.path_stamped + self.configname

# Functions
# For generating sample results
fakedf = pd.DataFrame({
            'Time': range(100),
            'Voltage': [*range(25), *(np.multiply(range(25),-1)), *range(25), *(np.multiply(range(25),-1))],
})

def fake_data(data, decade):
    data["Voltage"] = data["Voltage"] + np.random.randn(len(data["Voltage"]))/5
    data["Current"] = (0.5 + np.abs(np.random.randn(len(data["Voltage"])))/3)*data["Voltage"]
    return data

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
    else:
        plt_update_axis(ax[n - Decade_start], data)

def plt_new_axis(data, n):
    fig = plt.figure(n)
    fig.suptitle("Decade "+str(n))
    ax1 = fig.add_subplot(111)
    ax1.plot(data["Time"], data["Voltage"], "silver")
    ax1.set_xlabel(str(data.columns[0]))
    ax1.set_ylabel(str(data.columns[1]))

    ax2 = ax1.twinx()
    ax2.plot(data["Time"], data["Current"])
    ax2.set_ylabel(str(data.columns[2]))
    plt.tight_layout()
    fig.show()
    fig.canvas.draw()
    fig.canvas.flush_events()
    return fig

def plt_update_axis(fig, data):
    if plt.fignum_exists(fig.number):
        fig.axes[0].plot(data["Time"], data["Voltage"], "silver")
        fig.axes[1].plot(data["Time"], data["Current"])
        fig.canvas.draw()
        fig.canvas.flush_events()

def plt_save(axis, decade, paths):
    # Warning "Decade_start" is accessed through the global scope
    axis[decade - Decade_start].get_figure().savefig(paths.path_stamped + "Chronograms of decade " + str(decade), bbox_inches='tight')

def plt_save_singles(axis, decade, paths):
    # Warning "Decade_start" is accessed through the global scope
    axis[decade - Decade_start].get_figure().savefig(paths.path_stamped + "Chronograms of singles at decade " + str(decade), bbox_inches='tight')


def callWGFMU(type, decade, number, paths, excelf):
    start_t = time.time()
    # Type 1 = Standard, Type 2 = Singles
    retval = call([paths.exe_path, str(type), str(decade), str(number), paths.configpath])
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
    paths = pathFerro(Material, Motif, sample, FTJ)
    program_args["path"] = paths.path_stamped
    
    # Write params to .json file, so it can be passed to the cpp .exe
    os.makedirs(paths.path_stamped, exist_ok=True) 
    with open(paths.configpath, "w", encoding="utf_8") as outfile:
        json.dump(program_args, outfile, indent=4, sort_keys=False)

    if mode == "Standard":
        standard_Measure(Decade_start, Decade_stop, paths, program_args)
    elif mode == "Single":
        single_Measure(singleNumber, Decade_start, paths)
    elif mode == "NLS":
        NLS(NLS_pulse_widths, NLS_voltages, paths)

def standard_Measure(Decade_start, Decade_stop, paths, program_args):
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
                df = callWGFMU(1, PUND_decade, 0, paths, excelf)
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
                        with open(paths.configpath, "w", encoding="utf_8") as outfile:
                            json.dump(program_args, outfile, indent=4, sort_keys=False)
                        break # Create new config with new current range
                    else:
                        print("Invalid current range.")
                break # Decade 0 only has 1 PUND in it, correspoding to cycle number 1

            if PUND_decade == 1:
                df = callWGFMU(1, PUND_decade, 0, paths, excelf)
                plt_results(ax, df, PUND_decade)
                break # Decade 1 only has 1 PUND in it, correspoding to cycle number 10

            df = callWGFMU(1, PUND_decade, PUND_number, paths, excelf)
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
    input("Done press enter to exit")

def single_Measure(singleNumber, Decade, paths):
    ax = []
    excelf = pd.ExcelWriter(paths.excelpath)

    for PUND_number in range(singleNumber):
        df = callWGFMU(2, Decade, PUND_number, paths, excelf)
        plt_results(ax, df, Decade)

    plt_save_singles(ax, Decade, paths) # Save fig of each decade as .png
    excelf.close()
    shutil.copy2(paths.excelpath, paths.base_path)
    shutil.copy2(paths.configpath, paths.base_path)
    if mode == "Single": # Only pause when the function is called directly by the user
        input("Done press enter to exit")

def NLS(pulse_widths, voltages, paths):
    ax = []
    excelf = pd.ExcelWriter(paths.excelpath)
    NLS_dict = {"pulse_widths":list(pulse_widths), "voltages":list(voltages)}
    with open(paths.path_stamped + "NLS_indexes.json", "w", encoding="utf_8") as indexes:
        json.dump(NLS_dict, indexes, indent=4, sort_keys=False)

    """
    # Create a log file with the switching times and pulse voltages used
    with open(paths.path_stamped + "NLS_indexes.csv", "w", encoding="utf_8") as indexes:
        indexes.write("Type;Index;Val")
        for i, time in enumerate(times):
            indexes.write("time" + ";" + str(i) + ";" + str(time) + '\n')
        for j, voltage in enumerate(voltages):
            indexes.write("voltage" + ";" + str(j) + ";" + str(voltage) + '\n')
    """
        
    for i, voltage in enumerate(voltages):
        program_args["aging_shape"]["Vpulse"] = voltage
        for j, pulse_width in enumerate(pulse_widths):
            program_args["aging_shape"]["twidth"] = pulse_width
            with open(paths.configpath, "w", encoding="utf_8") as outfile:
                # "config0.json" gets overwritten, but the WGFMU exe will also output a unique
                # config on each test, where the relevant modifications can be tracked
                json.dump(program_args, outfile, indent=4, sort_keys=False)
            df = callWGFMU(4, j, i, paths, excelf)            
            plt_results(ax, df, i)
            time.sleep(1) # Actual pause of 1 second, to make 100% sure the sample "settles"
        plt.savefig(paths.path_stamped + "Chronograms_at_" +  str(voltage) + "_V" + ".png")
        plt.close()

    excelf.close()
    shutil.copy2(paths.excelpath, paths.base_path)
    shutil.copy2(paths.configpath, paths.base_path)
    shutil.copy2(paths.path_stamped + "NLS_indexes.json", paths.base_path)
    input("")

main()