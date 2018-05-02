import java.io.*;
import java.lang.IllegalArgumentException;

public class StudentDatabase
{
	//The StudentDatabase class handles the Student objects
	//in a predefined, abstracted manner.  It contains methods
	//for sorting, finding, and inserting Student objects into
	//the database.
	SNode[] table;
  Student last;
	int Access = 0, comps = 0, max = 101, students = 0; //max = 89 is best efficiancy
	
	public StudentDatabase()
	{//Constructor.
		table = new SNode[max];
		last = null;
		//Initializes globals.
	}//StudentDatabase
	
	public void addStudent(String ssn, String name, String street, String city, String phone)
	{//Adds a new student object with the attributes passed in to it.
		Student temp = last = new Student(ssn, name, street, city, phone);
		students++;
		addStudent(temp);
	}//addStudent
	
	public void addStudent(Student temp)
	{//Adds a new student object with the attributes passed in to it.
		if(((float)students/max) > 1.0)
			rehash();		
		int code = hashgen(temp.ssn);
		if(table[code] == null)
		{
			table[code] = new SNode();
			table[code].s = temp;
		}
		else
		{
			SNode now;
			for(now = table[code]; now.next != null; now = now.next);
			now.next = new SNode();
			now.next.s = temp;
		}
		//puts a new student at end of list
	}//addStudent
	
	public void addCourse(String course)
	{//adds a course to the last created student
		last.addCourse(course);
		//throws a course onto the vector internal to the Student object
	}//addCourse
	
	public void dumpToFile() throws IOException
	{//Outputs the database in the same format it was read in
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("dump.txt")));
		for(int i = 0; i < max; i++)
		{
			//outfile.println("Bucket " + (i+1) + ":");
			for(SNode temp = table[i]; temp != null; temp = temp.next)
			{
				outfile.print(temp.s);
			}			
		}
		outfile.close();
		//iterates across the array, dumping the data to file
	}//dumpToFile
	
	public String findName(String ssn)
	{//returns the name to match the ssn, or an appropriate error.
		int code = hashgen(ssn);
		if(table[code] != null)
			for(SNode temp = table[code]; temp != null; temp = temp.next)
			{
				Access++;
				if(temp.s.ssn.equals(ssn))
					return temp.s.name;
			}
		return null;		
	}//findName
	
	public void deleteBySSN(String ssn)
	{
		int code = hashgen(ssn);
		if(table[code] == null)
			return;
		else
		{
			for(SNode temp = table[code], tempp = null; temp != null; tempp = temp, temp = temp.next)
			{
				if(temp.s.ssn.equals(ssn))
				{	
					if(tempp == null)
						table[code] = temp.next;
					else
						tempp.next = temp.next;
					students--;
					return;
				}	
			}
			return;
		}
	}//deleteBySSN
	
	public void resetAccessCount()
	{//resets the # of accesses
		Access = 0;
	}//resetAccessCount
	
	public int getAccessCount() 
	{//returns the current # of accesses
		return Access;
	}//getAccessCount
	
	private int hashgen(String hash)
	{
		int hashed = 1;
		int C = max;
		for(int i = 0; i < 9; i++)
		{
			hashed = hashed*C + Integer.parseInt("" + hash.charAt(i));
		}
		int Hcode = Math.abs(hashed%(max));
		return Hcode;
	}
	
	private void rehash()
	{
		int tempi = max;
		max = (max * 2) + 1;
		SNode[] tempt = table;
		table = new SNode[max];		
		for(int i = 0; i < tempi; i++)
		{
			for(SNode tempn = tempt[i]; tempn != null; tempn = tempn.next)
			{
				addStudent(tempn.s);
			}			
		}
	}//rehash
	
	public float studentsOverUsed()
	{
		int used = 0;
		for(int i = 0; i < max; i++)
			if(table[i] != null)
				used++;
		return (float)students/used;
	}
} // class StudentDatabase

class SNode
{
	SNode next;
	Student s;
}

