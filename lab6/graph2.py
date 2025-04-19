import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))


with open('data/2q.txt') as f:
    lines = f.readlines()
    x = [float(line.split()[0]) for line in lines]
    y = [float(line.split()[1]) for line in lines]

plt.scatter(x,y)
err, = plt.plot(x,y)

line, = plt.plot(x,[k**3 for k in x])


plt.grid()

plt.semilogx()
plt.semilogy()

# bis, = plt.plot([0,.10],[0,.10])

plt.xlabel('Величина шага')
plt.ylabel('Фактическая точность')

plt.legend([err,line],['Кривая фактической точности', 'линия h^p'])

#plt.show()

plt.savefig('plots/2.png')