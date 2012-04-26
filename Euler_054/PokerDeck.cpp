// PokerDeck.cpp : implementation file
// by Adrian Dale 2002
//

#include "PokerDeck.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define FALSE 0
#define TRUE 1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int compare( const void *e1, const void *e2 )
{
   int *c1 = (int *)e1;
   int *c2 = (int *)e2;

   return *c1 - *c2;
}

// This function takes five cards and rates them according to
// standard poker rules:
// piMainRank=
// 10 = Royal Flush
//  9 = Straight Flush
//  8 = Four of a Kind
//  7 = Full House
//  6 = Flush
//  5 = Straight
//  4 = Three of a Kind
//  3 = Two Pairs
//  2 = Pair
//  1 = High Card
//
// piSubRank is used to compare hands when Main Ranks are equal
// Fn returns TRUE or FALSE if an error occurs
int CPokerDeck::RankHand( int iC1, int iC2, int iC3, int iC4, int iC5,
                          int *piMainRank, int *piSubRank )
{
   int iStraightHighCard = 0;

   // Sanity check the input
   if ( piMainRank == NULL || piSubRank == NULL )
      return FALSE;

   // Reset rank to zero (NOT a valid rank)
   *piMainRank = 0;
   *piSubRank = 0;

   if ( iC1 < 0 || iC1 > 51 ||
        iC2 < 0 || iC2 > 51 ||
        iC3 < 0 || iC3 > 51 ||
        iC4 < 0 || iC4 > 51 ||
        iC5 < 0 || iC5 > 51 )    
   {
      return FALSE;
   }

   int aiHand[5];
   aiHand[0] = CardIndex(iC1);
   aiHand[1] = CardIndex(iC2); 
   aiHand[2] = CardIndex(iC3);
   aiHand[3] = CardIndex(iC4); 
   aiHand[4] = CardIndex(iC5);

   qsort( (void *)aiHand, 5, sizeof(int), compare );

   if ((iStraightHighCard = IsStraight( iC1, iC2, iC3, iC4, iC5 )) > 0)
   {
      *piMainRank = 5;
      *piSubRank = iStraightHighCard;
   }

   // See if we have a flush (straight, royal, or otherwise)
   if ( CardSuit(iC1) == CardSuit(iC2) &&
        CardSuit(iC2) == CardSuit(iC3) &&
        CardSuit(iC3) == CardSuit(iC4) &&
        CardSuit(iC4) == CardSuit(iC5) )
   {
      if ( iStraightHighCard == 12 )
      {
         *piMainRank = 10; // Royal Flush
         return TRUE;      // Unbeatable, so return now!
      }
      else if ( iStraightHighCard > 0 )
      {
         *piMainRank = 9;  // Straight Flush
                           // SubRank already populated by Straight code.
         return TRUE;      // Unbeatable after Royal Flush
      }
      else
         *piMainRank = 6;  // Flush (NB Beats straight, hence overwrite)
                           // Can't return 'cos it might be a Full House     
   }

   // Not a straight/royal flush - see if it is 4ofKind
   // which beats all else we've tested for so far.
   if ( aiHand[1] == aiHand[2] &&
        aiHand[2] == aiHand[3] )
   {
      if ( aiHand[0] == aiHand[1] )
      {
         // Four of a kind, kicker is aiHand[4]
         // Works because aiHand is sorted
         *piMainRank = 8;
         *piSubRank = aiHand[4] + aiHand[0]*13;
         return TRUE;
      }
      else if ( aiHand[3] == aiHand[4] )
      {
         // Four of a kind, kicker is aiHand[0]
         *piMainRank = 8;
         *piSubRank = aiHand[0] + aiHand[3]*13;
         return TRUE;
      }
   }   

   // See if we've got a full house
   // Again, this works because we sorted indices
   if ( (aiHand[0] == aiHand[1]) &&
        (aiHand[2] == aiHand[3]) && (aiHand[3] == aiHand[4]) )
   {
      *piMainRank = 7;
      *piSubRank = aiHand[2]*13 + aiHand[0];
      return TRUE;
   }
   else if ( (aiHand[3] == aiHand[4]) &&
        (aiHand[0] == aiHand[1]) && (aiHand[1] == aiHand[2]) )
   {
      *piMainRank = 7;
      *piSubRank = aiHand[0]*13 + aiHand[3];
      return TRUE;
   }

   if (*piMainRank == 6)
   {
      //// SubRank of the flush
      *piSubRank = 28561*aiHand[4] +
                    2197*aiHand[3] +
                     169*aiHand[2] +
                      13*aiHand[1] +
                         aiHand[0];
      return TRUE;
   }

   // Don't want to record a straight as High Card
   if (*piMainRank == 5)
      return TRUE;

   // See if we've got three of a kind
   // NB Full house & 4ofKind already catered for
   if ( aiHand[0] == aiHand[1] && aiHand[1] == aiHand[2] )
   {
      *piMainRank = 4;
      *piSubRank = 169*aiHand[0] + 13*aiHand[4] + aiHand[3];
      return TRUE;
   }
   else if ( aiHand[1] == aiHand[2] && aiHand[2] == aiHand[3] )
   {
      *piMainRank = 4;
      *piSubRank = 169*aiHand[1] + 13*aiHand[4] + aiHand[0];
      return TRUE;
   }
   else if ( aiHand[2] == aiHand[3] && aiHand[3] == aiHand[4] )
   {
      *piMainRank = 4;
      *piSubRank = 169*aiHand[2] + 13*aiHand[1] + aiHand[0];
      return TRUE;
   }

   // See if we've got two pairs - again, only three possible layouts
   if ( aiHand[0] == aiHand[1] && aiHand[2] == aiHand[3] )
   { 
      *piMainRank = 3;
      *piSubRank = aiHand[2]*169 + aiHand[0]*13 + aiHand[4];
      return TRUE;
   }
   else if ( aiHand[0] == aiHand[1] && aiHand[3] == aiHand[4] )
   { 
      *piMainRank = 3;
      *piSubRank = aiHand[3]*169 + aiHand[0]*13 + aiHand[2];
      return TRUE;
   }
   else if ( aiHand[1] == aiHand[2] && aiHand[3] == aiHand[4] )
   { 
      *piMainRank = 3;
      *piSubRank = aiHand[3]*169 + aiHand[1]*13 + aiHand[0];
      return TRUE;
   }

   // One Pair - Four possible layouts
   if ( aiHand[0] == aiHand[1] )
   {
      *piMainRank = 2;
      *piSubRank = aiHand[0]*2197 +
                   aiHand[4]*169 + aiHand[3]*13 + aiHand[2];
      return TRUE;
   }
   else if ( aiHand[1] == aiHand[2] )
   {
      *piMainRank = 2;
      *piSubRank = aiHand[1]*2197 +
                   aiHand[4]*169 + aiHand[3]*13 + aiHand[0];
      return TRUE;
   }
   else if ( aiHand[2] == aiHand[3] )
   {
      *piMainRank = 2;
      *piSubRank = aiHand[2]*2197 +
                   aiHand[4]*169 + aiHand[1]*13 + aiHand[0];
      return TRUE;
   }
   else if ( aiHand[3] == aiHand[4] )
   {
      *piMainRank = 2;
      *piSubRank = aiHand[3]*2197 +
                   aiHand[2]*169 + aiHand[1]*13 + aiHand[0];
      return TRUE;
   }

   // High Card is only remaining possibility
   *piMainRank = 1;
   *piSubRank = 28561*aiHand[4] +
                 2197*aiHand[3] +
                  169*aiHand[2] +
                   13*aiHand[1] +
                      aiHand[0];
   return FALSE;
}



