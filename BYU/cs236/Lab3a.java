// Lab3.java by Ged Crimsonclaw
// I should desciribe it here ;P

//Do a lookahead on all methods w/more than one option!

import java.io.*;

class Lab3a
{
  public static void main(String args[]) throws IOException, FileNotFoundException
	{
		String infilename = null;
		
		try
		{
			infilename = args[0];
		}
		catch(Exception e){}
		
		if(infilename == null)
		{
			System.out.print("Please enter the name of the file to read from: ");
			BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
			infilename = stdin.readLine();
		}
		
	  System.out.println((new yesno(infilename).DatalogProgram())?"Success.":"Failure.");
	}
}

class yesno
{
  LexicalAnalyzer lexA = null;
	
	yesno(String infilename)
	{
	  lexA = new LexicalAnalyzer(infilename);
	}
	
	boolean DatalogProgram()
	{
	  //System.out.println("DatalogProgram");
		if(lexA.currentToken().getType() == Token.SCHEMES)//Schemes
		{
		  lexA.nextToken();
			if(lexA.currentToken().getType() == Token.COLON)//:
			{
        lexA.nextToken();
				if(!SchemeList())
				  return false;
			}
			else
		    return false;
		}//end Schemes
		else
		  return false;
		
		if(lexA.currentToken().getType() == Token.FACTS)//Facts
		{
		  lexA.nextToken();
			if(lexA.currentToken().getType() == Token.COLON)//:
			{
        lexA.nextToken();
				if(!FactList())
				  return false;
			}
			else
		    return false;
		}//end Facts
		else
		  return false;
		
		if(lexA.currentToken().getType() == Token.RULES)//Rules
		{
		  lexA.nextToken();
			if(lexA.currentToken().getType() == Token.COLON)//:
			{
        lexA.nextToken();
				if(!RuleList())
				  return false;
			}
			else
		    return false;
		}//end Rules
		else
		  return false;
		
		if(lexA.currentToken().getType() == Token.QUERIES)//Queries
		{
		  lexA.nextToken();
			if(lexA.currentToken().getType() == Token.COLON)//:
			{
        lexA.nextToken();
				if(!QueryList())
				  return false;
			}
			else
			  return false;
		}//end Queries
		else
		  return false;
		
		if(lexA.currentToken().getType() == Token.EOF)//EOF
		  return true;
		else
		  return false;
	}
	//Note: The Schemes, Facts, Rules, and Queries sections above 
	//are all required in the order shown.
	
	boolean SchemeList()
	{
	  //System.out.println("SchemeList");
		if(Scheme()&&SchemeListTail())
		  return true;
		return false;
	}
	
	boolean SchemeListTail()
 	{
		//System.out.println("SchemeListTail");
		if(SchemeList())
		  return true;
		else //null is possible
		  return true;
	}
	
