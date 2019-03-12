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




int testFullDeckCount()
{
	// initialize game state
	struct gameState G = {};
	memset(&G, '\0', sizeof (struct gameState));
	
	int success = 1;
	printf("\nTesting fullDeckCount():\n\n");

	int cardTypes[27] = { curse, estate, duchy, province, copper, silver, gold, adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map };

	
/* these tests took way too long to run through (5 - 10 mins)
 so I'm leaving the top loop commented out and only testing player 0,
 and only testing hands/decks/discards up to 50 cards instead of full deck 500
*/
	 

	// number of players ranges from 2 - 4
//	for (int i = 2; i <= 4; i++){			

		// test each player as the current player
		//for(int j = 0; j < i; j++){	
		int j = 0;	// just test 0 so it doesn't take forever	

			// test each card type
			for (int m = 0; m < 27; m++){		

				// test total range of possible cards in hand -> max hand and max deck are 500
				//for (int k = 0; k <= 500; k++){	
				for (int k = 0; k <= 50; k++){	

					G.deckCount[j] = G.handCount[j] = G.discardCount[j] = k;

					//assign arrays a default value of -1 (so that it doesn't inadvertently indicate one of the 27 cards)
					for (int p = 0; p < MAX_PLAYERS; p++){
						for (int r = 0; r < MAX_DECK; r++){
							G.deck[p][r] = -1;
							G.hand[p][r] = -1;
							G.discard[p][r] = -1;
						}
					}	
				
					// handle n as 0 in special case (so we can test with the arrays unchanged)
					asserttrue(fullDeckCount(j, cardTypes[m], &G), 0, "Return count of a card in all of a player's piles", &success); 

					// steadily fill array from front to back to try the possibilities from none to all cards being the card of interest 
					for(int n = 0; n < k; n++){
						// set current player's deck at n position to the card of interest (this steadily fills in the array)
						G.deck[j][n] = cardTypes[m]; 	
						//test with array filled with X card up to n index
						asserttrue(fullDeckCount(j, cardTypes[m], &G), n+1, "Return count of a card in all of a player's piles", &success); 
					}
				}
			}
		//}
	//}

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

