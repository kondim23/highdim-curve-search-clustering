from keras.models import Model
import matplotlib.pyplot as plt
import keras
import pandas as pd
import numpy as np
from keras.layers import *
from sklearn.preprocessing import MinMaxScaler
import sys
from common_utils import *

models_path = "./python/models/"

def windowing(serie, window_size):
    X=[]

    for i in range(window_size, len(serie), window_size):
        X.append(serie[i-window_size:i, 0])

    return np.array(X)
    
def split_serie_convolutions(time_serie, scaler, train_serie_len, window_length):
    Serie_train = time_serie.iloc[:train_serie_len]
    Serie_test = time_serie.iloc[train_serie_len - window_length:]
    Y_test = time_serie.iloc[train_serie_len:]

    Serie_train = scaler.fit_transform(Serie_train)
    Serie_test = scaler.transform(Serie_test)

    X_train = windowing(Serie_train, window_length)
    X_test = windowing(Serie_test, window_length)

    return (X_train, X_test)

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
        online_training = True if str(sys.argv[i+1])=="True" else False



dataset = pd.read_csv(input_dataset_filename, '\t', header=None, index_col=0)
queryset = pd.read_csv(input_queryset_filename, '\t', header=None, index_col=0)


window_length = 50
epochs = 100
batch_size = 32
split_percentage = 0.8
n, time_serie_len = dataset.shape

input_window = Input(shape=(window_length,1))
x = Conv1D(16, 3, activation="relu", padding="same")(input_window)
x = MaxPooling1D(2, padding="same")(x)
x = Conv1D(16, 3, activation="relu", padding="same")(x)
x = MaxPooling1D(2, padding="same")(x)
x = Conv1D(1, 3, activation="relu", padding="same")(x)
encoded = MaxPooling1D(2, padding="same")(x)
encoder = Model(input_window, encoded)

x = Conv1D(1, 3, activation="relu", padding="same")(encoded)
x = UpSampling1D(2)(x)
x = Conv1D(16, 2, activation='relu')(x)
x = UpSampling1D(2)(x)
x = Conv1D(16, 2, activation='relu')(x)
x = UpSampling1D(2)(x)
decoded = Conv1D(1, 3, activation='sigmoid', padding='same')(x)
autoencoder = Model(input_window, decoded)
autoencoder.compile(optimizer='adam', loss='binary_crossentropy')

train_serie_len = int(time_serie_len * split_percentage)
scaler          = MinMaxScaler(feature_range=(0,1))


if online_training:
    all_sets = []
    history = []

    for serie_index in range(0,n):
        time_serie = pd.DataFrame(np.array(dataset.iloc[serie_index]),columns=[dataset.index[serie_index]])
        all_sets.append(split_serie_convolutions(time_serie, scaler, train_serie_len, window_length))

    (X_train, X_test) = group_sets(all_sets)
    history = autoencoder.fit(
        X_train, X_train, 
        epochs=epochs, 
        batch_size=batch_size, 
        validation_data=(X_test, X_test)
        )
    plot_training_loss(history)

    for (_, X_test) in all_sets[:10]:
        compare_training_results(encoder, autoencoder, X_test)

    encoder.save(models_path+"encoder")
    autoencoder.save(models_path+"autoencoder")
else:
    encoder = keras.models.load_model(models_path+"encoder")
    autoencoder = keras.models.load_model(models_path+"autoencoder")


for cur_df in [(dataset, output_dataset_filename), (queryset, output_queryset_filename)]:
    compressed_output = pd.DataFrame()
    for serie_index in range(cur_df[0].shape[0]):
        output_index = cur_df[0].index[serie_index]
        time_serie = pd.DataFrame(np.array(cur_df[0].iloc[serie_index]),columns=[output_index])
        time_serie = scaler.fit_transform(time_serie)
        time_serie = windowing(time_serie, window_length)
        time_serie = encoder.predict(time_serie)
        time_serie = time_serie.flatten()
        if (len(compressed_output.columns)==0):
            compressed_output = pd.DataFrame(columns=range(time_serie.shape[0]))
        compressed_output.loc[output_index] = time_serie

    compressed_output.to_csv(cur_df[1], sep='\t', header=False)

keras.backend.clear_session()