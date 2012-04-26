NB. l=: 'C:\Users\Adrian\Documents\Projects\Puzzles\ProjectEuler\project_022.ijs'
NB. load l

NB. Specify the file to load
namesFile =: 'C:\Users\Adrian\Documents\Projects\Puzzles\ProjectEuler\names.txt'
NB. Read it in to data variable
data =: 1!:1 < namesFile
NB. remove all the " characters
noQuotesData =: (-.(data e. '"')) # data
NB. Cut up by commas and box the results.
NB. Sort them, while we're at it
boxedNames =: /:~ < ;. _2 noQuotesData,','

NB. Fn to calculate Name Value
NB. ie COLIN, which is worth 3 + 15 + 12 + 9 + 14 = 53
nameValue =: 3 : 0
+/ letterValue y
)

NB. Fn to calculate value of a single letter
NB. Position of letter in ASCII codes - asc('A') + 1
letterValue =: 3 : '64 -~ a. i. y'

NB. Score for the name is nameValue multiplied by
NB. the position of the name in the list
nameScore =: 3 : 0
nameIndex =. >: I. (=&(<y)) boxedNames
nameIndex * nameValue y
)

NB. Calculate nameScore for each name and sum up the score
result =: +/ > nameScore &.> boxedNames

NB. 871198282