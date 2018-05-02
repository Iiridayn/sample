/**************************************
CS235.class           1/28/2002
Michael Clark/Ged Crimsonclaw
**************************************/

import java.io.*;

public class CS235Lab2
{
  public static void main (String[] args) throws IOException, FileNotFoundException
	{
	  int i = 125;
		String sort = "m";
		String SSN, Name, Street, City, Phone, temp = "You don't see this";
		int Access;
		String infilename = "names.txt";
		
		try
		{
			i = Integer.parseInt(args[0]);
			sort = args[1];
		}
		catch(Exception e){}
		
		FileReader fr = new FileReader(infilename);
		BufferedReader infile = new BufferedReader(fr);
		
    Student db = new Student(); 
		
		SSN = infile.readLine();
	  while((i >= 0)&&(temp != null))
	  {
	    Name = infile.readLine();
			Street = infile.readLine();
			City = infile.readLine();
			Phone = infile.readLine();
			db.AddStudent(SSN, Name, Street, City, Phone);
			
			while(temp != null)
			{
			  temp = infile.readLine();
			  if(temp == null);
			  else if(Character.isDigit(temp.charAt(0)))
			  {
				  SSN = temp;
					break;
				}
	  		else							
	  	    db.AddClass(temp);			
		  }
			i--;
		}
		infile.close();
		
		db.SortByNumber(sort);
		
		Access = db.GetAccessCount();
		
		System.out.println("No. of Accesses: " + Access);
		
  }
}
