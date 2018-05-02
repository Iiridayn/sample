import java.io.*;
import java.lang.IllegalArgumentException;

public class StudentDatabase
{
	//The StudentDatabase class handles the Student objects
	//in a predefined, abstracted manner.  It contains methods
	//for sorting, finding, and inserting Student objects into
	//the database.
	SNode head, last;
	int Access = 0, comps = 0;
	
	public StudentDatabase()
	{//Constructor.
		head = null;
		last = null;
		//Initializes globals.
	}//StudentDatabase
	
	public void addStudent(String ssn, String name, String street, String city, String phone)
	{//Adds a new student object with the attributes passed in to it.
		if(last == null)
		{
			head = new SNode();
			head.s = new Student(ssn, name, street, city, phone);
			last = head;
		}
		else
		{
			last.next = new SNode();
			last.next.prev = last;
			last = last.next;				
			last.s = new Student(ssn, name, street, city, phone);
		}
		//puts a new student at end of list
	}//addStudent
	
	public void addCourse(String course)
	{//adds a course to the last created student
		last.s.addCourse(course);
		//throws a course onto the vector internal to the Student object
	}//addCourse
	
	public void dumpToFile() throws IOException
	{//Outputs the database in the same format it was read in
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("dump.txt")));
		
		for(SNode temp = head; temp != null; temp = temp.next)
		{
			outfile.print(temp.s);
		}
		outfile.close();
		//iterates across the array, dumping the data to file
	}//dumpToFile
	
	public void reverseDumpToFile() throws IOException
	{//Outputs the database in the same format it was read in
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("dumpR.txt")));
		
		for(SNode temp = last; temp != null; temp = temp.prev)
		{
			outfile.print(temp.s);
		}
		outfile.close();
		//iterates across the array, dumping the data to file
	}//reverseDumpToFile
	
	public String findName(String ssn)
	{//returns the name to match the ssn, or an appropriate error.
		Student temp = deleteBySSN(ssn);
		if(temp == null)
			return null;
		else
			return temp.name;
		//searches the array iterativly for the ssn
	}//findName
	
	public Student deleteBySSN(String ssn)
	{
		SNode temp2 = null;
		Student delete = null;
		for(SNode temp = head; temp != null; temp2 = temp, temp = temp.next)
		{
			if(temp.s.ssn.equals(ssn))
			{
				delete = temp.s;
				/*if(temp2 == null)
				{
					delete = head.s;
					head = head.next;
					if(head != null)
						head.prev = null;
					else
						last = null;
				}
				else
				{
					delete = temp2.next.s;
					temp2.next = temp.next;
					if(temp2.next != null)
						temp2.next.prev = temp2;
					else
						last = temp2;
				}*/
				
				return delete;
			}
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
	
	public long sort(char type)
	{
		long start = System.currentTimeMillis();
		switch(type)
		{
		case 'm':
			head = MergeSort(head);
			if(head != null)
			{	
				for(last = head; last.next != null; last = last.next);
				head.prev = null;
			}
			else
				last = null;
			break;
		case 's':
			selectionSort(head);
			if(head == null)
				last = null;
			else
				head.prev = null;
			break;
		default:
			IllegalArgumentException e = new IllegalArgumentException("Bad sort char");
			throw e;
		}
		return System.currentTimeMillis() - start;
	}
	
	private void selectionSort(SNode list)
	{
		SNode small, i;
		boolean swap = false;
		while (list != null)
		{
			small = list;
			i = list.next;
			
			while(i != null)
			{
				Access++;
				if (i.s.ssn.compareTo(small.s.ssn) < 0)
				{
					small = i;
					swap = true;
				}
				i = i.next;
			}
			
			if((swap)&&(small != list))
			{
				small.prev.next = small.next;
				if(small.next != null)
					small.next.prev = small.prev;
				else
					last = small.prev;
				small.next = list;
				small.prev = list.prev;
				if(small.prev != null)
					small.prev.next = small;
				else
					head = small;
				small.next.prev = small;
			}
			
			list = small;				
			list = list.next;
		}	
	}
	
	
	private SNode MergeSort(SNode list)
	{
		SNode SecondList;
		
		if(list == null) return null;
		else if (list.next == null) return list;
		else {
			SecondList = split(list);
			return merge(MergeSort(list), MergeSort(SecondList));
		}
	}
	
	private SNode split(SNode list)
	{
		SNode pSecondCell;
		
		if (list == null) return null;
		else if (list.next == null) return null;
		else {
			pSecondCell = list.next;
			list.next = pSecondCell.next;
			if(list.next != null)
				list.next.prev = list;
			SNode temp = split(pSecondCell.next);
			pSecondCell.next = temp;
			if(temp != null)
				temp.prev = pSecondCell;
			return pSecondCell;
		}
	}
	
	private SNode merge(SNode list1, SNode list2)
	{
		SNode temp;
		if (list1 == null) return list2;
		else if (list2 == null) return list1;
		else if (list1.s.ssn.compareTo(list2.s.ssn) <= 0) {
			comps++;
			temp = merge(list1.next, list2);
			list1.next = temp;
			temp.prev = list1;
			return list1;
		}
		else {
			comps++;
			temp = merge(list1, list2.next);
			list2.next = temp;
			temp.prev = list2;
			return list2;
		}
	}
	
	public void resetComparisons()
	{//resets the # of Comparisons
		comps = 0;
	}//resetComparisons
	
	public int getComparisons()
	{//returns the # of Comparisons
		return comps;
	}//getComparisons
} // class StudentDatabase

class SNode
{
	SNode next;
	SNode prev;
	Student s;
}

