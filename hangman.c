#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "hangman.h"

void get_guessed_word(const char [], const char [], char []);
void translate(const char [], const char [], const char [], char []); // letters_guessed, znak _, spravne slovo-secret,translated
int find_position(const char letters_guessed[], const char letter);

int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[]){
    size_t length_s = strlen(secret);
    size_t length_g = strlen(letters_guessed);
    int tmp = 0;
    int iwg = 0;
    for (int i = 0; i < length_s; i++){
        for (int j = 0; j < length_g; j++){
            if (secret[i] == letters_guessed[j]){
                tmp = tmp + 1;
            }
        }
    }
    if (tmp == length_s){
        iwg = 1;
    }   
    return iwg;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]){
    size_t length_s = strlen(secret);           
    char znak[length_s];                        
    for (int i = 0; i<length_s; i++){           
        znak[i] = '_' ;
    }                           
    translate(letters_guessed, znak, secret, guessed_word);           
}

void translate(const char letters_guessed[], const char znak[], const char secret[], char translated[]){
    int len = (int) strlen(secret);      
    for (int i = 0; i < len; i++){                                    
        int position = find_position(letters_guessed, secret[i]);       
        translated[i] = (position != -1) ? znak[position] : secret[i];  
    }
    translated[len] = '\0';                                             
}

int find_position(const char letters_guessed[], const char letter) {    
    int len = (int) strlen(letters_guessed);                            
    for (int i = 0; i < len; i++) {                          
        if (letter == letters_guessed[i]) {                             
            return -1;
        }
    }
    return 0;                                                           
} 

void get_available_letters(const char letters_guessed[], char available_letters[]){
    int len = strlen(letters_guessed);                      
    char znak = 'a';
    int j = 0;
    int k= 0;
    int i = 0;
	while(znak <= 'z'){
		while(j < len){
			if(letters_guessed[j] == znak){
			    k = 1;
				break;
			}
			j++;
		}
		if(k == 0){
			available_letters[i] = znak ;
			i++;
		}
        k = 0;
        znak++;
        j = 0;
	}    
    available_letters[i]='\0';
    printf("%s\n", available_letters);
}

void hangman(const char secret[]){
    int len = strlen(secret);
    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %d letters long.", len);
    printf("\n-------------\n");

    int pokusy = 8;
    char letters_guessed[30]= "";
    char available_letters[30] = "";
    int m = 0;
    char v;
    char kontrola[len];
    int cv = 0;
    int first = 0;
    
    while ((is_word_guessed(secret, letters_guessed) == 0 && pokusy > 0) || (is_word_guessed(secret, letters_guessed) == 0 && first == 0 && pokusy > 0)){
        printf("You have %d guesses left.\n", pokusy);      
        printf("Available letters: ");
        get_available_letters(letters_guessed, available_letters);
        printf("Please guess a letter: ");
        scanf("%s", kontrola);
        int len2 = strlen(kontrola);
        
        if (len2 > 1){                                      //pokus o tipnutie slova
            for (int mn = 0; mn <= len; mn++){
                if (kontrola[mn] == secret[mn]){
                    cv++;
                }
                if (cv == len+1){
                    first = 1;
                }
                if (cv != len+1){
                    first = 2;
                }
            }   
        }
        
        if (first == 1 || first == 2){                      //break while ak bol pokus o uhadnutie celeho slova
            break;
        }

        v = kontrola[0];                        //zapis prveho pismena do v        
        while (getchar() != '\n')               //odstranit enter
            ;        
        v = tolower(v);                         //zmena velkosti pismena na male

        if (!strchr (available_letters, v)) {              //ak nie je v medzi available pismenami  
            if (v < 'a' || v > 'z'){                        //ak tip nie je pismeno
                printf("Oops! '%c' is not a valid letter: ",v);
                char guessed_word[30];
                get_guessed_word(secret, letters_guessed, guessed_word);
                printf("%c", guessed_word[0]);
                for(int m = 1; m < len; m++){
                    printf(" %c", guessed_word[m]);
            }}
            
            else {                          //ak pismeno uz bolo pouzite
                printf("Oops! You've already guessed that letter: ");
                char guessed_word[30];
                get_guessed_word(secret, letters_guessed, guessed_word);
                printf("%c", guessed_word[0]);
                for(int m = 1; m < len; m++){
                    printf(" %c", guessed_word[m]);
            }}
            printf("\n-------------\n");
            continue;
        }
        letters_guessed[m++] = v;           //pridanie pismena medzi pouzite

        if (strchr(secret, v)){             //ak je znak z v vo secret                    
            printf("Good guess: ");             //dobry pokus
            char guessed_word[30];
            get_guessed_word(secret, letters_guessed, guessed_word);
            printf("%c", guessed_word[0]);
            for(int m = 1; m < len; m++){
                printf(" %c", guessed_word[m]);
            }
            printf("\n-------------\n");
        }        
        else {                  
            printf("Oops! That letter is not in my word: ");            //zly pokus
            char guessed_word[30];
            get_guessed_word(secret, letters_guessed, guessed_word);
            printf("%c", guessed_word[0]);
            for(int m = 1; m < len; m++){
                printf(" %c", guessed_word[m]);
            }
            pokusy--;
            printf("\n-------------\n");
       }     

    }

    if (is_word_guessed(secret, letters_guessed) == 0 && pokusy == 0){          //neuhadnute slovo, dosli pokusy
        printf("Sorry, you ran out of guesses. The word was %s.\n", secret);
    } 
    if ((is_word_guessed(secret, letters_guessed) == 1) || first == 1){     //uhadnute slovo, uhadnute na prvy pokus
        printf("Congratulations, you won!\n");
    }
    if (first == 2){                    //zly pokus o uhadnutie celeho slova 
        printf("Sorry, bad guess. The word was %s.\n",secret);
    }
}
