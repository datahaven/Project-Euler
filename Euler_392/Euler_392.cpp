// Project Euler 392
// Enmeshed Unit Circle
// Adrian Dale 02/09/2012

// For N=10  ==> 3.3469640797
// For N=400 ==> 3.1486734435

/*
Congratulations, the answer you gave to problem 392 is correct.

You are the 122nd person to have solved this problem.

Return to Problems page or go to thread 392 in the forum.
*/

// This is the rough version of the code - it needed to be run in two stages to
// produce the correct answer. Once with the code calling the function that moves
// the points one at a time, then re-built and re-run starting from those results but using the
// function that moves points in pairs. This caused the final answer to change by
// a single digit in the tenth decimal place. Not entirely sure why my code has had
// this problem.

// Obvious optimisation - run it over an eigth of the circle, rather than a quarter.
// (My attempt at this caused a huge bug first time round!)
#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <fstream>

using namespace std;

vector<long double> vars;

void fillVars(int N)
{
	if (N%2 == 1)
	{
		cout << "ERROR - N must be even" << endl;
		return;
	}

	int steps = N/2 + 1; // +1 to allow for outer x==1 grid line

	// Starting guesses evenly spaced
	for(int i=0; i<=steps; ++i)
	{
		vars.push_back((i/static_cast<long double>(steps)) );
	}
	
	// pre-store results from previous run
	if (N==400)
	{
		vars[0] = 0;
vars[1] = 0.039984059079150841;
vars[2] = 0.059968125168495479;
vars[3] = 0.076614092302722966;
vars[4] = 0.09144328929903045;
vars[5] = 0.10506155677401012;
vars[6] = 0.11778864576886129;
vars[7] = 0.12981955420516522;
vars[8] = 0.14128434146313254;
vars[9] = 0.15227528833379983;
vars[10] = 0.16286088335422044;
vars[11] = 0.17309372495323749;
vars[12] = 0.18301535627727328;
vars[13] = 0.19265926928878793;
vars[14] = 0.20205301551580174;
vars[15] = 0.21121958020113893;
vars[16] = 0.2201784019307475;
vars[17] = 0.22894610822358322;
vars[18] = 0.23753703982959287;
vars[19] = 0.24596370009677876;
vars[20] = 0.25423707614855973;
vars[21] = 0.26236688759480986;
vars[22] = 0.27036176097293363;
vars[23] = 0.27822943038877707;
vars[24] = 0.2859768319438078;
vars[25] = 0.29361023143650772;
vars[26] = 0.30113532821562189;
vars[27] = 0.30855729702099166;
vars[28] = 0.31588089202481823;
vars[29] = 0.32311043804320583;
vars[30] = 0.33024994641322819;
vars[31] = 0.33730308049218372;
vars[32] = 0.34427323396537851;
vars[33] = 0.35116358642200141;
vars[34] = 0.35797707122580469;
vars[35] = 0.36471639921986898;
vars[36] = 0.37138413510571955;
vars[37] = 0.37798266243211165;
vars[38] = 0.38451420407976455;
vars[39] = 0.39098085831733509;
vars[40] = 0.39738459156345718;
vars[41] = 0.40372723916364822;
vars[42] = 0.41001056985482853;
vars[43] = 0.4162362308683078;
vars[44] = 0.42240576639997646;
vars[45] = 0.42852064661919786;
vars[46] = 0.43458227069678906;
vars[47] = 0.44059196829472175;
vars[48] = 0.44655099241789881;
vars[49] = 0.45246052210358406;
vars[50] = 0.45832170379629683;
vars[51] = 0.46413562911866607;
vars[52] = 0.46990331704547733;
vars[53] = 0.47562574836088578;
vars[54] = 0.48130386428772343;
vars[55] = 0.48693856940286773;
vars[56] = 0.49253070926176334;
vars[57] = 0.49808111009680778;
vars[58] = 0.50359054664635561;
vars[59] = 0.50905975812607829;
vars[60] = 0.51448947677288037;
vars[61] = 0.51988040008747205;
vars[62] = 0.52523318461148261;
vars[63] = 0.53054847554568796;
vars[64] = 0.53582686942319335;
vars[65] = 0.5410689613379579;
vars[66] = 0.5462752963719365;
vars[67] = 0.55144643691465844;
vars[68] = 0.55658288756944418;
vars[69] = 0.56168515876754432;
vars[70] = 0.56675372282754788;
vars[71] = 0.5717890633834809;
vars[72] = 0.57679163033580749;
vars[73] = 0.58176182929109888;
vars[74] = 0.58670007807017366;
vars[75] = 0.59160679650374515;
vars[76] = 0.59648237323598641;
vars[77] = 0.60132717735814678;
vars[78] = 0.60614154510015894;
vars[79] = 0.6109258523963994;
vars[80] = 0.61568041527125084;
vars[81] = 0.62040556326179463;
vars[82] = 0.6251016196882806;
vars[83] = 0.62976887648047641;
vars[84] = 0.63440762930798633;
vars[85] = 0.63901813261518237;
vars[86] = 0.64360068578376606;
vars[87] = 0.64815555162041916;
vars[88] = 0.65268297161539257;
vars[89] = 0.65718319501680811;
vars[90] = 0.66165644300715876;
vars[91] = 0.66610294608647136;
vars[92] = 0.67052292223187637;
vars[93] = 0.67491658416139566;
vars[94] = 0.67928413313246239;
vars[95] = 0.68362576326028257;
vars[96] = 0.68794167690781538;
vars[97] = 0.6922320430358091;
vars[98] = 0.69649704540950663;
vars[99] = 0.70073684161343663;
vars[100] = 0.70495159264481777;
vars[101] = 0.70914145619933056;
vars[102] = 0.71330657849896073;
vars[103] = 0.71744709922524352;
vars[104] = 0.72156316794558328;
vars[105] = 0.72565489802501337;
vars[106] = 0.72972243010772975;
vars[107] = 0.73376587633466461;
vars[108] = 0.73778534857289435;
vars[109] = 0.74178095181322623;
vars[110] = 0.74575280035896108;
vars[111] = 0.7497009825500548;
vars[112] = 0.75362560327211292;
vars[113] = 0.75752673150747052;
vars[114] = 0.76140446829681974;
vars[115] = 0.76525887964246575;
vars[116] = 0.76909004133136394;
vars[117] = 0.77289802707471189;
vars[118] = 0.77668287792316815;
vars[119] = 0.78044466452714867;
vars[120] = 0.78418343937378998;
vars[121] = 0.78789926000921073;
vars[122] = 0.79159215037901332;
vars[123] = 0.7952621742763456;
vars[124] = 0.79890933901688543;
vars[125] = 0.80253368143198445;
vars[126] = 0.80613522540410731;
vars[127] = 0.80971398314108622;
vars[128] = 0.81326997418889679;
vars[129] = 0.81680321394966804;
vars[130] = 0.82031370129857251;
vars[131] = 0.82380142150226521;
vars[132] = 0.82726638649148498;
vars[133] = 0.83070857453672842;
vars[134] = 0.83412796978266757;
vars[135] = 0.83752455894740341;
vars[136] = 0.84089829160838903;
vars[137] = 0.84424914633659787;
vars[138] = 0.84757708005327459;
vars[139] = 0.85088205067998246;
vars[140] = 0.85416400538153192;
vars[141] = 0.85742288590221771;
vars[142] = 0.86065863828056999;
vars[143] = 0.86387118426598097;
vars[144] = 0.86706044902252322;
vars[145] = 0.87022634900477813;
vars[146] = 0.8733687934155141;
vars[147] = 0.87648768369600483;
vars[148] = 0.87958291590632953;
vars[149] = 0.88265437571884231;
vars[150] = 0.88570194347979248;
vars[151] = 0.88872549915636734;
vars[152] = 0.891724895626265;
vars[153] = 0.89469998603074707;
vars[154] = 0.89765061334683338;
vars[155] = 0.90057661258277621;
vars[156] = 0.90347780401019384;
vars[157] = 0.90635400203481564;
vars[158] = 0.90920500685479855;
vars[159] = 0.91203060971370797;
vars[160] = 0.91483058641513459;
vars[161] = 0.91760470872017919;
vars[162] = 0.92035271495844384;
vars[163] = 0.92307433701801556;
vars[164] = 0.9257692913580986;
vars[165] = 0.92843727966814205;
vars[166] = 0.93107798340954495;
vars[167] = 0.93369106253780221;
vars[168] = 0.93627615795011276;
vars[169] = 0.93883289305796624;
vars[170] = 0.94136085083183785;
vars[171] = 0.94385960547258652;
vars[172] = 0.94632868920736346;
vars[173] = 0.9487676119240307;
vars[174] = 0.95117584731061655;
vars[175] = 0.95355283048514872;
vars[176] = 0.95589796311313557;
vars[177] = 0.95821058463949571;
vars[178] = 0.96049000462249057;
vars[179] = 0.96273545710184183;
vars[180] = 0.96494613120756578;
vars[181] = 0.96712113683204193;
vars[182] = 0.96925951150229195;
vars[183] = 0.97136020289837022;
vars[184] = 0.97342205543079796;
vars[185] = 0.97544379762995381;
vars[186] = 0.97742402656518557;
vars[187] = 0.97936116835050502;
vars[188] = 0.98125347436817612;
vars[189] = 0.98309897322312945;
vars[190] = 0.9848954052622152;
vars[191] = 0.98664020211818604;
vars[192] = 0.98833036189503654;
vars[193] = 0.98996236005961002;
vars[194] = 0.99153196556205081;
vars[195] = 0.99303400414569254;
vars[196] = 0.99446198596003876;
vars[197] = 0.99580744700298185;
vars[198] = 0.99705882283822733;
vars[199] = 0.9981990661112915;
vars[200] = 0.99919977189956599;
vars[201] = 1;
	}
}
// Calculate the area of the red squares specified by vector v
// Return -1.0 if v is invalid
long double area(vector<long double> &v)
{
	// Check values in v are in ascending order
	long double prev = 0.0;
	for( auto it = v.begin(); it != v.end(); ++it)
	{
		if (*it < prev)
			return -1.0;
		prev = *it;
	}

	// TODO? - Check gaps shrink at each step?
	// Needed?

	// Calculate area
	long double area_sum = 0.0;
	for(int i=0; i<v.size()-1; ++i)
	{
		long double y = sqrt(1.0 - v[i]*v[i]);
		area_sum += y * (v[i+1] - v[i]);
	}
	
	area_sum *= 4.0L;

	return area_sum;
}

