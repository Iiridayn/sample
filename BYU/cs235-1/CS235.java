/**************************************
CS235.class           1/28/2002
Michael Clark/Ged Crimsonclaw
**************************************/

import java.io.*;

public class CS235
{
  public static void main (String[] args) throws IOException, FileNotFoundException
	{
	  final int TRY = 5000;
		String SSN, Name, Street, City, Phone, temp = "You don't see this";
		int count = 0, Access = 0, i = 0;
		String outfilename = "found.txt", infilename1 = "names.txt", infilename2 = "REQUESTS.TXT";
		boolean worky = true;
		
		try{
		i = Integer.parseInt(args[0]);
		}
		catch(Exception e)
		{
		  worky = false;
		}
		
		if(worky != true) i = TRY;
				
		FileReader fr1 = new FileReader(infilename1);
		BufferedReader infile1 = new BufferedReader(fr1);
	
    Student db = new Student(); 

		SSN = infile1.readLine();
	  while((i > 0)&&(temp != null))
	  {
	    Name = infile1.readLine();
			Street = infile1.readLine();
			City = infile1.readLine();
			Phone = infile1.readLine();
			db.AddStudent(SSN, Name, Street, City, Phone);
						 
			while(temp != null)
			{
			  temp = infile1.readLine();
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
		infile1.close();
		  
		FileReader fr2 = new FileReader(infilename2);
		FileWriter fr = new FileWriter(outfilename);
		BufferedReader infile2 = new BufferedReader(fr2);
		BufferedWriter br = new BufferedWriter(fr);
		PrintWriter outfile = new PrintWriter (br);
    
		String ssn = "You don't see this", name = null;
		
	  while(ssn != null)
		{
    	db.ResetAccessCount();
    	ssn = infile2.readLine();
			if (ssn != null)
			  name = db.GetName(ssn);
			
			if (name != null)
			{
			  Access += db.GetAccessCount();
				count++;
				outfile.println(ssn);
				outfile.println(name);
				outfile.println();
			}
			
			name = null;
		}
		
		infile2.close();
		outfile.println("Avg access: " + Access/count);
    outfile.close();    
		db.DumpToFile();  
  }
}
