#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<fcntl.h> 

int main() {

	//compile find_correct_word.c because the program won't work 
	system("gcc find_correct_word.c -o find_correct_word");


	char *words[12];	//all words 
	char array[12][9]; //from the file 

	char cor_words[12][20]; //corrected words
	

	//Opening file 
	FILE *fp=fopen("nonsense_words.txt","r");
	int counter = 0;

	for(int i=0; i<12; i++) {

		words[i]=malloc(128);
		fscanf(fp,"%s",words[i]);
		counter++;
	
	}	
	fclose(fp);

	printf("\nEncrypted message: \n\n");

        for(int k=0; k<12; k++) {
    
			strcpy(array[k],words[k]);  //copy first word
			printf("%s",array[k]);
        }

	printf("\n\nUnencrypted message: \n\n");
	

 //create a temporary file
	FILE *file=fopen("cheat.txt","w");
	fclose(file);


	int fd[2]; // Used to store two ends of pipe 
	 
	//keep count to print words after 
    int num=0;

	pid_t p; 

	if (pipe(fd)==-1) { 
		
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	

    for(int i=0; i<counter; i++){


		p = fork(); 

		if (p < 0) { 

			fprintf(stderr, "fork Failed" ); 
			return 1; 


		}else if (p > 0) {	// Parent process 


			close(fd[0]); // Close reading end of first pipe 

			// Write input string and close writing end of first 
			// pipe. 
			write(fd[1], &i, sizeof(i) ); 
        		close(fd[1]);

			// Wait for child to send a string 
			wait(NULL); 

			if(i+1==counter) {
                
				FILE *in_file;
    			char word[50];

    			in_file = fopen("cheat.txt", "r");

					
   					if(in_file == NULL) 
        				
						printf("Can't open for reading.\n");
    				
					else {
        				while(fscanf(in_file, "%s", word) != EOF) {
							
							strcpy(cor_words[num],word);
							printf("%s ", cor_words[num]);
							num++;
        				}
        			fclose(in_file);
                    remove("cheat.txt"); //remove temp file
    				}				

			}

			
            

		}else { 	// child process

			close(fd[1]); // Close writing end of pipe 

			// Read an int  using first pipe 
			read(fd[0], &i, sizeof(i) ); 


			int file_desc = open("cheat.txt", O_WRONLY | O_APPEND);
      
    		// here the file_desc is the file descriptor of stdout (i.e. 1) 
    		dup2(file_desc, 1);
			
			//words are being saved in new line everytime
			printf("\n");

			//execute find_correct_word.c to return the right word 
			char *args[3]={"./find_correct_word","dictionary.txt", array[i]}; 
			execvp(args[0],args); 
			

			// Close both reading ends 
			close(fd[0]); 
			
			exit(0); 
		} 


    }
	printf("\n\n");
	
} 
