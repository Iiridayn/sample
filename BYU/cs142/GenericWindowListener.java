//***********************************************************
//  GenericWindowListener.java     Author: Lewis and Loftus
//
//  Represents a generic listener for window components.
//***********************************************************

import java.awt.event.*;

public class GenericWindowListener extends WindowAdapter
{
  //---------------------------------------------------------
  //  Terminates the program when the window is closed.
  //---------------------------------------------------------
  public void windowClosing (WindowEvent event)
	{
	  System.exit(0);
	}
}
