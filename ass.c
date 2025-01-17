#include <stdio.h>
#include <string.h>
#include <ctype.h>
/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
int iserror=0;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
void getNonBlankEnt();
int lex();
void term();
void expr();
void factor();
void error();
void stmt();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define ENTER 101


/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26



/******************************************************/
/* main driver */
	int main() {
	/* Open the input data file and process its contents */
	in_fp = fopen("front.txt", "r");
	if (in_fp == NULL)
		printf("ERROR - cannot open front.in \n");
		else {
			getChar();
			do{
			getNonBlank();
			// printf("here\n");
			lex();
			// printf("%d\n", nextToken);
			// printf("%d\n", charClass);
			do {
			if (nextToken == -1 && charClass == 101){
				break;
			}
				iserror = 0;
				stmt();
				// printf("%d\n", nextToken);
				// printf("%d\n", charClass);
				// printf("prev\n");
				if (iserror != 0){
					break;
				}

			} while (nextToken != EOF);
			getChar();
			// printf("%d\n", charClass);
				// printf("now\n");
			// if (charClass != EOF){
			// 	// printf("%d\n", charClass);
			// 	while(charClass == ENTER){
			// 		// printf("here\n");
			// 		getChar();
			// 	}
			// 	// printf("%d\n", charClass);
			// 	// printf("after\n");
			// }
			
			// printf("why\n");
			// printf("%d\n", nextToken);
			// printf("%d\n", charClass );
			if (nextToken == EOF && charClass == -1){
				break;
			}
			if (iserror != 0 && charClass == -1){
				break;
			}
			} while (1==1);
		}
		return 0;
	}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses
 and return the token */
int lookup(char ch) {
 switch (ch) {
 case '=':
 addChar();
 nextToken = ASSIGN_OP;
 break;
 case '(':
 addChar();
 nextToken = LEFT_PAREN;
 break;
 case ')':
 addChar();
 nextToken = RIGHT_PAREN;
 break;
 case '+':
 addChar();
 nextToken = ADD_OP;
 break;
 case '-':
 addChar();
 nextToken = SUB_OP;
 break;
 case '*':
 addChar();
 nextToken = MULT_OP;
 break;
 case '/':
 addChar();
 nextToken = DIV_OP;
 break;
 default:
 addChar();
 nextToken = EOF;
 break;
 }
 return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
 if (lexLen <= 98) {
 lexeme[lexLen++] = nextChar;
 lexeme[lexLen] = 0;
 }
 else
 printf("Error - lexeme is too long \n");
}

/*****************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar() {
 if ((nextChar = getc(in_fp)) != EOF) {
 if (isalpha(nextChar)){
 charClass = LETTER;
	}
 else if (isdigit(nextChar))
 {
 	charClass = DIGIT;
 }
 else if (nextChar == '\n')
 {
 	charClass = ENTER;
 }
 else {
 	charClass = UNKNOWN;
 }
 }
 else{
 charClass = EOF;
 }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank() {
 while (isspace(nextChar)&& nextChar != '\n')
 getChar();
}

void getNonBlankEnt() {
	getNonBlank();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
 expressions */
int lex() {
 lexLen = 0;
 getNonBlank();
 switch (charClass) {
 /* Parse identifiers */
 case LETTER:
 addChar();
 getChar();
 while (charClass == LETTER || charClass == DIGIT) {
 addChar();
 getChar();
 }
 nextToken = IDENT;
 break;
/* Parse integer literals */
 case DIGIT:
 addChar();
 getChar();
 while (charClass == DIGIT) {
 addChar();
 getChar();
 }
 nextToken = INT_LIT;
 break;
/* Parentheses and operators */
 case UNKNOWN:
 lookup(nextChar);
 getChar();
 break;

/* ENTER */
 case ENTER:
 nextToken = EOF;
 lexeme[0] = 'E';
 lexeme[1] = 'O';
 lexeme[2] = 'F';
 lexeme[3] = 0;
 break;

/* EOF */
 case EOF:
 nextToken = EOF;
 lexeme[0] = 'E';
 lexeme[1] = 'O';
 lexeme[2] = 'F';
 lexeme[3] = 0;
 break;
 } 

 /* End of switch */
 // printf("%s\n", lexeme );
 // printf("%d\n", nextToken );
 // printf("%d\n",charClass );
 printf("Next token is: %d, Next lexeme is %s\n",
 nextToken, lexeme);
 return nextToken;
} 
/* End of function lex */

