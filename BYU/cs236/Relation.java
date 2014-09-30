import java.io.*;
import java.util.*;

class Relation {
	Scheme scheme;
	TupleLL tuples;
	int LENGTH = 0;
	
	Relation(Scheme sch)
	{
		scheme = sch;
		tuples = new TupleLL();
	}
	//Creates a Relation where scheme = sch AND tuples is empty
	
	Relation(Scheme sch, String file_name)throws FileNotFoundException, IOException
	{
		scheme = sch;
		boolean hasLines = true;
		FileReader fr = new FileReader(file_name);
		BufferedReader infile = new BufferedReader(fr);
		tuples = new TupleLL();
		TupleLL temp = tuples;
		String now = infile.readLine();
		//System.out.println("INPUT:");
		while(hasLines)
		{
			StringTokenizer value = new StringTokenizer(now);
			int values = value.countTokens();
			Value[] vals = new Value[values];
			
			for(int i = 0; i < values; i++)
			{
				vals[i] = new Value(value.nextToken());
			}
			
			temp.tuple = new Tuple(vals);
			//System.out.println(temp.tuple);
			now = infile.readLine();
			if(now == null)
				hasLines = false;
			else
			{
				temp.next = new TupleLL();
				temp = temp.next;
			}
		}
		//System.out.println();
		//System.out.println();
		//System.out.println();
	}
  /*Creates a Relation where scheme = sch. Fills tuples
	from a TextInputFile with name file_name by reading one tuple at a
	time and for each tuple by reading one attribute value at a time. A typical
	file for a relation with scheme {"ABC"} is:
	12 5 9
	6 87 7
	34 66 11
	(etc)
  */
	
	//Queries
	Scheme getScheme()
	{
		return scheme;
	}
	//returns scheme
	
	boolean isIn(Tuple tupl)
	{
		TupleLL temp = tuples;
		while(temp != null)
		{		
			if(tuples.tuple.equals(tupl))
				return true;
			temp = temp.next;
		}
		return false;
	}
	//returns true if tupl is in tuples; otherwise return false
	
  Relation project(Scheme schm)
	{
		//System.out.println("Project:");
		//System.out.println(scheme + " to " + schm);
		if(schm.isSubsetOf(scheme))
		{
			Relation output = new Relation(schm);
			TupleLL temp = tuples;
			int length = schm.length();
			int[] positions = new int[length];
			for (int i = 0; i < length; i++)
			{
				positions[i] = scheme.getIndex(schm.attrib[i]);
				//System.out.print(positions[i] + " ");
			}
			
			while(temp != null)
			{
				Value[] nexttup = new Value[length];
				if(temp.tuple != null)
				{
					//System.out.println("Tuple: " + temp.tuple);
					for(int i = 0; i < length; i++)
					{
						nexttup[i] = temp.tuple.getIthValue(positions[i]);					
					}
					Tuple outinput = new Tuple(nexttup);
					output.insert(outinput);
				}
				temp = temp.next;
			}
			//System.out.println();
			//System.out.println();
			return output;
		}
		else
		{
			System.out.println("97: bad call in project");
			return null;
		}//ERROR  Insert error code here
	}
	/*Informally this function strips columns from the relation. The function
	creates a new relation with scheme schm with a tuple for every tuple
	in the target relation.  You should strip the values from the new tuples
	that correspond to the attributes stripped from the scheme.  In other words,
	the new tuples have just the values for the attributes specified in
	schm. There should be no duplicate tuples in the resulting relation.
	
	Example:
	Assume that r1 is an instance of Relation with the scheme {"ABC"}.
	
	
	Relation res = r1.project(new Scheme("AC"));
	
	This operation strips the column corresponding to attribute "B".
	After stripping the center column, all duplicate tuples are removed.
	The result is placed in a new relation referred to by "res".
	
	It is an error if schm is not a subset of scheme.
	*/
	