// Returns 0 if run is not a straight. Returns index of high card, otherwise
// NB Input Card numbers, NOT index numbers
int CPokerDeck::IsStraight( int iC1, int iC2, int iC3, int iC4, int iC5 )
{
   int aiHand[5];

   aiHand[0] = CardIndex(iC1);
   aiHand[1] = CardIndex(iC2); 
   aiHand[2] = CardIndex(iC3);
   aiHand[3] = CardIndex(iC4); 
   aiHand[4] = CardIndex(iC5);

   qsort( (void *)aiHand, 5, sizeof(int), compare );

   // Return largest card index if gap between all
   // the cards is one 
   if ( (aiHand[4] - aiHand[3] == 1) &&
        (aiHand[3] - aiHand[2] == 1) &&
        (aiHand[2] - aiHand[1] == 1) &&
        (aiHand[1] - aiHand[0] == 1) )
   {
      return aiHand[4];
   }
     
   // NB Special case of Ace-low five-high straight
   // Convert aces to -1 (ie lower than 2s which are 0)
   for( int i=0; i<5; i++ )
      if (aiHand[i] == 12)
         aiHand[i] = -1;
   
   qsort( (void *)aiHand, 5, sizeof(int), compare );

   // Return largest card index if gap between all
   // the cards is one 
   if ( (aiHand[4] - aiHand[3] == 1) &&
        (aiHand[3] - aiHand[2] == 1) &&
        (aiHand[2] - aiHand[1] == 1) &&
        (aiHand[1] - aiHand[0] == 1) )
   {
      return aiHand[4];
   }
            
   return 0; 
}

