// victory cards in this set are just: curse, estate, duchy, province, great_hall, gardens

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>
/*
cards in all places; victory pts only in: 1 - only deck, 1 only discard, 1 only hand, 1 no victory points anywhere
cards only in, victory points only in: ''
no cards, no victory points
cards in all places, victory points in all places
*/


void runTest(struct gameState* state, int player, int expected, int* success){
	int result = scoreFor(player, state);

	if (result != expected){
		printf("TEST FAILED: ");
		*success = -1;
		printf("scoreFor result of %i, Expected %i\n\n", result, expected);
	} else {
		printf("TEST SUCCEEDED\n");
		printf("scoreFor result of %i, Expected %i\n\n", result, expected);
	}
}



int testScoreFor()
{
	// initialize game state
	struct gameState G = {};
	memset(&G, '\0', sizeof (struct gameState));
	
	int success = 1;
	int result, expected;
	printf("Testing scoreFor():\n\n");



//-------------Test 1 (x4 players) ----------------------------------------------------------------------------------------------------------------------------------	
// none of the 4 players have any cards (therefore no victory points either)		[all should return 0 ] 
// [predominantly to make sure all player scores are zero]
//
	printf("Test 1: test all players having no cards");
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
			runTest(&G, i, 0, &success);
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
	runTest(&G, 0, 0, &success);
	// test player 1
	runTest(&G, 1, 9, &success);
	// test player 2
	runTest(&G, 2, 1, &success);
	// test player 3
	runTest(&G, 3, 0, &success);







	
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