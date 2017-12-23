#include <stdio.h>
#define NUMBER 400
void printGrid(int num, int col);
int test_bit( int value, int bit_number, int col);
void clear_bit(unsigned int* value, int bit_number);
void set_bit(unsigned int* value, int bit_number);
int main(int argc,char** argv){
	
	int plays = 0;
	char block[40][80];
	if(argc == 3)
		plays = atoi(argv[2]);
//	printf("%d", plays);
	FILE* file = fopen(argv[1], "r");
	unsigned char array[400];
	if(fread(array, 1, 400, file) != 400){
		printf("%s\n", "Error");
	}
	fclose(file);
	int y ;
	int num = 0;
	int in = 0;
	int row = 0;
        int col = 0;
	int ret;
	int mask = 1;
	char alive = 'o';
	char dead = ' ';

	for(y=399; y >= 0; y--){
		int x ;
		char byte = array[num]; 
		for(x=7; x >= 0; x--){
			if(col == 80){
				row++;
				col = 0;
			//	printf("\n");
			}
			 
//			printf("%d", );
			unsigned int n  = (unsigned int)byte;
			ret = test_bit(n,x,col);
			if(ret == 1){
			//	printf("%s", alive);
				block[row][col] = alive;
			}
			else if(ret == 0){
			//	printf("%s", dead);
				block[row][col] = dead;
			}
			else{
				printf("%s\n", "Error");
			}
		//	printf("%c", block[row][col]);
	
			col++;
				
		}
		num++;
	//	printf("%d", num);
	}
	printf("\n");
	//printf("%s", "Made it to while");
	
	while(plays > 0){
	//	printf("%s %d\n", "Made it in loop",plays);
		char extra[40][80];
		int r ;
		int c ;
		for(r=0; r< 40; r++){
			for(c=0;c<80; c++){
				int count = 0;
				if(r == 0 && c == 0){
					if(block[r][c] == alive){
						if(block[r][c+1] == alive)
							count++;
						if(block[r+1][c] == alive)
							count++;
						if(block[r+1][c+1] == alive)
                                                        count++;
						if(count >=2)
							extra[r][c] = alive;
						else
							extra[r][c] = dead;
					}
					else{
						if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c+1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
					}
				}
				else if(r == 0 && c == 79){
                                        if(block[r][c] == alive){
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c-1] == alive)
                                                        count++;
                                                if(count >=2)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                        else{
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c-1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                }
				else if(r == 39 && c == 0){
                                        if(block[r][c] == alive){
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c+1] == alive)
                                                        count++;
                                                if(count >=2)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                        else{
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c+1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                }
				else if(r == 39 && c == 79){
                                        if(block[r][c] == alive){
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c-1] == alive)
                                                        count++;
                                                if(count >=2)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                        else{
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c-1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                }
				else if(r == 0 && c > 0 && c <79){
                                        if(block[r][c] == alive){
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c+1] == alive)
                                                        count++;
						if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r+1][c-1] == alive)
                                                        count++;
                                                if(count > 1 && count<4)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                        else{
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c+1] == alive)
                                                        count++;
						if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r+1][c-1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                }
				else if(r == 39 && c > 0 && c < 79){
                                        if(block[r][c] == alive){
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c+1] == alive)
                                                        count++;
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r-1][c-1] == alive)
                                                        count++;
                                                if(count > 1 && count<4)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                        else{
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c+1] == alive)
                                                        count++;
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r-1][c-1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                }
				 else if(r >0&&   r<39 && c == 0){
                                        if(block[r][c] == alive){
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c+1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c+1] == alive)
                                                        count++;
                                                if(count > 1 && count<4)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                        else{
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c+1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c+1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                }
					
				else if(r >0&&   r<39 && c == 79){
                                        if(block[r][c] == alive){
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c-1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c-1] == alive)
                                                        count++;
                                                if(count > 1 && count<4)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                        else{
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c-1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c-1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                }
				else{
					if(block[r][c] == alive){
                                                if(block[r-1][c+1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c-1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c-1] == alive)
                                                        count++;
						if(block[r+1][c+1] == alive)
							count++;
						if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(count > 1 && count<4)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
                                        else{
                                                if(block[r-1][c+1] == alive)
                                                        count++;
                                                if(block[r-1][c] == alive)
                                                        count++;
                                                if(block[r-1][c-1] == alive)
                                                        count++;
                                                if(block[r+1][c] == alive)
                                                        count++;
                                                if(block[r+1][c-1] == alive)
                                                        count++;
						if(block[r+1][c+1] == alive)
                                                        count++;
                                                if(block[r][c-1] == alive)
                                                        count++;
                                                if(block[r][c+1] == alive)
                                                        count++;
                                                if(count ==3)
                                                        extra[r][c] = alive;
                                                else
                                                        extra[r][c] = dead;
                                        }
				}
				if(plays == 1){
		//			printf("%s %d\n", "Made it in print",plays);
					printf("%c", extra[r][c]);
					if(c==79)
						printf("\n");
				}
			}
		}
		int i;
		int j;
		for(i = 0; i< 40; i++){
			for(j = 0; j< 80; j++){
				block[i][j] = extra[i][j];
			}
		}
		plays--;	
	}
		 
					
}
int test_bit( int value, int bit_number, int col) {
		
                int mask = 1 << bit_number;
                if (value & mask){
		//	printGrid(1,col);
                        return 1;
		}
                else{
		//	printGrid(0,col);
                        return 0;
		}
 }

void printGrid(int num, int col){
	if(num == 1)
		printf("%s", "o");
	else
		printf("%s", " ");
	if(col == 79)
		printf("\n");
	/*int row = 0;
        int col = 0;

        for(row; row < 40; row++){
                for(col; col< 80; col++){
                       printf("%s", array[row][col]);
                }
                printf("\n");
        }*/
}
void set_bit(unsigned int* value, int bit_number) {
    int mask = (1 << bit_number);
    *value = *value | mask;
}
void clear_bit(unsigned int* value, int bit_number) {
    int mask = ~(1 << bit_number);
    *value = *value & mask;
}
