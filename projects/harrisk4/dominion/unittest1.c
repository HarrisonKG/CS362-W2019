/* 
*/

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>




// manually set supply of one card
void setSupply(struct gameState* state, int cardType, int count)
{
	state->supplyCount[cardType] = count;
}



// manually set supply of all cards
void setAll(struct  gameState* state, int cardTypes[], int counts[])
{
	for (int i = 0; i < 27; i++){
		setSupply(state, cardTypes[i], counts[i]);
	}
}



int testIsGameOver()
{
	// initialize game state
	struct gameState G = {};
	memset(&G, '\0', sizeof (struct gameState));
	
	int success = 1;
	int result, expected;
	printf("Testing isGameOver():\n\n");

	// use card array to manually set supply values for each card
	int cardTypes[27] = { curse, estate, duchy, province, copper, silver, gold, adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map };


//-------------Test 1 ----------------------------------------------------------------------------------------------------------------------------------	
// # provinces = 0; no other empty supply piles:	[ T||F should return 1 (game is over) ] 
// [predominantly to test provinces being zero]
	printf("Test with 0 provinces left, and all other cards with 1 remaining.\n");

	int supplies[27] = {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	setAll(&G, cardTypes, supplies); 

	result = isGameOver(&G);
	expected = 1;	// game should be over because we are out of provinces

	if (result != expected){
		printf("TEST FAILED: ");
		success = -1;
	} else {
		printf("TEST SUCCEEDED: ");
	}
	printf("isGameOver result of %i, Expected %i\n\n", result, expected);


//-------------Test 2 ----------------------------------------------------------------------------------------------------------------------------------	
// # provinces = 10; 3 empty supply piles 	[ F||T should return 1] (game is over) 
// [predominantly to test empty supply piles]
// prioritize testing empties in back of array and front since the edges are more vulnerable to 
// off by one errors, overstepping bounds, etc.

	printf("Test with 10 provinces left, and 3 cards with empty piles.\n");

	int supplies_2[27] = {0, 2, 3, 10, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 0, 0};
	setAll(&G, cardTypes, supplies_2); 

	result = isGameOver(&G);
	expected = 1;	// game should be over because we have three empty piles

	if (result != expected){
		printf("TEST FAILED: ");
		success = -1;
	} else {
		printf("TEST SUCCEEDED: ");
	}
	printf("isGameOver result of %i, Expected %i\n\n", result, expected);

	// ** bug found in isGameOver: loop only counts from 0 to 24 (misses 2 cards)


//-------------Test 3 ----------------------------------------------------------------------------------------------------------------------------------	
// # provinces = 1; 2 empty supply piles 	[ F||F should return 0] (game is not over)
// [test an edge case: the dividing line between true and false occurs between 2 and 3 empty supply piles]

	printf("Test with 1 province left, and a total of 2 cards with empty piles.\n");

	int supplies_3[27] = {1, 2, 3, 1, 0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
	setAll(&G, cardTypes, supplies_3); 

	result = isGameOver(&G);
	expected = 0;	// game should not be over because we have provinces and <3 empty piles

	if (result != expected){
		printf("TEST FAILED: ");
		success = -1;
	} else {
		printf("TEST SUCCEEDED: ");
	}
	printf("isGameOver result of %i, Expected %i\n\n", result, expected);


//-------------Test 4 ----------------------------------------------------------------------------------------------------------------------------------	
// # provinces = 0; max empty supply piles	[ T||T should return 1] (game is over)

	printf("Test with 0 provinces left, and all other cards empty as well.\n");

	int supplies_4[27] = {0};
	setAll(&G, cardTypes, supplies_4); 

	result = isGameOver(&G);
	expected = 1;	// game should be over because we have no provinces and 3+ empty piles

	if (result != expected){
		printf("TEST FAILED: ");
		success = -1;
	} else {
		printf("TEST SUCCEEDED: ");
	}
	printf("isGameOver result of %i, Expected %i\n\n", result, expected);


//-------------Test 5 ----------------------------------------------------------------------------------------------------------------------------------	
// # provinces = 4; no empty supply piles	[ F||F should return 0] (game not over)

	printf("Test with 4 provinces left, and no other card piles empty.\n");

	int supplies_5[27] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
	setAll(&G, cardTypes, supplies_5); 

	result = isGameOver(&G);
	expected = 0;	// game should not be over because we still have provinces and no empty piles

	if (result != expected){
		printf("TEST FAILED: ");
		success = -1;
	} else {
		printf("TEST SUCCEEDED: ");
	}
	printf("isGameOver result of %i, Expected %i\n\n", result, expected);


//------End of tests -------------------------------------------------------------------------------------------------------------

	if (success == 1){
		printf("All tests passed!\n");
	}

	return 0;
}




int main()
{
	testIsGameOver();
	return 0;
}

