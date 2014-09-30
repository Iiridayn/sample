import java.awt.*;

/*
// Ged, 10/12/2001
*/

public class Prog6b extends java.applet.Applet
{
	public void init()
	{
		resize(300,300);
	}

	public void paint(Graphics page)
	{
		//CHEAP WAY!!!  CHANGE LATER!!!
		setBackground (Color.black);
		//CHEAP WAY!!!  CHANGE LATER!!!
		page.setColor (Color.gray);
		page.fillOval (0, 0, 300, 300);
		
		page.setColor (Color.black);
		int random, size, x, y;
		int irad = (int) (Math.random()*100);
		for (int i = 0; i < irad; i++)
		{
			random = (int) (Math.random()*100);
			
			if (random < 50)
				size = 6;
			else 
				if (random < 75)
					size = 10;
				else 
					if (random < 85)
						size = 14;
					else 
						if (random < 95)
							size = 18;
						else
							size = 22;
			
			x = (int) (Math.random()*300);
			y = (int) (Math.random()*300);
			page.fillOval (x, y, size, size);
			page.drawString ("" + x, 0, 0);
		}
	}
}


