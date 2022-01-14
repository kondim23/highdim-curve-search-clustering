#!/usr/bin/python3

from common_utils import *
from keras.layers import Input, Dense, Conv1D, MaxPooling1D, UpSampling1D, BatchNormalization, LSTM, RepeatVector, Reshape
from keras.models import Model
import matplotlib.pyplot as plt
import keras
import pandas as pd
import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM
from keras.layers import Dropout
from keras.layers import *
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import mean_squared_error
import sys
import os


#current file path + /models/
model_path=os.getcwd()+"/"+os.path.dirname(__file__)+"/models/"


#predict and plot anomalies
def detect_anomalies(model, X_test, Y_test, train_serie_len, time_serie_len, mae):
    Pred_test = model.predict(X_test)
    Loss_test = np.mean(np.abs(Pred_test - X_test), axis=1)

    anomaly_index = np.where(Loss_test > mae)[0]
    anomaly_value = Y_test[anomaly_index]
    anomaly_index += train_serie_len

    plt.figure(figsize=(10,10))
    plt.plot(range(train_serie_len,time_serie_len),Y_test, color = 'red', label = 'Time Serie')
    plt.scatter(anomaly_index, anomaly_value, color = 'blue', label = 'Anomalies')
    plt.title('Time Series Anomaly Detection')
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.legend()
    plt.show()


#chech arguments

if len(sys.argv)!=9:
    print("Error: Wrong arguments")
    exit()


for i in range(1,len(sys.argv),2):
    if sys.argv[i]=="-d":
        dataset_filename = str(sys.argv[i+1])
    elif sys.argv[i]=="-n":
        n = int(sys.argv[i+1])
    elif sys.argv[i]=="-mae":
        mae = float(sys.argv[i+1])
    elif sys.argv[i]=="-t":
        online_training = True if sys.argv[i+1]=="online_all" else False


if not os.path.isfile(dataset_filename):
    print("Error: File "+dataset_filename+" does not exist")
    exit()


#load input dataframe
df = pd.read_csv(dataset_filename, '\t', header=None, index_col=0)


#model hyperparameters
time_steps = 60
epochs = 20
batch_size = 128
split_percentage = 0.8

#model layers
model = keras.Sequential()
model.add(keras.layers.LSTM(
    units=32,
    input_shape=(time_steps,1),
    activation="relu"
))
model.add(keras.layers.Dropout(rate=0.2))
model.add(keras.layers.RepeatVector(n=time_steps))
model.add(keras.layers.LSTM(units=32, return_sequences=True, activation="relu"))
model.add(keras.layers.Dropout(rate=0.2))
model.add(keras.layers.LSTM(units=32, return_sequences=True, activation="relu"))
model.add(keras.layers.Dropout(rate=0.2))
model.add(keras.layers.LSTM(units=32, return_sequences=True, activation="relu"))
model.add(keras.layers.Dropout(rate=0.2))
model.add(keras.layers.LSTM(units=32, return_sequences=True, activation="relu"))
model.add(keras.layers.Dropout(rate=0.2))
model.add(keras.layers.LSTM(units=32, return_sequences=True, activation="relu"))
model.add(keras.layers.Dropout(rate=0.2))
model.add(
  keras.layers.TimeDistributed(
    keras.layers.Dense(units=1)
  )
)
model.compile(loss='mae', optimizer='adam')


time_serie_len  = df.shape[1]
train_serie_len = int(time_serie_len * split_percentage)
scaler          = MinMaxScaler(feature_range=(0,1))


#all_sets contains objs of type (X_train, Y_train, X_test, Y_test) of time series
all_sets = []

#fill all_sets
for serie_index in range(df.shape[0]):
    time_serie = pd.DataFrame(np.array(df.iloc[serie_index]),columns=[df.index[serie_index]])
    all_sets.append(split_serie(time_serie, scaler, train_serie_len, time_steps))


#train the model on current execution
if online_training:

    #concatenate all sets
    (X_train, Y_train, X_test, Y_test) = group_sets(all_sets)

    history = model.fit(
        X_train, Y_train, 
        epochs = epochs, 
        batch_size = batch_size, 
        validation_data=(X_test,Y_test)
        )

    #plot train and validation losses from previous fit
    plot_training_loss(history)
    model.save(model_path+"anomaly_detection")

#load an offline model
else:
    model = keras.models.load_model(model_path+"anomaly_detection")


#predict and plot anomaly detection results for test sets
for (_, _, X_test, Y_test) in all_sets[:n]:
    detect_anomalies(model, X_test, Y_test, train_serie_len, time_serie_len, mae)

keras.backend.clear_session()