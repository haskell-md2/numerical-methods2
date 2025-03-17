import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/3q.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y = [float(line.split()[1]) for line in lines]

plt.scatter(x,y)
amm, = plt.plot(x,y)

line, = plt.plot(x,[k**4 for k in x])

plt.grid()

plt.semilogx()
plt.semilogy()

plt.xlabel('Длина разбиения')
plt.ylabel('Фактическая точность')


#plt.show()

plt.savefig('plots/3.png')