
#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>

// great hall: +1 card, +1 action



int testGreatHall()
{
/* 
bug: nothing added to discard (unchanged) [should have received the great_hall card] -- problem with discardCard
*/
	struct gameState G = {};	// use this to test function
	struct gameState endState = {};	// use this as the answer sheet to compare G to after test runs
	memset(&G, '\0', sizeof (struct gameState));
	memset(&endState, '\0', sizeof (struct gameState));
	int success = 1;	// set to -1 if any test fails

	// set all cards and piles to -1
	for (int i = 0; i < MAX_PLAYERS; i++){
		for (int j = 0; j < MAX_DECK; j++){
			G.deck[i][j] = -1;
			G.hand[i][j] = -1;
			G.discard[i][j] = -1;
			endState.deck[i][j] = -1;
			endState.hand[i][j] = -1;
			endState.discard[i][j] = -1;
		}
		G.handCount[i] = -1;
		G.deckCount[i] = -1;	
		G.discardCount[i] = -1;
	}

	// add cards to decks
	for (int i = 0; i < 28; i++){
		G.deck[0][i] = i;
		endState.deck[0][i] = i;
	}
 	
 	// add cards to hands
 	for (int i = 0; i < 27; i++){
		G.hand[0][i] = i;
		endState.hand[0][i] = i;
	}

	G.handCount[0] = 27;
	G.deckCount[0] = 28;	
	endState.handCount[0] = 27;
	endState.deckCount[0] = 28; 
	G.numActions = 1;	// turn starts at 1

	G.discardCount[0] = 0;	// discard pile starts empty
	G.hand[0][1] = great_hall;	// include great_hall (16)



	/* set up struct to compare G to after test */

	// add top deck card to hand (+1 draw)
	endState.hand[0][endState.handCount[0]] = endState.deck[0][endState.deckCount[0] - 1];
	endState.handCount[0]++;	
	endState.deckCount[0]--;

	// remove great hall card from hand by swapping it with the last card, setting it to -1, and decrementing handCount
	endState.hand[0][1] = endState.hand[0][endState.handCount[0]-1];
	endState.hand[0][endState.handCount[0]-1] = -1;	
	endState.handCount[0]--;

	// discard the great_hall and increment discardCount
	endState.discard[0][0] = great_hall; 	
	endState.discardCount[0] = 1;	



	// cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
	cardEffect(16, -1, -1, -1, &G, 1, 0); 


/*
	// can use this to examine contents if piles are small
	for (int j = 0; j < G.deckCount[0]; j++){
		printf("G deck[%i]: %i\n", j, G.deck[0][j]);
	}
	for (int j = 0; j < endState.deckCount[0]; j++){
		printf("endState deck[%i]: %i\n", j, endState.deck[0][j]);
	}
	for (int j = 0; j < G.handCount[0]; j++){
		printf("G hand[%i]: %i\n", j, G.hand[0][j]);
	}
	for (int j = 0; j < endState.handCount[0]; j++){
		printf("endState hand[%i]: %i\n", j, endState.hand[0][j]);
	}
	for (int j = 0; j < G.discardCount[0]; j++){
		printf("G discard[%i]: %i\n", j, G.discard[0][j]);
	}
	for (int j = 0; j < endState.discardCount[0]; j++){
		printf("endState discard[%i]: %i\n", j, endState.discard[0][j]);
	}
*/



printf("\nTesting great hall card:\n\n");



//-------------Test 1 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the number of actions has incremented by 1
//
	asserttrue(G.numActions, 2, "Test number of actions incremented by 1", &success);



//-------------Test 2 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the top card has been taken off the deck [compare deckCount and contents of deck]
//
	asserttrue(G.deckCount[0], endState.deckCount[0], "Test deckCount decremented", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(G.deck[0][j], endState.deck[0][j], "Test that top card taken off deck", &success);
	}


//-------------Test 3 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the great hall card has been moved to the discard pile [compare discardCount and contents of discard[]]
//
	asserttrue(G.discardCount[0], endState.discardCount[0], "Test discardCount incremented", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(G.discard[0][j], endState.discard[0][j], "Test that great hall card added to discard pile", &success);
	}


//-------------Test 4 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the top card from deck was added to the hand, last card switched with great hall, and last slot removed to remove great hall card
// [compare handCount and contents of hand[]]
	asserttrue(G.handCount[0], endState.handCount[0], "Test handCount is unchanged (+1 and great hall removed)", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(G.hand[0][j], endState.hand[0][j], "Test hand updated with top card from deck and great hall removed", &success);
	}


//-------------Test 5 ----------------------------------------------------------------------------------------------------------------------------------	
// Test that every other player's piles and counters were unchanged [all of those should still be set to -1]
//
	for (int i = 1; i < MAX_PLAYERS; i++){
		for (int j = 0; j < MAX_DECK; j++){
			asserttrue(G.deck[i][j], -1, "Test that other players' piles are unchanged", &success);
			asserttrue(G.hand[i][j], -1, "Test that other players' piles are unchanged", &success);
			asserttrue(G.discard[i][j], -1, "Test that other players' piles are unchanged", &success);
		}
			asserttrue(G.deckCount[i], -1, "Test that other players' counters are unchanged", &success);
			asserttrue(G.handCount[i], -1, "Test that other players' counters are unchanged", &success);
			asserttrue(G.discardCount[i], -1, "Test that other players' counters are unchanged", &success);
	}




	if (success == 1){
		printf("All tests passed!\n");
	}

	return 0;
}



int main()
{
	testGreatHall();
	return 0;
}



/* for reference
case great_hall:
      //+1 Card
      drawCard(currentPlayer, state);
      
      //+1 Actions
      state->numActions++;
      
      //discard card from hand
      discardCard(handPos, currentPlayer, state, 0);
      return 0;
    
*/