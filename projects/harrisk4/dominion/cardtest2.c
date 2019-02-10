/* Kristen Harrison
362, assignment 3
*/
#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>

// draw cards from deck until find 2 treasure cards -> put those into hand and discard the others



int testAdventurer()
{
	struct gameState G = {};	// use this to test function
	struct gameState endState = {};	// use this as the answer sheet to compare G to after test runs
	memset(&G, '\0', sizeof (struct gameState));
	memset(&endState, '\0', sizeof (struct gameState));
	int success = 1;	// set to -1 if any test fails
	G.whoseTurn = 0;	// player 0

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
	for (int i = 0; i < 8; i++){
		G.deck[0][i] = i;
		endState.deck[0][i] = i;
	}
 	
 	// add cards to hands
 	for (int i = 0; i < 7; i++){
		G.hand[0][i] = i;
		endState.hand[0][i] = i;
	}

	G.handCount[0] = 7;
	G.deckCount[0] = 8;	
	endState.handCount[0] = 7;
	endState.deckCount[0] = 8; 


	G.numActions = 1;	// turn starts at 1
	endState.numActions = 1;

	G.discardCount[0] = 0;	// discard pile starts empty
	G.hand[0][1] = adventurer;	// include adventurer (7)


	/* modify struct to compare to after test */
	
	// cards 5, 6, and 7 were drawn into hand [5 and 6 kept, 7 discarded]
	endState.deckCount[0] = 5;

	// first treasure added to hand
	endState.hand[0][7] = 6;
	// second treasure added to hand, then swapped with adventurer in index 1 to discard adventurer
	endState.hand[0][1] = 5;

	// original handCount was 7, then we added +2 cards, and removed adventurer
	endState.handCount[0] = 8;

	// non-treasure card we drew, plus adventurer card
	endState.discard[0][0] = 7;
	endState.discard[0][1] = 7;
	endState.discardCount[0] = 2;



	// cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
	cardEffect(7, -1, -1, -1, &G, 1, 0); 


/*
	// examine contents if piles are small
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



printf("\nTesting adventurer card:\n\n");



//-------------Test 1 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the number of actions has not changed
//
	asserttrue(G.numActions, 1, "Test number of actions unchanged", &success);


//-------------Test 2 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the deck has been drawn from until two treasure found [compare deckCount and contents of deck]
//
	asserttrue(G.deckCount[0], endState.deckCount[0], "Test deckCount decremented correctly", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(G.deck[0][j], endState.deck[0][j], "Test that deck was drawn from until 2 treasures were found", &success);
	}


//-------------Test 3 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the adventurer card has been moved to the discard pile [compare discardCount and contents of discard[]], along with non-treasures that were drawn
//
	asserttrue(G.discardCount[0], endState.discardCount[0], "Test discardCount incremented by 2", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(G.discard[0][j], endState.discard[0][j], "Test that adventurer card and drawn non-treasures were added to discard pile", &success);
	}


//-------------Test 4 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the top card from deck was added to the hand, last card switched with great hall, and last slot removed to remove great hall card
// [compare handCount and contents of hand[]]
	asserttrue(G.handCount[0], endState.handCount[0], "Test handCount is incremented by 1 (kept two treasure cards and removed adventurer)", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(G.hand[0][j], endState.hand[0][j], "Test hand updated with the first two treasures drawn from deck and adventurer removed", &success);
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
	testAdventurer();
	return 0;
}




/* for reference
int playAdventurer(int drawntreasure, struct gameState *state, int currentPlayer, int temphand [], int z)
{
  while(drawntreasure<2){
    if (state->deckCount[currentPlayer] <1){//if the deck is empty we need to shuffle discard and add to deck
      shuffle(currentPlayer, state);
    }
    drawCard(currentPlayer, state);
//    int cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
    int cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]];// bug: off by one error so the card referenced is outside the bounds of the hand
    if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
      drawntreasure++;
    else{
      temphand[z]=cardDrawn;
      state->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
      z++;
    }
  }
  while(z-1>=0){
    state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
    z=z-1;
  }
  return 0;
}
*/