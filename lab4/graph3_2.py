import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/3_2q.txt') as f:
    lines = f.readlines()
    x = [int(line.split()[0]) for line in lines]
    y = [float(line.split()[1]) for line in lines]

plt.scatter(x,y)
err, = plt.plot(x,y)

plt.grid()

plt.semilogy()


plt.ylabel('Точность по Рунге')
plt.xlabel('Число разбиений')


#plt.show()

plt.savefig('plots/3_2.png')