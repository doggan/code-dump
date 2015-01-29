################################################################################
# E-Cookbook - electronic recipe organizer
# Copyright (C) 2006 Shyam "Doggan" Guthikonda.
#
# Email: shyamguth@gmail.com
# URL: http://shy.am
#
# Project Page: http://sourceforge.net/projects/ecookbook
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free 
# Software Foundation; either version 2 of the License, or (at your option) 
# any later version.
#
# This software is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with this library; if not, write to the Free Software Foundation, Inc., 
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
################################################################################

import math

# Could be made more efficient by not reducing after each operation,
#     but this suits my purposes.

class Fraction:
    # For a fraction: 8/5. If this is true, printing the fraction will
    #  result in 1 3/5. If false, 8/5.
    __mUseMixedFracForm = True
    
    # Fraction(3) = 3/1
    def __init__(self, num, den = 1):
        assert(den != 0)
        self.num = num
        self.den = den
        
    def __add__(self, other):
        # Handle integers.
        if type(other) == type(1):
            other = Fraction(other)
        f = Fraction(self.num*other.den + other.num*self.den,
                        self.den*other.den)
        f.reduce()
        return f
    __radd__ = __add__
    
    def __sub__(self, other):
        # Handle integers.
        if type(other) == type(1):
            other = Fraction(other)
        f = Fraction(self.num*other.den - other.num*self.den,
                        self.den*other.den)
        f.reduce()
        return f
    __rsub__ = __sub__
    
    def __mul__(self, other):
        # Handle integers.
        if type(other) == type(1):
            other = Fraction(other)
        f = Fraction(self.num * other.num, self.den * other.den)
        f.reduce()
        return f
    __rmul__ = __mul__
    
    def __div__(self, other):
        # Handle integers.
        if type(other) == type(1):
            other = Fraction(other)
        f = self.__mul__(Fraction(other.den, other.num))
        f.reduce()
        return f
    __rdiv__ = __div__
        
    def __neg__(self):
        return Fraction(self.num * -1, self.den)
    
    def __lt__(self, other):
        return self.toDecimal() < other.toDecimal()
    
    def __le__(self, other):
        return self.toDecimal() <= other.toDecimal()
    
    def __eq__(self, other):
        return self.toDecimal() == other.toDecimal()
    
    def __ne__(self, other):
        return self.toDecimal() != other.toDecimal()
    
    def __gt__(self, other):
        return self.toDecimal() > other.toDecimal()
    
    def __ge__(self, other):
        return self.toDecimal() >= other.toDecimal()
        
    def __str__(self):
        return self.toString()
    def toString(self):
        self.reduce()
        
        # i.e. 4a / a
        if self.num % self.den == 0:
            return `self.num / self.den`
        
        # Mixed fractions.
        if self.__mUseMixedFracForm and abs(self.num) > abs(self.den):
            i = abs(self.num) // abs(self.den)
            newNum = abs(self.num) - (i * abs(self.den))
            newDen = abs(self.den)
            if self.num * self.den < 0:
                i *= -1
            return "%d %d/%d" % (i, newNum, newDen)
        
        return "%d/%d" % (self.num, self.den)
    
    def toDecimal(self):
        return float(self.num) / float(self.den)
    
    def reduce(self):
        g = gcd(self.num, self.den)
        self.num /= g
        self.den /= g
        
    def pow(self, exp):
        f = Fraction(self.num ** exp, self.den ** exp)
        f.reduce()
        return f
        
    def sqrt(self):
        return math.sqrt(self.num) / math.sqrt(self.den)
    
    if __mUseMixedFracForm:
        def getWhole(self):
            return abs(self.num) // abs(self.den)
        def getNum(self):
            i = abs(self.num) // abs(self.den)
            return abs(self.num) - (i * abs(self.den))
    else:
        def getNum(self):
            return self.num
        
    def getDen(self):
        return self.den
    
# Return the Greatest-Common-Divisor of m and n
def gcd(m, n):
    if m % n == 0: 
        return n 
    else: 
        return gcd(n, m % n)

# Convert a mixed fraction to an improper fraction (don't use w/ neg values).
def toFraction(whole, num = 0, den = 1):
    if whole == "":
        whole = 0
    if num == "":
        num = 0
    if den == "" or den == 0:
        den = 1
    whole = int(whole)
    num = int(num)
    den = int(den)
    return Fraction(whole * den + num, den)

# Parses a fraction string of the form: 3 1/3, 2/5, or a single integer.
# Returns an actual fraction object.
def toFractionParse(fractionStr):
    index1 = fractionStr.find(" ")
    index2 = fractionStr.find("/")
    
    # It's simply a whole number
    if index1 == -1 and index2 == -1:
        return Fraction(int(fractionStr))
    # It's a standard fraction
    elif index1 == -1:
        num = fractionStr[:index2]
        den = fractionStr[index2 + 1:]
        return Fraction(int(num), int(den))
    
    # If we get here, it's a mixed fraction
    whole = int(fractionStr[:index1])
    num = int(fractionStr[index1 + 1:index2])
    den = int(fractionStr[index2 + 1:])
    return toFraction(whole, num, den)

# Test cases:
#f = Fraction(3, 2)
#print f.getWhole(), f.getNum(), f.getDen()
#f += Fraction(-1, 2)
#print f
#f = Fraction(1, 2)
#g = Fraction(1, 4)
#print f < g
#print f <= Fraction(1, 2)
#print f <= g
#print f == g
#print f != g
#print f > g
#print f >= g
#print Fraction(3, 2).pow(3)
#print Fraction(3, 2).sqrt()
#print Fraction(1, 2) / Fraction(1, 4)
#print Fraction(1, 8) * 4
#print 4 * Fraction(1, 8)
#print Fraction(1, 4) - Fraction(1, 4)
#print Fraction(1, 4) + Fraction(2, 4)
#print Fraction(2, 10).toDecimal()
#print Fraction(1, -4) + Fraction(1, 4)
#print Fraction(1, 8) + -4
#print "-" * 20
#print Fraction(8, 5)
#print Fraction(-8, 5)
#print Fraction(8, -5)
#print Fraction(-8, -5)
#print toFraction(-3, 3, 2)
#print Fraction(-9, 2)