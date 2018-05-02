import java.io.*;
import java.lang.IllegalArgumentException;

public class StudentDatabase
{
//The StudentDatabase class handles the Student objects
//in a predefined, abstracted manner.  It contains methods
//for sorting, finding, and inserting Student objects into
//the database.
		int max, head = 0, curr = 0, Access = 0, comps = 0;
		Student pps[];
		
		public StudentDatabase(int max_size)
		{//Constructor.  Takes the max size for the array.
      max = max_size;
			pps = new Student[max];
			//Initializes globals.
    }//StudentDatabase

    public void addStudent(String ssn, String name, String street, String city, String phone)
		{//Adds a new student object with the attributes passed in to it.
    	for(curr = head; curr < max; curr++)
			{
				if(pps[curr] == null)
					break;
			}
			if(curr != max)
				pps[curr] = new Student(ssn, name, street, city, phone);
			else
			{
				IllegalArgumentException e = new IllegalArgumentException("Too many Students");
				throw e;
			}
			//puts a new student at end of array
    }//addStudent

    public void addCourse(String course_info)
		{//adds a course to the last created student
      pps[curr].courses.add(course_info);			
			//throws a course onto the vector internal to the Student object
    }//addCourse

    public void dumpToFile() throws IOException
		{//Outputs the database in the same format it was read in
    	PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("dump.txt")));
			
			for(int i = head; i < max; i++)
			{
				outfile.print(pps[i]);
			}
			outfile.close();
			//iterates across the array, dumping the data to file
    }//dumpToFile

    public String findName(String ssn)
		{//returns the name to match the ssn, or an appropriate error.
			for(int i = head; i < max; i++)
			{
				Access++;
				if(pps[i].ssn.equals(ssn))
					return pps[i].name;
			}
			return "Name not Found";
			//searches the array iterativly for the ssn
    }//findName

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
			switch(type)
			{
				case 's':
					return selectionSort();
				case 'i':
					return insertionSort();
				case 'b':
					return bubbleSort();
				default:
					IllegalArgumentException e = new IllegalArgumentException("Bad sort char");
					throw e;
			}
		}
		
		public long selectionSort()
		{//preforms a selection sort on the database
			//start and timer, then do the sort
			long time = System.currentTimeMillis();
			Student temp;
			for(int i = 0; i < max - 1; i++)
			{
				int small = i;
				for(int j = i+1; j < max; j++)
				{
					comps++;
					if(pps[j].ssn.compareTo(pps[small].ssn) < 0)
						small = j;
				}
				temp = pps[small];
				pps[small] = pps[i];
				pps[i] = temp;
			}
			//iterate through the list, finding the smallest
			//element and putting that first.  Then ignore the
			//smallest part, and continue doing the same until
			//only one element remains.			
			return System.currentTimeMillis() - time;
			//stop timer and return the beginning - the end
		}//selectionSort
		
		public long insertionSort()
		{//preforms an insertion sort on the database
			//start and timer, then do the sort
			long time = System.currentTimeMillis();
			Student temp = null, temp2;
			boolean placed = false;
			for(int i = head+1; i < max; i++)
			{
				placed = false;
				for(int j = head; j <= i; j++)
				{
					if(!placed)
					{
						comps++;
						if(pps[i].ssn.compareTo(pps[j].ssn) < 0)
						{
							temp = pps[j];
							pps[j] = pps[i];
							placed = true;
						}
					}
					else
					{
						if(j != i)
						{
							temp2 = pps[j];
							pps[j] = temp;
							temp = temp2;
						}
						else
						{
							pps[j] = temp;
							break;
						}						
					}//else
				}//internal for
			}//for
			//iterate through the list, grabbing the next element
			//and sorting it with the previously sorted elements
			//until at end of list
			return System.currentTimeMillis() - time;
			//stop timer and return the beginning - the end
		}//insertionSort
		
		public long bubbleSort()
		{//preforms a bubble sort on the database
			//start and timer, then do the sort
			long time = System.currentTimeMillis();
			boolean swapped = false;
			Student temp;
			for(int i = head; i < max; i++)
			{
				swapped = false;
				for(int j = head; j < max-1; j++)
				{
					comps++;
					if(pps[j+1].ssn.compareTo(pps[j].ssn) < 0)
					{
						temp = pps[j];
						pps[j] = pps[j+1];
						pps[j+1] = temp;
						swapped = true;
					}
				}
				if(!swapped)
					break;
			}
			//compare all elements, from first to last, with
			//their upper neighbor, sorting where neccisary.
			//If no swaps were made, break out of the for loop
			return System.currentTimeMillis() - time;
			//stop timer and return the beginning - the end
		}//bubbleSort
		
		public void resetComparisons()
		{//resets the # of Comparisons
			comps = 0;
		}//resetComparisons
		
		public int getComparisons()
		{//returns the # of Comparisons
			return comps;
		}//getComparisons
		
		public String findNameBinarySearch(String ssn)
		{//preforms a binary search, looking for the ssn
			boolean go = true;
			int top = max-1;
			int bottom = head;
			while(go)
			{
				if(top < bottom)
					go = false;
				else
				{
					Access++;
					int half = (top + bottom)/2;
					int compare = pps[half].ssn.compareTo(ssn);
					if(compare == 0)
					{
						return pps[half].name;
					}
					else if(compare > 0)
						top = half-1;
					else if(compare < 0)
						bottom = half+1;
				}//else
			}//while
			return null;
			//splits list in half, going for the half with
			//the variable in it.  Does this intil only one
			//item left.  This is either it, or it is not to
			//be found.  (Could mebby check to see if is == ?)
		}//findNameBinarySearch
} // class StudentDatabase

