#!/usr/bin/python3

from common_utils import *
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
import sys
import os

#current file path + /models/
model_path=os.getcwd()+"/"+os.path.dirname(__file__)+"/models/"


#predict and plot forecast results
def forecast(model, scaler, X_test, Y_test, train_serie_len, time_serie_len):
    Pred_test = model.predict(X_test)
    Pred_test = scaler.inverse_transform(Pred_test)
    Y_test = scaler.inverse_transform(Y_test.reshape(-1,1))

    plt.figure(figsize=(10,10))
    plt.plot(range(train_serie_len, time_serie_len), Y_test, color = 'red', label = 'Validating Time Serie')
    plt.plot(range(train_serie_len, time_serie_len), Pred_test, color = 'blue', label = 'Forecasting Time Serie')
    plt.title('Time Series Forecasting')
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.legend()
    plt.show()


#chech arguments

if len(sys.argv)!=7:
    print("Error: Wrong arguments")
    exit()


online_training, self_time_serie_training = False, False
for i in range(1,len(sys.argv),2):
    if sys.argv[i]=="-d":
        dataset_filename = str(sys.argv[i+1])
    elif sys.argv[i]=="-n":
        n = int(sys.argv[i+1])
    elif sys.argv[i]=="-t":
        if sys.argv[i+1]=="online_all":
            online_training = True
        elif sys.argv[i+1]=="online_self":
            self_time_serie_training = True


if not os.path.isfile(dataset_filename):
    print("Error: File "+dataset_filename+" does not exist")
    exit()


#get input data
df = pd.read_csv(dataset_filename, '\t', header=None, index_col=0)


#model hyperparameters
time_steps = 60
epochs = 20
batch_size = 128
split_percentage = 0.8


#model layers
model = Sequential()
model.add(LSTM(units = 128, return_sequences = True, input_shape = (time_steps, 1)))
model.add(Dropout(0.2))
model.add(LSTM(units = 128, return_sequences = True))
model.add(Dropout(0.2))
model.add(LSTM(units = 128, return_sequences = True))
model.add(Dropout(0.2))
model.add(LSTM(units = 64, return_sequences = True))
model.add(Dropout(0.2))
model.add(LSTM(units = 64, return_sequences = True))
model.add(Dropout(0.2))
model.add(LSTM(units = 64, return_sequences = True))
model.add(Dropout(0.2))
model.add(LSTM(units = 64, return_sequences = True))
model.add(Dropout(0.2))
model.add(LSTM(units = 64, return_sequences = True))
model.add(Dropout(0.2))
model.add(LSTM(units = 64))
model.add(Dropout(0.2))
model.add(Dense(units = 1))
model.compile(optimizer = 'adam', loss = 'mean_squared_error')


time_serie_len  = df.shape[1]
train_serie_len = int(time_serie_len * split_percentage)
scaler          = MinMaxScaler(feature_range=(0,1))


#train model based on self time serie
if self_time_serie_training:

    #can only be online
    for serie_index in range(n):
        
        #get time serie
        time_serie = pd.DataFrame(np.array(df.iloc[serie_index]),columns=[df.index[serie_index]])
        
        #split time serie to sets
        X_train, Y_train, X_test, Y_test = split_serie(time_serie, scaler, train_serie_len, time_steps)
        
        #train the model
        history = model.fit(X_train, Y_train, epochs = epochs, batch_size = batch_size, validation_data=(X_test,Y_test))
        
        #plot train and validation losses from previous fit
        plot_training_loss(history)

        #predict and plot forecast results
        forecast(model, scaler, X_test, Y_test, train_serie_len, time_serie_len)

        keras.backend.clear_session()

    exit()


#all_sets contains objs of type (X_train, Y_train, X_test, Y_test) of time series
all_sets = []

#fill all_sets
for serie_index in range(df.shape[0]):
    time_serie = pd.DataFrame(np.array(df.iloc[serie_index]),columns=[df.index[serie_index]])
    all_sets.append(split_serie(time_serie, scaler, train_serie_len, time_steps))


#train the model on current execution
if online_training:

    #concatenate all sets
    (X_train,Y_train,X_test,Y_test) = group_sets(all_sets)
    
    history = model.fit(
        X_train, Y_train,
        epochs = epochs, 
        batch_size = batch_size, 
        validation_data=(X_test,Y_test)
        )
    
    #plot train and validation losses from previous fit
    plot_training_loss(history)
    model.save(model_path+"forecasting")

#load an offline model
else:
    model = keras.models.load_model(model_path+"forecasting")


#predict and plot forecast results for test sets
for (_, _, X_test, Y_test) in all_sets[:n]:
    forecast(model, scaler, X_test, Y_test, train_serie_len, time_serie_len)

keras.backend.clear_session()