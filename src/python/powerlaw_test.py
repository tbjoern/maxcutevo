import matplotlib.pyplot as plt
import math
import random
from numpy.random import exponential

def randomPowerLawNumber(n, x0, x1):
    y = random.random()
    x = ((x1**(n+1) - x0**(n+1))*y + x0**(n+1))**(1/(n+1))
    return math.floor(x)

if __name__ == "__main__":
    x = []
    y = []
    beta = 1.5
    start = 1
    end = 10000
    for i in range(2,10000):
        y.append(exponential(beta))

    plt.plot(y,"bo")
    plt.show()
