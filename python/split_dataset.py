#!/usr/bin/python3
import pandas as pd
import sys
import os


if len(sys.argv)==1:
    print("Error: File Path was not provided")
    exit()

if not os.path.isfile(sys.argv[1]):
    print("Error: File "+sys.argv[1]+" does not exist")
    exit()


df = pd.read_csv(sys.argv[1], '\t', header=None, index_col=0)
df[:350].to_csv("./data/time-series/dataset.csv", sep='\t', header=False)
df[350:].to_csv("./data/time-series/queryset.csv", sep='\t', header=False)