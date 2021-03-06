/* Kristen Harrison
362, assignment 3
*/
// victory cards in this set are just: curse, estate, duchy, province, great_hall, gardens
#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>




int testScoreFor()
{
	// initialize game state
	struct gameState G = {};
	memset(&G, '\0', sizeof (struct gameState));

	int vpCards[27] = { curse, estate, duchy, province, great_hall, gardens };
	
	int success = 1;
	printf("\nTesting scoreFor():\n\n");



//-------------Test 1 (x4 players) ----------------------------------------------------------------------------------------------------------------------------------	
// none of the 4 players have any cards (therefore no victory points either)		[all should return 0 ] 
// [predominantly to make sure all player scores are zero]
//
	// set all decks, discards, hands to -1 (so no card is indicated)
	for (int i = 0; i < MAX_PLAYERS; i++){
		for (int j = 0; j < MAX_DECK; j++){
			G.deck[i][j] = -1;
			G.hand[i][j] = -1;
			G.discard[i][j] = -1;
			// set as max even though players have no cards so that scoreFor iterates through arrays
			G.deckCount[i] = MAX_DECK;	
			G.handCount[i] = MAX_HAND;
			G.discardCount[i] = MAX_DECK;
		}
	}

	for (int i = 0; i < MAX_PLAYERS; i++){
		for (int j = 0; j < MAX_DECK; j++){
			// test each player, each has expected score of 0
			asserttrue(scoreFor(i, &G), 0, "Every player has score of zero", &success); 
		}
	}



/*-------------Test 2 (x4 players) ----------------------------------------------------------------------------------------------------------------------------------	
player 0 only has cards / victory points in their hand; player 1 only has cards / vp in their deck, 
player 2 only has cards / vp in their discard; player 3 has cards in all places but no victory points
[verifies each array reports accurately]
*/
	for (int i = 0; i < MAX_PLAYERS; i++){
		// set all decks, discards, hands to -1 (so no card is indicated)
		for (int j = 0; j < MAX_DECK; j++){
			G.deck[i][j] = -1;
			G.hand[i][j] = -1;
			G.discard[i][j] = -1;
		}
	}

	// set up player 0 [hand only]: expected score = -1 + 1 = 0
	G.hand[0][0] = curse;
	G.hand[0][1] = estate;
	G.handCount[0] = 2;
	G.deckCount[0] = 0;
	G.discardCount[0] = 0;

	// set up player 1 [deck only] expected score = 3 + 6 = 9
	G.deck[1][0] = duchy;
	G.deck[1][1] = province;
	G.deckCount[1] = 2;
	G.discardCount[1] = 0;
	G.handCount[1] = 0;

	// set up player 2 [discard only] expected score = 1 + (2/10 = 0) = 1
	G.discard[2][0] = great_hall;
	G.discard[2][1] = gardens;
	G.discardCount[2] = 2;
	G.deckCount[2] = 0;
	G.handCount[2] = 0;

	// set up player 3 [cards in each but no victory points] score = 0
	G.hand[3][0] = copper;
	G.deck[3][0] = mine;
	G.deck[3][1] = sea_hag;
	G.discard[3][0] = baron;
	G.discard[3][1] = minion;
	G.discard[3][2] = feast;
	G.handCount[3] = 1;
	G.deckCount[3] = 2;
	G.discardCount[3] = 3;


	// test player 0
	asserttrue(scoreFor(0, &G), 0, "Test2, Player 0", &success); 
	// test player 1
	//*** test fails because of a bug in scoreFor(): counts up to discardCount, not deckCount, when checking deck
	asserttrue(scoreFor(1, &G), 9, "Test2, Player 1", &success); 
	// test player 2
	asserttrue(scoreFor(2, &G), 1, "Test2, Player 2", &success); 
	// test player 3
	asserttrue(scoreFor(3, &G), 0, "Test2, Player 3", &success); 




/*-------------Test 3 ----------------------------------------------------------------------------------------------------------------------------------	
one player has all victory point cards, in hand, deck, and discard 			
[checks all cards in all 3 arrays]
*/

	
	for (int i = 0; i < MAX_DECK; i++){
		G.deck[0][i] = -1;
		G.hand[0][i] = -1;
		G.discard[0][i] = -1;
	}

	// put one of each card into each array
	for(int j = 0; j < 28; j++)
	{
		G.hand[0][j] = j;		// set to card's number
		G.deck[0][j] = j;	
		G.discard[0][j] = j;	
	}

	// update counts
	G.handCount[0] = 27;
	G.deckCount[0] = 27;
	G.discardCount[0] = 27;

	// expected 3(-1 + 1 + 3 + 6 + 1 + (81/10 = 8)) = 3 * 18 = 54
	asserttrue(scoreFor(0, &G), 54, "Player has all victory point cards in hand, deck, and discard", &success); 
	// *** test fails because of a bug in dominion.c -> does not handle gardens cards correctly



/*-------------Test 4 ----------------------------------------------------------------------------------------------------------------------------------	
cycle through filling out the hand with each victory card			
[tests each victory points card individually]
*/
	// start with all cards in all arrays as non-cards
	for (int j = 0; j < MAX_DECK; j++){
		G.deck[0][j] = -1;
		G.hand[0][j] = -1;
		G.discard[0][j] = -1;
	}
	G.handCount[0] = G.deckCount[0] = G.discardCount[0] = MAX_DECK;

	// fill entire hand with each vp card
	for (int j = 0; j < MAX_DECK; j++){
		G.hand[0][j] = vpCards[0];
	}
	asserttrue(scoreFor(0, &G), -500, "Test curse", &success); 

	for (int j = 0; j < MAX_DECK; j++){
		G.hand[0][j] = vpCards[1];
	}
	asserttrue(scoreFor(0, &G), 500, "Test estate", &success); 

	for (int j = 0; j < MAX_DECK; j++){
		G.hand[0][j] = vpCards[2];
	}
	asserttrue(scoreFor(0, &G), 1500, "Test duchy", &success); 

	for (int j = 0; j < MAX_DECK; j++){
		G.hand[0][j] = vpCards[3];
	}
	asserttrue(scoreFor(0, &G), 3000, "Test province", &success); 

	for (int j = 0; j < MAX_DECK; j++){
		G.hand[0][j] = vpCards[4];
	}
	asserttrue(scoreFor(0, &G), 500, "Test great hall", &success); 

	for (int j = 0; j < MAX_DECK; j++){
		G.hand[0][j] = vpCards[5];
	}
	//*** test fails because of a bug in scoreFor: doesn't handle gardens correctly
	asserttrue(scoreFor(0, &G), 50, "Test gardens", &success); 
	


	if (success == 1){
		printf("All tests passed!\n");
	}

	return 0;
}




int main()
{
	testScoreFor();
	return 0;
}