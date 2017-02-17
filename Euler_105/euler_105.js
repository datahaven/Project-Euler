// Project Euler 105
// Adrian Dale
// 17/02/2017

// Congratulations, the answer you gave to problem 105 is correct.
// You are the 5660th person to have solved this problem.

var fs = require('fs');
var readline = require('readline');

function BaseThreeCounter(numDigits)
{
	this.digits = new Array(numDigits).fill(0);
	
	this.showCount = function ()
	{
		var digitStr = "";
		for (var i=0; i<this.digits.length; i++)
		{
			digitStr += this.digits[i];
		}
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

function isSpecialSum(theSet)
{
	var baseThreeCounter = new BaseThreeCounter(theSet.length);
	do
	{
		var setA = [];
		var setB = [];
		for(var i=0; i<baseThreeCounter.digits.length; i++)
		{
			if (baseThreeCounter.digits[i] == 1)
			{
				setA.push(theSet[i]);
			}
			else if (baseThreeCounter.digits[i] == 2)
			{
				setB.push(theSet[i]);
			}
			if (setA.length == 0 || setB.length == 0)
			{
				continue;
			}
			var sumA = setA.reduce((a, b)=>a+b, 0);
			var sumB = setB.reduce((a, b)=>a+b, 0);
			if (sumA == sumB)
			{
				return false;
			}
			if (setB.length > setA.length && sumB <= sumA)
			{
				return false;
			}
		}
	} while(baseThreeCounter.increment());
	
	return true;
}

function euler_105()
{
	var sum = 0;
	
	const rl = readline.createInterface({
		input: fs.createReadStream(__dirname + '/p105_sets.txt')
	})
	
	rl.on('line', function(line) {
		// Convert line to array of ints
		var theSetChars = line.split(",");
		var theSet = [];
		for( var i=0; i<theSetChars.length; i++)
		{
			theSet.push(parseInt(theSetChars[i]));
		}
		
		if (isSpecialSum(theSet))
		{
			var setSum = theSet.reduce((a, b)=>a+b, 0);
			sum += setSum;
			console.log( theSet + " sum=" + setSum );
		}
	});
	
	rl.on('close', function() {
      console.log("sum total = " + sum);
	});
}

euler_105();
//console.log( isSpecialSum([157, 150, 164, 119, 79, 159, 161, 139, 158]) );
//console.log( isSpecialSum([81,88,75,42,87,84,86,65]) );
