// Lab5.java by Ged Crimsonclaw

import java.io.*;

class Lab5
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
		

		if(!prog.start())
		{
		  System.out.println("Bad program.  Terminated." + "\n  __,_____\n / __.==--\"\n/#(-'\n`-'");
			return;
		}
		
		//now insert new code
		makeRels(prog);
		boolean changed = true;
		while (changed)
		{
		  changed = false;
		  for(int i = 0; i < prog.ruleList.length; i++)
		  {
			  //execute the relational-algebra assignment for R ???
			  if(parseRule(prog.ruleList.ruleAt(i), prog)) //the assignment added tuples to the relation
			    changed = true;
		  }  
		}
		
		System.out.println(prog);  // only works for one round
		
		//Queries
		System.out.println("Queries:");
		for(int i = 0; i < prog.queryList.length; i++)
		{
		  Query tempq = prog.queryList.qAt(i);
			System.out.println(tempq);
			for(int j = 0; j < prog.relationList.length; j++)
			{
			  //System.out.println(prog.relationList[j].scheme.ID  + "   " + tempq.pred.value);
			  if(prog.relationList[j].scheme.ID.equals(tempq.pred.value))
				{ //is the one to query on 
				  //Relation tempr = new Relation(prog.relationList[j].scheme);
					Relation tempr = prog.relationList[j];//new Relation(prog.relationList[j].scheme);
				  for(int k = 0; k < tempq.pred.arguments.length; k++)
					{
					  Argument tempa = tempq.pred.arguments.argAt(k);
						//System.out.println(k + "" + tempa);
					  if(!tempa.var)
						{
							tempr = tempr.select(prog.relationList[j].scheme.getAttrAt(k), "=", new Value(tempa.value));
							//Relation tt = tempr.select(prog.relationList[j].scheme.getAttrAt(k), "=", new Value(tempa.value));
							//System.out.println(tt);
							//tempr = tempr.intersection(tt);
						}
					}
					System.out.println(tempr);
				}
			}
		}		
  }
	
	static void makeRels(DatalogProgram prog)
	{
		for(int i = 0; i < prog.ruleList.length; i++)
		{
			Rule rule = prog.ruleList.ruleAt(i);
			
			Relation ruleout = null;
			for(int j = 0; j < prog.relationList.length; j++)
			{ //does relation already exist?
				if(rule.head.value.equals(prog.relationList[j].scheme.ID))
				{  
					ruleout = prog.relationList[j];
					break;
				}
			} //end for
			if(ruleout == null)
			{ //doesn't exist, create one
				Relation[] temp = new Relation[prog.relationList.length + 1];
				int j;
				for(j = 0; j < prog.relationList.length; j++)
					temp[j] = prog.relationList[j];
				prog.relationList = temp;
				prog.relationList[j] = new Relation(new Scheme(rule.head.arguments.toTrib()));
				prog.relationList[j].scheme.ID = rule.head.value;
			}
		}
	}
	
	static boolean parseRule(Rule rule, DatalogProgram prog)
	{ //just assume things on the right would be caught first ;p
	  Relation ruleout = null;
		int loc = 0;
		boolean changes = false;
		for(int i = 0; i < prog.relationList.length; i++)
		{ //does relation already exist?  IT MUST
		  if(rule.head.value.equals(prog.relationList[i].scheme.ID))
			{  
				ruleout = prog.relationList[i];
				loc = i;
				break;
			}
		} //end for
		/*if(ruleout == null)
		{ //doesn't exist, create one
		Relation[] temp = new Relation[prog.relationList.length + 1];
		int i;
		for(i = 0; i < prog.relationList.length; i++)
		temp[i] = prog.relationList[i];
		prog.relationList = temp;
		prog.relationList[i] = new Relation(new Scheme(rule.head.arguments.toTrib()));
		prog.relationList[i].scheme.ID = rule.head.value;
		loc = i;
		}*/  //MUST do BEFORE FPA.  ~_~
		
		//now make and add in tuples (ezer?  mebby?)
		Relation[] useRels = new Relation[rule.body.length];
		for(int i = 0; i < rule.body.length; i++)
		{ //for each of the rules in the predicate list
		  for(int j = 0; j < prog.relationList.length; j++)
			{ //does it equal the thing
			  if(rule.body.predAt(i).value.equals(prog.relationList[j].scheme.ID))
				{ // rigth relation
				  //System.out.println(prog.relationList[j] + "before");
				  useRels[i] = prog.relationList[j].rename(new Scheme(rule.body.predAt(i).arguments.toTrib()));
					//System.out.println(useRels[i].scheme.ID + "\n" + i);
					//System.out.println(i + " " + useRels[i]);
					break;
				}
			}// /for
		}// /for
		
		//join all involved rels
		ruleout = useRels[0];
	  //System.out.println("Ruleout: \n" + ruleout);
		for(int i = 1; i < useRels.length; i++)
		{
			//System.out.print("Ruleout: \n" + ruleout);
			//System.out.print("Joined to: \n" + useRels[i]);
			ruleout = ruleout.join(useRels[i]);
			//System.out.print("Ruleout after: \n" + ruleout);
		}
		//System.out.println("Ruleout after: \n" + ruleout);
		//System.out.println(useRels[1]);
		//project (even if extranious)
		ruleout = ruleout.project(new Scheme(rule.head.arguments.toTrib()));
		int relint = prog.relationList[loc].LENGTH;
		prog.relationList[loc] = prog.relationList[loc].union(ruleout);//ruleout;
		if(relint != prog.relationList[loc].LENGTH) changes = true;
		//get length of before and after, if !=, then changes = true;
		//prog.relationList[loc].union(ruleout);
		//prog.relationList[loc].scheme.ID = rule.head.value;
		return changes;
	}
}

