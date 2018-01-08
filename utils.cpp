#include "utils.h"


void Transformer(Canvas *canvas, int col, int row, int r, int g, int b){
	int x = col;	
	int y = row;
	
	// First Panel
	if((col >= 0) && (col <= 15)){
		//First Panel
		if((row >= 0) && (row <= 7)){
			x = col + 16;
			y = row;
		}
		else if((row >= 8) &&  (row <= 15)){
			x = col;
			y = row - 8;
		}
		else if((row >= 16) && (row <= 23)){
			x = col + 16;
			y = row - 8;
		}
		else if((row >= 24) && (row <= 31)){
			x = col;
			y = row - 16;
		}

		//Second Panel
		else if((row >= 32) && (row <= 39)){
			x = col + 16;
			y = row;
		}
		else if((row >= 40) &&  (row <= 47)){
			x = col;
			y = row - 8;
		}
		else if((row >= 48) && (row <= 55)){
			x = col + 16;
			y = row - 8;
		}
		else if((row >= 56) && (row <= 63)){
			x = col;
			y = row - 16;
		}
	}	

	//Second Panel
	if((col >= 16) && (col <= 31)){
		//First Panel
		if((row >= 0) && (row <= 7)){
			x = col;
			y = row + 16;
		}
		else if((row >= 8) &&  (row <= 15)){
			x = col - 16;
			y = row + 8;
		}
		else if((row >= 16) && (row <= 23)){
			x = col;
			y = row + 8;
		}
		else if((row >= 24) && (row <= 31)){
			x = col - 16;
			y = row;
		}

		//Second Panel
		else if((row >= 32) && (row <= 39)){
			x = col;
			y = row +16;
		}
		else if((row >= 40) &&  (row <= 47)){
			x = col - 16;
			y = row + 8;
		}
		else if((row >= 48) && (row <= 55)){
			x = col;
			y = row + 8;
		}
		else if((row >= 56) && (row <= 63)){
			x = col - 16;
			y = row;
		}
	}

	//Third Panel
	if((col >= 32) && (col <= 47)){
		//First panel		
		if((row >= 0) && (row <= 7)){
			x = col +16;
			y = row + 16;
		}
		else if((row >= 8) &&  (row <= 15)){
			x = col;
			y = row + 8;
		}
		else if((row >= 16) && (row <= 23)){
			x = col + 16;
			y = row + 8;
		}
		else if((row >= 24) && (row <= 31)){
			x = col;
			y = row;
		}	
		//Second Panel	
		else if((row >= 32) && (row <= 39)){
			x = col +16;
			y = row + 16;
		}
		else if((row >= 40) &&  (row <= 47)){
			x = col;
			y = row + 8;
		}
		else if((row >= 48) && (row <= 55)){
			x = col + 16;
			y = row + 8;
		}
		else if((row >= 56) && (row <= 63)){
			x = col;
			y = row;
		}	
	
	}
	
	//Fourth Panel
	if((col >= 48) && (col <= 63)){
		//First Panel
		if((row >= 0) && (row <= 7)){
			x = col;
			y = row;
		}
		else if((row >= 8) &&  (row <= 15)){
			x = col - 16;
			y = row - 8;
		}
		else if((row >= 16) && (row <= 23)){
			x = col;
			y = row - 8;
		}
		else if((row >= 24) && (row <= 31)){
			x = col - 16;
			y = row - 16;
		}	
		
		//Second Panel
		else if((row >= 32) && (row <= 39)){
			x = col;
			y = row;
		}
		else if((row >= 40) &&  (row <= 47)){
			x = col - 16;
			y = row - 8;
		}
		else if((row >= 48) && (row <= 55)){
			x = col;
			y = row - 8;
		}
		else if((row >= 56) && (row <= 63)){
			x = col - 16;
			y = row - 16;
		}	
		
	}	
	
	canvas->SetPixel(x, y, r, g, b);

}










