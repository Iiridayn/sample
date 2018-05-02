import java.io.*;

public class prog7
{
//This class handles the student database, and
//the insertion of information therein.  It has
//some slight parseing capabilities, and can put
//things where they belong.  It also keeps track
//of the efficiancy of various parts of the
//database, and prints those out too.

	public static void main(String[] args) throws IOException
	{//all the work is done here
		int max = 2000;
		String file = "NAMES.TXT";
		try
		{
			max = Integer.parseInt(args[0]);
			file = args[1];
		}
		catch(Exception e){}
		
		BufferedReader infile = new BufferedReader(new FileReader(file));
		StudentDatabase db = new StudentDatabase();
		
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
			max--;
		}
		//read in the input file, parseing it into the database.
		infile.close();
		
		infile = new BufferedReader(new FileReader("REQUESTS.TXT"));
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("Found.txt")));		
		int totcomp = 0, totalAccess = 0;		
		int queries = 0;
		float ratio = db.studentsOverUsed();
		String query = infile.readLine();
		while(query != null)
		{
			String name = db.findName(query);
			if(name != null)
			{
				outfile.println(query + ":" + name);
				//totcomp += db.getComparisons();
				totalAccess += db.getAccessCount();
				queries++;				
			}
			db.deleteBySSN(query);
			//db.resetComparisons();
			db.resetAccessCount();
			query = infile.readLine();
		}
		//Next, read in the queries, asking the database if it is in it.
		infile.close();
		
		/*infile = new BufferedReader(new FileReader("DELETES.TXT"));
		String delete = infile.readLine();
		while(delete != null)
		{
			db.deleteSSN(delete);
			delete = infile.readLine();
		}*/
		
		//long time = db.sort(type);
		db.dumpToFile();
		outfile.println();
		//outfile.println("Time to sort: " + time);
		//outfile.println("Comparisons: " + totcomp);
		outfile.println("Access/Queries: " + (float)totalAccess/queries);
		outfile.println("Students/Used: " + ratio);
		outfile.close();
		//Finally, print out the efficiancy info.
	}//main
}//prog7
