import math
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8,5))

def real_solut(x):
    return 1/((x+1) * math.log(x+1))

with open('data/1q_1.txt') as f:
    lines = f.readlines()
    x_h2 = [float(line.split()[0]) for line in lines]
    y_h2 = [float(line.split()[1]) for line in lines]



with open('data/1q_2.txt') as f:
    lines = f.readlines()
    x_h = [float(line.split()[0]) for line in lines]
    y_h = [float(line.split()[1]) for line in lines]


err_h2 = [abs(real_solut(x_h2[i]) - y_h2[i]) for i in range(len(x_h2))]
plt.scatter(x_h2,err_h2)
h2, = plt.plot(x_h2,err_h2)


err_h = [abs(real_solut(x_h[i]) - y_h[i]) for i in range(len(x_h))]
plt.scatter(x_h,err_h)
h, = plt.plot(x_h,err_h)

plt.grid()


# bis, = plt.plot([0,.10],[0,.10])

plt.xlabel('X')
plt.ylabel('Фактическая ошибка')

plt.legend([h2,h],['Шаг 2h','Шаг h'])

#plt.show()

plt.savefig('plots/1_2.png')