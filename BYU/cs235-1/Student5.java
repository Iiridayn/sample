/*********************************************************
|                  StudentList.class                    |
|         (c) Ged Crimsonclaw, 1/28/2002 - 2/5/2002     |
|               Handles the binary tree                 |
*********************************************************/

import java.io.*;

public class Student5
{
  public BTree init;
	private BTree current;
	private int Access;	
	
	Student5()
	{
	  init = null;
		current = init;
	}
	
	public void AddStudent(String SSN, String Name, String Street, String City, String Phone, BTree search)
	{
		if(search == null)
		{
			BTree node = new BTree(SSN, Name, Street, City, Phone);
			init = node;
			current = init;
		}
		else
		{
			int faster = SSN.compareTo(search.SSN);
			
			if(faster < 0)
				if(search.left == null)
				{
					BTree node = new BTree(SSN, Name, Street, City, Phone);
					search.left = node;
					current = search.left;
				}
				else
					AddStudent(SSN, Name, Street, City, Phone, search.left);
			else if(faster > 0)
				if(search.right == null)
				{
					BTree node = new BTree(SSN, Name, Street, City, Phone);
					search.right = node;
					current = search.right;
				}
				else
					AddStudent(SSN, Name, Street, City, Phone, search.right);
			else
				System.out.println("Error!  More than on instance of the SSN " + SSN);
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
	
	private void DumpToFile(BTree search, PrintWriter outfile)
	{
		if(search != null)
		{
			DumpToFile(search.left, outfile);
			
			outfile.println(search.SSN);
			outfile.println(search.Name);
			outfile.println(search.Street);
			outfile.println(search.City);
			outfile.println(search.Phone);
						
			BTree.Course temp = search.list;
			if (temp != null)
				while(temp != null)
				{
					outfile.println(temp.classGrade);
					temp = temp.next;
				}//end while
				
			DumpToFile(search.right, outfile);
		}//end if
	}
	
	
	public String GetName(String ssn, int times, BTree search)
	{
	  int faster = ssn.compareTo(search.SSN);
		
		if(faster == 0)
		{
			Access += times;
			return search.Name;
		}
		else if((faster < 0)&&(search.left != null))
			return GetName(ssn, times + 1, search.left);
		else if((faster > 0)&&(search.right != null))
			return GetName(ssn, times + 1, search.right);
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
	
	
	
	private class BTree
	{
	  String SSN, Name, Street, City, Phone;
		BTree left, right;
		Course list;
		
		BTree(String SSN, String Name, String Street, String City, String Phone)
		{
		  this.Name = Name;
		  this.SSN = SSN;
  		this.Street = Street;
  		this.City = City;
  		this.Phone = Phone;
			left = right = null;
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
