#include "hangman.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
    char secret[16];

    srand(time(NULL));  
    get_word(secret);
    hangman(secret);
    
    //printf("%d\n", is_word_guessed("secret", "aeiou"));
    // prints: 0
    //printf("%d\n", is_word_guessed("hello", "aeihoul"));
    // prints: 1

    //char result[30];
    //get_guessed_word("container", "arpstxgoieyu", result);
    
    //char result[30];
    //get_available_letters("arpstxgoieyu", result);

    return 0;
}