bool jiggleIt(vector<long double> &v, long double inamount, long double &bestSoFar)
{
	
	bool returnImproved = false;

	for(int jigPos=1; jigPos<v.size()-1; ++jigPos)
	{
		for(int d=0; d<2; ++d)
		{
			long double amount = inamount;
			if (d==1) amount = inamount * -1.0;
	
			// Keep searching in same direction until we fail.
			bool improved = true;
			while(improved)
			{
				v[jigPos] += amount;
				long double new_area = area(v);
				if (new_area < bestSoFar && new_area != -1.0)
				{
					bestSoFar = new_area;
					improved = true;
					returnImproved = true;
				}
				else
				{
					v[jigPos] += (amount*-1.0); // put v back how it was
					improved = false;
				}
			}
		}
	}
	return returnImproved;
}

// Try jiggling each variable by amount to see if we get an improvement
bool jiggleItII(vector<long double> &v, long double amount, long double &bestSoFar)
{
	for(int jp=1; jp<v.size()-1; ++jp)
	{
		v[jp] += amount;
		long double new_area = area(v);
		if (new_area < bestSoFar && new_area != -1.0)
		{
			bestSoFar = new_area;
			return true;
		}
		else
		{
			v[jp] += amount * -1.0;
		}

		v[jp] -= amount;
		new_area = area(v);
		if (new_area < bestSoFar && new_area != -1.0)
		{
			bestSoFar = new_area;
			return true;
		}
		else
		{
			v[jp] -= amount * -1.0;
		}
	}

	return false;
}

