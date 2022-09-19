"""
2022-09-19
Nicolas Gariépy
Runs a PUND measurement, using WGFMU.exe on the Keysight B1500A with a B1530.`
Define pulse params directly in the "program_args" dict.
"""
import math
from subprocess import call
from pathlib import Path
import json
import os
import matplotlib.pyplot as plt
import numpy as np
from time import perf_counter

tstart = perf_counter()

# Params
# Data output location
Material='TiN'
Motif= ''
sample= "Q294E"
FTJ = "br32"
Cycle_shape='Squared'
Waiting = True                     # define if there's a pause included in the file before the PUND signal
pund_1_detailed = False            # Do PUNDs 2 to 9 iaf true, else skip

if Motif=='':
    path_unix = Path("/home/ngariepy/Documents/UdeS/T4/Keysight/"+str(Material)+'/'+str(sample)+'/'+str(FTJ))
    path_win = "C:\\Users\\ngariepy\\Documents\\UdeS\\T4\\Keysight\\"+str(Material)+'\\'+str(sample)+'\\'+str(FTJ)
else:
    path_unix = Path("/home/ngariepy/Documents/Udes/T4/Keysight/"+str(Material)+'/'+str(Motif)+'/'+str(sample)+'/'+str(FTJ))
    path_win = "C:\\Users\\ngariepy\\Documents\\UdeS\\T4\\Keysight\\"+str(Material)+'\\'+str(Motif)+'\\'+str(sample)+'\\'+str(FTJ)

configname = "config0.json"

PUND_decade = 0
program_args = {
    "PUND_decade":PUND_decade,
    "currentrange":1, # in µA
    "npoints":1000, # Current reading temporal resolution
    "path": path_win,
    # PUND
    "PUND_shape":{
        "Vpulse": 3,
        "trise": 50e-6, #rising time of the pulse in s
        "twidth":0, #width of the pund pulse in s
        "tspace": 10e-6 #time between pulses in s
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
os.makedirs(str(path_unix), exist_ok=True)
with open( str(path_unix) + "/" + configname, "w", encoding="utf_8") as outfile:
    json.dump(program_args, outfile, indent=4, sort_keys=False)

# Do PUND00 (special case, one measurement, no aging, asks for current range adjust)¸
exepath = "./Release/WGFMU.exe"
retval = call([exepath, "1", str(PUND_decade), "0", path_win + "\\" + configname])

if retval != 0:
    print("WGFMU.exe finished execution with error code:" + str(retval))
    print("Most likely an error with the python script, the exe will run \"correctly\" even if not connected to the machine")
    exit()
# Show graph

PUND_decade +=1
# Ask for current range adjust
while True:
    choice = input("Change current range : (default = keep same)")
    if choice == "":
        break # Keep same config, nothing to do

    choice = float(choice)
    if(choice>=1 and math.log10(choice).is_integer() and math.log10(choice) < 5):
        program_args["currentrange"] = choice # new current range
        program_args["PUND_decade"] = PUND_decade # new current range
        configname = "config" + str(PUND_decade) + ".json"
        with open( str(path_unix) + "/" + configname, "w", encoding="utf_8") as outfile:
            json.dump(program_args, outfile, indent=4, sort_keys=False)
        break # Create new config with new current range
    else:
        print("Invalid current range.")
# Do all other PUNDS
for PUND_decade in range(1,8): #8 decades is approx max?
    for PUND_number in range(10):
        call([exepath, "1", str(PUND_decade), str(PUND_number), path_win + "\\" + configname])
        # Show graph


# Do next pund 
# end

