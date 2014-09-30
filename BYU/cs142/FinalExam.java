/*
Final.class Made By: Ged Crimsonclaw, Michael Clark
Date: 12/18/2001
Goal: To Calculate some simple functions.
*/

import Final;
import GenericWindowListener; // Importing from page 412 of the textbook
import java.awt.event.*;

public class FinalExam
{
  public static void main (String[] args)
	{
	  Final frame = new Final();
		frame.addWindowListener (new GenericWindowListener());
		frame.show();
	}
}
