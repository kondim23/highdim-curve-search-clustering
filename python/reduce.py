from keras.models import Model
import matplotlib.pyplot as plt
import keras
import pandas as pd
import numpy as np
from keras.layers import *
from sklearn.preprocessing import MinMaxScaler
import sys
import os
from common_utils import *


#current file path + /models/
model_path=os.getcwd()+"/"+os.path.dirname(__file__)+"/models/"


#create X set through windowing/dividing the time serie
def windowing(serie, window_size):
    X=[]

    for i in range(window_size, len(serie), window_size):
        X.append(serie[i-window_size:i, 0])

    return np.array(X)
    

#split serie to training and test serie
#perform scaling and windowing
def split_serie_convolutions(time_serie, scaler, train_serie_len, window_length):
    Serie_train = time_serie.iloc[:train_serie_len]
    Serie_test = time_serie.iloc[train_serie_len - window_length:]
    Y_test = time_serie.iloc[train_serie_len:]

    Serie_train = scaler.fit_transform(Serie_train)
    Serie_test = scaler.transform(Serie_test)

    X_train = windowing(Serie_train, window_length)
    X_test = windowing(Serie_test, window_length)

    return (X_train, X_test)


#predict and plot encoded and decoded time serie
def compare_training_results(encoder, autoencoder, X_test):
    Pred_encode_test = encoder.predict(X_test)
    Pred_decode_test = autoencoder.predict(X_test)

    X_test = [item for list_ in X_test for item in list_]
    Pred_encode_test = [item for list_ in Pred_encode_test for item in list_]
    Pred_decode_test = [item for list_ in Pred_decode_test for item in list_]

    plt.figure(figsize=(10,10))
    plt.plot(range(len(X_test)),X_test, color = 'red', label = 'Validating Time Serie')
    plt.plot(range(len(Pred_encode_test)),Pred_encode_test, color = 'green', label = 'Encoded Time Serie')
    plt.plot(range(len(Pred_decode_test)),Pred_decode_test, color = 'blue', label = 'Decoded Time Serie')
    plt.title('Time Series Compression')
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.legend()
    plt.show()


#chech arguments

if len(sys.argv)!=11:
    print("Error: Wrong arguments")
    exit()

for i in range(1,len(sys.argv),2):
    if sys.argv[i]=="-d":
        input_dataset_filename = str(sys.argv[i+1])
    elif sys.argv[i]=="-q":
        input_queryset_filename = str(sys.argv[i+1])
    elif sys.argv[i]=="-od":
        output_dataset_filename = str(sys.argv[i+1])
    elif sys.argv[i]=="-oq":
        output_queryset_filename = str(sys.argv[i+1])
    elif sys.argv[i]=="-t":
        online_training = True if str(sys.argv[i+1])=="online_all" else False


#load dataset and queryset
dataset = pd.read_csv(input_dataset_filename, '\t', header=None, index_col=0)
queryset = pd.read_csv(input_queryset_filename, '\t', header=None, index_col=0)


#model hyperparameters
window_length = 50
epochs = 100
batch_size = 32
split_percentage = 0.8
n, time_serie_len = dataset.shape

#model layers
input_window = Input(shape=(window_length,1))
x = Conv1D(16, 3, activation="relu", padding="same")(input_window)
x = MaxPooling1D(2, padding="same")(x)
x = Conv1D(16, 3, activation="relu", padding="same")(x)
x = MaxPooling1D(2, padding="same")(x)
x = Conv1D(1, 3, activation="relu", padding="same")(x)
encoded = MaxPooling1D(2, padding="same")(x)

#the encoder produced
encoder = Model(input_window, encoded)

x = Conv1D(1, 3, activation="relu", padding="same")(encoded)
x = UpSampling1D(2)(x)
x = Conv1D(16, 2, activation='relu')(x)
x = UpSampling1D(2)(x)
x = Conv1D(16, 2, activation='relu')(x)
x = UpSampling1D(2)(x)
decoded = Conv1D(1, 3, activation='sigmoid', padding='same')(x)

#the autoencoder produced
autoencoder = Model(input_window, decoded)
autoencoder.compile(optimizer='adam', loss='binary_crossentropy')


train_serie_len = int(time_serie_len * split_percentage)
scaler          = MinMaxScaler(feature_range=(0,1))


#train the model on current execution
if online_training:
    all_sets = []

    #fill all_sets
    for serie_index in range(0,n):
        time_serie = pd.DataFrame(np.array(dataset.iloc[serie_index]),columns=[dataset.index[serie_index]])
        all_sets.append(split_serie_convolutions(time_serie, scaler, train_serie_len, window_length))

    #concatenate all sets
    (X_train, X_test) = group_sets(all_sets)
    
    history = autoencoder.fit(
        X_train, X_train, 
        epochs=epochs, 
        batch_size=batch_size, 
        validation_data=(X_test, X_test)
        )
    
    #plot train and validation losses from previous fit
    plot_training_loss(history)

    #predict and plot encoded and dedoded time serie
    for (_, X_test) in all_sets[:10]:
        compare_training_results(encoder, autoencoder, X_test)

    #save models
    encoder.save(model_path+"encoder")
    autoencoder.save(model_path+"autoencoder")

#load an offline model
else:
    encoder = keras.models.load_model(model_path+"encoder")
    autoencoder = keras.models.load_model(model_path+"autoencoder")


#generate dataset and queryset
for cur_df in [(dataset, output_dataset_filename), (queryset, output_queryset_filename)]:

    #will contain the final output
    compressed_output = pd.DataFrame()

    for serie_index in range(cur_df[0].shape[0]):

        #the time serie id
        output_index = cur_df[0].index[serie_index]

        #get the time serie
        time_serie = pd.DataFrame(np.array(cur_df[0].iloc[serie_index]),columns=[output_index])

        #scale the time seire
        time_serie = scaler.fit_transform(time_serie)

        #divide the time serie into convolutions
        time_serie = windowing(time_serie, window_length)

        #generate the encoded time serie
        time_serie = encoder.predict(time_serie)
        time_serie = time_serie.flatten()

        #initialize the column headers (first time only)
        if (len(compressed_output.columns)==0):
            compressed_output = pd.DataFrame(columns=range(time_serie.shape[0]))

        #add the new encoded time serie
        compressed_output.loc[output_index] = time_serie

    #export to csv and download
    compressed_output.to_csv(cur_df[1], sep='\t', header=False)

keras.backend.clear_session()