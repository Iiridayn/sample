// Lab4.java by Ged Crimsonclaw

import java.io.*;

class Lab4
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
		
    DatalogProgram temp = new DatalogProgram(infilename);
		if(!temp.start()) System.out.println("Bad program.  Terminated." + "\n  __,_____\n / __.==--\"\n/#(-'\n`-'");
		else System.out.println(temp);
	}
}

class RuleList
{
  Rule head = null;
	
	RuleList(Rule rule)
	{
	  head = rule;
	}
	
	void insert(Rule rule)
	{
	  if(head == null)
		{
		  head = rule;
			return;
		}
		else
		{
		  Rule temp;
		  for(temp = head; temp.next != null; temp = temp.next)
			{
			  if(rule.equals(temp))
				  return;
			}
			temp.next = rule;
		}
	} //insert
	
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
	
	PredicateList(Predicate p)
	{
	  head = p;
	}
	
	void insert(Predicate p)
	{
	  Predicate temp = head;
		head = p;
		head.next = temp;
		
		boolean isin = false;
		for(temp = head; temp.next != null; temp = temp.next)
		{
		  if(p.equals(temp))
			{
			  if(isin)
				{
				  head = head.next;
					return;
				}
			  isin = true;
			}
		}
	} //insert
	
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
	Argument head;
	
	ArgumentList(Argument a)
	{
	  head = a;
	}
	
	void insert(Argument a)
	{
	  Argument temp = head;
		head = a;
		head.next = temp;
		
		boolean isin = false;
		for(temp = head; temp.next != null; temp = temp.next)
		{
		  if(temp.equals(a))
			{
				if(isin)
				{
				  head = head.next;
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
	
	QueryList(Query q)
	{
	  head = q;
	}
	
	void insert(Query q)
	{
	  if(head == null)
		{
		  head = q;
			return;
		}
		else
		{
		  Query temp;
			for(temp = head; temp.next != null; temp = temp.next)
			{
			  //System.out.println("QueryList.insert()");
			  if(temp.equals(q)) return;
			}
			temp.next = q;
		}
	} //insert
	
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
