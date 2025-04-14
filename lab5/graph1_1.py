import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/1q_1.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y_h2 = [float(line.split()[1]) for line in lines]



with open('data/1q_2.txt') as f:
    lines = f.readlines()
    y_h = [float(line.split()[1]) for line in lines]

with open('data/1q_3.txt') as f:
    lines = f.readlines()
    y_ac = [float(line.split()[1]) for line in lines]


plt.scatter(x,y_h2)
h2, = plt.plot(x,y_h2)

plt.scatter(x,y_h)
h, = plt.plot(x,y_h)

plt.scatter(x,y_ac)
ac, = plt.plot(x,y_ac)


plt.grid()


# bis, = plt.plot([0,.10],[0,.10])

plt.xlabel('X')
plt.ylabel('Y')

plt.legend([h2,h,ac],['Шаг 2h','Шаг h','Точное решение'])

#plt.show()

plt.savefig('plots/1_1.png')