	Relation select(Attribute leftop,	String op, Attribute rightop)//orig
	{
		Relation output = new Relation(scheme);
		TupleLL temp = tuples;
		
		if((scheme.getIndex(leftop) == -1)||(scheme.getIndex(rightop) == -1))
			op = "";
		
		if(op.equals("="))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).equals(temp.tuple.getValue(scheme, rightop)))
					output.insert(temp.tuple);
			}
		}
		else if(op.equals("!="))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(!temp.tuple.getValue(scheme, leftop).notEqualTo(temp.tuple.getValue(scheme, rightop)))
					output.insert(temp.tuple);
			}
		}
		else if(op.equals("<"))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).lessThan(temp.tuple.getValue(scheme, rightop)))
					output.insert(temp.tuple);
			}
		}
		else if(op.equals("<="))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).lessThanOrEqual(temp.tuple.getValue(scheme, rightop)))
					output.insert(temp.tuple);
			}
		}
		else if(op.equals(">"))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).greaterThan(temp.tuple.getValue(scheme, rightop)))
					output.insert(temp.tuple);
			}
		}
		else if(op.equals(">="))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).greaterThanOrEqual(temp.tuple.getValue(scheme, rightop)))
					output.insert(temp.tuple);
			}
		}
		else
		{
			System.out.println("179: problem with select");
			return null;
		}//ERROR  Insert error code here
		
		return output;
	}
	/*
	This operation selects all tuples in tuples that satisfy the
	attribute-attribute expression.
	
	An attribute-attribute expression is formed from the leftop,
	op, and rightOp parameters. It has the form leftop op rightOp. 
	For instance, if leftop is the Attribute 'A', op = "<=", and 
	rightOp is the Attribute 'C' then the expression is "A <= C". 
	A tuple satisfies this expression if the value in the tuple associated 
	with the attribute A is less than or equal to the value in the tuple 
	associated with the attribute C.
	
	The only legal comparison operators are: "=", "!=", "<=", "<", 
	">=", or ">"
	
	Examples:
	Assume that r1 is an instance of Relation with the scheme {"ABC"}.
	
	Attribute bAtt = new Attribute("B");
	Attribute cAtt = new Attribute("C");
	Relation r = r1.select(bAtt, "<", cAtt);
	The variable r will refer to a new relation with a
	scheme the same as r1's scheme, and it will contain every tuple
	in r1 where the value of the second or B attribute
	is less than the value of the third or C attribute.
	
	It is an error if: 1)leftop is not in rel's scheme, 2) if rightOp is
	not in rel's scheme, 3) the op is not one of the six valid operators 
	*/
	Relation select(Attribute leftop,	String op, Value rightop) //Modded.
	{
		Relation output = new Relation(scheme);
		TupleLL temp = tuples;
		//System.out.println("SELECT: ");
		if(scheme.getIndex(leftop) == -1)
			op = "";
		//System.out.println(leftop + "\n" + op + "\n" + rightop);
		if(op.equals("="))
		{
		  while(temp != null)
			{
			  //if(temp.tuple != null) System.out.println(temp.tuple.getValue(scheme, leftop) + " = " + rightop);
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).equals(rightop))
					output.insert(temp.tuple);
				//System.out.println(output);
				temp = temp.next;
			}
		}
		else if(op.equals("!="))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(!temp.tuple.getValue(scheme, leftop).notEqualTo(rightop))
					output.insert(temp.tuple);
				temp = temp.next;
			}
		}
		else if(op.equals("<"))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).lessThan(rightop))
					output.insert(temp.tuple);
				temp = temp.next;
			}
		}
		else if(op.equals("<="))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).lessThanOrEqual(rightop))
					output.insert(temp.tuple);
				temp = temp.next;
			}
		}
		else if(op.equals(">"))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).greaterThan(rightop))
					output.insert(temp.tuple);
				temp = temp.next;
			}
		}
		else if(op.equals(">="))
		{
			while(temp != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.getValue(scheme, leftop).greaterThanOrEqual(rightop))
					output.insert(temp.tuple);
				temp = temp.next;
			}
		}
		else
		{
			System.out.println("179: problem with select");
			return null;
		}//ERROR  Insert error code here
		
		//System.out.println(output);
		return output;
	}
	
	
	Relation join(Relation r)
	{
		int same = -1;
		int same2 = -1;
		int length = scheme.length();
		int length2 = r.scheme.length();
		int length3 = length + length2;
		TupleLL temp = tuples;
		TupleLL temp2 = r.tuples;
		Relation output;
		//System.out.println("JOIN:");
		//System.out.println(scheme);
		//System.out.println(r.scheme);
		for(int i = 0; i < length; i++)
		{
			for(int j = 0; j < length2; j++)
			{
			  //System.out.println(scheme.getAttrAt(i) + " =? " + r.scheme.getAttrAt(j));
				if(scheme.getAttrAt(i).equals(r.scheme.getAttrAt(j)))
				{
					same = i;
					same2 = j;
					break;
				}
			}
			if(same != -1)
				break;
		}//done finding same Attrib
		
		if(same == -1)
		{
			Attribute[] tribs = new Attribute[length3];
			
			for(int i = 0; i < length; i++)
				tribs[i] = scheme.getAttrAt(i);
			for(int i = 0; i < (length3 - length); i++)
				tribs[i+length] = r.scheme.getAttrAt(i);
			Scheme topor = new Scheme(tribs);
			output = new Relation (topor);
			
			Value[] temptup = new Value[length3];
			
			while(temp != null)
			{
			  if(temp.tuple != null)
				while(temp2 != null)
				{
				  if(temp2.tuple != null)
					for(int i = 0; i < length3; i++)
					{
						if(i < length)
							temptup[i] = temp.tuple.getIthValue(i);
						else if(i-length < same2 + 1)
							temptup[i] = temp2.tuple.getIthValue(i - length);
					}//load all Vals into val[]
					Tuple temporary = new Tuple(temptup);
					output.insert(temporary);
					//put into tuple into output
					temp2 = temp2.next;
				}
				temp2 = r.tuples;
				temp = temp.next;
			}
		}
		else
		{
			Attribute[] tribs = new Attribute[length3-1];
			//System.out.println("is different");
			for(int i = 0; i < length; i++)
				tribs[i] = scheme.getAttrAt(i);
			boolean isIn = false;
			boolean wasIn = false;
			for(int i = 0; i < (length3 - length); i++)
			{
				if(!isIn)
					for(int j = 0; j < length; j++)
						if(r.scheme.getAttrAt(i).equals(tribs[j]))
							isIn = true;
				if(!isIn)
					tribs[i+length] = r.scheme.getAttrAt(i);
				if(wasIn)
					tribs[i+length-1] = r.scheme.getAttrAt(i);
				if(isIn)
					wasIn = true;
			}
			
			Scheme topor = new Scheme(tribs);
			//System.out.println("New scheme: " + topor);
			output = new Relation (topor);
			
			Value[] temptup = new Value[length3 - 1];
			
			while(temp != null)
			{
				//if(temp.tuple != null) System.out.println(temp.tuple);
				if(temp.tuple != null)
				while(temp2 != null)
				{
					//if(temp.tuple != null) System.out.println(temp2.tuple);
					if(temp2.tuple != null)
					if(temp.tuple.getIthValue(same).equals(temp2.tuple.getIthValue(same2)))
					{
						temptup = new Value[length3 - 1];
						for(int i = 0; i < length3-1; i++)
						{
							if(i < length)
							{
								temptup[i] = temp.tuple.getIthValue(i);
								//System.out.println("0");
							}
							else
							{
								//System.out.println("same2: " + same2);
								if(i-length < same2)
								{
									temptup[i] = temp2.tuple.getIthValue(i - length);
									//System.out.println("1");
								}
								else if (i-length >= same2-1)
								{
									temptup[i] = temp2.tuple.getIthValue(i - length + 1);
									//System.out.println("2");
								}
							}
							//System.out.println(temptup[i]);
						}//load all into val[]
						//output.tuplechecker(0);
						Tuple temporary = new Tuple(temptup);
					  //System.out.println("Add tuple: " + temporary);
						output.insert(temporary);
						//put into tuple into output
					}
					temp2 = temp2.next;
				}
				temp2 = r.tuples;
				temp = temp.next;
			}//end top while
			
		}//end the big if
		//System.out.println();
		//System.out.println();
		//System.out.println();
		return output;
	}//end method
	/*Creates and returns a new relation that is the natural join of
	"this" relation and r . Returns a reference to the newly created
	relation. The join function needs to handle only two cases:
	(1) no attributes in common, (2) one attribute in common.
	If you wish, you may handle the general case, as well, so
	that your join handles zero or more attributes in common.
	
	Example:
	Relation r = r1.join(r2);
	*/
	
	Relation intersection(Relation r)
	{
		if(!r.scheme.equals(scheme))
		{
			System.out.println("330: scheme in intersection not equal");
			return null;
		}//ERROR  Insert error code here
		else
		{
			Relation output = new Relation(scheme);
			TupleLL temp = tuples;
			TupleLL temp2 = r.tuples;						
			int length = scheme.length();
			int[] arr = new int[length];
			boolean isntsame = false;
			
			for(int i = 0; i < length; i++)
			{
				arr[i] = r.scheme.getIndex(scheme.getAttrAt(i));
			}
			
			boolean in = true;
			boolean isin = false;
			while(temp != null)
			{
				if(temp.tuple != null)
				while(temp2 != null)
				{
					if(!isin)
					{
					if(temp2.tuple != null)
					for(int i = 0; i < length; i++)
					{
						if(!temp2.tuple.getIthValue(arr[i]).equals(temp.tuple.getIthValue(i)))
							in = false;							
					}
					//System.out.println(temp.tuple + "  =   " + temp2.tuple);
					if(in)
						isin = true;
					in = true;
					}
					temp2 = temp2.next;
				}
				//System.out.println(!isin);
				if(isin)
					output.insert(temp.tuple);
				isin = false;
				temp2 = r.tuples;
				temp = temp.next;
			}
			
			return output;
		}
	}
	
	Relation union(Relation r)
	{
		if(scheme.equals(r.scheme))
		{
			TupleLL temp = tuples;
			TupleLL temp2 = r.tuples;
			Relation output = new Relation(scheme);
			
			while(temp != null)
			{
				if(temp.tuple != null)
				output.insert(temp.tuple);
				temp = temp.next;
			}
			while(temp2 != null)
			{
				if(temp2.tuple != null)
				output.insert(temp2.tuple);
				temp2 = temp2.next;
			}
			return output;
		}
		else
		{
			System.out.println("536: scheme in union not equal!");
			return null;
		}
	} //so simpol;  I could even doing it myself!
	/*Creates and returns a new relation that is the union of this relation
	and r. There should be no duplicate tuples in the resulting
	relation.  The scheme of the new relation is the same as the scheme 
	of "this".
	
	Example:
	Relation r = r1.Union(r2);
	
	It is an error if scheme is not equal to the scheme of r.
	*/
	
  Relation minus(Relation r)
	{
		if(!r.scheme.equals(scheme))
		{
			System.out.println("366: scheme in minus not equal");
			return null;
		}//ERROR  Insert error code here
		else
		{
			//System.out.println("MINUS:");
			//System.out.println(this);
			//System.out.println(r);
			Relation output = new Relation(scheme);
			TupleLL temp = tuples;
			TupleLL temp2 = r.tuples;						
			int length = scheme.length();
			int[] arr = new int[length];
			boolean isntsame = false;
			
			for(int i = 0; i < length; i++)
			{
				arr[i] = r.scheme.getIndex(scheme.getAttrAt(i));
			}
			
			boolean in = true;
			boolean isin = false;
			while(temp != null)
			{
				if(temp.tuple != null)
				while(temp2 != null)
				{
					if(!isin)
					{
						if(temp2.tuple != null)
						{
							for(int i = 0; i < length; i++)
							{
								if(!temp2.tuple.getIthValue(arr[i]).equals(temp.tuple.getIthValue(i)))
									in = false;							
							}
							//System.out.println(temp.tuple + "  =   " + temp2.tuple);
							if(in)
								isin = true;
							in = true;
						}
					}
					temp2 = temp2.next;
				}
				//System.out.println(!isin);
				if(!isin && temp.tuple != null)
					output.insert(temp.tuple);
				isin = false;
				temp2 = r.tuples;
				temp = temp.next;
			}
			
			return output;
		}
	}
	/*Creates and returns a new relation that is the set difference of
	relations "this" and r (i.e. this - r).  The scheme of the new
	relation is the same as the scheme of "this".
	
	Example:
	Relation r = r1.Minus(r2);
	
	It is an error if scheme is not equal to the scheme of r.
	*/
	
	Relation rename(Scheme schm)
	{
		if(schm.length() != scheme.length())
		{
			System.out.println("407: in rename, not even same length!");
			return null;
		}//ERROR  Insert error code here
		else
		{
			Relation output = new Relation(schm);
			output.tuples = tuples;
			return output;
		}
	}
	/*Creates a new relation that has the same tuples as "this" but with a new
	with scheme schm.
	
	Example:
	
	Relation r = r1.Rename(new Scheme("XYZ"));
	
	It is an error if the length of schm is not equal to the length
	of scheme.
	*/
	
	//Commands
	void insert(Tuple t)
	{
		//System.out.println("Insert: " + t);
		//tuplechecker(1);
		if(t.length() != scheme.length()) return; //is bad
		
		TupleLL temp = tuples;
		boolean isIn = false;
		while(temp != null)
		{
			if(temp.tuple != null)
			{
				//System.out.println("Comparing: " + temp.tuple);
				if(temp.tuple.equals(t))
				{
					isIn = true;
					//System.out.println("Match");
				}
			}
			temp = temp.next;
		}
		if(!isIn)
		{
			temp = tuples;
			//System.out.println("No match; inserting");
			LENGTH++;
			if(temp != null)
			{
				while(temp.next != null)
				{
					temp = temp.next;
				}
			}
			else
			{
				temp = new TupleLL();
				temp.next = tuples;
			}
			temp.next = new TupleLL();
			temp = temp.next;
			temp.tuple = new Tuple(t);
		}
		//tuplechecker(2);
		//System.out.println();
	}
	//after insert, t is in tuples
	
	/*
	void insert(Tuple t)
	{
		if(tuples == null)
		{
			tuples = new TupleLL();
			tuples.tuple = new Tuple(t);
		}
		else
		{
			TupleLL temp = tuples;
			while(temp.next != null)
			{
				if(temp.tuple != null)
				if(temp.tuple.equals(t)) return;
				temp = temp.next;				
			}
			temp.next = new TupleLL();
			temp.next.tuple = new Tuple(t);
			tuplechecker(3);
		}		
	}
	*/
	void tuplechecker(int i)
	{
		//System.out.println("TupleChecker");
		TupleLL temp = tuples;
		while(temp != null)
		{
			if(temp.tuple != null)
				System.out.println("TupleChecker" + i + ": " + temp.tuple);
			temp = temp.next;
		}
	}
	
	//I/O
	public String toString()
	{
		String schm = scheme.toString();
		char[] templine = new char[schm.length()-1];
		for(int i = 0; i < templine.length; i++)
		  templine[i] = '-';
		String intempline = new String(templine);
    String line = "\n" + intempline + "\n";
		String alltups = "";
		TupleLL temp = tuples;
		while(temp != null)
		{
			if(temp.tuple != null)
			alltups += temp.tuple.toString() + "\n";
			temp = temp.next;
		}
		String output = schm + line + alltups + "\n";
		return output;
	}
  //Prints the contents of the relation.  First it prints the scheme followed
  //by a line, follwed by each of the tuples.  The values of the tuples
  //should line up under the attribute names from the scheme.
	
	/* note for anyone interested:
	you could instead define a method called toString() that returns
	a String.  In Java, this method is standard for I/O, so you
	could use System.out.println(r1) in place of r1.show().
	
	If that didn't make any sense, don't worry, it's not required
	for this class. */
}

