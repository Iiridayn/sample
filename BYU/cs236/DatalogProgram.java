/* DatalogProgram.java by Ged Crimsonclaw 11/18/2002 */

class DatalogProgram
{
  LexicalAnalyzer lexA = null;
	
	RuleList ruleList;
	QueryList queryList;
	Relation[] relationList;
	
	DatalogProgram(String infilename)
	{
	  lexA = new LexicalAnalyzer(infilename);
	}
	
	boolean start()
	{
	  Scheme[] schemeList;
		if(lexA.currentToken().getType() == Token.SCHEMES)//Schemes
		{
		  lexA.nextToken();
			if(lexA.currentToken().getType() == Token.COLON)//:
			{
        lexA.nextToken();
				schemeList = SchemeList();
				if(schemeList == null)
				  return false;
			}
			else
		    return false;
		}//end Schemes
		else
		  return false;
		
		Tuple[] factList;
		if(lexA.currentToken().getType() == Token.FACTS)//Facts
		{
		  lexA.nextToken();
			if(lexA.currentToken().getType() == Token.COLON)//:
			{
        lexA.nextToken();
				factList = FactList();
				if(factList == null)
				  return false;
			}
			else
		    return false;
		}//end Facts
		else
		  return false;
	  
		makeRelations(schemeList, factList);
		
		if(lexA.currentToken().getType() == Token.RULES)//Rules
		{
		  lexA.nextToken();
			if(lexA.currentToken().getType() == Token.COLON)//:
			{
        lexA.nextToken();
				ruleList = RuleList();
				if(ruleList == null)
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
				queryList = QueryList();
				if(queryList == null)
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
	
	void makeRelations(Scheme[] schemeList, Tuple[] factList)
	{
	  relationList = new Relation[schemeList.length];
		for(int i = 0; i < schemeList.length; i++)
		{
		  relationList[i] = new Relation(schemeList[i]);
			for(int j = 0; j < factList.length; j++)
			{
			  if(factList[j].ID.equals(schemeList[i].ID))
				  relationList[i].insert(factList[j]);
			}
		}
	}
	
	public String toString()
	{
	  String output = "";
		for(int i = 0; i < relationList.length; i++)
		{
		  output += relationList[i];
		}
		output += ruleList;
		output += queryList;
		return output;
	}
	
	Scheme[] SchemeList()
	{
		Scheme temp = Scheme();
		Scheme[] temp2 = SchemeListTail();		
		if(temp != null)
		{
		  if(temp2 == null)
			{
			  Scheme[] temp3 = {temp};
				return temp3;
			}
		  else
			{
			  Scheme[] temp3 = new Scheme[temp2.length+1];
				int i;
				for(i = 0; i < temp2.length; i++)
				  temp3[i] = temp2[i];
				temp3[i] = temp;
				return temp3;
			}
		}
		return null;
	}
	
	Scheme[] SchemeListTail()
 	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		  return SchemeList();
		else
		  return null;
	}
	
	Scheme Scheme() //fix?
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  String id = lexA.currentToken().getValue();
		  lexA.nextToken();
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
			{
			  lexA.nextToken();
				Attribute[] temp = AttributeList();
				if(temp != null)
				{
				  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
					{
					  lexA.nextToken();
						Scheme tempS = new Scheme(temp);
						tempS.ID = id;
						return tempS;
					}
				}
			}
		}
		return null; //else all
	}
	
	Attribute[] AttributeList()
	{
	  Attribute temp = Attribute();
		Attribute[] temp2 = AttributeListTail();
		if(temp != null)
		{
		  if(temp2 == null)
			{
			  Attribute[] temp3 = {temp};
				return temp3;
			}
			else
			{
			  Attribute[] temp3 = new Attribute[temp2.length+1];
				int i;
				for(i = 1; i < temp3.length; i++)
				  temp3[i] = temp2[i-1];
				temp3[0] = temp;
				return temp3;
			}
		}
		return null;
	}
	
