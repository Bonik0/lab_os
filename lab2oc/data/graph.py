import matplotlib.pyplot as plt
import csv

X = []
Y = []

with open('data.csv', 'r') as datafile:
    plotting = csv.reader(datafile, delimiter=';')

    for ROWS in plotting:
        X.append(float(ROWS[0]))
        Y.append(float(ROWS[1]))

plt.plot(X, Y)
plt.xlim([1, 12])
plt.ylim([0, 600])
plt.ylabel(r'Время, мc', fontsize = 14)
plt.xlabel(r'Количество потоков', fontsize = 14)
plt.title(r'Время / число потоков. Массив из 100000 элементов', fontsize = 16)
plt.grid(True)
plt.show()