// Project Euler 109
// Darts
// https://projecteuler.net/problem=109
// Adrian Dale
// 22/02/2017
//
// Congratulations, the answer you gave to problem 109 is correct.
//
// You are the 5778th person to have solved this problem.

// Not exactly optimal solution but runs instantly anyway.
// You really don't need to use a dictionary to solve this.
var t0 = new Date().getTime();

var sum = 0;

var Darts = [];

function populateDarts()
{
	Darts.push( {score:0, type:"M"});
    for(var d=1; d<=20; d++)
	{
		Darts.push( {score:d, type:"S"} );
		Darts.push( {score:(d*2), type:"D"} );
		Darts.push( {score:(d*3), type:"T"} );
	}
	Darts.push( {score:25, type:"S"}); // Outer bull
	Darts.push( {score:50, type:"D"}); // Inner bull
}

function countCheckouts(n)
{
	var dartCombinations = {};
	for(var d1=0; d1<Darts.length; d1++)
	{
		for(var d2=d1; d2<Darts.length; d2++)
		{
			for(var d3=0; d3<Darts.length; d3++)
			{
				if (Darts[d1].score + Darts[d2].score + Darts[d3].score == n && Darts[d3].type == "D")
				{
					// If I don't include the commas I get a very subtle bug which throws the answer
					// off by one, when you get the digits misaligned.
					var dartString = d1.toString() + "," + d2.toString() + "," + d3.toString();
					dartCombinations[dartString] = true;
				}
			}
		}
	}
	// I haven't messed with Object prototype, so I really don't
	// care about hasOwnProperty
	return Object.keys(dartCombinations).length;
}

populateDarts();

for(var i=2; i<100; i++)
{
	sum += countCheckouts(i);
}

console.log("sum=" + sum);
console.log("Execution took " + ((t1 - t0)/1000) + " seconds.");

var t1 = new Date().getTime();


