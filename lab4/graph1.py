import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/1q.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y = [float(line.split()[1]) for line in lines]
plt.scatter(x,y)
err, = plt.plot(x,y)


with open('data/r1q.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y = [float(line.split()[1]) for line in lines]

plt.scatter(x,y)
r_err, = plt.plot(x,y)

line, = plt.plot(x,[k**4 for k in x])

plt.grid()

plt.semilogx()
plt.semilogy()

plt.xlabel('Длина разбиения')
plt.ylabel('Фактическая точность')

plt.legend([line,err,r_err],["h^p","Фактическая точность","Точность I + R"])

# plt.show()

plt.savefig('plots/1.png')