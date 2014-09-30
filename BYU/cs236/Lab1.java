//Code by Ged Crimsonclaw, 5/17/2002

import java.io.*;

class Lab1
{
	public static void main(String[] args) throws FileNotFoundException, IOException
	{
		Relation r1 = new Relation(new Scheme("ABC"), "a:\\proj1\\r1.txt");
		System.out.println(r1);

		Relation r2 = new Relation(new Scheme("ABC"), "a:\\proj1\\r2.txt");
		System.out.println(r2);

		Relation r3 = r1.minus(r2);
		System.out.println(r3);

		Relation r4 = new Relation(new Scheme("ABC"), "a:\\proj1\\r4.txt");

		Relation r5 = r1.minus(r4);
		System.out.println(r5);

		Relation r6 = new Relation(new Scheme("ABC"), "a:\\proj1\\r6.txt");

		Relation r7 = r1.minus(r6);
		System.out.println(r7);

		Relation r8 = new Relation(new Scheme("BCA"), "a:\\proj1\\r8.txt");

		Relation r9 = r1.minus(r8);
		System.out.println(r9);
	}
}
