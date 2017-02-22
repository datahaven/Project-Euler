// Project Euler 116
// Red, green or blue tiles
// https://projecteuler.net/problem=116
// Adrian Dale
// 22/02/2017
//
// Congratulations, the answer you gave to problem 116 is correct.
// You are the 8809th person to have solved this problem.

var t0 = new Date().getTime();

var Cache = {};

function CountFittings(depth, length, tileSize)
{
	if (length in Cache)
	{
		return Cache[length];
	}
	
	if (length < tileSize)
	{
		Cache[length] = 0;
		return 0;
	}
	
	if (length == tileSize)
	{
		Cache[length] = 1;
		return 1;
	}
	
	var count = 0;
	for(var tilePos=0; tilePos < length - tileSize + 1; tilePos++)
	{
		var countR = 0;
		if (tilePos + tileSize < length - tileSize + 1)
		{
			countR = CountFittings(depth+1, length-(tilePos+tileSize), tileSize);
		}
		count += 1 + countR;
	}
	Cache[length] = count;
	return count;
}

function GetTotal(n)
{
	var total = 0;
	Cache = {};
	total += CountFittings(0, n, 2);
	Cache = {};
	total += CountFittings(0, n, 3);
	Cache = {};
	total += CountFittings(0, n, 4);
	return total;
}

console.log("sum=" + GetTotal(50));

var t1 = new Date().getTime();
console.log("Execution took " + ((t1 - t0)/1000) + " seconds.");

