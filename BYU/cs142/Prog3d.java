//  Ged Crimosnclaw
//  Prog3d.class
//  Lab 3, project D (4)
//  Displays Moroni and a scripture

import java.awt.*;

/*
// header - edit "Data/yourJavaAppletHeader" to customize
// contents - edit "EventHandlers/Java Applet/onCreate" to customize
*/

public class Prog3d extends java.applet.Applet
{
	public void init()
	{
		resize(400,430);
	}

	public void paint(Graphics page)
	{
		setBackground (Color.white);
		page.setColor (Color.yellow);
		page.fillRect(50, 55, 100, 200); //body
		page.fillRect(60, 255, 75, 100); //legs
		page.fillRect(20, 60, 30, 150); //left side arm
		page.fillOval(75, 5, 50, 50); //head
		page.fillOval(63, 355, 75, 75); //ball
		page.fillArc(275, 5, 50, 50, 315, 90); //end of trump
		page.fillRect(100, 25, 205, 10); //trump tube
		page.fillRect(150, 25, 30, 70); //arm w/trump
		page.setColor (Color.black);
		page.drawRect(180, 115, 200, 300);
		
		//scripture
		page.drawString("And it came to pass that I, Nephi,", 190, 135);
		page.drawString ("said unto my father: I will go and do", 190, 155); 
		page.drawString ("the things which the Lord hath", 190, 175);
		page.drawString	("commanded, for I know that the Lord", 190, 195);
		page.drawString	("giveth no commandments unto the", 190, 215);
		page.drawString ("children of men, save he shall prepare", 190, 235);
		page.drawString	("a way for them that they may", 190, 255);
		page.drawString	("accomplish the thing which he", 190, 275);
		page.drawString ("commandeth them.", 190, 295);
		page.drawString ("-- Nephi 3:7", 240, 395);
	}
}


