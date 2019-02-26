/*
Kristen Harrison
CS 362, Assignment 4
random test generator for smithy card
*/
#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>
#include <time.h>
//int num = (rand() % (upper - lower + 1)) + lower; 

/* 
bug: nothing added to discard (unchanged) [should have received the smithy card] -- problem with discardCard
my bug: player draws extra card, so failing tests for hand and deck
*/


int randTestSmithy()
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

		// randomize counters
		post.handCount[player] = (rand() % MAX_HAND) + 1;
		post.deckCount[player] = (rand() % (MAX_DECK - 2)) + 3;
		post.discardCount[player] = (rand() % MAX_DECK) + 1;

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

		// add top 3 deck cards to hand (+3 draw)
		for (int i = 0; i < 3; i++){
			pre.hand[player][pre.handCount[player]] = pre.deck[player][pre.deckCount[player] - 1];
			pre.handCount[player]++;	
			pre.deckCount[player]--;
		}

		// make sure a smithy is in the hand
		int handPos;
		int notPresent = 1;
		for (int i = 0; i < pre.handCount[player]; i++){
			if (pre.hand[player][i] == smithy){
				handPos = i;
				notPresent = 0;
			}
		}
		if (notPresent){
			handPos = rand() % pre.handCount[player];
			pre.hand[player][handPos] = smithy;
			post.hand[player][handPos] = smithy;
		}

		// remove smithy card from hand by swapping it with the last card, setting it to -1, and decrementing handCount
		pre.hand[player][handPos] = pre.hand[player][pre.handCount[player]-1];
		pre.hand[player][pre.handCount[player]-1] = -1;	
		pre.handCount[player]--;

		// discard the smithy and increment discardCount
		pre.discard[player][pre.discardCount[player]] = smithy; 	
		pre.discardCount[player]++;	



		// cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
		cardEffect(smithy, -1, -1, -1, &post, handPos, 0); 


	/*
		// can use this to examine contents if piles are small
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



	printf("\nTesting smithy card:\n\n");



	//-------------Test 1 ----------------------------------------------------------------------------------------------------------------------------------	
	// check that the number of actions is unchanged
	//
		asserttrue(post.numActions, pre.numActions, "Test number of actions is unchanged", &success);



	//-------------Test 2 ----------------------------------------------------------------------------------------------------------------------------------	
	// check that the top 3 cards has been taken off the deck [compare deckCount and contents of deck]
	//
		asserttrue(post.deckCount[player], pre.deckCount[player], "Test deckCount decremented by 3", &success);
		for (int j = 0; j < MAX_DECK; j++){
			asserttrue(post.deck[player][j], pre.deck[player][j], "Test that top 3 cards taken off deck", &success);
		}


	//-------------Test 3 ----------------------------------------------------------------------------------------------------------------------------------	
	// check that the smithy card has been moved to the discard pile [compare discardCount and contents of discard[]]
	//
		asserttrue(post.discardCount[player], pre.discardCount[player], "Test discardCount incremented", &success);
		for (int j = 0; j < MAX_DECK; j++){
			asserttrue(post.discard[player][j], pre.discard[player][j], "Test that smithy card added to discard pile", &success);
		}


	//-------------Test 4 ----------------------------------------------------------------------------------------------------------------------------------	
	// check that the top 3 cards from deck were added to the hand, last card switched with smithy, and last slot removed to remove smithy card
	// [compare handCount and contents of hand[]]
		asserttrue(post.handCount[player], pre.handCount[player], "Test handCount is incremented by 2 (+3 and smithy removed)", &success);
		for (int j = 0; j < MAX_DECK; j++){
			asserttrue(post.hand[player][j], pre.hand[player][j], "Test hand updated with top 3 cards from deck and smithy removed", &success);
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
	randTestSmithy();
	return 0;
}


/* for reference 
int playSmithy(int currentPlayer, struct gameState *state, int handPos)
{
  int i;
  //+3 Cards
//  for (i = 0; i < 3; i++) // correct line
  for (i = 0; i <= 3; i++) // bug: player gets an extra card
  {
    drawCard(currentPlayer, state);
  }
      
  //discard card from hand
  discardCard(handPos, currentPlayer, state, 0);
  return 0;
}
*/
