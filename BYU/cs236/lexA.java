//--------------------------------------------------------------------------
// A Java version of the lexical analyzer
//
// *************************************************************************
// This is a modified version of lex.java that has the following changes:
//	No exception handling
//	Strips the quotes off strings
//	The methods are named more like Java standards--ie. nextToken() instead of next_token()
// *************************************************************************

// USEAGE INSTRUCTIONS:
//  There are 2 classes provided here: Token and LexicalAnalyzer
//	The methods of interest in Token are:
//	  String getValue()  -- Returns the value of this token
//	  int getType()      -- Returns one of the 21 types that a Token can be (enumerated in class Token)
//	The methods of interest in LexicalAnalyzer are:
//	  LexicalAnalyzer(String filename) -- Constructor that opens the file and initializes the first token
//	  void nextToken()       -- Advances to the next token (but doesn't return it)
//	  Token currentToken()   -- Returns the current token
//
//   There aren't any other methods from these classes that you
//	should use in your Parser
//--------------------------------------------------------------------------

import java.util.*;
import java.io.*;

class Token{
	//Each Token has two parts: a value (of types string) and a type.
	//The type may have any of the values defined below.  I would have
	//used an enumerated type to define type but Java does not have one so
	//we do it with a bunch of final static ints.


	//The Constructors
	public Token(){
		type = UNDEFINED;
		value = "";
	}
	public Token(int new_type, String new_value){
		type = new_type;
		value = new_value;
	}

	//Access methods
	public String getValue(){return value;}
	public int    getType(){return type;}

	//Update methods
	public void setValue(String new_value){value = new_value;}
	public void setType(int new_type){type = new_type;}

	//Enumeration of possible token types
	public final static int COLON		= 0;
	public final static int COLON_DASH	= 1;
	public final static int COMMA		= 2;
	public final static int LEFT_PAREN	= 3;
	public final static int PERIOD		= 4;
	public final static int Q_MARK		= 5;
	public final static int RIGHT_PAREN	= 6;
	public final static int EQ		= 7;
	public final static int NE		= 8;
	public final static int GT		= 9;
	public final static int GE		= 10;
	public final static int LT		= 11;
	public final static int LE		= 12;
	public final static int STRING		= 13;
	public final static int SCHEMES		= 14;
	public final static int FACTS		= 15;
	public final static int RULES		= 16;
	public final static int QUERIES		= 17;
	public final static int ID		= 18;
	public final static int EOF		= 19;
	public final static int UNDEFINED	= 20;

	public static String[] typeStrings = { //Useful for debugging
		"COLON", "COLON_DASH", "COMMA", "LEFT_PAREN", "PERIOD",
		"Q_MARK", "RIGHT_PAREN", "EQ", "NE", "GT", "GE", "LT", "LE",
		"STRING", "SCHEMES", "FACTS", "RULES", "QUERIES", "ID",
		"EOF", "UNDEFINED" };
  
	public String toString()
	{
	  return value;
	}
	//-----------------------------------------------------------------
	//Hidden Section
	//-----------------------------------------------------------------

	private String value;
	private int    type;

};

class LexicalAnalyzer{
	//Logically this is a pipe that converts a sequence of characters from
	//the named input file into a sequence of tokens.  The lexical analyzer
	//only reads from the input file when it needs to.

	//We implement the lexical analyzer with a one character buffer.  That
	//is, the next character to be read is in a local character,
	//"current_character" and the rest of the characters are in the input
	//file.  Thus, we can check to see if we are at the end of a file by
	//checking to see if the current_character is -1.  We also use this
	//feature to be able to differentiate between one and two character
	//symbols such as ":-" and ":".  The one character buffer basically
	//provides a one character look ahead into the input file.  Thus,
	//after seeing a ":" we can look one character ahead to see if we
	//should return a ":-" or just a ":".  Similar decisions are made for
	//"<=" vs. "<" and ">=" vs. ">".
	
	//The constructor.  The user must provide the name of the input file.
	//It is considered an error if the file cannot be found or opened.
	public LexicalAnalyzer(String file_name)
	{
		try{
			input_file = new FileInputStream(file_name);
			nextChar();	//Gets the first character,  initializes current_character,
					//and sets eof appropriately
			skip_ws();	//We need to skip all of the whitespace at the beginning,
					//if there is any.
			curr_token = new Token();
			nextToken();	//set up the first token (which could be EOF)
		} catch (IOException e){
			System.out.println("Error initializing file");
		}
	}

	public Token currentToken(){return curr_token;}

