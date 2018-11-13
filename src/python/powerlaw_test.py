import matplotlib.pyplot as plt
import math
import random

def randomPowerLawNumber(n, x0, x1):
    y = random.random()
    x = ((x1**(n+1) - x0**(n+1))*y + x0**(n+1))**(1/(n+1))
    return math.floor(x)

if __name__ == "__main__":
    x = []
    y = []
    n = -2.5
    start = 1
    end = 10000
    for i in range(2,10000):
        y.append(randomPowerLawNumber(n,start,end))

    plt.plot(y,"bo")
    plt.show()
