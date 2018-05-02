/*********************************************************
 |                  StudentList.class                    |
 |         (c) Ged Crimsonclaw, 1/28/2002 - 2/5/2002     |
 |               Handles the linked lists                |
 *********************************************************/

import java.io.*;

public class Student
{
  private SList init, current;
	private int Access;	
	
	Student()
	{
	  init = null;
	}
	
	public void AddStudent(String SSN, String Name, String Street, String City, String Phone)
	{
	  SList node = new SList (SSN, Name, Street, City, Phone);
				
		if (init == null)
		{
			init = node;
			current = node;
		}
		else
		{
			current = init;
			while(current.next != null)
				current = current.next;
			current.next = node;
		}
	}
	
	public void AddClass(String course)
	{
	  if (current.next == null)
		  current.AddClass(course);
		else
		  current.next.AddClass(course);
  }
	
	public void DumpToFile() throws IOException
	{
	  SList search = init;
		String filename = "dump.txt";
	  FileWriter fr = new FileWriter(filename);
		BufferedWriter br = new BufferedWriter(fr);
		PrintWriter outfile = new PrintWriter (br);
    
		boolean stop = false, stopinner = false;
		
		while(stop == false)
		{
		  outfile.println(search.SSN);
			outfile.println(search.Name);
			outfile.println(search.Street);
			outfile.println(search.City);
			outfile.println(search.Phone);
			
			SList.Course temp = search.list;
			if (temp != null)
			  while(stopinner == false)
			  {
		  	  outfile.println(temp.classGrade);
	  		  if(temp.next == null)
					  stopinner = true;
					else
				    temp = temp.next;
				}//end inner while
			stopinner = false;
			if(search.next == null)
			  stop = true;
			else
			  search = search.next;
		}//end outer while
		
		outfile.close();
	}
	
	public String GetName(String ssn)
	{
	  SList search = init;
		
		while(search.next != null)
		{
		  Access++;
		  
			if(ssn.compareTo(search.SSN) == 0)
			  return search.Name;
			else
			  search = search.next;
		}
		return null;
	}
	
	public void ResetAccessCount()
	{
	  Access = 0;
	}
	
	public int GetAccessCount()
	{
	  return Access;
	}
	
	public void SortByNumber(String type)
	{
	  if (type.charAt(0) == 'm')
		  MergeSort(init);
		else if (type.charAt(0) == 's')
		  selectionSort(init);
	}
	
	//Very heavily modified version of code found in Section 2.2
	private void selectionSort(SList list)
	{
	  SList small, temp, i, j;
		while (list != null)
		{
		  small = j = list;
			temp = null;
			i = list.next;
			while(i != null)
			{
			  Access++;
				if (i.SSN.compareTo(small.SSN) <= 0)
				{
				  small = i;
					temp = j;
				}
				j = i;
				i = i.next;
			}
			if (temp != null)
			{
			  temp.next = small.next;
			  small.next = list;
			}
			list = small;
			list = list.next;
		}	
	}
	
	//All taken directly from Section 2.8,  slight mods
	private SList MergeSort(SList list)
	{
	  SList SecondList;
				
		if(list  == null) return null;
		else if (list.next == null) return list;
		else {
			SecondList = split(list);
			return merge(MergeSort(list), MergeSort(SecondList));
		}
	}
	
	private SList split(SList list)
	{
	  SList pSecondCell;
		
		if (list == null) return null;
		else if (list.next == null) return null;
		else {
		  pSecondCell = list.next;
		  list.next = pSecondCell.next;
			pSecondCell.next = split(pSecondCell.next);
			return pSecondCell;
		}
	}
	
	private SList merge(SList list1, SList list2)
	{
	  Access++;
	  if (list1 == null) return list2;
		else if (list2 == null) return list1;
		else if (list1.SSN.compareTo(list2.SSN) <= 0) {
		  list1.next = merge(list1.next, list2);
			return list1;
		}
		else {
		  list2.next = merge(list1, list2.next);
			return list2;
		}
	}
	
	
	
	private class SList
	{
	  String Name, SSN, Street, City, Phone;
		SList next;
		Course list;
		
		SList(String SSN, String Name, String Street, String City, String Phone)
		{
		  this.Name = Name;
		  this.SSN = SSN;
  		this.Street = Street;
  		this.City = City;
  		this.Phone = Phone;
			next = null;
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