// Make the best poker hand possible with the cards supplied
int CPokerDeck::FindBestHand( int aiCards[], int iNoOfCards, int *piMainRank, int *piSubRank )
{
   // Sanity check the input
   if ( piMainRank == NULL || piSubRank == NULL )
      return FALSE;

   // Reset rank to zero (NOT a valid rank)
   *piMainRank = 0;
   *piSubRank = 0;

   if ( iNoOfCards < 5 || iNoOfCards > 7 )
      return FALSE;

   // TODO - Should check user hasn't entered dumb cards

   RankHand( aiCards[0], aiCards[1], aiCards[2], aiCards[3], aiCards[4],
                       piMainRank, piSubRank );

   if ( iNoOfCards == 5 )
      return TRUE; 

   int iBestRank = 0;
   int iBestSubRank = 0;
   int i;
   int iTempCard;

   // See if a combination where we add in the sixth card is better
   for( i=0; i<5; i++ )
   {
      // Re-rank the hand but swapping in the extra card each time
      iTempCard = aiCards[i];
      aiCards[i] = aiCards[5];
      RankHand( aiCards[0], aiCards[1], aiCards[2], aiCards[3], aiCards[4],
                piMainRank, piSubRank );
      aiCards[i] = iTempCard;

      // See how this combination did
      if ( (*piMainRank > iBestRank) ||
           (*piMainRank >= iBestRank && *piSubRank >= iBestSubRank ))
      {
         iBestRank = *piMainRank; iBestSubRank = *piSubRank;
      }
   }

   *piMainRank = iBestRank; *piSubRank = iBestSubRank;

   // If we only want the result for six cards, then exit with best so far
   if ( iNoOfCards == 6 )
      return TRUE;
   

   // This one's a little trickier!
   int iTempCard2;
   int j;

   // At this point, no. of cards = 7
   
   // Repeat algorithm for best hand from six, only using the seventh card
   for( i=0; i<5; i++ )
   {
      // Re-rank the hand but swapping in the extra card each time
      iTempCard = aiCards[i];
      aiCards[i] = aiCards[6];
      RankHand( aiCards[0], aiCards[1], aiCards[2], aiCards[3], aiCards[4],
                piMainRank, piSubRank );
      aiCards[i] = iTempCard;

      // See how this combination did
      if ( (*piMainRank > iBestRank) ||
           (*piMainRank >= iBestRank && *piSubRank >= iBestSubRank ))
      {
         iBestRank = *piMainRank; iBestSubRank = *piSubRank;
      }
   } 

   // Now see if we can beat that hand using both extra cards      
   for( i=0; i<5; i++ )
   {
      // Re-rank the hand but swapping in the extra card each time
      iTempCard = aiCards[i];
      aiCards[i] = aiCards[5];

      for( j=0; j<5; j++ )
      {
         if (j==i)
            continue;

         iTempCard2 = aiCards[j];
         aiCards[j] = aiCards[6];

         RankHand( aiCards[0], aiCards[1], aiCards[2], aiCards[3], aiCards[4],
                   piMainRank, piSubRank );
         aiCards[j] = iTempCard2;

         // See how this combination did
         if ( (*piMainRank > iBestRank) ||
           (*piMainRank >= iBestRank && *piSubRank >= iBestSubRank ))
         {
            iBestRank = *piMainRank; iBestSubRank = *piSubRank;
         }
      }
      aiCards[i] = iTempCard;       
   }

   *piMainRank = iBestRank; *piSubRank = iBestSubRank;
      
   return TRUE;
}

