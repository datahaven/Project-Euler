// Project Euler 117
// Red, green and blue tiles
// https://projecteuler.net/problem=117
// Adrian Dale
// 22/02/2017
//

// Congratulations, the answer you gave to problem 117 is correct.
//
// You are the 8089th person to have solved this problem.

var t0 = new Date().getTime();

var Cache = {};

function CountFittingsForTileSize(length, tileSize)
{
	var count = 0;
	for(var tilePos=0; tilePos < length - tileSize + 1; tilePos++)
	{
		count += 1 + CountFittings(length-(tilePos+tileSize));
	}
	return count;
}

function CountFittings(length)
{
	if (length in Cache)
	{
		return Cache[length];
	}
	
	var count = CountFittingsForTileSize(length, 2);
	count += CountFittingsForTileSize(length, 3);
	count += CountFittingsForTileSize(length, 4);
	
	Cache[length] = count;
	return count;
}
	if (length in Cache)
	{
		return Cache[length];
	}
	
	var count = 0;
	
	var tileSize = 2;
	for(var tilePos=0; tilePos < length - tileSize + 1; tilePos++)
	{
		count += 1 + CountFittings(length-(tilePos+tileSize));
	}
	
	tileSize = 3;
	
	for(var tilePos=0; tilePos < length - tileSize + 1; tilePos++)
	{
		count += 1 + CountFittings(length-(tilePos+tileSize));
	}
	tileSize = 4;
	
	for(var tilePos=0; tilePos < length - tileSize + 1; tilePos++)
	{
		count += 1 + CountFittings(length-(tilePos+tileSize));
	}
	
	Cache[length] = count;
	return count;
}

// Plus one extra case where we don't place any coloured tiles
console.log("result=" + (CountFittings(50) + 1));

var t1 = new Date().getTime();
console.log("Execution took " + ((t1 - t0)/1000) + " seconds.");

