/*******************************************************************
				  C Calculator Program Skeleton File
							  Version 2.0
*******************************************************************/

#include <stdlib.h> 
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/* ---------------------------------------------------------------------
 * Turns on verbose debug output.  Example usage:
 *
 * #ifdef DEBUG
 *   printf( "debug stuff" );
 * #endif
 *
 * If DEBUG is 1 then the "debug stuff" is printed after the program
 * is recompiled.
 * ---------------------------------------------------------------------
 */
#define DEBUG		  0

/* ---------------------------------------------------------------------
 * TRUE and FALSE for readability
 * ---------------------------------------------------------------------
 */
#define TRUE			  1
#define FALSE			 0


/* ---------------------------------------------------------------------
 * Output strings
 * ---------------------------------------------------------------------
 */
#define WELCOME_MSG	  "LBIC (Low Budget Integer Calculator)\n"
#define QUIT_MSG		 "END OF LINE\n"
#define BAD_INPUT_MSG	"ERROR: not a valid number format or command\n"
#define OVERFLOW_MSG	 "ERROR: stack overflow\n"
#define UNDERFLOW_MSG	"ERROR: stack underflow\n"
#define EMPTY_STACK_MSG  "ERROR: stack empty\n"
#define NUM_OVERFLOW_MSG "ERROR: numerical overflow\n"
#define TOO_FEW_ARGS_MSG "ERROR: too few arguments\n"
#define TODO_MSG		 "TODO: implement this command\n"
#define PROMPT		   "> "


/* ---------------------------------------------------------------------
 * Magic number and constants
 * ---------------------------------------------------------------------
 */
#define DEL			   0x7f  /* ASCII value of DEL character	   */
#define SPACE			 ' ' 
#define MAX_STACK_HEIGHT  100
#define MAX_INPUT_STRING  50
#define EMPTY			 -1


/* ---------------------------------------------------------------------
 * Arithmetic operations
 * ---------------------------------------------------------------------
 */
#define ADD			   '+'
#define SUB			   '-'
#define MULT			  '*'
#define DIV			   '/'
#define MOD		  '%'
#define AND		  '&'
#define OR		  '|'
#define XOR		  '^'
#define NOT		  '~'
#define FACTORIAL		 '!'

/* Matching command functions */
void add_command();
void sub_command();
void mult_command();
void div_command();
void mod_command();
void and_command();
void or_command();
void xor_command();
void not_command();
void factorial_command();

/* Support functions		  */
int factorial(int i);


/* ---------------------------------------------------------------------
 * Output operations
 * ---------------------------------------------------------------------
 */
#define TOGGLE_HEX	  'x'  /* Switches between dec and hex output */
#define TOGGLE_BUG	  'd'  /* Turns on and off debug output	   */
#define PRINT_STACK	   's'  
#define PRINT_TOP		 't'  

/* Matching command functions */
void toggle_hex();
void toggle_bug();
void print_stack();
void print_top();

/* Support functions		  */
void debug_string(int, int, char, int, int);

/* ---------------------------------------------------------------------
 * Stack operations
 * ---------------------------------------------------------------------
 */
#define CLEAR			 'c'  /* Clears all entries in the stack	 */
#define EXCHANGE	  'e'  /* Swaps the top 2 stack entries	   */
#define POP			   'p'

/* Matching command functions */
void clear_command();
void exchange_command();
int pop();

/* Support functions		  */
void push( int a );
int twofew();


/* ---------------------------------------------------------------------
 * Misc operations
 * ---------------------------------------------------------------------
 */
#define HELP			  'h'
#define QUIT			  'q'

/* Matching command functions */
void help_command();


/* ---------------------------------------------------------------------
 * Global variables
 *
 * static: this restricts the scope of the global variables to this
 * file only.
 *
 * ---------------------------------------------------------------------
 */
static int g_stack[MAX_STACK_HEIGHT]; /* Global stack for the LBIC	*/
static int g_top = EMPTY;			 /* points to top of stack	   */
static int g_debug = 0;
static int g_hex_output = 0;


/* ---------------------------------------------------------------------
 * Misc functions
 * ---------------------------------------------------------------------
 */
int is_valid_number( char buffer[] );
void execute_commands();




/**********************************************************************/
/*					   FUNCTION DEFINITIONS						 */
/**********************************************************************/   

/* 
 * main function
 * 
 */
