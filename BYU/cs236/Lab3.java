// Lab3.java by Ged Crimsonclaw

import java.io.*;

class Lab3
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
		
	  System.out.println((new DatalogProgram(infilename).start())?"Success.":"Failure.");
	}
}

class DatalogProgram
{
  LexicalAnalyzer lexA = null;
	
	DatalogProgram(String infilename)
	{
	  lexA = new LexicalAnalyzer(infilename);
	}
	
	boolean start()
	{
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
		if(Scheme()&&SchemeListTail())
		  return true;
		return false;
	}
	
	boolean SchemeListTail()
 	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
			if(SchemeList())
			  return true;
			else //null is possible
			  return false;
		}
		else
		  return true;
	}
	
	boolean Scheme() //fix?
	{
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
		return false; //else all
	}
	
	boolean AttributeList()
	{
		if(Attribute()&&AttributeListTail())
		  return true;
		return false;
	}
	
	boolean AttributeListTail()//fix?
	{
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			if(AttributeList())
			  return true;
			else
			  return false;
		}
		return true; //null is the case
	}
	
	boolean Attribute()
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier()
		{
		  lexA.nextToken();
		  return true;
		}
		return false;
	}
	
	boolean FactList()
	{
		if(Fact()&&FactListTail())
		  return true;
		return false;
	}
	
	boolean FactListTail()
 	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  if(FactList())
		    return true;
		  else //null is case
		    return false;
	  }
		else
		  return true;
	}
	
	boolean Fact() //fix?
 	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  lexA.nextToken();
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
			{
			  lexA.nextToken();
				if(ConstantList())
				{
				  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
					{
					  lexA.nextToken();
						if(lexA.currentToken().getType() == Token.PERIOD)//.
						{
						  lexA.nextToken();
							return true;
						}
					}
				}
			}
		}
		return false; //else all
	}
	
	boolean ConstantList() //fix?
	{
		if(lexA.currentToken().getType() == Token.STRING)//String
		{
		  lexA.nextToken();
			if(ConstantListTail())
			{
			  lexA.nextToken();
		    return true;
		  }
		}
		return false;
	}
	
	boolean ConstantListTail() //fix?
 	{
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			if(ConstantList())
			  return true;
			else
			  return false;
		}
	  return true; //null is case
	}
	
	boolean RuleList()
	{
		if(Rule()&&RuleListTail())
		  return true;
		return false;
	}
	
	boolean RuleListTail()
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
			if(RuleList())
			  return true;
			else //null is case
			  return false;
		}
		else
		  return true;
	}
	
	boolean Rule() //fix?
 	{
		if(Head())
		{
		  if(lexA.currentToken().getType() == Token.COLON_DASH)//:-
			{
			  lexA.nextToken();
				if(Body())
				{
				  if(lexA.currentToken().getType() == Token.PERIOD)//.
					{
					  lexA.nextToken();
						return true;
					}
				}
		  }
		}
		return false;
	}
	
	boolean Head()
	{
		if(SimplePredicate())
		  return true;
		return false;
	}
	
	boolean SimplePredicate() //fix?
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  lexA.nextToken();
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
			{
			  lexA.nextToken();
				if(ArgumentList())
				{
				  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
					{
					  lexA.nextToken();
						return true;
					}
				}
			}
		}
		return false; //else all
	}
	
	boolean ArgumentList()
	{
		if(Argument()&&ArgumentListTail())
		  return true;
		return false;
	}
	
	boolean ArgumentListTail()
	{
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			if(ArgumentList())
			  return true;
			else
			  return false;
		}
    return true; //null is case
	}
	
	boolean Argument()
	{
		if(lexA.currentToken().getType() == Token.STRING)//String
		{
		  lexA.nextToken();
			if(Variable())
		    return true;
		}
		return false;
	}
	
	boolean Variable()
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  lexA.nextToken();
			return true;
		}
		return false;
	}
	
	boolean Body()
	{
		if(PredicateList())
		  return true;
		return false;
	}
	
	boolean PredicateList()
	{
		if(Predicate()&&PredicateListTail())
		  return true;
		return false;
	}
	
	boolean PredicateListTail() //fix?
	{
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			if(PredicateList())
			  return true;
			else
			  return false;
		}
	  return true; //null is case
	}
	
	boolean Predicate()
	{
		if(SimplePredicate()&&BuiltInPredicate())
		  return true;
		return false;
	}
	
	boolean BuiltInPredicate() //fix?
	{
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
		return false; //else all
	}
	
	boolean Comparator()
	{
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
		if(Query()&&QueryListTail())
		  return true;
		return false;
	}
	
	boolean QueryListTail()
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
			if(QueryList())
			  return true;
			else //null is case
			  return false;
		}
		else
		  return true;
	}
	
	boolean Query()
	{
		if(SimplePredicate())
		{
		  if(lexA.currentToken().getType() == Token.Q_MARK)//?
			{
			  lexA.nextToken();
				return true;
			}
		}
		return false;
	}
}