/*class TupleLL
{
	TupleLL next;
	Tuple tuple;
	TupleLL(){}
}*/

class Tuple {
	Value[] values;
	String ID = "";
	
	Tuple(Value[] new_values)
	{
		values = new_values;
	}

	/*         --- prolly not going to use a Vector... we'll see
  Tuple(Vector new_values)
  //Creates a new tuple from the Vector of values
  //Some will want to use this constructor instead of the Value array
  //constructor.
	*/
	
	Tuple(Tuple t)
	{
		values = t.values;
	}
	
	//Queries
	int length()
	{
		return values.length;
	}
	//returns the length of values
	
	Value getIthValue(int i)
	{
		int length = values.length;
		if((i < 0)||(i > length))
		{
			System.out.println("512");
			return null;
		}//ERROR  insert error code here
		else
			return values[i];
	}
	//returns the value in the ith location of values.
	//It is an error if i is  not in the range 0..length() - 1
	
	Value getValue(Scheme s, Attribute a)
	{
		int length = values.length;
		if(s.length() == length)
		{
			int index = s.getIndex(a);
			if(index > -1)
				return values[index];
			else
			{
				System.out.println("531");
				return null;
			}//ERROR  insert error code
		}
		else
		{
			System.out.println("537");
			return null;
		}//ERROR  insert error code
	}
	//Gets value of attribute a in scheme s //It is an error if 1)the length of s is not equal to length(), and
	//2) if a is not in s
	
	
	Tuple copy()
	{
		Tuple copy = new Tuple(values);
		return copy;
	}
	//Returns a copy of this tuple.  Similar to the copy constructor.
	
