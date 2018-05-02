import java.io.*;
import java.lang.IllegalArgumentException;

public class StudentDatabase
{
	//The StudentDatabase class handles the Student objects
	//in a predefined, abstracted manner.  It contains methods
	//for sorting, finding, and inserting Student objects into
	//the database.
	SNode root, last;
	int Access = 0, comps = 0;
	
	public StudentDatabase()
	{//Constructor.
		root = null;
		last = null;
		//Initializes globals.
	}//StudentDatabase
	
	public void addStudent(String ssn, String name, String street, String city, String phone)
	{//Adds a new student object with the attributes passed in to it.
		Student temp = new Student(ssn, name, street, city, phone);
		root = addStudent(root, temp);
		//puts a new student at end of list
	}//addStudent
	
	private SNode addStudent(SNode temp, Student temps)
	{
		if(temp == null)
		{
			SNode node = new SNode();
			node.s = temps;
			last = node;
			return node;
		}
		else
		{
			int comped = temps.ssn.compareTo(temp.s.ssn);
			if(comped > 0)
				temp.right = addStudent(temp.right, temps);
			else if(comped < 0)
				temp.left = addStudent(temp.left, temps);
			else
				throw new IllegalArgumentException("SSN inserted twice");
		}	
		return temp;
	}
	
	public void addCourse(String course)
	{//adds a course to the last created student
		last.s.addCourse(course);
		//throws a course onto the vector internal to the Student object
	}//addCourse
	
	public void dumpToFile() throws IOException
	{//Outputs the database in the same format it was read in
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("dump.txt")));
		dumpToFile(root, outfile);
		outfile.close();
		//iterates across the array, dumping the data to file
	}//dumpToFile
	
	private void dumpToFile(SNode temp, PrintWriter outfile) throws IOException
	{
		if(temp != null)
		{
			dumpToFile(temp.left, outfile);
			outfile.print(temp.s);
			dumpToFile(temp.right, outfile);
		}
	}
	
	public void reverseDumpToFile() throws IOException
	{//Outputs the database in the same format it was read in
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("dumpR.txt")));
		reverseDumpToFile(root, outfile);
		outfile.close();
		//iterates across the array, dumping the data to file
	}//reverseDumpToFile
	
	private void reverseDumpToFile(SNode temp, PrintWriter outfile) throws IOException
	{
		if(temp != null)
		{
			reverseDumpToFile(temp.right, outfile);
			outfile.print(temp.s);
			reverseDumpToFile(temp.left, outfile);
		}
	}
	
	public String findName(String ssn)
	{//returns the name to match the ssn, or an appropriate error.
		return findName(ssn, root);
		//searches the array iterativly for the ssn
	}//findName
	
	private String findName(String ssn, SNode temp)
	{
		if(temp != null)
		{
			int comped = temp.s.ssn.compareTo(ssn);
			Access++;
			if(comped == 0)
				return temp.s.name;
			else if(comped > 0)
				return findName(ssn, temp.left);
			else if(comped < 0)
				return findName(ssn, temp.right);
		}
		return null;
	}
	
	public void deleteBySSN(String ssn)
	{
		root = deleteBySSN(ssn, root);
	}
	
	private SNode deleteBySSN(String ssn, SNode temp)
	{
		if(temp != null)
		{
			int comped = temp.s.ssn.compareTo(ssn);
			if(comped == 0)
			{
				if(temp.left == null)
					return temp.right;
				else if(temp.right == null)
					return temp.left;
				else
				{
					SNode temp1, temp2;
					for(temp1 = temp.right, temp2 = null; temp1.left != null; temp2 = temp1, temp1 = temp1.left);
					if(temp2 != null)
					{
						temp2.left = temp1.right;
						temp1.right = temp.right;
					}
					temp1.left = temp.left;					
					return temp1;
				}
			}
			else if(comped > 0)
				temp.left = deleteBySSN(ssn, temp.left);
			else if(comped < 0)
				temp.right = deleteBySSN(ssn, temp.right);
			return temp;
		}
		return null;
	}
	
	public void resetAccessCount()
	{//resets the # of accesses
		Access = 0;
	}//resetAccessCount
	
	public int getAccessCount() 
	{//returns the current # of accesses
		return Access;
	}//getAccessCount
} // class StudentDatabase

class SNode
{
	SNode right;
	SNode left;
	Student s;
}

