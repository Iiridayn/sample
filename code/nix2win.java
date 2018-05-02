import java.io.*;

class nix2win
{
	public static void main(String[] args) throws IOException
	{
		String infilename = null, outfilename = "output.txt";
		
		try
		{
			infilename = args[0];
		}
		catch(Exception e){return;}
		
		try
		{
			outfilename = args[1];
		}
		catch(Exception e){}
		
		BufferedReader infile = new BufferedReader(new FileReader(infilename));
		PrintWriter outfile = new PrintWriter(new BufferedWriter(new FileWriter(outfilename)));
		String temp = null;
		for(temp = infile.readLine(); temp != null; temp = infile.readLine())
		{
			System.out.println(temp);
			outfile.println(temp);
		}
	}
}
