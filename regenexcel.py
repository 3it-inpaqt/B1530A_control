import numpy as np
import pandas as pd
import os
import re
import shutil

#C:\Users\ngariepy\Documents\UdeS\T4\Keysight\W\Q294A\ml41

samples = ["mr22",]
base_path = "/home/ngariepy/Documents/UdeS/T4/Keysight/TiN/P474FB/"
#path_stamped = path + "Data_2022-10-07_132849/"
#excelf = pd.ExcelWriter(path_stamped + "PUND_Data.xlsx")

stamp = re.compile("Data")
pat = re.compile("PUND_\d{2,3}.csv")

for sample in samples:
    path = next((base_path + sample + '/' + file + '/' for file in os.listdir(base_path + sample) if stamp.match(file)),None)
    excelf = pd.ExcelWriter(path + "PUND_Data.xlsx")
    csvs = [file for file in os.listdir(path) if pat.fullmatch(file)]
        
    for csv in csvs:
        df = pd.read_csv(path + csv, sep=';')
        csv_new = csv.rstrip(".csv")
        csv_new = csv_new.lstrip("PUND_")
        decade = csv_new[0]
        number = csv_new[1:]
        df.to_excel(excelf, sheet_name="PUND_"+str(decade)+str(number))
    excelf.save()
    shutil.copy2(path + "PUND_Data.xlsx", base_path+sample)
    #shutil.copy2(path + "config0.json", base_path+sample)    



