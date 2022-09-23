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

# Params
# Data output location
Material='Test'
Motif= ''
sample= "Q294A"
FTJ = "square100"
Cycle_shape='Squared'
Waiting = True                     # define if there's a pause included in the file before the PUND signal
pund_1_detailed = False            # Do PUNDs 2 to 9 if true, else skip

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

Decade_start = 0
Decade_stop = 7
program_args = {
    "PUND_decade":0,
    "currentrange":10000, # in µA
    "npoints":800, # Current reading temporal resolution
    "path": path_win + "Data_" + timestamp,
    # PUND
    "PUND_shape":{
        "Vpulse": 2.5,
        "trise": 50e-6, #rising time of the pulse in s
        "twidth":50e-6, #width of the pund pulse in s
        "tspace": 50e-6 #time between pulses in s
        #tpulse = 2*pentep + palierp + inter
    },
    # Cycling square wave
    "aging_shape":{
        "Vpulse": 3,
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

def fake_data(data, decade):
    data["Current"] = np.abs(np.random.randn(100))*data["Voltage"]
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
    return ax2

def plt_update_axis(axis, data):
    if plt.fignum_exists(axis.get_figure().number):
        axis.plot(data["Time"], data["Current"])
        axis.get_figure().canvas.draw()
        axis.get_figure().canvas.flush_events()

def callWGFMU(decade, number, configpath):
    retval = call([exe_path, "1", str(decade), str(number), configpath])
    if retval != 0:
        print("WGFMU.exe finished execution with error code:" + str(retval))
        print("Most likely an error with the python script, the exe will run \"correctly\" even if not connected to the machine")
        exit()
    # LOG HERE
    with open(path_unix + "Allmeasurements.log", 'a') as logfile:
        logfile.write(timestamp + ';' + str(decade) + ';' + str(number) + ';' + configpath + '\n')
    return get_results(decade, number)
    #return fake_data(fakedf, decade)

ax = []
excelf = pd.ExcelWriter(path_unix + "PUND_Data.xlsx")

for PUND_decade in range(Decade_start, Decade_stop+1):
        # PUND_number : 9 mesures, de 2 à 10 (11 est exclu du range). 10x10^(n-1) = 1x10^(n)
        for PUND_number in range(2,11): # [2x10^(n-1), 3x10^(n-1), 4x10^(n-1), ... 9x10^(n-1), 10x10^(n-1)]
            if PUND_decade == 0:
                # Do PUND00 (special case, one measurement, no aging, asks for current range adjust)¸
                df = callWGFMU(PUND_decade, 0, path_win_stamped + configname)
                plt_results(ax, df, PUND_decade)

                # Ask for current range adjust
                while True:
                    choice = input("Change current range : (default = keep same)")
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
                df = callWGFMU(PUND_decade, 0, path_win_stamped + configname)
                plt_results(ax, df, PUND_decade)
                break # Decade 1 only has 1 PUND in it, correspoding to cycle number 10 
            
            else:
                df = callWGFMU(PUND_decade, PUND_number, path_win_stamped + configname)
                # Show graph
                plt_results(ax, df, PUND_decade)

excelf.save()