	boolean equals(Tuple tup)
	{
		int length = values.length;
		int length2 = tup.values.length;
		//System.out.println(tup);
		//System.out.println(toString());
		if(length == length2)
		{
			for(int i = 0; i < length; i++)
			{
				if(values[i] != null)
				if(!values[i].equals(tup.values[i]))
					return false;			
			}
			return true;
		}
		else
			return false;
	}
	//returns true if and only if values equals tup.values
	
	//Commands
	void setIthValue(Value val, int i)
	{
		int length = values.length;
		if((i < 0)||(i > length))
			System.out.println("575");
			//ERROR  insert error code here
		else
			values[i] = val;
	}
	//sets the ith value of tuple to val.  
	//It is an error if i is  not in the range 0..length() - 1
	
	//I/O
	public String toString()
	{
		String output = "";
		int length = values.length;
		for(int i = 0; i < length; i++)
		{
			output += values[i] + "\t";
		}
		return output;
	}
	//Concatenates all of the values.  The values are separated by one or more
	//blanks.  Each tuple is printed on a single line.
}


class Scheme 
{
	Attribute[] attrib;
	String ID = "";
	
	Scheme(Attribute[] new_attributes)
	{
		int length = new_attributes.length;
		attrib = new Attribute[length];
		boolean isnt_twice = true;
		
		for(int i = 0; i < length; i++)
		{
			Attribute temp = new_attributes[i];
			for(int j = 0; j < i; j++)
			{
				if(temp.equals(attrib[j]))
					isnt_twice = false;
			}
			if(isnt_twice)
				attrib[i] = temp;
			else
				System.out.println("619");
				//ERROR insert error code here
			}
		//Constructor that initializes attributes from new_attributes
		//It is an error if the same attribute appears twice in new_attributes.
	}
	
