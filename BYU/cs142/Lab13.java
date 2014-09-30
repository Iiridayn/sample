/*
	File: Lab13.java
	Modified form of code at http://students.cs.byu.edu/~cs142ta/Labs/Assigns/theMandelbrot.java
	Edited by: Ged Crimsonclaw, Michael C. Clark
*/

//import these
import java.awt.*;
import java.awt.image.*;
import java.applet.*;
import javax.swing.*;
import java.awt.event.*;

/**
* This program draws a Mandelbrot fractal to a window
* based on coordinates entered by the user.
*/
public class Lab13 extends java.applet.Applet implements ActionListener
{
	//declare all your Labels, TextFields, and Buttons;
	private Button Redraw;
	private TextField xmin, xmax, ymin, ymax;
	private Label a,b,c,d;
	
	/**
	* Init Method: Prepares Screen.
	* This method is run first at the start of the program.
	* The Window is set up, all the labels and fields are
	* given positions in the window and added.
	*/
	public void init()
	{
		// Set up Window 
		setSize(700,700);
		setLocation(20,20); 
		
		//create your Labels and TextBoxes and Buttons
		Redraw = new Button ("ReDraw");
		Redraw.addActionListener(this);
		a = new Label ("XMin");
		xmin = new TextField ("-2.25", 5);
		b = new Label ("XMax");
		xmax = new TextField (".75", 5);
		c = new Label ("YMin");
		ymin = new TextField ("-1.5", 5);
		d = new Label ("YMax");
		ymax = new TextField ("1.5", 5);
			
		// Put on Screen all your stuff on the screen
		add(Redraw);
		add(a);
		add(xmin);
		add(b);
		add(xmax);
		add(c);
    add(ymin);
		add(d);
		add(ymax);
	
	} // End Method Init


	/**
	* Paint/actionPerformed Method: Creates Fractal.
	* This method is called when the button is pushed.
	* It then steps through every pixel on the
	* canvas. For each pixel, it figures what coordinate
	* it coresponds to, from the range given by the user.
	* These coordinates are sent to the Calculate method.
	* The current pixel is set to a color cooresponding to
	* the value returned, and the window's graphics object to
	* draw the pixel on the screen, using setColor and drawLine.
	*/
	public void paint(Graphics g)
	{
		// Vars
		int Count;
		double MinX, MaxX, MinY, MaxY; //these are the ranges
		double calc_x, calc_y, StepX, StepY;

		// Get the Ranges now by calling getText on your text boxes
		MinX = Double.parseDouble(xmin.getText());
		MaxX = Double.parseDouble(xmax.getText());
		MinY = Double.parseDouble(ymin.getText());
		MaxY = Double.parseDouble(ymax.getText());
		
			// Figure Space Between Coordinates
			StepX=(MaxX-MinX)/600;
			StepY=(MaxY-MinY)/600;

			// Set Start Coodinates
			calc_x=MinX;
			calc_y=MinY;

			// Calculate each pixel color
			// use a doublly nested 'for' loop to go through
			//everly pixel on the canvas
			for (int i = 0; i <= 600; i++)
			{
			  for (int j = 0; j <= 600; j++)
				{ 
					// Get Manelbrot number by calling the calculate method
					Count=Calculate(calc_x,calc_y,0,0,0);

					// Chop into a color
					Count=(int)(((double)Count/400)*255);

					// Limit and Lighten Color
					if(Count!=0) Count*=10;
					if(Count>255)Count=255;

					// create a new color for DefineColor
					// hint: pass count as all 3 params
					Color color = new Color(Count, Count, Count);
					
					//set the color and then draw the pixel
					g.setColor(color);
					g.drawLine(j+50,i+75,j+50,i+75);
					

					// Go to next pixel 
					calc_x=calc_x+StepX;	
				} // End Y Loop

				// Go to next Coordinate
				calc_x=MinX;
				calc_y=calc_y+StepY;
			} // End X Loop
	} // End Method paint


	public void actionPerformed(ActionEvent e){
		//when they click the button then just repaint the set.
		repaint();
	}


	/**
	* Calculate Method: Performs the Mandelbrot Calculation. <BR>
	* This method recieves the coordinates of a point. From this
	* point it figures the next point in sequence. If the new point
	* is a long distance from the first point, then the method will
	* return the number of recursive calls it took to get this result.
	* If the new point is close to the original point, then the method
	* recursively calls itself and passes all the information on the
	* points. To avoid infinite loops, if the method is recursively
	* called 400 times, the method returns 400.
	*/
	private int Calculate(double OrigX,double OrigY,double CurX,double CurY,int Count)
	{
		// Vars
		double NewX,NewY;

		// Figure Number for this Z Value
		// straight from the web page
		NewX=(CurX*CurX)+(CurY*CurY*-1)+OrigX;
		NewY=(2*CurX*CurY)+OrigY;


		if((NewX*NewX)+(NewY*NewY)>4)
		{
			return Count;
		} // End If Test>2
		else {
			if(Count > 400)
			  return 400;
			else
			  return Calculate(OrigX, OrigY, NewX, NewY, ++Count);
		}// End else
	} // End Method Calculate

} // End Class Lab13
