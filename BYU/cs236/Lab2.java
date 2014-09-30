//Code by Ged Crimsonclaw, 5/22/2002

import java.io.*;

class Lab2
{
	public static void main(String[] args) throws FileNotFoundException, IOException
	{
		/*      QUERY 1      */
		/*    -----------    */
		
		Relation snap = new Relation(new Scheme("SNAP"), "A:\\proj2\\snap.txt");
		Relation csg = new Relation(new Scheme("CSG"), "A:\\proj2\\csg.txt");
		
		Relation temp1 = snap.join(csg);
		Relation temp2 = temp1.project(new Scheme("CN"));
		
		System.out.println("** Results for query 1 **");		
		System.out.println(temp2);
		System.out.println();
		System.out.println();
		
		
		/*      QUERY 2      */
		/*    -----------    */
		
		Relation cp = new Relation(new Scheme("CP"), "A:\\proj2\\cp.txt");
		
		Relation temp3 = cp.select (new Attribute("C"), "=", new Value("CS206"));
		Relation temp4 = temp3.project(new Scheme("P"));
		Relation temp5 = temp4.rename(new Scheme("C"));
		Relation temp6 = temp5.join(cp);
		Relation temp7 = temp6.project(new Scheme("P"));
		
		System.out.println("** Results for query 2 **");
		System.out.println(temp7);
		System.out.println();
		System.out.println();
		
		
		/*      QUERY 3      */
		/*    -----------    */
		
		Relation cdh = new Relation(new Scheme("CDH"), "A:\\proj2\\cdh.txt");
		
		Relation disj1 = cdh.select(new Attribute("D"), "=", new Value("M"));
		Relation disj2 = cdh.select(new Attribute("D"), "=", new Value("W"));
		Relation disj3 = cdh.select(new Attribute("D"), "=", new Value("F"));
		
		Relation t1 = disj1.union(disj2);
		Relation t2 = disj3.union(t1);
		Relation t3 = t2.project(new Scheme("C"));
		Relation t4 = cdh.project(new Scheme("C"));
		Relation t5 = t4.minus(t3);
		
		System.out.println("** Results for query 3 **");
		System.out.println(t5);
	}
}