class RuleList
{
  Rule head = null;
	int length = 0;
	
	RuleList(Rule rule)
	{
	  head = rule;
		length = 1;
	}
	
	void insert(Rule rule)
	{
	  Rule temp = head;
		head = rule;
		head.next = temp;
		length++;
		
		boolean isin = false;
		for(temp = head; temp.next != null; temp = temp.next)
		{
		  if(rule.equals(temp))
			{
			  if(isin)
				{
				  head = head.next;
					length--;
					return;
				}
			  isin = true;
			}
		}
	} //insert
	
	Rule ruleAt(int i)
	{
	  if(i > length) return null;
		
	  Rule temp = head;
		while(i != 0)
		{
		  temp = temp.next;
			i--;
		}
		return temp;
	}
	
	public String toString()
	{
	  String output;
		output = "Rules\n";
		Rule temp = head;
		int i = 1;
    while(temp != null)
		{
		  output += i + "\n";
			i++;
			output += temp;
			temp = temp.next;
		}
		return output;
	}//toString
}

class Rule
{
  Rule next;
	Predicate head;
	PredicateList body;
	
	Rule(Predicate p, PredicateList pl)
	{
	  head = p;
		body = pl;
	}
	
	boolean equals(Rule rule)
	{
	  if((head.equals(rule.head))&&(body.equals(rule.body)))
		  return true;
		return false;
	}
	
	public String toString()
	{
	  String output;
		output = "\tHead";
		output += head;
		output += "\n\tBody";
		output += body;
		return output;
	}
}

class PredicateList
{
	Predicate head = null;
	int length = 0;
	
	PredicateList(Predicate p)
	{
	  head = p;
		length = 1;
	}
	
	void insert(Predicate p)
	{
	  Predicate temp = head;
		head = p;
		head.next = temp;
		length++;
		
		boolean isin = false;
		for(temp = head; temp.next != null; temp = temp.next)
		{
		  if(p.equals(temp))
			{
			  if(isin)
				{
				  head = head.next;
					length--;
					return;
				}
			  isin = true;
			}
		}
	} //insert
	
	Predicate predAt(int i)
	{
	  if(i > length) return null;
		
		Predicate temp = head;
		while(i != 0)
		{
		  temp = temp.next;
			i--;
		}
		return temp;
	}
	
	public String toString()
	{
	  String output = "";
		Predicate temp = head;
		while(temp != null)
		{
		  output += temp;
			temp = temp.next;
		}
		return output;
	}
	
