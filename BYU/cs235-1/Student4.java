/*********************************************************
|                  StudentList.class                    |
|         (c) Ged Crimsonclaw, 1/28/2002 - 2/5/2002     |
|               Handles the tries                       |
*********************************************************/

import java.io.*;

public class Student4
{
  public STrie init;
	private STrie current;
	private int Access;	
	
	Student4()
	{
	  init = new STrie("0","","","","");
		current = init;
	}
	
	public void AddStudent(String SSN, String Name, String Street, String City, String Phone, int depth, STrie adder)
	{
	  if(depth == 8)
		{
			STrie node = new STrie (SSN, Name, Street, City, Phone);
			if(adder.down[Integer.parseInt("" + SSN.charAt(8))] == null)
			{
				adder.down[Integer.parseInt("" + SSN.charAt(8))] = node;
				current = adder.down[Integer.parseInt("" + SSN.charAt(8))];
			}
			else
				System.out.println("Error!  More than on instance of the SSN " + SSN);
		}
		else
		{
			if(adder.down[Integer.parseInt("" + SSN.charAt(depth))] == null)
				adder.down[Integer.parseInt("" + SSN.charAt(depth))] = new STrie("0","","","","");
			AddStudent(SSN, Name, Street, City, Phone, depth + 1, adder.down[Integer.parseInt("" + SSN.charAt(depth))]);
		}			
	}
	
	public void AddClass(String course)
	{
	  current.AddClass(course);
	}
	
	public void DumpToFile() throws IOException
	{
		String filename = "dump.txt";
	  FileWriter fr = new FileWriter(filename);
		BufferedWriter br = new BufferedWriter(fr);
		PrintWriter outfile = new PrintWriter (br);
		
		DumpToFile(init, outfile);
		
		outfile.close();
	}
	
	private void DumpToFile(STrie initi, PrintWriter outfile)
	{
	  STrie search = initi;
		
		if(!search.SSN.equals("0"))
		{
			outfile.println(search.SSN);
			outfile.println(search.Name);
			outfile.println(search.Street);
			outfile.println(search.City);
			outfile.println(search.Phone);
			
			
			STrie.Course temp = search.list;
			if (temp != null)
				while(temp != null)
				{
					outfile.println(temp.classGrade);
					temp = temp.next;
				}//end while
		}
			
		for(int i = 0; i < 10; i++)
		{
			if(search.down[i] != null)
				DumpToFile(search.down[i], outfile);
		}		
	}
	
	public String GetName(String ssn, int depth, STrie initi)
	{
	  STrie search = initi;
		
		if (depth == 9)
		{
			Access += depth;
			return search.Name;
		}
		else if(search.down[Integer.parseInt("" + ssn.charAt(depth))] != null)
			return GetName(ssn, depth + 1, search.down[Integer.parseInt("" + ssn.charAt(depth))]);
		else
			return "Name not found";
	}
	
	public void ResetAccessCount()
	{
	  Access = 0;
	}
	
	public int GetAccessCount()
	{
	  return Access;
	}
	
	
	
	private class STrie
	{
	  String SSN, Name, Street, City, Phone;
		STrie[] down;
		Course list;
		
		STrie(String SSN, String Name, String Street, String City, String Phone)
		{
		  this.Name = Name;
		  this.SSN = SSN;
  		this.Street = Street;
  		this.City = City;
  		this.Phone = Phone;
			down = new STrie[10];
			for(int i = 0; i < 10; i++)
				down[i] = null;
			list = null;
		}
		
		private void AddClass(String course)
	  {
			Course node = new Course(course);
		  Course current;
		  if (list == null)
		  	list = node;
			else
	  	{
			  current = list;
	  		while(current.next != null)
		  		current = current.next;
		  	current.next = node;
		  }
	  }
		
		
		private class Course
		{
		  String classGrade;
			Course next;
			
			Course(String course)
			{
			  classGrade = course;
				next = null;
			}
		}//end of class in class in class...
		
		
	}//end of internal class
	
	
}
