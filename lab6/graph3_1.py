import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/3q_1.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y = [float(line.split()[1]) for line in lines]

plt.scatter(x,y)
err, = plt.plot(x,y)

plt.grid()

plt.semilogx()
plt.semilogy()

bis, = plt.plot([0,.10],[0,.10])

plt.xlabel('Заданная точность')
plt.ylabel('Фактическая ошибка')

plt.legend([err,bis],['Фактическая точность','Биссектриса'])

#plt.show()

plt.savefig('plots/3_1.png')