	Scheme(String s)//Assuming that the attributes aren't seperated by whitespace
	{
		int length = s.length();
		attrib = new Attribute[length];
		char[] chr = s.toCharArray();
		
		for(int i = 0; i < length; i++)
		{
			attrib[i] = new Attribute(chr[i]+"");
		}
	}
	
	Scheme(Scheme s)
	{
		attrib = s.attrib;
	}
	
	//Query Operations
	int getIndex(Attribute a)
	{
		int index = -1;
		int length = attrib.length;
		for(int i = 0; i < length; i++)
		{
			if(attrib[i].equals(a))
			{
				index = i;
				i = length; // essentially a break statement
			}
		}
		return index;
	}
	//Returns the index of attribute a in the scheme.  Returns -1 if a is not
	//in the scheme.
	
	int length()
	{
		return attrib.length;
	}
	//Return the number of attributes in the scheme
	
	boolean equals(Scheme scm)
	{
		int length = attrib.length;
		int length2 = scm.attrib.length;
		boolean isin = false;
		if(length == length2)
		{
			for(int i = 0; i < length; i++)
			{
				for(int j = 0; j < length2; j++)
				{
					if(attrib[i].equals(scm.attrib[j]))
						isin = true;
				}
				if(!isin)
					return false;
				isin = false;
			}
			return true;
		}
		else
			return false;
	}
	//Return true if and only if scm.attributes equals attributes
	
