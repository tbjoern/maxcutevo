import math
import random

def randomPowerLawNumber(n, x0, x1):
    y = random.random()
    x = ((x1**(n+1) - x0**(n+1))*y + x0**(n+1))**(1/(n+1))
    return math.floor(x)

def main():
    exponent = -1.5
    interval_start = 1
    interval_end = 1000

    with open("power_law_data.csv", "w") as file:
        for i in range((interval_end - interval_start) * 10):
            number = randomPowerLawNumber(exponent, interval_start, interval_end)
            file.write(str(i) + "," + str(number) + "\n")

main()
