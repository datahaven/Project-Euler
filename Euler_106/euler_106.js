// Project Euler 106
// Adrian Dale
// 25/02/2017

// Congratulations, the answer you gave to problem 106 is correct.
// You are the 4547th person to have solved this problem.

// Run with node.js

function BaseThreeCounter(numDigits)
{
	this.digits = new Array(numDigits).fill(0);
	
	this.getDigitsString = function()
	{
		var s = "";
		for (var i=0; i<this.digits.length; i++)
		{
			s += this.digits[i];
		}
		return s;
	}
	
	this.showCount = function ()
	{
		console.log(this.getDigitsString());
	}
	
	this.increment = function()
	{
		for (var i=0; i<this.digits.length; i++)
		{
			this.digits[i]++;
			if (this.digits[i] == 3)
			{
				if (i == this.digits.length - 1)
				{
					return false;
				}
				this.digits[i] = 0;
			}
			else
			{
				break;
			}
		}
		return true;
	}
}

function euler_106(n)
{
	var results = {};
	
	var b3c = new BaseThreeCounter(n);
	var setCount = 0;
	do
	{
		var ones = 0;
		var twos = 0;
		var firstNonZero = -1;
		for(var i=0; i<b3c.digits.length; i++)
		{
			if (firstNonZero == -1 && b3c.digits[i] != 0)
			{
				firstNonZero = b3c.digits[i];
			}
			
			if (b3c.digits[i] == 1){
				ones++;
			} else if (b3c.digits[i] == 2){
				twos++;
			}
		}
		
		// If sets contain different number of elements, then
		// rule ii means we don't need to test them.
		// Also, if sets only contain one of each number, the fact they
		// are strictly increasing means they don't sum to the same number
		// Sets have to be non-empty
		// Also only check the sets where the first non-zero digit is a 1, so we weed out
		// all the equivalent mirrored AB/BA sets
		if (firstNonZero == 1 && ones != 0 && ones != n && twos != 0 && twos != n // OK so far
		    && ones == twos && ones != 1 && twos != 1)
		{
			// We need to test all the set pairs where above conditions are met.
			// However, we can rule out some more by the strictly increasing rule.
			// This builds an example version of set A and set B with strictly increasing
			// values, then checks to see if that rule gets broken for this possible
			// disjoint pair
			var ds = b3c.getDigitsString();
			var a = [];
			var b = [];
			for(var i=0; i<ds.length; i++)
			{
				if (ds[i] == '1')
				{
					a.push(i);
				}
				else if (ds[i] == '2')
				{
					b.push(i);
				}
			}
			
			for(var i=0; i<a.length; i++)
			{
				if (a[i] > b[i])
				{
					setCount++;
					break;
				}
			}
		}
	} while(b3c.increment());
	
	console.log("Answer="+setCount );
}

euler_106(12);

