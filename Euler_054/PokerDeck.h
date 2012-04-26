#ifndef _POKERDECK_H
#define _POKERDECK_H

class CPokerDeck
{
   public:
      CPokerDeck() {}
      ~CPokerDeck() {}

      int RankHand( int iC1, int iC2, int iC3, int iC4, int iC5,
                    int *piMainRank, int *piSubRank );
      
      int FindBestHand( int aiCards[], int iNoOfCards,
                        int *piMainRank, int *piSubRank );
      int DescribeHand( int iMainRank, int iSubRank, char *pBuf );

      int RankHole( int iC1, int iC2, int iPlayers,
                    int *piRank, int *piPosition );

      int HandCount( int iC1, int iC2, int iC3, int iC4, int iC5,
                     int iC6, int *paCounts );
   private:

      int CardIndex( int iC ) { return iC % 13; }
      int CardSuit( int iC ) { return iC / 13; }
      char *DescribeIndex(int iIndex);
      int IsStraight( int iC1, int iC2, int iC3, int iC4, int iC5 );
};

#endif /* _POKERDECK_H */