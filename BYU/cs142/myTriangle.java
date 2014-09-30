//	Ged Crimsonclaw
//	myTriangle.class
//	Draws a Triangle

import java.awt.*;

public class myTriangle extends myShape
{
	private int[] xs=new int[3];
	private int[] ys=new int[3];
	private int height;
	private int half;
	private int length;
	
	
	public myTriangle(int x, int y, Color c, int height)
	{
		super(x,y,c);
		this.height=height;
		loader();
	}
	
	private void loader()
	{
		length=height/(int)Math.tan(60*Math.PI/180);
		half=(int)Math.tan(30*Math.PI/180)*length;
		xs[0]=x;
		xs[1]=x-length*2/3;
		xs[2]=x+length*2/3;
		ys[0]=y-(height-half)+(y/6);
		ys[1]=y+(half*2/3)+(y/6);
		ys[2]=y+(half*2/3)+(y/6);
	}
	
	public void paint(Graphics page)
	{
		page.setColor(c);
		page.drawPolygon(xs, ys, xs.length);
		drawCenter(page);
	}
}
