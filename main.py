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

# Params
# Data output location
Material='W'
Motif= ''
sample= "Q294A"
FTJ = "mr104"
print(FTJ)

if Motif=='':
    path_unix = "/home/ngariepy/Documents/UdeS/T4/Keysight/"+str(Material)+'/'+str(sample)+'/'+str(FTJ)+'/'
    path_win = "C:\\Users\\ngariepy\\Documents\\UdeS\\T4\\Keysight\\"+str(Material)+'\\'+str(sample)+'\\'+str(FTJ) + '\\'
else:
    path_unix = "/home/ngariepy/Documents/Udes/T4/Keysight/"+str(Material)+'/'+str(Motif)+'/'+str(sample)+'/'+str(FTJ)+'/'
    path_win = "C:\\Users\\ngariepy\\Documents\\UdeS\\T4\\Keysight\\"+str(Material)+'\\'+str(Motif)+'\\'+str(sample)+'\\'+str(FTJ) + '\\'

configname = "config0.json"
exe_path = "./Release/WGFMU.exe"
timestamp = datetime.now().strftime("%Y-%m-%d_%H%M%S")
path_unix_stamped = path_unix + "Data_" + timestamp + '/'
path_win_stamped = path_win + "Data_" + timestamp + '\\'

# modes : "Standard", "Single"
mode = "Single"
Decade_start = 4 # For "Standard" measurements, also the filename in "Single" measurements
Decade_stop = 4 # For "Standard" measurements
singleNumber = 10 # For "Single" measurements
program_args = {
    "PUND_decade":0,
    "currentrange":1000, # in µA
    "npoints":800, # Current reading temporal resolution
    "path": path_win_stamped,
    # PUND
    "PUND_shape":{
        "Vpulse": 2.5,
        "trise": 50e-6, #rising time of the pulse in s
        "twidth":0, #width of the pund pulse in s
        "tspace": 10e-6 #time between pulses in s
        #tpulse = 2*pentep + palierp + inter
    },
    # Cycling square wave
    "aging_shape":{
        "Vpulse": 2.5,
        "trise":500e-9, #rise time in s
        "twidth":50e-6, #pulse width in s
        "tspace":10e-6 #time between 2 pulses in s
    }
}

# Write params to .json file, so it can be passed to the cpp .exe
os.makedirs(path_unix_stamped, exist_ok=True)
with open( path_unix_stamped + configname, "w", encoding="utf_8") as outfile:
    json.dump(program_args, outfile, indent=4, sort_keys=False)

# For generating sample results
fakedf = pd.DataFrame({
            'Time': range(100),
            'Voltage': [*range(25), *(np.multiply(range(25),-1)), *range(25), *(np.multiply(range(25),-1))],
})

ax = []
excelf = pd.ExcelWriter(path_unix_stamped + "PUND_Data.xlsx")
#----------------------------------------------------------------------------------------------------------------------------------

# Functions
def fake_data(data, decade):
    data["Voltage"] = data["Voltage"] + np.random.randn(len(data["Voltage"]))/5
    data["Current"] = (0.5 + np.abs(np.random.randn(len(data["Voltage"])))/3)*data["Voltage"]
    return data

"""
for decade in range(8):
    for number in range(2, 11):
        df = fake_data(df, 0)
        if decade < 2:
            number = 0
            df.to_csv(path_unix+"PUND_"+str(decade)+str(number)+".csv", sep=';', index=False)
            break
        df.to_csv(path_unix+"PUND_"+str(decade)+str(number)+".csv", sep=';', index=False)

exit()
"""

### Get and plot data
def get_results(decade, number):
    df = pd.read_csv(path_unix_stamped +"PUND_"+str(decade)+str(number)+".csv", sep=';')
    df.to_excel(excelf, sheet_name="PUND_"+str(decade)+str(number))
    return df 

def plt_results(ax, data, n):
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
        ax1 = fig.axes[0]
        fig.axes[0].plot(data["Time"], data["Voltage"], "silver")
        fig.axes[1].plot(data["Time"], data["Current"])
        fig.canvas.draw()
        fig.canvas.flush_events()

def plt_save(axis, decade):
    axis[decade - Decade_start].get_figure().savefig(path_unix_stamped + "Chronograms of decade : " + str(decade), bbox_inches='tight')

def plt_save_singles(axis, decade):
    axis[decade - Decade_start].get_figure().savefig(path_unix_stamped + "Chronograms of singles at decade : " + str(decade), bbox_inches='tight')


def callWGFMU(type, decade, number, configpath):
    retval = call([exe_path, str(type), str(decade), str(number), configpath])
    if retval != 0:
        print("WGFMU.exe finished execution with error code:" + str(retval))
        print("Most likely an error with the python script, the exe will run \"correctly\" even if not connected to the machine")
        exit()
    # LOG HERE
    with open(path_unix + "Allmeasurements.log", 'a') as logfile:
        logfile.write(timestamp + ';' + str(type) + ';' + str(decade) + ';' + str(number) + ';' + configpath + '\n')
    return get_results(decade, number)
    #return fake_data(fakedf, decade)

#----------------------------------------------------
if mode == "Standard":
    for PUND_decade in range(Decade_start, Decade_stop+1):
            # PUND_number : 9 mesures, de 2 à 10 (11 est exclu du range). 10x10^(n-1) = 1x10^(n)
            for PUND_number in range(2,11): # [2x10^(n-1), 3x10^(n-1), 4x10^(n-1), ... 9x10^(n-1), 10x10^(n-1)]
                if PUND_decade == 0:
                    # Do PUND00 (special case, one measurement, no aging, asks for current range adjust)¸
                    df = callWGFMU(1, PUND_decade, 0, path_win_stamped + configname)
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
                            configname = "config1.json"
                            with open(path_unix_stamped + configname, "w", encoding="utf_8") as outfile:
                                json.dump(program_args, outfile, indent=4, sort_keys=False)
                            break # Create new config with new current range
                        else:
                            print("Invalid current range.")
                    break # Decade 0 only has 1 PUND in it, correspoding to cycle number 1

                elif PUND_decade == 1:
                    df = callWGFMU(1, PUND_decade, 0, path_win_stamped + configname)
                    plt_results(ax, df, PUND_decade)
                    break # Decade 1 only has 1 PUND in it, correspoding to cycle number 10 
                
                else:
                    df = callWGFMU(1, PUND_decade, PUND_number, path_win_stamped + configname)
                    # Show graph
                    plt_results(ax, df, PUND_decade)

            plt_save(ax, PUND_decade) # Save fig of each decade as .png


    excelf.save()
    shutil.copy2(path_unix_stamped + "PUND_Data.xlsx", path_unix)
    shutil.copy2(path_unix_stamped + configname, path_unix)
    input("Done press enter to exit")

elif mode == "Single":
    for PUND_number in range(singleNumber):
        df = callWGFMU(2, Decade_start, PUND_number, path_win_stamped + configname)
        plt_results(ax, df, Decade_start)

    plt_save_singles(ax, Decade_start) # Save fig of each decade as .png
    excelf.save()
    shutil.copy2(path_unix_stamped + "PUND_Data.xlsx", path_unix)
    shutil.copy2(path_unix_stamped + configname, path_unix)
    input("Done press enter to exit")
