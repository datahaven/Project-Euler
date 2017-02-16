// Project Euler 146
// Adrian Dale
// 16/02/2017
//
// Congratulations, the answer you gave to problem 146 is correct.
// You are the 3594th person to have solved this problem.
//
// NB This takes a good half hour or so to run, so it's not a great solution
var t0 = new Date().getTime();

var mr = require('miller-rabin').create();
var bn = require('bn.js');

var loopMax = 150000000;
var loopMin = 1;

var sum = 0;

// http://www.math.uconn.edu/~kconrad/blurbs/ugradnumthy/squaresandinfmanyprimes.pdf

for(var i=loopMin; i<loopMax; i = i + 1)
{
	
	if (i%1000000 == 0)
	{
		console.log("Testing i==" + i + " sum="+sum);
		var t2 = new Date().getTime();
		console.log("Execution time so far is " + ((t2 - t0)/1000) + " seconds.");
	}
	
	var testValue = new bn(i);
	testValue.isqr();
	
	if ((testValue.mod(new bn(4))).isZero() && // Must be of form n^2 + 1. This line reduced 1000000 time from 112s to 15s
		(testValue.add(new bn(3))).mod(new bn(3)) == 1 && // Must be of form n^2 + 3
		(testValue.mod(new bn(2))).isZero() && // Must be even
		//(testValue + 1)%8 == 5 &&
		//(testValue + 1)%12 == 5 && // This code is missing n==8146100
		//(testValue + 3)%12 == 7 &&
		mr.test(testValue.add(new bn(1))) &&
		mr.test(testValue.add(new bn(3))) &&
		mr.test(testValue.add(new bn(7))) &&
		mr.test(testValue.add(new bn(9))) &&
		mr.test(testValue.add(new bn(13))) &&
		mr.test(testValue.add(new bn(27))) &&
		mr.test(testValue.add(new bn(5))) == false &&
		mr.test(testValue.add(new bn(11))) == false &&
		mr.test(testValue.add(new bn(15))) == false &&
		mr.test(testValue.add(new bn(17))) == false &&
		mr.test(testValue.add(new bn(19))) == false &&
		mr.test(testValue.add(new bn(21))) == false &&
		mr.test(testValue.add(new bn(23))) == false &&
		mr.test(testValue.add(new bn(25))) == false )
	{
		console.log("n=" + i );
		sum += i;
	}
}

var t1 = new Date().getTime();

console.log("sum=" + sum);
console.log("Execution took " + ((t1 - t0)/1000) + " seconds.");
