// Project Euler 103
// Adrian Dale
// 17/02/2017

// Run with node.js

// Congratulations, the answer you gave to problem 103 is correct.
// You are the 5629th person to have solved this problem.

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

function euler_103()
{
	var bestSetSum = 1000;
	var bestSet = [];
	
	var guessMin = 18;
	var guessMax = 50;
	for(var a=guessMin; a<guessMax; a++)
	{
		for(var b=a+1; b<guessMax; b++)
		{
			for(var c=b+1; c<guessMax; c++)
			{
				for(var d=c+1; d<guessMax; d++)
				{
					for(var e=d+1; e<guessMax; e++)
					{
						for(var f=e+1; f<guessMax; f++)
						{
							for(var g=f+1; f<guessMax; f++)
							{
								var testSet = [a,b,c,d,e,f,g];
								
								if (isSpecialSum(testSet))
								{
									var setSum = testSet.reduce((a, b)=>a+b, 0);
									if (setSum < bestSetSum)
									{
										bestSetSum = setSum;
										bestSet = testSet;
										
										console.log("Best Set: " + bestSet);
										console.log("Best Set Sum: " + bestSetSum);
										bestSet.sort();
										console.log("Best Set String: " + bestSet.join(''));
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

euler_103();

