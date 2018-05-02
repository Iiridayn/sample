public class Student
{
	//The student class is an object, and stores verious information
	//about a student.
	
	String ssn, name, street, city, phone;
	CNode head, last;
	
	Student(String ssn, String name, String street, String city, String phone)
	{//sets up the student, according to the passed in values
		this.ssn = ssn;
		this.name = name;
		this.street = street;
		this.city = city;
		this.phone = phone;
		head = null;
		last = null;
		//initialize globals
	}//Student
	
	public void addCourse(String course)
	{
		if(head == null)
		{	
			head = new CNode();
			head.c = new Course(course);
			last = head;
		}
		else
		{
			last.next = new CNode();
			last = last.next;
			last.c = new Course(course);
		}		
	}
	
	public String toString()
	{//Returns a string representatin of this object
		String output = "";
		output += ssn + "\r\n" + name + "\r\n" + street + "\r\n" + city + "\r\n" + phone + "\r\n";
		CNode temp = head;
		while(temp != null)
		{
			output += temp.c + "\r\n";
			temp = temp.next;
		}
		
		return output;
		//concat all information into one big, "\n\r" (Windows)
		//separated string, and pass it back
	}//toString
}

class CNode
{
	Course c;
	CNode next;
}

class Course
{
	String course;
	Course(String c)
	{
		course = c;
	}
	public String toString()
	{
		return course;
	}
}