/* expr
 Parses strings in the language generated by the rule:
 <expr> -> <term> {(+ | -) <term>}
 */
void expr() {
 printf("Enter <expr>\n");
/* Parse the first term */
 term();
/* As long as the next token is + or -, get
 the next token and parse the next term */
 while (nextToken == ADD_OP || nextToken == SUB_OP) {
 lex();
 term();
 }
 if (iserror != 0){
 	return;
	}
 printf("Exit <expr>\n");
} /* End of function expr */

/* term
 Parses strings in the language generated by the rule:
 <term> -> <factor> {(* | /) <factor>)
 */
void term() {
 printf("Enter <term>\n");
/* Parse the first factor */
 factor();
/* As long as the next token is * or /, get the
 next token and parse the next factor */
 while (nextToken == MULT_OP || nextToken == DIV_OP) {
 lex();
 factor();
 }
 if (iserror != 0){
 	return;
	}
 printf("Exit <term>\n");
} /* End of function term */

/* factor
 Parses strings in the language generated by the rule:
 <factor> -> id | int_constant | ( <expr )
 */
void factor() {
 printf("Enter <factor>\n");
/* Determine which RHS */
 if (nextToken == IDENT || nextToken == INT_LIT)
/* Get the next token */
 lex();
/* If the RHS is ( <expr>), call lex to pass over the
 left parenthesis, call expr, and check for the right
 parenthesis */
 else {
 if (nextToken == LEFT_PAREN) {
 lex();
 expr();
 if (nextToken == RIGHT_PAREN)
 lex();
 else
 error();
 } /* End of if (nextToken == ... */
/* It was not an id, an integer literal, or a left
 parenthesis */
 else
 error();
 } /* End of else */
 if (iserror != 0){
 	return;
	}
 printf("Exit <factor>\n");;
} /* End of function factor */

void error(){
	printf("Error\n");
	iserror = 1;
}

// stmt
// Parses string in language generated by the rule:
// <stmt> -> id = <expr>

void stmt(){
	printf("Enter <stmt>\n");

	// printf("%d\n",charClass );
	// printf("%d\n",nextToken );
	if (nextToken != 11){
		error();
		while(charClass != 101){
			// printf("testing\n");
			getChar();
			// printf("%d\n", charClass);
			if (charClass == -1){
				break;
			}
		}
		return;
	}
	getNonBlank();
	// printf("%d\n",nextToken );
	// printf("%d\n",charClass );
	lex();
	// printf("%d\n",nextToken );
	// printf("%d\n",charClass );
	if (charClass == -1 || nextToken != 20){
		error();
		while(charClass != 101){
			// printf("testing\n");
			getChar();
			// printf("%d\n", charClass);
			if (charClass == -1){
				break;
			}
		}
		return;
	}
	getNonBlank();
	lex();
	expr();
	if (iserror != 0){
		while(charClass != 101){
			getChar();
			// printf("%d\n", charClass);
			if (charClass == -1){
				break;
			}
		}
 	return;
	}
	// printf("%s\n",lexeme );
	// printf("%d\n",nextToken );
	// printf("%d\n",charClass );
	if (nextToken != -1 || (nextToken == -1 && charClass != -1 && charClass != 101 ) || (nextToken == -1 && strcmp(lexeme, "EOF"))){
		error();
		while(charClass != 101){
			getChar();
			// printf("%d\n", charClass);
			if (charClass == -1){
				break;
			}
		}
	}
	// printf("inside stmt\n");
	// printf("%d\n",charClass );
	// printf("%d\n", nextToken);
	// printf("%d\n", iserror);
	if (iserror != 0){
 	return;
	}
	printf("Exit <stmt>\n");
}