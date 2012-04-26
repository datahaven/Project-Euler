// Project Euler 151
// Adrian Dale
//
// Had a bit of help on this - first attempt was WAY off, although it
// looked flash. Used matrices to calculate probability of getting each
// sheet. It worked, but unfortunately this is not what was required.
// Sounds like it was possible to use my idea, though.
//
// You are the 1479th person to have solved this problem.
// Answer: 0.4643988
//
#include <iostream>
#include <iomanip>
using namespace std;

typedef unsigned int EnvelopeState;

// Return how many of sheet are in env.
// Sheets are 1,2,4,8 for A5,A4,A3,A2 respectively.
// Numbers double as sheet size doubles - not really necessary and a
// hangover from previous version of this code.
int numSheets( EnvelopeState env, int sheet )
{
	int sc = 0;

	switch( sheet )
	{
	case 1:
		sc = env & 0xf;
		break;
	case 2:
		sc = (env & 0xf0) >> 4;
		break;
	case 4:
		sc = (env & 0xf00) >> 8;
		break;
	case 8:
		sc = (env & 0xf000) >> 12;
		break;
	default:
		cout << "ERROR" << endl;
		exit(0);
	};
	return sc;
}

EnvelopeState setSheets( EnvelopeState env, int sheet, int sheetcount )
{
	EnvelopeState ns = env;

	switch( sheet )
	{
	case 1:
		ns &= 0xfff0;
		ns |= sheetcount;
		break;
	case 2:
		ns &= 0xff0f;
		ns |= sheetcount << 4;
		break;
	case 4:
		ns &= 0xf0ff;
		ns |= sheetcount << 8;
		break;
	case 8:
		ns &= 0x0fff;
		ns |= sheetcount << 12;
		break;
	default:
		cout << "ERROR" << endl;
		exit(0);
	};
	return ns;
}

EnvelopeState removeSheet( EnvelopeState env, int sheet )
{
	int sc = numSheets(env, sheet);
	if ( sc == 0 )
	{
		cout << "ERROR - removeSheet" << endl;
		exit(0);
	}
	--sc;
	return setSheets( env, sheet, sc );
}

EnvelopeState addSheet( EnvelopeState env, int sheet )
{
	int sc = numSheets(env, sheet);
	++sc;
	return setSheets( env, sheet, sc );
}

double memo[0x10000];

double buildGraphII(EnvelopeState envelope)
{
	if (memo[envelope] != -1.0)
		return memo[envelope];

	double result = 0.0;

	// If there is a single sheet left then add it to our
	// expected number of sheets
	int totalsheetcount = 0;
	for( int sheet = 1; sheet<=8; sheet *= 2 )
		totalsheetcount += numSheets(envelope, sheet);
	// NB Don't include the final single sheet as this can
	// only appear after the 15th day
	if (totalsheetcount == 1 && numSheets(envelope, 1) != 1)
		result += 1.0;

	// For each possible size sheet in the envelope
	for( int sheet = 1; sheet<=8; sheet *= 2 )
	{
		EnvelopeState nextState = envelope;

		int sheetcount = numSheets(envelope, sheet);
		if ( sheetcount == 0 )
			continue;

		double probability = (double)sheetcount / (double)totalsheetcount;

		nextState = removeSheet(nextState, sheet);

		// Cut the selected sheet as per rules
		// and put pieces back in envelope
		if (sheet == 2)
		{
			nextState = addSheet(nextState, 1);
		}
		if (sheet == 4)
		{
			nextState = addSheet(nextState, 1);
			nextState = addSheet(nextState, 2);
		}
		if (sheet == 8)
		{
			nextState = addSheet(nextState, 1);
			nextState = addSheet(nextState, 2);
			nextState = addSheet(nextState, 4);
		}

		if ( nextState != 0 )
			result += probability * buildGraphII(nextState);
	}

	memo[envelope] = result;
	return result;
}

int main()
{
	//startContents is 1,2,4,8
	EnvelopeState startContents = 0x1111;

	// Initialise memo
	for(int i=0; i<0x10000; ++i)
		memo[i] = -1.0;

	cout << fixed << setprecision(7) << buildGraphII(startContents) << endl;
	
	return 0;
}