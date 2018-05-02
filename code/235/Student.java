import java.util.Vector;

public class Student
{
	//The student class is an object, and stores verious information
	//about a student.
	
	String ssn, name, street, city, phone;
	Vector courses;
	
	Student(String ssn, String name, String street, String city, String phone)
	{//sets up the student, according to the passed in values
		this.ssn = ssn;
		this.name = name;
		this.street = street;
		this.city = city;
		this.phone = phone;
		courses = new Vector();
		//initialize globals
	}//Student
	
	public String toString()
	{//Returns a string representatin of this object
		String output = "";
		output += ssn + "\n" + name + "\n" + street + "\n" + city + "\n" + phone + "\n";
		
		int temp_size = courses.size();
		for(int i = 0; i < temp_size; i++)
		{
			output += courses.elementAt(i) + "\n";
		}
		
		return output;
		//concat all information into one big, "\n\r" (Windows)
		//separated string, and pass it back
	}//toString
}
