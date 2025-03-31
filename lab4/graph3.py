import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/3_1q.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y1 = [int(line.split()[1]) for line in lines]

plt.scatter(x,y1)
err_set, = plt.plot(x,y1)

with open('data/3_2q.txt') as f:
    lines = f.readlines()
    x = [int(line.split()[0]) for line in lines]
    y2 = [float(line.split()[1]) for line in lines]


plt.scatter(y2,x)
err_rung, = plt.plot(y2,x)

plt.grid()

plt.semilogx()


plt.xlabel('Точность')
plt.ylabel('Число разбиений')

plt.legend([err_set,err_rung],["Заданная","По Рунге"])

#plt.show()

plt.savefig('plots/3.png')