// This fn ranks the hole cards to help the player decide how
// to play them.
// Stats come from an internet poker
int CPokerDeck::RankHole( int iC1, int iC2, int iPlayers, int *piRank, int *piPosition )
{
   if ( iPlayers != 10 )
   {
      // Ooops - only a full table is supported at the moment
      return FALSE;
   }

   // TODO - Rate the cards!
   // Doing this the tedious method for now - in future, maybe some kind
   // of data table driven method may be better?
   int iCI1 = CardIndex(iC1);
   int iCI2 = CardIndex(iC2);
   int iTemp;

   int iCS1 = CardSuit(iC1);
   int iCS2 = CardSuit(iC2);

   // Order the cards - bound to be simpler!
   if ( iCI1 < iCI2 )
   {
      iTemp = iCI1;
      iCI1 = iCI2;
      iCI2 = iTemp;
      iTemp = iCS1; // Remember to swap the suit, too!
      iCS1 = iCS2;
      iCS2 = iTemp;
   }

   if ( (iCI1 == iCI2 && iCI1 > 8 ) ||       // ie Pair of J's, Q's, K's, A's OR
        (iCS1 == iCS2 && iCI1 == 12 && iCI2 == 11) ) //AKs
   {
      *piRank = 1;
   }
   else if ( (iCI1 == iCI2 && iCI1 == 8) || // ie Pair of T's OR
             (iCS1 == iCS2 && iCI1 == 12 && iCI2 == 10) || // AQs OR
             (iCS1 == iCS2 && iCI1 == 12 && iCI2 == 9) || // AJs OR
             (iCS1 == iCS2 && iCI1 == 11 && iCI2 == 10) || // KQs OR
             (iCI1 == 12 && iCI2 == 11) )                  // AKo
   {
      *piRank = 2;
   }
   else if ( (iCI1 == 7 && iCI2 == 7) || // Pair of 9's OR
             (iCS1 == iCS2 && iCI1 == 12 && iCI2 == 8) || // ATs
             (iCS1 == iCS2 && iCI1 == 11 && iCI2 == 9) || // KJs
             (iCS1 == iCS2 && iCI1 == 10 && iCI2 == 9) || // QJs
             (iCS1 == iCS2 && iCI1 == 9 && iCI2 == 8) || // JTs
             (iCI1 == 12 && iCI2 == 10) )                //AQo
   {
      *piRank = 3;
   }
   else if ( (iCI1 == 6 && iCI2 == 6) || // Pair of 8's OR
             (iCS1 == iCS2 && iCI1 == 11 && iCI2 == 8) || // KTs
             (iCS1 == iCS2 && iCI1 == 10 && iCI2 == 8) || // QTs
             (iCS1 == iCS2 && iCI1 == 9 && iCI2 == 7) || // J9s
             (iCS1 == iCS2 && iCI1 == 7 && iCI2 == 6) || // 98s
             (iCI1 == 12 && iCI2 == 9) ||                 // AJo
             (iCI1 == 11 && iCI2 == 10))                //KQo
   {
      *piRank = 4;
   }
   else if ( (iCI1 == 5 && iCI2 == 5) || // Pair of 7's OR
             (iCI1 == 12 && iCS1 == iCS2) || // Axs
             (iCS1 == iCS2 && iCI1 == 10 && iCI2 == 7) || // Q9s
             (iCS1 == iCS2 && iCI1 == 8 && iCI2 == 6) || // T8s
             (iCS1 == iCS2 && iCI1 == 7 && iCI2 == 5) || // 97s
             (iCS1 == iCS2 && iCI1 == 6 && iCI2 == 5) || // 87s
             (iCS1 == iCS2 && iCI1 == 5 && iCI2 == 4) || // 76s
             (iCS1 == iCS2 && iCI1 == 4 && iCI2 == 3) || // 65s
             (iCI1 == 11 && iCI2 == 9) ||                 // KJo
             (iCI1 == 10 && iCI2 == 9) ||                 // QJo
             (iCI1 == 9 && iCI2 == 8))                //JTo
   {
      *piRank = 5;
   }
   else if ( (iCI1 == 4 && iCI2 == 4) || // Pair of 6's OR
             (iCI1 == 3 && iCI2 == 3) || // Pair of 5's OR
             (iCS1 == iCS2 && iCI1 == 11 && iCI2 == 7) || // K9s
             (iCS1 == iCS2 && iCI1 == 9 && iCI2 == 6) || // J8s
             (iCS1 == iCS2 && iCI1 == 6 && iCI2 == 4) || // 86s
             (iCS1 == iCS2 && iCI1 == 5 && iCI2 == 3) || // 75s
             (iCS1 == iCS2 && iCI1 == 3 && iCI2 == 2) || // 54s
             (iCI1 == 12 && iCI2 == 8) ||                 // ATo
             (iCI1 == 11 && iCI2 == 8) ||                 // KTo
             (iCI1 == 10 && iCI2 == 8))                //QTo
   {
      *piRank = 6;
   }
   else if ( (iCI1 == 2 && iCI2 == 2) || // Pair of 4's OR
             (iCI1 == 1 && iCI2 == 1) || // Pair of 3's OR
             (iCI1 == 0 && iCI2 == 0) || // Pair of 2's OR
             (iCI1 == 11 && iCS1 == iCS2) || // Kxs
             (iCS1 == iCS2 && iCI1 == 10 && iCI2 == 6) || // Q8s
             (iCS1 == iCS2 && iCI1 == 8 && iCI2 == 5) || // T7s
             (iCS1 == iCS2 && iCI1 == 4 && iCI2 == 2) || // 64s
             (iCS1 == iCS2 && iCI1 == 2 && iCI2 == 1) || // 43s
             (iCI1 == 9 && iCI2 == 7) ||                 // J9o
             (iCI1 == 8 && iCI2 == 7) ||                 // T9o
             (iCI1 == 7 && iCI2 == 6))                //98o
   {
      *piRank = 7;
   }
   else if ( (iCS1 == iCS2 && iCI1 == 9 && iCI2 == 5) || // J7s
             (iCS1 == iCS2 && iCI1 == 7 && iCI2 == 4) || // 96s
             (iCS1 == iCS2 && iCI1 == 6 && iCI2 == 3) || // 85s
             (iCS1 == iCS2 && iCI1 == 3 && iCI2 == 1) || // 53s
             (iCS1 == iCS2 && iCI1 == 2 && iCI2 == 0) || // 42s
             (iCS1 == iCS2 && iCI1 == 1 && iCI2 == 0) || // 32s
             (iCS1 == iCS2 && iCI1 == 5 && iCI2 == 2) || // 74s
             (iCI1 == 12 && iCI2 == 7) ||                // A9o
             (iCI1 == 11 && iCI2 == 7) ||                // K9o
             (iCI1 == 10 && iCI2 == 7) ||                // Q9o
             (iCI1 == 9 && iCI2 == 6) ||                // J8o
             (iCI1 == 8 && iCI2 == 6) ||                // T8o
             (iCI1 == 6 && iCI2 == 5) ||                // 87o
             (iCI1 == 5 && iCI2 == 4) ||                // 76o
             (iCI1 == 4 && iCI2 == 3) ||                // 65o
             (iCI1 == 3 && iCI2 == 2)                   // 54o
             )               
   {
      *piRank = 8;
   }
   else
      *piRank = 0; // Don't play it under any circumstances!

   // Which positions allow the cards to be played depends only
   // on the rating of the cards
   // Positions:
   // 0 = Never
   // 1 = Anywhere
   // 2 = Anywhere, except under the gun
   // 3 = As 2, and not early
   // 4 = Late, or on the button
   // 5 = On the button only
   switch (*piRank)
   {
   case 1:
   case 2:
   case 3:
   case 4:
      *piPosition = 1;
      break;
   case 5:
      *piPosition = 2;
      break;
   case 6:
      *piPosition = 3;
      break;
   case 7:
      *piPosition = 4;
      break;
   case 8:
      *piPosition = 5;
      break;
   default:
      *piPosition = 0;
      break;
   }

   return TRUE;
}



