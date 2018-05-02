import java.io.*;

public class StudentDatabase
{
		int head = 0, current = 0, access = 0, max_size = 0;
		Student[] array;
		
    public StudentDatabase(int max_size) {
        array = new Student[max_size];
				this.max_size = max_size;
    } // StudentDatabase

    public void addStudent(String ssn, String name, String street, String city, String phone)
		{
			int temp = head;
      while(array[temp] != null)
			{
				temp++;
			}
			array[temp] = new Student(ssn, name, street, city, phone);
			current = temp;
    }// addStudent

    public void addCourse(String course)
		{
    	array[current].classes.addElement(course);
    } // addCourse

    public void dumpToFile(String filename) throws IOException
		{
     	PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("dump.txt")));
			for(int i = 0; i < max_size; i++)
			{
				outfile.println(array[i].toString());
			}
			//Run through the *cough* ARRAY, and print out in order each thing
    }// dumpToFile

    public String findName(String ssn)
		{
			for(int i = 0; i < max_size; i++)
			{
				if(ssn.equals(array[i].ssn))
					return array[i].name;
				access++;
			}
      //do a for from head on, checking the ssn against what is in each
			
			return "Name not Found";
    }//findName

    public void resetAccessCount()
		{
       access = 0;
    } // resetAccessCount

    public int getAccessCount()
		{
       return access;
    }// getAccessCount
} // class StudentDatabase

