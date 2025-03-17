import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/2q.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y = [int(line.split()[1]) for line in lines]

plt.scatter(x,y)
amm, = plt.plot(x,y)

plt.grid()

plt.semilogx()
plt.semilogy()

plt.xlabel('Заданная точность')
plt.ylabel('Число разбиений')


#plt.show()

plt.savefig('plots/2.png')