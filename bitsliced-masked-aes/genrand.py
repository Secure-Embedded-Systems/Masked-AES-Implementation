#!/usr/bin/python
from random import randint

s = '{'
for i in range(0, 256/16):
    for j in range(0,16):
        s += '(word_t)'+str(hex(randint(0,1<<64))) + ', '
    s += '\n'
s += '}'

print 'random (8-64 bit):'
print s

def randslice():
    r = randint(0,1<<32)
    r2 = 0
    for i in range(0,32):
        b = r & (1 << i)
        r2 |= ((b << i)| (b << (i+1)))

    return r2


print
print
print 'random sliced (8-64 bit):'

s = '{'
for i in range(0, 256/6):
    for j in range(0,6):
        s += '(word_t)'+str(hex(randslice())).replace('L','') + 'ULL, '
    s += '\n'
s += '}'

print s
