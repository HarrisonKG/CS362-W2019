

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "asserttrue.h"
#include "rngs.h"
#include <string.h>



int testNumHandCards()
{
	// initialize game state
	struct gameState G = {};
	memset(&G, '\0', sizeof (struct gameState));
	
	int success = 1;
	int result, expected;
	printf("Testing numHandCards():\n\n");


	// number of players ranges from 2 - 4
	for (int i = 2; i <= 4; i++){			
		// test each player as the current player
		for(int j = 0; j < i; j++){			
			// test total range of possible cards in hand
			for (int k = 0; k <= 500; k++){	
				// set current player's hand to some number of cards, then test
				G.handCount[j] = k;
				G.whoseTurn = j;
				result = numHandCards(&G);
				expected = k;

				if (result != expected){
					printf("TEST FAILED: ");
					success = -1;
					printf("numHandCards result of %i, Expected %i\n\n", result, expected);
				} else {
					printf("TEST SUCCEEDED: ");
				}
				printf("numHandCards result of %i, Expected %i\n\n", result, expected);
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
	testNumHandCards();
	return 0;
}