	boolean equals(PredicateList plist)
	{
	  Predicate temp = head;
		Predicate temp2 = plist.head;
	  while(temp != null && temp2 != null)
		{
		  if(!temp.equals(temp2))
		    return false;
			temp = temp.next;
			temp2 = temp2.next;
		}
		if(temp == null && temp2 == null)  return true;
		return false;
	} //equals
}

class Predicate
{
  Predicate next;
  String value;
	ArgumentList arguments;
	
	Predicate(String in_value, ArgumentList a)
	{
	  value = in_value;
		arguments = a;
	}
	
	boolean equals(Predicate p)
	{
	  if(value.equals(p.value)&&arguments.equals(p.arguments))
		  return true;
		return false;
	}
	
	public String toString()
	{
	  String output;
		output = "\n\t\tPredicate\n";
		output += "\t\t\tName " + value;
		output += arguments;
		return output;
	}
}

class ArgumentList 
{
	Argument head = null;
	int length = 0;
	
	ArgumentList(Argument a)
	{
	  head = a;
		length = 1;
	}
	
	void insert(Argument a)
	{
	  Argument temp = head;
		head = a;
		head.next = temp;
		length++;
		
		boolean isin = false;
		for(temp = head; temp.next != null; temp = temp.next)
		{
		  if(temp.equals(a))
			{
				if(isin)
				{
				  head = head.next;
					length--;
					return;
				}
				isin = true;
			}
		}
	} //insert
	
	boolean equals(ArgumentList alist)
	{
	  Argument temp = head;
		Argument temp2 = alist.head;
		while(temp != null && temp2 != null)
		{
		  if(!temp.equals(temp2))
			  return false;
			temp = temp.next;
			temp2 = temp2.next;
		}
		if(temp == null && temp2 == null) return true;
		return false;
	} //equals
	
	Attribute[] toTrib()
	{
	  Attribute[] output = new Attribute[length];
		for(int i = 0; i < length; i++)
		  output[i] = argAt(i).toTrib();
		return output;
	}
	
	Argument argAt(int i)
	{
	  if(i > length) return null;
		
	  Argument temp = head;
		while(i != 0)
		{
		  temp = temp.next;
			i--;
		}
		return temp;
	}
	
	public String toString()
	{
	  String output;
		output = "\n\t\t\tArguments";
		Argument temp = head;
		while(temp != null)
		{
		  output += temp;
			temp = temp.next;
		}
		return output;
	}
}

class Argument
{
  Argument next;
	String value;
	boolean var; //on is var, off is const
	
	Argument(String in_val, boolean in_var)
	{
	  value = in_val;
		var = in_var;
	}
	
	boolean equals(Argument a)
	{
	  if(value.equals(a.value)&&(var == a.var))
		  return true;
		return false;
	}
	
	Attribute toTrib()
	{
	  return new Attribute(value);
	}
	
	public String toString()
	{
	  String output;
		output = "\n\t\t\t\tArgument ";
		output += value;
		output += " Type " + ((var)?"Variable":"Constant");
		return output;
	}
}

class QueryList
{
  Query head;
	int length = 0;
	
	QueryList(Query q)
	{
	  head = q;
		length = 1;
	}
	
	void insert(Query q)
	{
	  Query temp = head;
		head = q;
		head.next = temp;
		length++;
		
		boolean isin = false;
		for(temp = head; temp.next != null; temp = temp.next)
		{
		  if(temp.equals(q))
			{
				if(isin)
				{
				  head = head.next;
					length--;
					return;
				}
				isin = true;
			}
		}
	} //insert
	
	Query qAt(int i)
	{
	  Query temp = head;
		while( i != 0)
		{
		  temp = temp.next;
			i--;
		}
		return temp;
	}
	
	public String toString()
	{
	  String output = "\nQueries\n";
		int i = 1;
		Query temp = head;
		while(temp != null)
		{
		  output += i;
			output += temp;
			temp = temp.next;
		}
		return output;
	} //toString
}

class Query
{
  Query next;
	Predicate pred;
	
	Query(Predicate p)
	{
	  pred = p;
	}
	
	boolean equals(Query q)
	{
	  if(pred.equals(q.pred))
		  return true;
		return false;
	}
	
	public String toString()
	{
	  String output = "";
		output += pred;
		return output;
	}
}
