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

    public String findName(String ssn)
		{//returns the name to match the ssn, or an appropriate error.
			for(SNode temp = head; temp != null; temp = temp.next)
			{
				Access++;
				if(temp.s.ssn.equals(ssn))
					return temp.s.name;
			}
			return null;
			//searches the array iterativly for the ssn
    }//findName
		
		public void deleteSSN(String ssn)
		{
			SNode temp2 = null;
			for(SNode temp = head; temp != null; temp2 = temp, temp = temp.next)
			{
				if(temp.s.ssn.equals(ssn))
				{
					if(temp2 == null)
						head = head.next;
					else
						temp2.next = temp.next;
					
					return;
				}
			}			
		}

    public void resetAccessCount()
		{//resets the # of accesses
    	Access = 0;
    }//resetAccessCount

    public int getAccessCount() 
		{//returns the current # of accesses
    	return Access;
    }//getAccessCount
		
		public long sort()
		{
			long start = System.currentTimeMillis();
			head = MergeSort(head);
			return System.currentTimeMillis() - start;
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
				pSecondCell.next = split(pSecondCell.next);
				return pSecondCell;
			}
		}
		
		private SNode merge(SNode list1, SNode list2)
		{
		   if (list1 == null) return list2;
			else if (list2 == null) return list1;
			else if (list1.s.ssn.compareTo(list2.s.ssn) <= 0) {
			  comps++;
				list1.next = merge(list1.next, list2);
				return list1;
			}
			else {
				comps++;
			  list2.next = merge(list1, list2.next);
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
	Student s;
}

