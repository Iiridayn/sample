import java.io.*;

public class prog2
{
//This class handles the student database, and
//the insertion of information therein.  It has
//some slight parseing capabilities, and can put
//things where they belong.  It also keeps track
//of the efficiancy of various parts of the
//database, and prints those out too.

	public static void main(String[] args) throws IOException
	{//all the work is done here
		char type = 's';
		boolean mode = true;
		int max = 2000;
		try
		{
			max = Integer.parseInt(args[0]);
			if(args[1].equals("0"))
				mode = false;
			else
				type = args[1].charAt(0);
		}
		catch(Exception e){}
		
		BufferedReader infile = new BufferedReader(new FileReader("NAMES.TXT"));
		StudentDatabase db = new StudentDatabase(max);
		
		String ssn = infile.readLine();
		while((ssn != null)&&(max > 0))
		{
			String name = infile.readLine();
			String street = infile.readLine();
			String city = infile.readLine();
			String phone = infile.readLine();
			
			db.addStudent(ssn, name, street, city, phone);
			
			ssn = infile.readLine();
			while((ssn != null)&&(!Character.isDigit(ssn.charAt(0))))
			{
				db.addCourse(ssn);
				ssn = infile.readLine();
			}//end while
		}
		//read in the input file, parseing it into the database.
		infile.close();
		long time = 0;
		if(mode)
			time = db.sort(type);
		//Then sort the database.
		infile = new BufferedReader(new FileReader("REQUESTS.TXT"));
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("Found.txt")));
		int totcomp = 0, totalAccess = 0;		
		int queries = 0;
		String query = infile.readLine();
		while(query != null)
		{
			String name;
			if(mode)
				name = db.findNameBinarySearch(query);
			else
				name = db.findName(query);
			outfile.println(query + ":" + name);
			if((!mode)&&(name != "Name not Found"))
			{
				totalAccess += db.getAccessCount();
				queries++;				
			}
			if(!mode)
				db.resetAccessCount();
			query = infile.readLine();
		}
		//Next, read in the queries, asking the database if it is in it.
		if(mode)
			totcomp = db.getComparisons();
		infile.close();
		db.dumpToFile();
		outfile.println();
		outfile.println();
		outfile.println();
		if(mode)
		{
			outfile.println("Time to sort: " + time);
			outfile.println("Comparisons: " + totcomp);
		}
		else
			outfile.println("Access/Queries: " + totalAccess/queries);		
		outfile.close();
		//Finally, print out the efficiancy info.
	}//main
}//prog1
