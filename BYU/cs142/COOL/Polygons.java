//*****************************************************************************
// Author: Jason Judd
// Course: CS 142
// Date: 10/24/01
// This program will let you create a polygon which can be altered
//*****************************************************************************


import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;

public class Polygons extends Applet implements MouseListener, MouseMotionListener
{
	private int APPLET_WIDTH = 300;
	private int APPLET_HEIGHT = 250;
	
	private Label titleLabel;
	private Canvas canvas;
	private Button clearButton;
	int[] Xpoints;
	int[] Ypoints;
	int index;
	private Polygon poly;
	private boolean done;
	private int movingpoint;
	private final int CANVAS_WIDTH = 200;
	private final int CANVAS_HEIGHT = 200;
	
	public void init ()
	{
		titleLabel = new Label ("Create a polygon using the mouse.");
		titleLabel.setBackground (Color.gray);
		add (titleLabel);
		
		canvas = new Canvas();
		canvas.setBackground (Color.white);
		canvas.setSize (CANVAS_WIDTH, CANVAS_HEIGHT);		
		add (canvas);
		
		setBackground (Color.gray);
		setSize (APPLET_WIDTH, APPLET_HEIGHT);
		
		canvas.addMouseListener (this);
		canvas.addMouseMotionListener (this);
		

		
		poly = new Polygon();
		done = false;
		index=0;
	}

	
	public void paint (Graphics page)
	{
		page = canvas.getGraphics();
		page.drawRect (0,0,CANVAS_WIDTH,CANVAS_HEIGHT);
		page.drawPolygon(poly);
	}
	
	public void mousePressed (MouseEvent event)
	{
		if (done==false)
		{
			if (poly.npoints > 2)
			{
				if(Math.abs(event.getX() - poly.xpoints[0])<=5 || Math.abs(event.getY() - poly.ypoints[0])<=5)
				{
					done = true;					
					repaint();
				}
				else
				{			
					poly.addPoint (event.getX(),event.getY());
					repaint();
				}
			}
			else
			{
				poly.addPoint (event.getX(),event.getY());
				repaint();
			}	
		}
		else
		{
			movingpoint = -1;
			for(int i = 0; i< poly.npoints; i++)
			{
				if (Math.abs(event.getX() - poly.xpoints[i])<=10 || Math.abs(event.getY() - poly.ypoints[i])<=10)
				{
					movingpoint = i;
				}
			}
		}
	}
	
	
	public void mouseDragged (MouseEvent event)
	{
		if (done==true && movingpoint != -1)
		{
			poly.xpoints[movingpoint] = event.getX();
			poly.ypoints[movingpoint] = event.getY();
			repaint();
		}
	}
	
	
	
	public void mouseReleased (MouseEvent event) {}
	public void mouseClicked (MouseEvent event) {}
	public void mouseEntered (MouseEvent event) {}
	public void mouseExited (MouseEvent event) {}
	public void mouseMoved (MouseEvent event) {}
	
	
	
}
