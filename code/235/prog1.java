import java.io.*;

public class prog1
{
	public static void main(String[] args) throws IOException, FileNotFoundException
	{
		int max_stu = 5000;
		try
		{
			max_stu = Integer.parseInt(args[0]);
		}
		catch(Exception e){};
		
		//get max students from args or use max(5000)
		StudentDatabase db = new StudentDatabase(max_stu);
		BufferedReader infile = new BufferedReader(new FileReader("Y:\\CS235\\NAMES.TXT"));
		
		for(int i = 0; i < max_stu; i++)
		{
			String ssn = infile.readLine();
			String name = infile.readLine();
			String street = infile.readLine();
			String city = infile.readLine();
			String phone = infile.readLine();
			
			db.addStudent(ssn, name, street, city, phone);
			
			String temp = infile.readLine();
			while(Character.isDigit(temp.charAt(0)))
			{
				db.addCourse(temp);
				temp = infile.readLine();
			}//end while
		}//end for
		
		infile.close();
		//now the quieries
		
		infile = new BufferedReader(new FileReader("Y:\\CS235\\REQUESTS.TXT"));
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter("Found.txt")));
		int totalAccess = 0, queries = 0;
		String input = infile.readLine();
		while(input != null)
		{
			String name = db.findName(input);
			outfile.println(input + "\n" + name + "\n");
			totalAccess += db.getAccessCount();
			db.resetAccessCount();
			queries++;
			input = infile.readLine();
		}
		infile.close();
		outfile.print("\n\n\n");
		outfile.println((double) totalAccess / queries);
		outfile.close();
	}//nastiest method of all...  Has to deal with the readIn and parsing of files
}