int main ( int argc, char *argv[] ) {
  printf( WELCOME_MSG );
  execute_commands();
  printf( QUIT_MSG );
  return 0;
}


/* ---------------------------------------------------------------------
 * Arithmetic operations
 * ---------------------------------------------------------------------
 */

/* 
 * void add_command()
 *
 * Pops 2 entires from g_stack, adds the first to the second, and pushes 
 * the result.
 *
 * If there are not 2 entries on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void add_command() {
	if(twofew()) return;
	int a = pop();
	int b = pop();
	int c = a + b;
	push(c);
	debug_string(a,b,'+',c,1);
}


/* 
 * void sub_command()
 *
 * Pops 2 entires from g_stack, subtracts the first from the second, 
 * and pushes the result.
 *
 * If there are not 2 entries on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void sub_command() {
  if(twofew()) return;
	int a = pop();
	int b = pop();
	int c = b-a;
	push(c);
	debug_string(b,a,'-',c,1);
}


/* 
 * void mult_command()
 *
 * Pops 2 entires from g_stack, multiplies the second by the first, 
 * and pushes the result.
 *
 * If there are not 2 entries on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void mult_command() {
  if(twofew()) return;
	int a = pop();
	int b = pop();
	int c = a*b;
	push(c);
	debug_string(a,b,'*',c,1);
}


/* 
 * void div_command()
 *
 * Pops 2 entires from g_stack, divides the second by the first, 
 * and pushes the result.
 *
 * If there are not 2 entries on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void div_command() {
  if(twofew()) return;
	int a = pop();
	int b = pop();
	int c = b/a;
	push(c);
	debug_string(b,a,'/',c,1);
}


/* 
 * void mod_command()
 *
 * Pops 2 entires from g_stack, mods the second by the first, 
 * and pushes the result.
 *
 * If there are not 2 entries on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void mod_command() {
  if(twofew()) return;
	int a = pop();
	int b = pop();
	int c = b%a;
	push(c);
	debug_string(b,a,'%',c,1);
}


/* 
 * void and_command()
 *
 * Pops 2 entires from g_stack, computes their bitwise AND, 
 * and pushes the result.
 *
 * If there are not 2 entries on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void and_command() {
 	if(twofew()) return;
	int a = pop();
	int b = pop();
	int c = a&b;
	push(c);
	debug_string(a,b,'&',c,1);
}


/* 
 * void or_command()
 *
 * Pops 2 entires from g_stack, computes their bitwise OR, 
 * and pushes the result.
 *
 * If there are not 2 entries on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void or_command() {
  if(twofew()) return;
	int a = pop();
	int b = pop();
	int c = a|b;
	push(c);
	debug_string(a,b,'|',c,1);
}


/* 
 * void xor_command()
 *
 * Pops 2 entires from g_stack, computes their bitwise XOR, 
 * and pushes the result.
 *
 * If there are not 2 entries on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void xor_command() {
  if(twofew()) return;
	int a = pop();
	int b = pop();
	int c = a^b;
	push(c);
	debug_string(a,b,'^',c,1);
}


/* 
 * void not_command()
 *
 * Pops 1 entry from g_stack, complements its bits (NOT), 
 * and pushes the result.
 *
 * If there is not 1 entry on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void not_command() {
  if(g_top < 0) {
		printf(TOO_FEW_ARGS_MSG);
		return;
	}
	int a = pop();
	int b = ~a;
	push(b);
	debug_string(a,b,'~',0,0);
}


/* 
 * void factorial_command()
 *
 * Pops 1 entry from g_stack, computes its factorial, and pushes
 * the result; outputs NUM_OVERFLOW_MSG if the factorial overflows
 * its number representation.
 *
 * If there is not 1 entry on the STACK, reports TOO_FEW_ARGS_MSG, and
 * returns leaving g_stack and g_top unchanged.
 * 
 */
void factorial_command() {
  if(g_top < 0) {
		printf(TOO_FEW_ARGS_MSG);
		return;
	}
	int a = pop();
	int b = factorial(a);
	if(a < 0) {
		printf(NUM_OVERFLOW_MSG);
		return;
	}
	push(b);
	debug_string(a,b,'!',0,0);
}


/* 
 * int factorial( int i )
 *
 * Computes the factorial of i using recursion.  Returns -1 if i is
 * negative or the result overflows its number representation.
 * 
 */
