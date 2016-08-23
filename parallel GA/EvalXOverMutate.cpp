



void evaluate( Trip trip[CHROMOSOMES], int coordinates[CITIES][2] ){
		float sum = 0;
		
    #pragma omp parallel for firstprivate(sum) private(i, j, cNum, cNum, x1, y1, x2, y2)
		for ( i = 0; i < CHROMOSOMES; i++ ) {
			char start = trip[i].itinerary[0];
			int last = (start >= 'A' ) ? start - 'A' : start - '0' + 26;
			
			for ( j = 1; j < CITIES; j++ ) {
				char next = trip[i].itinerary[j];		
				cNum = (next >= 'A' ) ? next - 'A' : next - '0' + 26;
				x1 = coordinates[last][0];
				y1 = coordinates[last][1];
				x2 = coordinates[cNum][0];
				y2 = coordinates[cNum][1];
				sum += sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
				last = cNum;
			}
			trip[i].fitness = sum;
    }
    
    sort(trip, trip + CHROMOSOMES, less<Trip>());
}





void crossover( Trip parents[TOP_X], Trip offsprings[TOP_X], int coordinates[CITIES][2] ){
		
		#pragma omp parallel for private(i, j, k)
		for ( i = 0; i < TOP_X; i += 2 ) {
			//char offspring1[CITIES + 1];
			//char offspring2[CITIES + 1];
			//offspring1[CITIES] = 0;
			//offspring2[CITIES] = 0;
			offsprings[i].itinerary[0] = parents[i].itinerary[0];
			char start = parents[i].itinerary[0];
			char next1;
			char next2;
			int cNum1;
			int cNum2;
			for ( k = 1; k < CITIES; k++){
			
				for ( j = 0; j < CITIES; j++ ) {		
					if ( parents[i].itinerary[j] == start ) {
						if ( j != CITIES - 1 ) {
							next1 = parents[i].itinerary[j + 1];
						}
						else{
							next1 = parents[i].itinerary[0];
						}
					}
				
					if ( parents[i + 1].itinerary[j] == start ) {
						if ( j != CITIES -1 ) {
							next2 = parents[i + 1].itinerary[j + 1];
						}
						else{
							next2 = parents[i + 1].itinerary[0];
						}
					}
					//find_if
				}// end for j
			
				for ( j = 0; j < k; j++ ) {
					if ( offsprings[i].itinerary[j] == next1 && offsprings[i].itinerary[j] == next2 ) {
						char city = rand( ) % CITIES;
						if ( city < 26 )
							city += 'A';
  					else
  						city = city - 26 + '0';
  					next1 = city;
  					j = 0;
  					
					}
					else if ( offsprings[i].itinerary[j] == next1 ) {
						offsprings[i].itinerary[k] = next2;
						start = next2;
					}
					else if ( offsprings[i].itinerary[j] == next2 ) {
						offsprings[i].itinerary[k] = next1;
						start = next1;
					}
					else if(j=k-1){
						int last = (start >= 'A' ) ? start - 'A' : start - '0' + 26;
						cNum1 = (next1 >= 'A' ) ? next1 - 'A' : next1 - '0' + 26;
						cNum2 = (next2 >= 'A' ) ? next2 - 'A' : next2 - '0' + 26;
						x = coordinates[last][0];
						y = coordinates[last][1];
						x1 = coordinates[cNum1][0];
						y1 = coordinates[cNum1][0];
						x2 = coordinates[cNum2][0];
						y2 = coordinates[cNum2][1];
						float f1 = sqrt(pow((x1 - x), 2) + pow((y1 - y), 2));
				  	float f2 = sqrt(pow((x2 - x), 2) + pow((y2 - y), 2));
						if(f1<=f2){
							offsprings[i].itinerary[k] = next1;
							start = next1;
						}
						else{
							offsprings[i].itinerary[k] = next2;
							start = next2;
						}
				}
				}// end for j
				
				char sibling1 = offsprings[i].itinerary[k];
				int siNum = (sibling1 >= 'A' ) ? sibling1 - 'A' : sibling1 - '0' + 26;
				siNum = 35 - si;
				
				char sibling2;
				if ( siNum < 26 )
					sibling2 = siNum + 'A';
  			else
  				sibling2 = siNum - 26 + '0';
  				
  			offsprings[i + 1].itinerary[k] = sibling2;
			}// end for k
			
		}// end for i
		
}

void mutate( Trip offsprings[TOP_X] ){

		#pragma omp parallel for private(i, cNum1, cNum2)
		for ( i = 0; i < CHROMOSOMES; i++ ) {
			int mutate = rand() % 100;
			if ( mutate >= 50 ) {
				while (true) {
					cNum1 = rand() % CITIES;
					cNum2 = rand() % CITIES;
					if ( cNum1 != cNum2 ) {
						char temp = offsprings[i].itinerary[cNum1];
						offsprings[i].itinerary[cNum1] = offsprings[i].itinerary[cNum2];
						offsprings[i].itinerary[cNum2] = temp;
						break;
					}
				}
			}
		}

}
