	boolean Scheme() //fix?
	{
	  //System.out.println("Scheme");
	  LexicalAnalyzer lexa = lexA;
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  lexA.nextToken();
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
			{
			  lexA.nextToken();
				if(AttributeList())
				{
				  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
					{
					  lexA.nextToken();
						return true;
					}
				}
			}
		}
		lexA = lexa;
		return false; //else all
	}
	
	boolean AttributeList()
	{
	  //System.out.println("AttributeList");
		if(Attribute()&&AttributeListTail())
		  return true;
		return false;
	}
	
	boolean AttributeListTail()//fix?
	{
	  //System.out.println("AttributeListTail");
	  LexicalAnalyzer lexa = lexA;
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			if(AttributeList())
			  return true;
		}
		lexA = lexa;
		return true; //null is the case
	}
	
	boolean Attribute()
	{
	  //System.out.println("Attribute");
		if(lexA.currentToken().getType() == Token.ID)//Identifier()
		{
		  lexA.nextToken();
		  return true;
		}
		return false;
	}
	
	boolean FactList()
	{
	  //System.out.println("FactList");
		if(Fact()&&FactListTail())
		  return true;
		return false;
	}
	
	boolean FactListTail()
 	{
		//System.out.println("FactListTail");
		if(FactList())
		  return true;
		else if(temp)//null is case
		  return true;
		else
		{
		  temp = true;
			return false;
		}
	}
	
	boolean temp = true;
		
	boolean Fact() //fix?
 	{
		//System.out.println("Fact");
		LexicalAnalyzer lexa = lexA;
		//System.out.println("Fact: " + lexA.currentToken());
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{		  
		  lexA.nextToken();
			//System.out.println("Fact: " + lexA.currentToken());
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
			{
			  lexA.nextToken();
				//System.out.println("Fact: " + lexA.currentToken());
				if(ConstantList())
				{
				  //System.out.println("Fact: " + lexA.currentToken());
				  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
					{
					  lexA.nextToken();
						//System.out.println("Fact: " + lexA.currentToken());
						if(lexA.currentToken().getType() == Token.PERIOD)//.
						{
						  lexA.nextToken();
							//System.out.println("Fact: " + lexA.currentToken());
							return true;
						}
						else
						{
						  temp = false;
							return false;
						}
					}
				}
			}
		}
		lexA = lexa;
		return false; //else all
	}
	
	boolean ConstantList() //fix?
	{
	  //System.out.println("ConstantList");
	  LexicalAnalyzer lexa = lexA;
		//System.out.println("ConstantList: " + lexA.currentToken());
		if(lexA.currentToken().getType() == Token.STRING)//String
		{
		  lexA.nextToken();
			//System.out.println("ConstantList: " + lexA.currentToken());
			if(ConstantListTail())
			{
			  //lexA.nextToken(); // ^^^
				//System.out.println("ConstantList: " + lexA.currentToken());
		    return true;
		  }
		}
		lexA = lexa;
		return false;
	}
	
	boolean ConstantListTail() //fix?
 	{
		//System.out.println("ConstantListTail");
		LexicalAnalyzer lexa = lexA;
		//System.out.println("ConstantListTail: " + lexA.currentToken());
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			//System.out.println("ConstantListTail: " + lexA.currentToken());
			if(ConstantList())
			  return true;
		}
		lexA = lexa;
	  return true; //null is case
	}
	
	boolean RuleList()
	{
	  //System.out.println("RuleList");
		if(Rule()&&RuleListTail())
		  return true;
		return false;
	}
	
	boolean RuleListTail()
	{
		//System.out.println("RuleListTail");
		if(RuleList())
		  return true;
		else //null is case
		  return true;
	}
	
	boolean Rule() //fix?
 	{
		//System.out.println("Rule");
		LexicalAnalyzer lexa = lexA;
		//System.out.println("Rule: " + lexA.currentToken());
		if(Head())
		{
		  //System.out.println("Rule: " + lexA.currentToken());
		  if(lexA.currentToken().getType() == Token.COLON_DASH)//:-
			{
			  lexA.nextToken();
				//System.out.println("Rule: " + lexA.currentToken());
				if(Body())
				{
				  //System.out.println("Rule: " + lexA.currentToken());
				  if(lexA.currentToken().getType() == Token.PERIOD)//.
					{
					  lexA.nextToken();
						//System.out.println("Rule: " + lexA.currentToken());
						return true;
					}
				}
		  }
		}
		lexA = lexa;
		return false;
	}
	
	boolean Head()
	{
	  //System.out.println("Head");
		//System.out.println("Head: " + lexA.currentToken());
		if(SimplePredicate())
		  return true;
		return false;
	}
	
	boolean SimplePredicate() //fix?
	{
	  //System.out.println("SimplePredicate");
	  LexicalAnalyzer lexa = lexA;
		//System.out.println("SimplePredicate: " + lexA.currentToken());
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  lexA.nextToken();
			//System.out.println("SimplePredicate: " + lexA.currentToken());
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
			{
			  lexA.nextToken();
				//System.out.println("SimplePredicate: " + lexA.currentToken());
				if(ArgumentList())
				{
				  //System.out.println("SimplePredicate: " + lexA.currentToken()); //stops here
				  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
					{
					  lexA.nextToken();
						//System.out.println("SimplePredicate: " + lexA.currentToken());
						return true;
					}
				}
			}
		}
		lexA = lexa;
		return false; //else all
	}
	
	boolean ArgumentList()
	{
	  //System.out.println("ArgumentList");
		if(Argument()&&ArgumentListTail())
		  return true;
		return false;
	}
	
	boolean ArgumentListTail()
	{
	  //System.out.println("ArgumentListTail");
		LexicalAnalyzer lexa = lexA;
		//System.out.println("ArgumentListTail: " + lexA.currentToken());
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			if(ArgumentList())
			  return true;
    }
		lexA = lexa;
		return true;
	}
	
	boolean Argument() //fix?
	{
	  //System.out.println("Argument");
	  LexicalAnalyzer lexa = lexA;
		//System.out.println("Argument: " + lexA.currentToken());
		if(lexA.currentToken().getType() == Token.STRING)//String
		{
		  lexA.nextToken();
			//System.out.println("Argument: " + lexA.currentToken());
			return true;
		}
		if(Variable())
		{
		  //System.out.println("Argument: " + lexA.currentToken());
		  return true;
		}
		lexA = lexa;
		return false;
	}
	
	boolean Variable()
	{
	  //System.out.println("Variable");
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  lexA.nextToken();
			return true;
		}
		return false;
	}
	
	boolean Body()
	{
	  //System.out.println("Body");
		if(PredicateList())
		  return true;
		return false;
	}
	
	boolean PredicateList()
	{
	  //System.out.println("PredicateList");
		if(Predicate()&&PredicateListTail())
		  return true;
		return false;
	}
	
	boolean PredicateListTail() //fix?
	{
	  //System.out.println("PredicateListTail");
	  LexicalAnalyzer lexa = lexA;
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			if(PredicateList())			  
				return true;
		}
		lexA = lexa;
	  return true; //null is case
	}
	
	boolean Predicate()
	{
	  //System.out.println("Predicate");
		if(SimplePredicate()||BuiltInPredicate())
		  return true;
		return false;
	}
	
	boolean BuiltInPredicate() //fix?
	{
	  //System.out.println("BuiltInPredicate");
	  LexicalAnalyzer lexa = lexA;
		if(Comparator())
		{
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
		  {
			  lexA.nextToken();
				if(Argument())
				{
				  if(lexA.currentToken().getType() == Token.COMMA)//,
					{
					  lexA.nextToken();
						if(Argument())
						{
						  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
							{
							  lexA.nextToken();
								return true;
							}
						}
					}
				}
			}
		}
		lexA = lexa;
		return false; //else all
	}
	
	boolean Comparator()
	{
	  //System.out.println("Comparator");
	  switch(lexA.currentToken().getType())
		{
		  case Token.LT: //<
			  lexA.nextToken();
			  return true;
		  case Token.LE: //<=
			  lexA.nextToken();
			  return true;
		  case Token.EQ: //=
			  lexA.nextToken();
			  return true;
		  case Token.NE: //!=
			  lexA.nextToken();
			  return true;
		  case Token.GE: //>=
			  lexA.nextToken();
			  return true;
		  case Token.GT: //>
			  lexA.nextToken();
			  return true;
			default:
			  return false;
		}
	}
	
	boolean QueryList()
	{
	  //System.out.println("QueryList");
		if(Query()&&QueryListTail())
		  return true;
		return false;
	}
	
	boolean QueryListTail()
	{
	  //System.out.println("QueryListTail");
		if(QueryList())
		  return true;
		else
		  return true;
		}
  }
	
	boolean Query()
	{
	  //System.out.println("Query");
		if(SimplePredicate())
		{
		  //System.out.println("Query: " + lexA.currentToken());
		  if(lexA.currentToken().getType() == Token.Q_MARK)//?
			{
			  lexA.nextToken();
				//System.out.println("Query: " + lexA.currentToken());
				return true;
			}
		}
		return false;
	}
}
