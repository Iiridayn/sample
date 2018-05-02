import java.util.Vector;

public class Student
{
	String ssn, name, street, city, phone;
	Vector classes = new Vector();
	
	Student(String ssn, String name, String street, String city, String phone)
	{
		this.ssn = ssn;
		this.name = name;
		this.street = street;
		this.city = city;
		this.phone = phone;
	}
	
	public String toString()
	{
		String output = "";
		output += ssn + "\n" + name + "\n" + street + "\n" + city + "\n" + phone + "\n";
		output += classes.toString();
		
		/*int temp_size = classes.size();
		for(int i = 0; i < temp_size; i++)
		{
			output += 
		}*/
		//concat all vector junk
		
		return output;
	}	
}
