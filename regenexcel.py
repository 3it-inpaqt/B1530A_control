import numpy as np
import pandas as pd
import os
import re
import shutil

#C:\Users\ngariepy\Documents\UdeS\T4\Keysight\W\Q294A\ml41

path = "/home/ngariepy/Documents/UdeS/T4/Keysight/W/Q294A/ml41/"
path_stamped = path + "Data_2022-10-07_132849/"
excelf = pd.ExcelWriter(path_stamped + "PUND_Data.xlsx")

pat = re.compile("PUND_\d{2,3}.csv")
csvs = [file for file in os.listdir(path_stamped) if pat.fullmatch(file)]

for csv in csvs:
    df = pd.read_csv(path_stamped + csv, sep=';')
    csv_new = csv.rstrip(".csv")
    csv_new = csv_new.lstrip("PUND_")
    decade = csv_new[0]
    number = csv_new[1:]
    df.to_excel(excelf, sheet_name="PUND_"+str(decade)+str(number))

excelf.save()
shutil.copy2(path_stamped + "/PUND_Data.xlsx", path)    