// Try jiggling pairs of variables by amount to see if we get an improvement
bool jiggleItPairs(vector<long double> &v, long double amount, long double &bestSoFar)
{
	for(int pg=1; pg<v.size()-2; ++pg)
	{
		for(int jp=1; jp<v.size()-1-pg; ++jp)
		{
			v[jp] += amount;
			v[jp+pg] += amount;
			long double new_area = area(v);
			if (new_area < bestSoFar && new_area != -1.0)
			{
				bestSoFar = new_area;
				return true;
			}
			else
			{
				v[jp] += amount * -1.0;
				v[jp+pg] += amount * -1.0;
			}

			v[jp] -= amount;
			v[jp+pg] -= amount;
			new_area = area(v);
			if (new_area < bestSoFar && new_area != -1.0)
			{
				bestSoFar = new_area;
				return true;
			}
			else
			{
				v[jp] -= amount * -1.0;
				v[jp+pg] -= amount * -1.0;
			}
		}
	}
	return false;
}

void writeTestVars(vector<long double> &v)
{
	cout << "###################################### Writing Data" << endl;
	ofstream myfile;
	myfile.open ("output.txt");
	for(int i=0; i<v.size(); ++i)
	{
		myfile << "vars[" << i << "] = " << setprecision(25) << v[i] << ";" << endl;
	}
	myfile.close();
}

void test01()
{
	long double amount = 0.001;// 0.001 worked for initial run

	amount = 0.01;

	long double bestSoFar = 4.0;
	long long noBestCount = 0;
	while(amount > 0.0)
	{
		if (jiggleIt(vars, amount, bestSoFar))
		{
			cout << "New best area = " << setprecision(25) << bestSoFar << endl;
			noBestCount = 0;
		}
		else
		{
			++noBestCount;
			if (noBestCount > 1000000)
			{
				noBestCount = 0;
				amount /= 1.05;
				cout << "(bsf=" << setprecision(25) << bestSoFar << ") " << "New amount=" << amount << endl;
				
				writeTestVars(vars);
			}
		}
	}
}

void test02()
{
	long double amount = 0.00001;// 0.001 worked for initial run

	amount = 0.001;

	long double bestSoFar = 4.0;
	long long noBestCount = 0;
	while(amount > 0.0)
	{
		if (jiggleItPairs(vars, amount, bestSoFar))
		{
			cout << "New best area = " << setprecision(25) << bestSoFar << endl;
			noBestCount = 0;
		}
		else
		{
			
				amount /= 1.05;
				cout << "(bsf=" << setprecision(25) << bestSoFar << ") " << "New amount=" << amount << endl;
				
				if (amount < 1.0e-50)
				{
					writeTestVars(vars);
					return;
				}
			
		}
	}
}

int main()
{
	cout << "Project Euler 392" << endl;
	
	fillVars(400); // This works for 10

	test02();

	cout << "area=" << area(vars) << endl;

	return 0;
}