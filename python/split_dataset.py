#!/usr/bin/python3
import pandas as pd
import sys
import os

#check arguments
if len(sys.argv)==1:
    print("Error: File Path was not provided")
    exit()

if not os.path.isfile(sys.argv[1]):
    print("Error: File "+sys.argv[1]+" does not exist")
    exit()

#load input dataframe
df = pd.read_csv(sys.argv[1], '\t', header=None, index_col=0)

#export csv dataset and queryset
df[:350].to_csv(os.path.dirname(sys.argv[1])+"/dataset.csv", sep='\t', header=False)
df[350:].to_csv(os.path.dirname(sys.argv[1])+"/queryset.csv", sep='\t', header=False)