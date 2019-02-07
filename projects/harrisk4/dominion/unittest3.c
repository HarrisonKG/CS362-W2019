

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>


/*

void emptyArray(int arr[], int size){
	// sets entire array to -1
	for (int i = 0; i < size; i++){
		arr[i] = -1;
	}
}
*/


int testFullDeckCount()
{
	// initialize game state
	struct gameState G = {};
	memset(&G, '\0', sizeof (struct gameState));
	
	int success = 1;
	int result, expected;
	printf("Testing fullDeckCount):\n\n");

	int cardTypes[27] = { curse, estate, duchy, province, copper, silver, gold, adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map };

	
	 

	// number of players ranges from 2 - 4
	for (int i = 2; i <= 4; i++){					// these tests took way too long to run through -- 		

		// test each player as the current player
		for(int j = 0; j < i; j++){			

			// test each card type
			for (int m = 0; m < 27; m++){		

				// test total range of possible cards in hand -> max hand and max deck are 500
				for (int k = 0; k <= 500; k++){	

					G.deckCount[j] = G.handCount[j] = G.discardCount[j] = k;

					//assign arrays a default value of -1 (so that it doesn't inadvertently indicate one of the 27 cards)
					for (int p = 0; p < MAX_PLAYERS; p++){
						for (int r = 0; r < MAX_DECK; r++){
							G.deck[p][r] = -1;
							G.hand[p][r] = -1;
							G.discard[p][r] = -1;
							//printf("G.deck[p][r] = %i\n", G.deck[p][r]);
						}
					}	
				
					// handle n as 0 in special case (so we can test with the arrays unchanged)
					result = fullDeckCount(j, cardTypes[m], &G);
					expected = 0;

					if (result != expected){
						printf("TEST FAILED: ");
						success = -1;
						printf("fullDeckCount result of %i, Expected %i\n\n", result, expected);
					} else {
						printf("TEST SUCCEEDED: ");
					}
					printf("fullDeckCount result of %i, Expected %i\n\n", result, expected);

					// steadily fill array from front to back to try the possibilities from none to all cards being the card of interest 
					for(int n = 0; n < k; n++){
						// set current player's deck at n position to the card of interest (this steadily fills in the array)
						G.deck[j][n] = cardTypes[m]; 	
						//test with array filled with X card up to n index
						result = fullDeckCount(j, cardTypes[m], &G);
						expected = n+1;

						if (result != expected){
							printf("TEST FAILED: ");
							success = -1;
							printf("fullDeckCount result of %i, Expected %i\n\n", result, expected);
						} else {
							printf("TEST SUCCEEDED \n");
						}
						//printf("fullDeckCount result of %i, Expected %i\n\n", result, expected);
					}
				}
			}
		}
	}

	if (success == 1){
		printf("All tests passed!\n");
	}

	return 0;
}



int main()
{
	testFullDeckCount();
	return 0;
}