int factorial(int i) {
  if(i < 0) return( -1 );
	if(i == 1) return 1;
	int ret = i*factorial(i-1);
	return (ret >= 0)?ret:-1;
}



/* ---------------------------------------------------------------------
 * Output operations
 * ---------------------------------------------------------------------
 */

/* 
 * void toggle_hex()
 *
 * Toggles the g_hex_output global variable; a value of 1 causes LBIC
 * to output everything in hexadecimal; a value of 0 causes LBIC to 
 * output everything in decimal.
 * 
 */
void toggle_hex() {
  g_hex_output = (g_hex_output)?0:1;
}


/* 
 * void toggle_bug()
 *
 * Toggles the g_debug global variable; a value of 1 switches LBIC to
 * verbose mode to output extra debug information.
 *
 * This is a nice alternative to the DEBUG preprocessor directive
 * because it does not require recompilation to see the output.
 * 
 */
void toggle_bug() {
  g_debug = (g_debug)?0:1;
}

/*
 * void debug_string(int, int, char, int, int)
 *
 * Shows debugging information
 */
void debug_string(int a, int b, char o, int c, int three) {
	if(three != 0)
		if(g_hex_output)
			(g_debug)?printf("\t%x %c %x = %x\n", a, o, b, c),print_stack():0;
		else
			(g_debug)?printf("\t%d %c %d = %d\n", a, o, b, c),print_stack():0;
	else
		if(g_hex_output)
			(g_debug)?printf("\t%c%x = %x\n", o, a, b),print_stack():0;
		else
			(g_debug)?printf("\t%c%d = %d\n", o, a, b),print_stack():0;
}


/* 
 * void print_stack()
 *
 * Prints the current contents of g_stack on 1 line separated by tabs 
 * in either hexadecimal or decimal depending on the value of 
 * g_hex_output.  The last stack entry must be followed by a '\n' 
 * character.
 *
 * If g_stack is empty, then it outputs EMPTY_STACK_MSG
 * 
 */
void print_stack() {
  if(g_top == EMPTY) {
		printf(EMPTY_STACK_MSG);
		return;
	}
	int i;
	for(i = g_top; i > EMPTY; i--) {
		if(g_hex_output)
			printf("\t%x",g_stack[i]);
		else
			printf("\t%d",g_stack[i]);
	}
	printf("\n");
}


/* 
 * void print_top()
 *
 * Prints the top entry in g_stack in either decimal or hexadecimal 
 * format depending on g_hex_output followed by a '\n' character.
 *
 * If g_stack is empty, then it outputs EMPTY_STACK_MSG
 * 
 */
void print_top() {
	if(g_top == EMPTY) {
		printf(EMPTY_STACK_MSG);
		return;
	}
	if(g_hex_output)
		printf("\t%x\n",g_stack[g_top]);
	else
		printf("\t%d\n",g_stack[g_top]);
}

/* ---------------------------------------------------------------------
 * Stack operations
 * ---------------------------------------------------------------------
 */

/* 
 * void clear_command()
 *
 * Reset g_top to EMPTY effectively deleting all stack entries.
 * 
 */
void clear_command() {
	g_top = EMPTY;
}

/* 
 * void exchange_command()
 *
 * Pops 2 entries from g_stack and pushes them back in
 * reverse order.  For example:
 *
 * LBIC (Low Budget Integer Calculator)
 * > 10
 * > 20
 * > s
 *	   20	  10
 * > e
 * > s
 *	   10	  20
 *
 * If there are not 2 entries on the STACK, report TOO_FEW_ARGS_MSG, and
 * return leaving g_stack and g_top unchanged.
 * 
 */
void exchange_command() {
	if(twofew()) return;
	int a = pop();
	int b = pop();
	push(a);
	push(b);
	return;
}


/* 
 * int pop()
 *
 * Returns the top entry from g_stack and updates g_top to remove
 * it; outputs UNDERFLOW_MSG if the pop fails.
 * 
 */
int pop() {
	if(g_top != EMPTY)
		return g_stack[g_top--];
	printf(UNDERFLOW_MSG);
  return( 0 );
}


/* 
 * void push( int a )
 *
 * Pushes variable a on g_stack and updates g_top; outputs OVERFLOW_MSG
 * if the push fails.
 * 
 */