	Attribute getAttrAt(int indx)
	{
		int length = attrib.length;
		if((indx < 0)||(indx > length))
		{
			System.out.println("690");
			return null;
		}//ERROR  insert error code here
		else
			return attrib[indx];
	}
	//Returns the attribute at the location indicated by indx
	//It is an error if indx is not in the range 0..length()-1
	
	boolean isSubsetOf(Scheme superSet)
	{
		boolean isin = false;
		int length = attrib.length;
		int length2 = superSet.attrib.length;
		if(length <= length2)
		{
			for(int i = 0; i < length; i++)
			{
				for(int j = 0; j < length2; j++)
					if(attrib[i].equals(superSet.attrib[j]))
						isin = true;				
						
				if(isin == false)
					return false;
				isin = false;
			}
			return true;
		}
		else
			return false;
	}
	//returns true if and only if attributes is a subset of
	//superSet.attributes
	
	//I/O
	public String toString()
	{
		String output = "";
		int length = attrib.length;
		for(int i = 0; i < length; i++)
		{
			output += attrib[i].name + "\t";
		}
		return output;
	}
}


class Attribute {
	String name;
	
	//Constructors
	Attribute(String new_name)
	{
		name = new_name;
	}
	
	Attribute(Attribute a)
	{
		name = a.name;
	}
	
