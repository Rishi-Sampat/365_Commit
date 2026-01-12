#1
print(sum(range(1,100)))



#2
numbers = [1, 2, 3, 4, 5, 6, 10, 15, 20]
count = 0

for n in numbers:
    if n % 2 == 0:
        count += 1

print(count)



#3
s = "Cars"
rev = ""
i = len(s) - 1

while i >= 0:
    rev += s[i]
    i -= 1

print ("reverse",rev)




#4
no = [10, 25, 7, 99, 43, 56]
largest = no[0]

for num in no:
    if num > largest:
        largest = num

print("largest:", largest)




#5
N = int(input("series: "))
a, b = 0, 1

for i in range(N):
    print(a, end=" ")
    a, b = b, a + b



#6
list1 = [1,2,2,4,6,2,3,2,6,7,6,6,9]

unique_list = []
for item in list1:
    if item not in unique_list:
        unique_list.append(item)

print("\n Duplicate: ", unique_list)




#7
r = "My Name is Rishi"
freq = dict()

for ch in r:
    if ch in freq:
        freq[ch] += 1
    else:
        freq[ch] = 1

print(freq)




#8
def factorial(n):
    result = 1
    for h in range(1, n+1):
        result *= h
    return result

print(factorial(5))




#9
def get_primes(lst):
    primes = []
    for n in lst:
        if n < 2:
            continue
        is_prime = True
        for i in range(2, int(n**0.5) + 1):
            if n % i == 0:
                is_prime = False
                break
        if is_prime:
            primes.append(n)
    return primes




#10
s = input("Enter string: ")

if s == s[::-1]:
    print("Palindrome")
else:
    print("Not Palindrome")





#11
matrix = [
    [1, 2, 3],
    [4, 5, 6]
]

transpose = [[matrix[j][i] for j in range(len(matrix))] for i in range(len(matrix[0]))]

print(transpose)




#12
d1 = {"a": 1, "b": 2}
d2 = {"c": 3, "d": 4}

merged = {**d1, **d2}
print(merged)




#13
lst = [10, 20, 4, 45, 99]

lst = list(set(lst))  # remove duplicates
lst.sort()

print("Second largest:", lst[-2])





#14
def count_vc(s):
    vowels = "aeiouAEIOU"
    v = c = 0
    for ch in s:
        if ch.isalpha():
            if ch in vowels:
                v += 1
            else:
                c += 1
    return v, c




#15
file = open("text.txt", "r")
text = file.read()
file.close()

lines = text.splitlines()
words = text.split()
chars = len(text)

print("Lines:", len(lines))
print("Words:", len(words))
print("Characters:", chars)




#16
lst = [1, 2, 3, 2, 4, 2, 5]
x = 2

lst = [i for i in lst if i != x]
print(lst)




#17
class Student:
    def __init__(self, name, marks):
        self.name = name
        self.marks = marks

    def display(self):
        print("Name:", self.name, " Marks:", self.marks)

s1 = Student("Riya", 85)
s2 = Student("Aman", 90)
s3 = Student("Kunal", 78)

s1.display()
s2.display()
s3.display()




#18
squares = [i*i for i in range(1, 11)]
print(squares)




#19
lst = [1, 2, 3, 4, 5]
lst = [lst[-1]] + lst[:-1]
print(lst)
