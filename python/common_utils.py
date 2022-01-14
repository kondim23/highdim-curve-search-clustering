#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt

def vectorize(serie, time_steps):
    X = []
    Y = []

    for i in range(time_steps, len(serie)):
        X.append(serie[i-time_steps:i, 0])
        Y.append(serie[i, 0])

    return np.array(X), np.array(Y)

def split_serie(time_serie, scaler, train_serie_len, time_steps):
    Serie_train = time_serie.iloc[:train_serie_len]
    Serie_test = time_serie.iloc[train_serie_len - time_steps:]

    Serie_train = scaler.fit_transform(Serie_train)
    Serie_test = scaler.transform(Serie_test)

    X_train, Y_train = vectorize(Serie_train, time_steps)
    X_test, Y_test = vectorize(Serie_test, time_steps)

    X_train = np.reshape(X_train, (X_train.shape[0], X_train.shape[1], 1))
    X_test = np.reshape(X_test, (X_test.shape[0], X_test.shape[1], 1))

    return (X_train, Y_train, X_test, Y_test)

def plot_training_loss(stats):
    plt.plot(stats.stats['loss'])
    plt.plot(stats.stats['val_loss'])
    plt.title('Model Total Loss')
    plt.ylabel('Loss')
    plt.xlabel('Epoch')
    plt.legend(['Train', 'Test'], loc='upper left')
    plt.show()

def group_sets(all_sets):
    grouped_sets = list(all_sets[0])
    for single_set in all_sets[1:]:
        for index in range(len(single_set)):
            grouped_sets[index] = np.concatenate((grouped_sets[index],single_set[index]))
    return grouped_sets