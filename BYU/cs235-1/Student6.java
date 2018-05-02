/*********************************************************
|                  StudentList.class                    |
|         (c) Ged Crimsonclaw, 1/28/2002 - 2/5/2002     |
|               Handles the binary tree                 |
*********************************************************/

import java.io.*;

public class Student6
{
  private SHash[] main;
	private SHash current;
	private int Access;	
	private int buckets;
	
	Student6(int x)
	{
	  main = new SHash[x];
		current = main[0];
		buckets = x;
	}
	
	public void AddStudent(String SSN, String Name, String Street, String City, String Phone)
	{
		int Hcode = Hash(SSN);
				
		if(main[Hcode] == null)
		{
			main[Hcode] = new SHash(SSN, Name, Street, City, Phone);
			current = main[Hcode];
		}
		else
		{	
			SHash search = main[Hcode];
			while(search.next != null)
			{
				search = search.next;
			}
			
			search.next = new SHash(SSN, Name, Street, City, Phone);
			current = search.next;
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
		
		for(int i = 0; i < buckets; i++)
		{
			SHash search = main[i];		
			while(search != null)
			{
				outfile.println(search.SSN);
				outfile.println(search.Name);
				outfile.println(search.Street);
				outfile.println(search.City);
				outfile.println(search.Phone);
				
				SHash.Course temp = search.list;
				if (temp != null)
					while(temp != null)
					{
						outfile.println(temp.classGrade);
						temp = temp.next;
					}//end while, never had to change
					
					search = search.next;
			}//end inner while
			
		}//end for
		outfile.close();
	}
	
	public String GetName(String ssn)
	{
	  int Hcode = Hash(ssn);
		SHash search = main[Hcode];		
		
		while(search != null)
		{
			Access++;
			if(search.SSN.equals(ssn))
				return search.Name;
			search = search.next;
		}
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
	
	//mod by prime # !!!!!!!!!  That is what lost me my points
	private int Hash(String hash)
	{
		int hashed = 1;
		int C = buckets/100;
		for(int i = 0; i < 9; i++)
		{
			hashed = hashed*C + Integer.parseInt("" + hash.charAt(i));
		}
		int Hcode = Math.abs(hashed%(buckets-1));
		//System.out.println("" + Hcode);
		return Hcode;
	}
	
	
	private class SHash
	{
	  String SSN, Name, Street, City, Phone;
		SHash next;
		Course list;
		
		SHash(String SSN, String Name, String Street, String City, String Phone)
		{
		  this.Name = Name;
		  this.SSN = SSN;
  		this.Street = Street;
  		this.City = City;
  		this.Phone = Phone;
			next = null;
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