	public void nextToken() {
		//This sets curr_token the next token from the input.
		//If there is no valid token then current_token is set to the EOF token;
		try{
			if(eof){
				curr_token.setType(Token.EOF);
				curr_token.setValue("");
			} else {
				StringBuffer buf = new StringBuffer();
					//Holds the value of the token being constructed.
				if (character() != '\'')
					buf.append(character());
					//The buffer is originally empty.	This appends the first character
					//to it.
				switch(character()){
					//Basically this switch covers each of the different types of tokens.
					//It looks at the characters one character at a time until it has
					//matched a symbol, string, or identifier.  It then creates a new
					//token from the accumlated characters (stored in "buf") and the
					//known type.

					//It is often considered bad style if we have methods this big.
					// However, for comprehension purposes it is better to have one long
					//switch rather than multiple methods just to satisfy an arbitrary law.
				case ':':
					nextChar();
					if(character() == '-'){
						buf.append(character());
						nextChar();
						curr_token.setType(Token.COLON_DASH);
						curr_token.setValue(buf.toString());
					} else {
						curr_token.setType(Token.COLON);
						curr_token.setValue(buf.toString());
					}
					break;
				case ',':
					nextChar();
					curr_token.setType(Token.COMMA);
					curr_token.setValue(buf.toString());
					break;
				case '(':
					nextChar();
					curr_token.setType(Token.LEFT_PAREN);
					curr_token.setValue(buf.toString());
					break;
				case '.':
					nextChar();
					curr_token.setType(Token.PERIOD);
					curr_token.setValue(buf.toString());
					break;
				case '?':
					nextChar();
					curr_token.setType(Token.Q_MARK);
					curr_token.setValue(buf.toString());
					break;
				case ')':
					nextChar();
					curr_token.setType(Token.RIGHT_PAREN);
					curr_token.setValue(buf.toString());
					break;
				case '=':
					nextChar();
					curr_token.setType(Token.EQ);
					curr_token.setValue(buf.toString());
					break;
				case '<':
					nextChar();
					if(character() == '='){
						buf.append(character());
						nextChar();
						curr_token.setType(Token.LE);
						curr_token.setValue(buf.toString());
					} else {
						curr_token.setType(Token.LT);
						curr_token.setValue(buf.toString());
					}
					break;
				case '>':
					nextChar();
					if(character() == '='){
						buf.append(character());
						nextChar();
						curr_token.setType(Token.GE);
						curr_token.setValue(buf.toString());
					} else {
						curr_token.setType(Token.GT);
						curr_token.setValue(buf.toString());
					}
					break;
				case '!':
					nextChar();
					if(character() == '='){
						buf.append(character());
						nextChar();
						curr_token.setType(Token.NE);
						curr_token.setValue(buf.toString());
					} else {
						System.out.println("Parse Error");
					}
					break;
				case '\'':
					nextChar();
					//we have seen a "'" so accumulate characters until we see another "'"
					while(!eof && character() != '\''){
						buf.append(character());
						nextChar();
					}
					if(eof){
						System.out.println("EOF found early");
						return;
					}
					//buf.append(character());
					nextChar();
					curr_token.setType(Token.STRING);
					curr_token.setValue(buf.toString());
					break;
				default:
					if(Character.isLetter(character())){
						nextChar();
						//Once we see a letter, accumulate letters and digits.
						while(!eof && Character.isLetterOrDigit(character())){
							buf.append(character());
							nextChar();
						}
						curr_token.setType(Token.ID);
						curr_token.setValue(buf.toString());
						check_for_keyword(curr_token);	//if the string is a keyword;
														//change the type approrpiately
					} else {
						System.out.println("Parse Error");
					}
					break;
				}	// end switch
			}	// end if (eof)...else

			skip_ws();	//skip any whitespace after the token

		} catch (IOException e){
			System.out.println("IOException caught in nextToken");
		}
	}	// end public void nextToken()


	//----------------------------------------------------------------------------
	//Hidden Section
	//----------------------------------------------------------------------------

	//Attributes
	private FileInputStream input_file;
	private Token curr_token;
	private boolean eof;
	private int current_character;
	//the current character is an int rather than a char so that eof (-1)
	//can be stored and tested.

	//Methods
	private char character(){
		//converts the current_character to a character and returns it.
		return (char)current_character;
	}

	private void nextChar()throws IOException{
		//Assumes the input file has been opened.
		//Gets the next character from the input file
		current_character = input_file.read();
		eof = current_character < 0;
	}

	private void skip_ws()throws IOException{
		//Skips a sequence of 0 or more whitespace
		//(space, table, newline, carriage_return)
		char ch = (char)current_character;
		while(!eof &&
			(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
			)
		{
			nextChar();
			ch = (char)current_character;
		}
	}

	private void check_for_keyword(Token token){
		//Changes the token type appropriately if the value of the token
		//type is a keyword.  Otherwise it leaves the type the way it was.
		if(token.getValue().compareTo("Schemes") == 0){
			token.setType(Token.SCHEMES);
		} else if(token.getValue().compareTo("Facts") == 0) {
			token.setType(Token.FACTS);
		} else if(token.getValue().compareTo("Rules") == 0) {
			token.setType(Token.RULES);
		} else if(token.getValue().compareTo("Queries") == 0) {
			token.setType(Token.QUERIES);
		}
	}
}