// Describes the hand input
int CPokerDeck::DescribeHand( int iMainRank, int iSubRank, char *pBuf )
{
   char acDesc[16];

   switch( iMainRank )
   {
   case 10:
      strcpy(pBuf, "Royal Flush");
      break;
   case 9:
      sprintf(pBuf, "Straight Flush, %s High", DescribeIndex(iSubRank) );
      break;
   case 8:
      sprintf(pBuf, "Four of a Kind, %ss", DescribeIndex(iSubRank / 13) );
      break;
   case 7:
      strcpy(acDesc, DescribeIndex(iSubRank / 13));
      sprintf(pBuf, "Full House, %ss and %ss",
              acDesc, DescribeIndex(iSubRank % 13) );
      break;
   case 6:
      sprintf(pBuf, "Flush, %s High", DescribeIndex(iSubRank / 28561));
      break;
   case 5:
      sprintf(pBuf, "Straight, %s High", DescribeIndex(iSubRank) );
      break;
   case 4:
      sprintf(pBuf, "Three of a Kind, %ss", DescribeIndex(iSubRank / 169) );
      break;      
   case 3:
      strcpy(acDesc, DescribeIndex(iSubRank / 169)); //TODO - This is because DescribeIndex is so badly coded
      sprintf(pBuf, "Two Pairs, %ss and %ss",
              acDesc, DescribeIndex((iSubRank%169) / 13) );
      break;      
   case 2:
      sprintf(pBuf, "Pair of %ss", DescribeIndex(iSubRank / 2197) );
      break;      
   case 1:
      sprintf(pBuf, "High Card, %s", DescribeIndex(iSubRank / 28561) );
      break;                  
   default:
      strcpy(pBuf, "ERR - Description not yet implemented"); 
      return FALSE;
   }

   return TRUE;
}