void push( int a ) {
  if ( g_top == MAX_STACK_HEIGHT - 1 ) {
	printf( OVERFLOW_MSG );
	return;
  }
  g_stack[++g_top] = a;
}

/*
 * int twofew()
 * 
 * Returns a 1 if there are less than 2 elements in the stack
 */
int twofew() {
	if(g_top < 1) {
		printf(TOO_FEW_ARGS_MSG);
		return 1;
	}
	return 0;
}


/* ---------------------------------------------------------------------
 * Misc operations
 * ---------------------------------------------------------------------
 */

/* 
 * void help_command()
 *
 * Outputs the command summary for the LBIC
 * 
 */
void help_command() {
  printf( "Arithmetic Operations:\n"
		  "%c = ADD%7c = SUB%7c = MULT%7c = DIV\n"
	  "%c = MOD%7c = AND%7c = OR%8c = XOR\n"
		  "%c = NOT%7c = FACTORIAL\n",
	  ADD, SUB, MULT, DIV,
	  MOD, AND, OR, XOR,
	  NOT, FACTORIAL );

  printf( "\nOutput Operations:\n"	
	  "%c = toggle hex/dec output%7c = toggle debug output\n"
	  "%c = print stack%17c = print top\n",
	  TOGGLE_HEX, TOGGLE_BUG,
	  PRINT_STACK, PRINT_TOP );

  printf( "\nStack Operations:\n"
	  "%c = clear%7c = exchange%7c = pop\n",
	  CLEAR, EXCHANGE, POP );

  printf( "\nOther Operations:\n"
	  "%c = quit\n",
	  QUIT );
}



/* ---------------------------------------------------------------------
 * Misc functions
 * ---------------------------------------------------------------------
 */


/*
 * int is_valid_number( char buffer[] )
 *
 * Returns TRUE if buffer contains only digits 0-9 and possibly
 * a leading '-' character to indicate a negative number, 
 * otherwise it returns false. 
 *
 * NOTE: leading white space (\t,\n,' ') should be ignored.
 *
 */
int is_valid_number( char buffer[] ) {
  int is_subtract;
  int i = 0;

  /* Ignore white space					   */
  while ( isspace( buffer[i] ) ) ++i;
  
  /* Check leading character				  */
  is_subtract = ( buffer[i] == '-' ) ? TRUE : FALSE;
  if ( !isdigit( buffer[i] ) && !is_subtract )
	return( FALSE );

  /* Check for only digits everything else */
  while ( buffer[++i] != '\0' ) {
	is_subtract = FALSE;
	if ( !isdigit( buffer[i] ) )
	  return( FALSE );
  }

  return( is_subtract == FALSE );
}


/*
 * void execute_commands()
 *
 * Processes commands until the QUIT command is entered
 *
 */
void execute_commands() {
  char buffer[MAX_INPUT_STRING];
  int num_chars = 0;

  while ( TRUE ) {
	
	printf( PROMPT );

	scanf("%s", buffer);
	num_chars = strlen(buffer);
	if ( is_valid_number( buffer ) ) {
	  push( atoi(buffer) );
	  continue;
	}

	if ( num_chars > 1 ) {
	  printf( BAD_INPUT_MSG );
	  continue;
	}

	if ( tolower( buffer[0] ) == QUIT ) return;

	/* Remove the TODO_MSG as you implement and test commands */
	switch( tolower( buffer[0] ) ){
	case ADD:
	  add_command();
	  break;
	case SUB:
	  sub_command();
	  break;
	case MULT:
	  mult_command();
	  break;
	case DIV:
	  div_command();
	  break;
	case MOD:
	  mod_command();
	  break;
	case AND:
	  and_command();
	  break;
	case OR:
	  or_command();
	  break;
	case XOR:
	  xor_command();
	  break;
	case NOT:
	  not_command();
	  break;
	case FACTORIAL:
	  factorial_command();
	  break;
	case TOGGLE_HEX:
	  toggle_hex();
	  break;
	case TOGGLE_BUG:
	  toggle_bug();
	  break;
	case PRINT_STACK:
	  print_stack();
	  break;
	case PRINT_TOP:
	  print_top();
	  break;
	case CLEAR:
	  clear_command();
	  break;
	case EXCHANGE:
	  exchange_command();
	  break;
	case POP:
	  pop();
	  break;
	case HELP:
	  help_command();
	  break;
	default:
	  printf( BAD_INPUT_MSG );
	}
  }
}

