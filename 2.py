# Write a Python program to create a 5×5 NumPy array with values from 1 to 25 and extract the first two rows.

import numpy as np

arr=np.arange(0,25).reshape(5,5)
rows=arr[:2,:]

print(rows)
# Write a Python program to find all the prime numbers within a given range.

for num in range(10, 30):
    if num > 1:
        for i in range(2, int(num ** 0.5) + 1):
            if num % i == 0:
                break
        else:
            print(num)