/*
Kristen Harrison
CS 362, Assignment 4
random test generator for adventurer card
*/
#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>
#include <time.h>


int randTestAdventurer()
{
	for (int i = 0; i < 2000; i++){
		int success = 1;
		struct gameState post = {};
		struct gameState pre = {};
		memset(&post, '\0', sizeof (struct gameState));
		memset(&pre, '\0', sizeof (struct gameState));

		// randomize number of players and current player
		post.numPlayers = (rand() % 3) + 2; 	// 2 - 4 players
		int player = rand() % post.numPlayers;
		post.whoseTurn = player;

		// set all cards and piles to -1
		for (int i = 0; i < MAX_PLAYERS; i++){
			for (int j = 0; j < MAX_DECK; j++){
				post.deck[i][j] = -1;
				post.hand[i][j] = -1;
				post.discard[i][j] = -1;
				pre.deck[i][j] = -1;
				pre.hand[i][j] = -1;
				pre.discard[i][j] = -1;
			}
		}

		// randomize counters
		post.handCount[player] = (rand() % MAX_HAND) + 1;
		post.deckCount[player] = (rand() % (MAX_DECK - 1)) + 2;
		post.discardCount[player] = rand() % MAX_DECK + 1;

		// randomize number of actions (turns start at 1 action, and wouldn't vary by more than a couple, so 10 should cover all normal gameplay situations)
		post.numActions = rand() % 10;

		// shallow copy post to pre (for everthing except the pointers)
		pre = post;

		// fill in random cards in piles and (deep) copy the piles
		for (int i = 0; i < post.handCount[player]; i++){
			post.hand[player][i] = rand() % 27;
			pre.hand[player][i] = post.hand[player][i];
		}
		for (int i = 0; i < post.deckCount[player]; i++){
			post.deck[player][i] = rand() % 27;
			pre.deck[player][i] = post.deck[player][i];
		}
		for (int i = 0; i < post.discardCount[player]; i++){
			post.discard[player][i] = rand() % 27;
			pre.discard[player][i] = post.discard[player][i];
		}


		/* modify pre struct to compare post to after test */

		// make sure an adventurer is in the hand
		int handPos;
		int notPresent = 1;
		for (int i = 0; i < pre.handCount[player]; i++){
			if (pre.hand[player][i] == adventurer){
				handPos = i;
				notPresent = 0;
			}
		}
		if (notPresent){
			handPos = rand() % pre.handCount[player];
			pre.hand[player][handPos] = adventurer;
			post.hand[player][handPos] = adventurer;
		}

		// make sure deck has at least two treasures
		int treasures = 0;
		for (int i = 0; i < pre.deckCount[player]; i++){
			if (pre.deck[player][i] == copper || pre.deck[player][i] == silver || pre.deck[player][i] == gold){
				treasures++;
			}
		}
		if (treasures < 2){
			pre.deck[player][0] = copper;
			post.deck[player][0] = copper;
			pre.deck[player][pre.deckCount[player] - 1] = silver;
			post.deck[player][pre.deckCount[player] - 1] = silver;
		}


		// draw cards from deck until find 2 treasures; keep these and discard the rest
		treasures = 0;
		int drawnCard = -1;
		int nontreasures[pre.deckCount[player]];
		int drawnNontreasures = 0;
		while (treasures < 2){
			drawnCard = pre.deck[player][pre.deckCount[player] - 1];
			pre.deck[player][pre.deckCount[player] - 1] = -1;
			pre.deckCount[player]--;
			if (drawnCard == copper || drawnCard == silver || drawnCard == gold){
				treasures++;
				pre.hand[player][pre.handCount[player]] = drawnCard;
				pre.handCount[player]++;	
			} else {
				nontreasures[drawnNontreasures] = drawnCard;
				drawnNontreasures++;
			}
		}
		for (int i = drawnNontreasures - 1; i >= 0; i--){
			pre.discard[player][pre.discardCount[player]] = nontreasures[i]; 	
			pre.discardCount[player]++;			
		}

		
		// remove adventurer card from hand by swapping it with the last card, setting it to -1, and decrementing handCount
		pre.hand[player][handPos] = pre.hand[player][pre.handCount[player]-1];
		pre.hand[player][pre.handCount[player]-1] = -1;	
		pre.handCount[player]--;

		// discard the adventurer and increment discardCount
		pre.discard[player][pre.discardCount[player]] = adventurer; 	
		pre.discardCount[player]++;	


		// cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
		cardEffect(adventurer, -1, -1, -1, &post, handPos, 0); 

	/*
		// examine contents if piles are small
		for (int j = 0; j < post.deckCount[player]; j++){
			printf("post deck[%i]: %i\n", j, post.deck[player][j]);
		}
		for (int j = 0; j < pre.deckCount[player]; j++){
			printf("pre deck[%i]: %i\n", j, pre.deck[player][j]);
		}
		for (int j = 0; j < post.handCount[player]; j++){
			printf("post hand[%i]: %i\n", j, post.hand[player][j]);
		}
		for (int j = 0; j < pre.handCount[player]; j++){
			printf("pre hand[%i]: %i\n", j, pre.hand[player][j]);
		}
		for (int j = 0; j < post.discardCount[player]; j++){
			printf("post discard[%i]: %i\n", j, post.discard[player][j]);
		}
		for (int j = 0; j < pre.discardCount[player]; j++){
			printf("pre discard[%i]: %i\n", j, pre.discard[player][j]);
		}
	*/


		printf("\nTesting adventurer card:\n\n");



	//-------------Test 1 ----------------------------------------------------------------------------------------------------------------------------------	
	// check that the number of actions has not changed
	//
		asserttrue(post.numActions, pre.numActions, "Test number of actions unchanged", &success);


	//-------------Test 2 ----------------------------------------------------------------------------------------------------------------------------------	
	// check that the deck has been drawn from until two treasure found [compare deckCount and contents of deck]
	//
		asserttrue(post.deckCount[player], pre.deckCount[player], "Test deckCount decremented correctly", &success);
		for (int j = 0; j < post.deckCount[player]; j++){
			asserttrue(post.deck[player][j], pre.deck[player][j], "Test that deck was drawn from until 2 treasures were found", &success);
		}


	//-------------Test 3 ----------------------------------------------------------------------------------------------------------------------------------	
	// check that the adventurer card has been moved to the discard pile [compare discardCount and contents of discard[]], along with non-treasures that were drawn
	//
		asserttrue(post.discardCount[player], pre.discardCount[player], "Test discardCount incremented by [1 adventurer + non-treasure drawn cards]", &success);
		for (int j = 0; j < post.discardCount[player]; j++){
			asserttrue(post.discard[player][j], pre.discard[player][j], "Test that adventurer card and drawn non-treasures were added to discard pile", &success);
		}


	//-------------Test 4 ----------------------------------------------------------------------------------------------------------------------------------	
	// check that the top card from deck was added to the hand, last card switched with adventurer, and last slot removed to remove adventurer card
	// [compare handCount and contents of hand[]]
		asserttrue(post.handCount[player], pre.handCount[player], "Test handCount is incremented by 1 (kept two treasure cards and removed adventurer)", &success);
		for (int j = 0; j < post.handCount[player]; j++){
			asserttrue(post.hand[player][j], pre.hand[player][j], "Test hand updated with the first two treasures drawn from deck and adventurer removed", &success);
		}



		if (success == 1){
			printf("All tests passed!\n");
		}

	}
	return 0;
}




int main()
{
	srand(time(0));
	randTestAdventurer();
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
//    int drawnCard = state->hand[currentPlayer][state->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
    int drawnCard = state->hand[currentPlayer][state->handCount[currentPlayer]];// bug: off by one error so the card referenced is outside the bounds of the hand
    if (drawnCard == copper || drawnCard == silver || drawnCard == gold)
      drawntreasure++;
    else{
      temphand[z]=drawnCard;
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