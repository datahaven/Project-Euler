# Project Euler 82
# Adrian Dale
# 13/04/2018
#
# Rough python code mostly just to format the input to feed into
# some code for Dijkstra's algorithm that I found on the internet.
# (And inevitably had to repair first)
from dijkstra import Graph
from dijkstra import dijkstra

filename = 'p082_matrix.txt'

def NodeName(r, c):
	return '{:03d}_{:03d}'.format(r, c)
	
g = Graph()
g.add_node('START_NODE')
with open(filename) as f:
	content = f.readlines()
	content = [x.strip() for x in content]
	matrix = []
	rowNum = 0
	for row in content:
		matrix.append( [int(x) for x in row.split(',')] )
	
	numRows = len(matrix)
	numCols = len(matrix[0])
	
	for rowNum in range(0, numRows):
		for colNum in range(0, numCols):
			nodeName = NodeName(rowNum, colNum)
			g.add_node(nodeName)
	
	for rowNum in range(0, numRows):
		for colNum in range(0, numCols):
			nodeName = NodeName(rowNum, colNum)
			
			if colNum == 0:
				g.add_edge('START_NODE', nodeName, matrix[rowNum][colNum])
			if colNum == numCols - 1:
				g.add_edge(nodeName, 'END_NODE', 0)
				
			if rowNum > 0:
				# Add connection to node above
				nr = rowNum - 1
				g.add_edge(nodeName, NodeName(nr, colNum), matrix[nr][colNum])
			if rowNum < numRows - 1:
				nr = rowNum + 1
				g.add_edge(nodeName, NodeName(nr, colNum), matrix[nr][colNum])
			if colNum < numCols - 1:
				# Add connection to node to the left
				nc = colNum + 1
				g.add_edge(nodeName, NodeName(rowNum, nc), matrix[rowNum][nc])

g.add_node('END_NODE')

v, p = dijkstra(g, 'START_NODE')
print(v['END_NODE'])