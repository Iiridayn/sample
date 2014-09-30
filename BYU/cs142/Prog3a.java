/*********************************************************
 |                  Prog3aclass                          |
 |         (c) Ged Crimsonclaw, 9/14/2001                |
 |    Discovers the temperature in C given F             |
 *********************************************************/

//Importing the buffer reader lot
import java.io.*;
import java.text.*;

public class Prog3a
{
  //The last bit is needed unless I want a compile error
	public static void main (String[] agrs) throws IOException
	{
		//Both used in the equation C = (F - 32)*(5/9)
		final int FREEZINGF = 32;
		final double CONVERSION_FRACTION = 5.0/9.0;
		
		//All number variables needed in program
		int F, Cint, Ff;
		double C;
		DecimalFormat shorter = new DecimalFormat ("0.#");
		
		//To get the value of F from user
		BufferedReader stdin = new BufferedReader 
		                      (new InputStreamReader(System.in));
		System.out.println ();
		System.out.println ("------------------------------------------" +
		                     "------------------------------------------");
		System.out.print ("Please enter the Farenheit temperature " 
		                     + "that you wish to have converted to Celcius: ");
		F = Integer.parseInt (stdin.readLine());
		
		//Solving for C
		C = (F - FREEZINGF)*CONVERSION_FRACTION;
		System.out.println ();
		System.out.println (F + " degrees Farenheit in Celcius is approximatly " + shorter.format(C) +
		                           " in floating point notation.");
		Cint = 	(int) C;
		System.out.println (F + " degrees Farenheit as an integer in Celcius is: " + Cint);
		
		//Farenhiet (sp?) above 0
		Ff = F - FREEZINGF;
		System.out.println ();
		System.out.println (F + " degrees Farenheit is: " + Ff + " degrees above 0.");
		System.out.println ("------------------------------------------" +
		                     "------------------------------------------");
		System.out.println ();
	}
}
