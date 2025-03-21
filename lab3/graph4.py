import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/4_1q.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y = [int(line.split()[1]) for line in lines]

plt.scatter(x,y)
p1, = plt.plot(x,y)


with open('data/4_2q.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y = [int(line.split()[1]) for line in lines]

plt.scatter(x,y)
p2, = plt.plot(x,y)

plt.grid()

plt.semilogx()
plt.semilogy()

plt.xlabel('Заданная точность')
plt.ylabel('Количество операций')

plt.legend([p1,p2],["Неоптимизированный алгоритм","Оптимизированный алгоритм"])
#plt.show()

plt.savefig('plots/4.png')