	Attribute[] AttributeListTail()//fix?
	{
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			return AttributeList();
		}
		return null; //null is the case
	}
	
	Attribute Attribute()
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier()
		{
		  Attribute temp = new Attribute(lexA.currentToken().getValue());
		  lexA.nextToken();
		  return temp;
		}
		return null;
	}
	
	Tuple[] FactList()
	{
	  //System.out.println("FactList()");
	  Tuple temp = Fact();
		Tuple[] temp2 = FactListTail();
		if(temp != null)
		{
		  if(temp2 == null)
			{
			  Tuple[] temp3 = {temp};
				return temp3;
			}
			else
			{
			  Tuple[] temp3 = new Tuple[temp2.length+1];
				int i;
				for(i = 0; i < temp2.length; i++)
			  	temp3[i] = temp2[i];
				temp3[i] = temp;
				return temp3;
			}
		}
		return null;
	}
	
	Tuple[] FactListTail()
 	{
		//System.out.println("FactListTail()");
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		 return FactList();
		else
		  return null;
	}
	
  Tuple Fact() //fix?
 	{
		//System.out.println("Fact()");
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  String id = lexA.currentToken().getValue();
		  lexA.nextToken();
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
			{
			  lexA.nextToken();
				Value[] temp = ConstantList();
				if(temp != null)
				{
				  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
					{
					  lexA.nextToken();
						if(lexA.currentToken().getType() == Token.PERIOD)//.
						{
						  lexA.nextToken();
							Tuple tempT = new Tuple(temp);
							tempT.ID = id;
							return tempT;
						}
					}
				}
			}
		}
		return null; //else all
	}
	
	Value[] ConstantList() //fix?
	{
		if(lexA.currentToken().getType() == Token.STRING)//String
		{
		  Value temp = new Value(lexA.currentToken().getValue());
		  lexA.nextToken();
			Value[] temp2 = ConstantListTail();
			if(temp2 != null)
			{
			  //lexA.nextToken();                                                                                 //sup w/dis?
				Value[] temp3 = new Value[temp2.length+1];
				int i;
				for(i = 1; i < temp3.length; i++)
					temp3[i] = temp2[i-1];
				temp3[0] = temp;
				return temp3;
		  }
			else
			{
			  Value[] temp3 = {temp};
				return temp3;
			}
		}
		return null;
	}
	
	Value[] ConstantListTail() //fix?
 	{
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			return ConstantList();
		}
	  return null; //null is case
	}
	
	RuleList RuleList()
	{
	  Rule temp = Rule();
		RuleList temp2 = RuleListTail();
		if(temp != null)
		{
		  if(temp2 != null)
			{
			  temp2.insert(temp);
				return temp2;
			}
			else
			{
			  temp2 = new RuleList(temp);
				return temp2;
			}
		}
		return null;
	}
	
	RuleList RuleListTail()
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		  return RuleList();
		else
		  return null;
	}
	
	Rule Rule() //fix?
 	{
		Predicate head = Head();
		if(head != null)
		{
		  if(lexA.currentToken().getType() == Token.COLON_DASH)//:-
			{
			  lexA.nextToken();
				PredicateList body = Body();
				if(body != null)
				{
				  if(lexA.currentToken().getType() == Token.PERIOD)//.
					{
					  lexA.nextToken();
						Rule temp = new Rule(head, body);
						return temp;
					}
				}
		  }
		}
		return null;
	}
	
	Predicate Head()
	{
	  return SimplePredicate();
	}
	
	Predicate SimplePredicate() //fix?
	{
		if(lexA.currentToken().getType() == Token.ID)//Identifier
		{
		  String ID = lexA.currentToken().getValue();
		  lexA.nextToken();
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
			{
			  lexA.nextToken();
				ArgumentList ls = ArgumentList();
				if(ls != null)
				{
				  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
					{
					  lexA.nextToken();
						Predicate temp = new Predicate(ID, ls);
						return temp;
					}
				}
			}
		}
		return null; //else all
	}
	
	ArgumentList ArgumentList()
	{
	  Argument temp = Argument();
		ArgumentList temp2 = ArgumentListTail();
		if(temp != null)
		{
		  if(temp2 != null)
			{
			  temp2.insert(temp);
				return temp2;
			}
			else
			{
			  temp2 = new ArgumentList(temp);
				return temp2;
			}
		}
		return null;
	}
	
  ArgumentList ArgumentListTail()
	{
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			return ArgumentList();
		}
    return null; //null is case
	}
	
	Argument Argument()
	{
		if(lexA.currentToken().getType() == Token.STRING)//String
		{
		  String val = lexA.currentToken().getValue();
		  lexA.nextToken();
			Argument temp = new Argument(val, false);
			return temp;
		}
		else if(lexA.currentToken().getType() == Token.ID) //Identifier
		{ //is var
		  String val = lexA.currentToken().getValue();
	    lexA.nextToken();
			Argument temp = new Argument(val, true);
			return temp;
		}
		else
			return null;
	}
	
	PredicateList Body()
	{
		return PredicateList();
	}
	
	PredicateList PredicateList()
	{
	  Predicate temp = Predicate();
		PredicateList temp2 = PredicateListTail();
		if(temp != null)
		{
		  if(temp2 != null)
			{
			  temp2.insert(temp);
				return temp2;
			}
			else
			{
			  temp2 = new PredicateList(temp);
				return temp2;
			}
		}
		return null;
	}
	
	PredicateList PredicateListTail() //fix?
	{
		if(lexA.currentToken().getType() == Token.COMMA)//,
		{
		  lexA.nextToken();
			return PredicateList();
		}
	  return null; //null is case
	}
	
	Predicate Predicate()
	{
	  Predicate temp = SimplePredicate();
		if(temp == null)
		  temp = BuiltInPredicate();
		return temp;
	}
	
	Predicate BuiltInPredicate() //fix?
	{
	  String val = lexA.currentToken().getValue();
		if(Comparator())
		{
		  if(lexA.currentToken().getType() == Token.LEFT_PAREN)//(
		  {
			  lexA.nextToken();
				Argument arg1 = Argument();
				if(arg1 != null)
				{
				  if(lexA.currentToken().getType() == Token.COMMA)//,
					{
					  lexA.nextToken();
						Argument arg2 = Argument();
						if(arg2 != null)
						{
						  if(lexA.currentToken().getType() == Token.RIGHT_PAREN)//)
							{
							  lexA.nextToken();
								ArgumentList alist = new ArgumentList(arg1);
								alist.insert(arg2);
								Predicate temp = new Predicate(val, alist);
								return temp;
							}
						}
					}
				}
			}
		}
		return null; //else all
	}
	
	boolean Comparator()
	{
	  switch(lexA.currentToken().getType())
		{
		  case Token.LT: //<
		  case Token.LE: //<=
			case Token.EQ: //=
			case Token.NE: //!=
		  case Token.GE: //>=
		  case Token.GT: //>
			  lexA.nextToken();
			  return true;
			default:
			  return false;
		}
	}
	
	QueryList QueryList()
	{
	  //System.out.println("QueryList()");
	  Query temp = Query();
		QueryList temp2 = QueryListTail();
		if(temp != null)
		{
		  if(temp2 != null)
      {
			  temp2.insert(temp);
				return temp2;
			}
			else
			{
			  temp2 = new QueryList(temp);
				return temp2;
			}
		}
		return null;
	}
	
	QueryList QueryListTail()
	{
	  //System.out.println("QueryListTail()");
		if(lexA.currentToken().getType() == Token.ID)//Identifier
			return QueryList();
		else
			return null;
	}
	
	Query Query()
	{
	  //System.out.println("Query()");
	  Predicate p = SimplePredicate();
		if(p != null)
		{
		  if(lexA.currentToken().getType() == Token.Q_MARK)//?
			{
			  lexA.nextToken();
				Query temp = new Query(p);
				return temp;
			}
		}
		return null;
	}
}
