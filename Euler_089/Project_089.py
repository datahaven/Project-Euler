# Project Euler 089
# Adrian Dale
#
# Congratulations, the answer you gave to problem 89 is correct.
#
# You are the 5283rd person to have solved this problem.

# I borrowed the code for int2roman from
# http://www.daniweb.com/software-development/python/code/216865
#
# The rest I did myself - simply had to read the strings backwards
# to work out if I needed to subtract
import string

def romanCharToDec(c):
   if c == "M":
      return 1000
   if c == "D":   
      return 500
   if c == "C":
      return 100
   if c == "L":   
      return 50
   if c == "X":   
      return 10
   if c == "V":
      return 5
   if c == "I":   
      return 1

def romanToDec(line):
   decVal = 0
   digits = []
   for c in line:
      digits.append( romanCharToDec(c) )
   digits.reverse()
   
   prevd = 0
   for d in digits:
      if d < prevd:
         decVal = decVal - d
      else:
         decVal = decVal + d
      prevd = d

   return decVal

def int2roman(number):
  numerals = { 1 : "I", 4 : "IV", 5 : "V", 9 : "IX", 10 : "X", 40 : "XL", 50 : "L", 90 : "XC", 100 : "C", 400 : "CD", 500 : "D", 900 : "CM", 1000 : "M" }
  result = ""
  for value, numeral in sorted(numerals.items(), reverse=True):
    while number >= value:
      result += numeral
      number -= value
  return result

def test01():
   charCount = 0
   newCharCount = 0
   maxVal = 0
   f = open("roman.txt")
   for line in f:
      line = line.replace("\n","")
      print(line)
      charCount = charCount + len(line)
      decVal = romanToDec(line)
      if decVal > maxVal:
         maxVal = decVal
      
      newLine = int2roman(decVal)
      print(newLine,)
      newCharCount = newCharCount + len(newLine)

   print("maxVal =", maxVal)
   print (charCount, "chars in file")
   print( newCharCount, "chars in new file")
   print( "difference=", charCount-newCharCount)

test01()