char *CPokerDeck::DescribeIndex( int iIndex )
{
   static char acDesc[10];

   switch( iIndex )
   {
      case 0:
         strcpy(acDesc, "Two");
         break;
      case 1:
         strcpy(acDesc, "Three");
         break;
      case 2:
         strcpy(acDesc, "Four");
         break;
      case 3:
         strcpy(acDesc, "Five");
         break;
      case 4:
         strcpy(acDesc, "Six");
         break;
      case 5:
         strcpy(acDesc, "Seven");
         break;
      case 6:
         strcpy(acDesc, "Eight");
         break;
      case 7:
         strcpy(acDesc, "Nine");
         break;
      case 8:
         strcpy(acDesc, "Ten");
         break;
      case 9:
         strcpy(acDesc, "Jack");
         break;
      case 10:
         strcpy(acDesc, "Queen");
         break;
      case 11:
         strcpy(acDesc, "King");
         break;
      case 12:
         strcpy(acDesc, "Ace");
         break;
      default:
         sprintf(acDesc,"%c", iIndex+'2');
         break;
   }

   return &acDesc[0];
}


// This function takes five cards (And an optional sixth card - set to -1 if not using)
// and cycles through all the remaining cards in the deck to produce an array of ints
// which counts the number of times each possible hand could occur
// Hence paCounts should be:
// int aCounts[11];
// to hold the results, indexed by Rank (10=Royal Flush, etc)
//
// If cards passed in are HH+FFF then this gives the odds of improving the hand
// If cards passed in are BBBBB then you get an idea of how many hands out there
// can beat yours
int CPokerDeck::HandCount( int iC1, int iC2, int iC3, int iC4, int iC5,
                           int iC6, int *paCounts )
{
   
   // TODO - Sanity check the inputs

   // Empty the paCounts array
   int i;
   for( i=0; i<11; i++)
      paCounts[i] = 0;

   int iCx, iCy;
   bool SixthLoopEnd = false;

   // Loop through every possible sixth card
   for( iCx=0; iCx<52 && SixthLoopEnd == false; iCx++ )
   {
      if ( iC6 != -1 )
      {
         // User must have supplied sixth card, so set iCx to
         // that value and end the loop next run through
         iCx = iC6;
         SixthLoopEnd = true;
      }

      // Make sure this card isn't one we've already seen
      if ( iCx == iC1 || iCx == iC2 || iCx == iC3 || iCx == iC4 || iCx == iC5 )
         continue;

      // Loop through every possible seventh card for every
      // possible sixth card
      for( iCy=0; iCy<52; iCy++ )
      {
         // Make sure this card isn't one we've already seen
         if ( iCy == iC1 || iCy == iC2 || iCy == iC3 ||
              iCy == iC4 || iCy == iC5 || iCy == iCx )
            continue;

         // Ooops! See how mess things get when you don't plan
         // your interfaces to functions properly!
         int aiCards[7];
         aiCards[0] = iC1; aiCards[1] = iC2; aiCards[2] = iC3;
         aiCards[3] = iC4; aiCards[4] = iC5;
         aiCards[5] = iCx; aiCards[6] = iCy;
         int iRank, iSubRank;
         FindBestHand( aiCards, 7, &iRank, &iSubRank );

         // Store the results
         paCounts[iRank]++;
      }
   }

   return TRUE;
}