	//Query
	boolean equals(Attribute att)
	{
		if(att.name.equals(name))
			return true;
		else
			return false;
	}
	//returns true if and only if att.name equals name
	
	//I/O
	public String toString()
	{
		return name;
	}
}


class Value {
	String value;
	
	//Constructors
	Value(String new_value)
	{
		value = new_value;
	}
	
	Value(Value v)
	{
		value = v.value;
	}
	
	//Query
	boolean equals(Value v)
	{
		if(v.value.equals(value))
			return true;
		else
			return false;
	}
	//Returns true if and only if v.value equals value
	
	boolean lessThan(Value v)
	{
		if(v.value.compareTo(value) < 0)
			return true;
		else
			return false;
	}
	//Returns true if and only if v.value is less than value
	
	boolean lessThanOrEqual(Value v)
	{
		if(v.value.compareTo(value) <= 0)
			return true;
		else
			return false;
	}
	//Returns true if and only if v.value is less than or equal to value
	
	boolean greaterThan(Value v)
	{
		if(v.value.compareTo(value) > 0)
			return true;
		else
			return false;
	}
	//Returns true if and only if v.value greater than value
	
	boolean greaterThanOrEqual(Value v)
	{
		if(v.value.compareTo(value) >= 0)
			return true;
		else
			return false;
	}
	//Returns true if and only if v.value greater than of equal to value
	
	boolean notEqualTo(Value v)
	{
		if(v.value.compareTo(value) != 0)
			return true;
		else
			return false;
	}
	//Returns true if and only if v.value does not equal value
	
	//I/O
	public String toString()
	{
		return value;
	}
} 
