#!/usr/bin/python

def rotate(b):
    Bbbb = (b & 0b11000000) >> 6
    bBbb = (b & 0b00110000) >> 4
    bbBb = (b & 0b00001100) >> 2
    bbbB = (b & 0b00000011) >> 0

    Bbbb = ((Bbbb & 1) << 1) | (Bbbb >> 1)
    bBbb = ((bBbb & 1) << 1) | (bBbb >> 1)
    bbBb = ((bbBb & 1) << 1) | (bbBb >> 1)
    bbbB = ((bbbB & 1) << 1) | (bbbB >> 1)

    return (Bbbb << 6) | (bBbb << 4) | (bbBb << 2) | (bbbB << 0)

s =''

s = '{'
for i in range(0, 256/16):
    for j in range(0,16):
        s += str(hex(rotate(i * 16 + j))) + ', '
    s += '\n'
s += '}'

print s
