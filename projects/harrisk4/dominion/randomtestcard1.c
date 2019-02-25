/*
Kristen Harrison
CS 362, Assignment 4
random test generator for great hall card
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



int randTestGreatHall()
{
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
	post.handCount[player] = (rand() % 10) + 1;//MAX_HAND) + 1;
	post.deckCount[player] = (rand() % 10) + 1;//MAX_DECK) + 1;
	post.discardCount[player] = rand() % 10 + 1;//MAX_DECK + 1;

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

	// increment number of actions
	pre.numActions = pre.numActions + 1;

	// add top deck card to hand (+1 draw)
	pre.hand[player][pre.handCount[player]] = pre.deck[player][pre.deckCount[player] - 1];
	pre.handCount[player]++;	
	pre.deckCount[player]--;

	// make sure a great hall is in the hand
	int handPos;
	int notPresent = 1;
	for (int i = 0; i < pre.handCount[player]; i++){
		if (pre.hand[player][i] == great_hall){
			handPos = i;
			notPresent = 0;
		}
	}
	if (notPresent){
		handPos = rand() % pre.handCount[player];
		pre.hand[player][handPos] = great_hall;
	}

	// remove great hall card from hand by swapping it with the last card, setting it to -1, and decrementing handCount
	pre.hand[player][handPos] = pre.hand[player][pre.handCount[player]-1];
	pre.hand[player][pre.handCount[player]-1] = -1;	
	pre.handCount[player]--;

	// discard the great_hall and increment discardCount
	pre.discard[player][pre.discardCount[player]] = great_hall; 	
	pre.discardCount[player]++;	



	// cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
	cardEffect(great_hall, -1, -1, -1, &post, handPos, 0); 


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



printf("\nTesting great hall card:\n\n");



//-------------Test 1 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the number of actions has incremented by 1
//
	asserttrue(post.numActions, pre.numActions, "Test number of actions incremented by 1", &success);



//-------------Test 2 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the top card has been taken off the deck [compare deckCount and contents of deck]
//
	asserttrue(post.deckCount[player], pre.deckCount[player], "Test deckCount decremented", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(post.deck[player][j], pre.deck[player][j], "Test that top card taken off deck", &success);
	}


//-------------Test 3 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the great hall card has been moved to the discard pile [compare discardCount and contents of discard[]]
//
	asserttrue(post.discardCount[player], pre.discardCount[player], "Test discardCount incremented", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(post.discard[player][j], pre.discard[player][j], "Test that great hall card added to discard pile", &success);
	}


//-------------Test 4 ----------------------------------------------------------------------------------------------------------------------------------	
// check that the top card from deck was added to the hand, last card switched with great hall, and last slot removed to remove great hall card
// [compare handCount and contents of hand[]]
	asserttrue(post.handCount[player], pre.handCount[player], "Test handCount is unchanged (+1 and great hall removed)", &success);
	for (int j = 0; j < MAX_DECK; j++){
		asserttrue(post.hand[player][j], pre.hand[player][j], "Test hand updated with top card from deck and great hall removed", &success);
	}



	if (success == 1){
		printf("All tests passed!\n");
	}

	return 0;
}




int main()
{
	srand(time(0));
	randTestGreatHall();
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
