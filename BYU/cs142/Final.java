/*
Final.class Made By: Ged Crimsonclaw, Michael Clark
Date: 12/18/2001
Goal: To Calculate some simple functions.
*/

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.applet.*;
import java.util.Stack;

public class Final extends JFrame
{
	private TextField IO;
	private Button CLEAR, sum, prod, rFact, rec;
	private JPanel top, bottom, all; 
	
	public Final ()
	{
	  super ("Final Exam");
	  setSize(320,100);
		
		IO = new TextField(20);
		CLEAR = new Button ("CLEAR");
		sum = new Button ("sum");
		prod = new Button ("prod");
		rFact = new Button ("rFact");
		rec = new Button ("rec");
		
		top = new JPanel();
		top.setLayout(new BoxLayout(top, BoxLayout.X_AXIS));
		top.add(IO);
		top.add(Box.createRigidArea (new Dimension(10,0)));
		top.add(CLEAR);
		
		bottom = new JPanel();
		bottom.setLayout(new BoxLayout(bottom, BoxLayout.X_AXIS));
		bottom.add(sum);
		top.add(Box.createRigidArea (new Dimension(5,0)));
		bottom.add(prod);
		top.add(Box.createRigidArea (new Dimension(5,0)));
		bottom.add(rFact);
		top.add(Box.createRigidArea (new Dimension(5,0)));
		bottom.add(rec);
		
		all = new JPanel();
		all.setLayout(new BorderLayout());
		all.add(top, BorderLayout.NORTH);
		all.add(bottom, BorderLayout.SOUTH);
		
		CLEAR.addActionListener (new FinalListener());
		sum.addActionListener (new FinalListener());
		prod.addActionListener (new FinalListener());
		rFact.addActionListener (new FinalListener());
		rec.addActionListener (new FinalListener());
		
		setContentPane (all);
	}
	
	private class FinalListener implements ActionListener
	{
	  public void actionPerformed (ActionEvent event)
		{
		  Object source = event.getSource();
			int temp = -1, temp1;
			
			if (source == CLEAR)
			  IO.setText("");
			else if (source == sum){
			  try{
					temp = Integer.parseInt(IO.getText());
					temp1 = temp;
					if(temp < 0)
					  IO.setText("INVALID");
					else
					  temp = Sum(temp);
					
					addRec(temp1, "sum", temp);
					
					if(temp != -1)
					  IO.setText("" + temp);
					temp = -1;
				}
				catch(NumberFormatException e){
					IO.setText("INVALID");
					addRec(-1, "sum", temp);
				}
			}
			else if (source == prod)
			{
			  try{
					temp = Integer.parseInt(IO.getText());
					temp1 = temp;
					
					if(temp < 1)
					  IO.setText("INVALID");
					else
					  temp = Prod(temp);
					
					addRec(temp1, "prod", temp);
					
					if(temp != -1)
					  IO.setText("" + temp);
					temp = -1;
				}
				catch(NumberFormatException e){
					IO.setText("INVALID");
					addRec(-1, "prod", temp);
				}
			}
			else if (source == rFact)
			{
				try{
					temp = Integer.parseInt(IO.getText());
					temp1 = temp;
					
					if(temp < 1)
					  IO.setText("INVALID");
					else
					  temp = RFact(temp);
						
					if (temp == 1){
					  temp = -1;
						IO.setText("Too Big");
						}
						
						addRec(temp1, "rFact", temp);
					
					if(temp != -1)
					  IO.setText("" + temp);
					temp = -1;
				}
				catch(NumberFormatException e){
					IO.setText("INVALID");
					addRec(-1, "rFact", temp);
				}
			}
			else if (source == rec)
			{
				Rec();
			}
			else
			  IO.setText("How you do that?!");
		}	
	}
	
	private int Sum(int value){
		int runningSum = 0;
    for(int i = 0; i <= value; i++)
		{	
			if ((runningSum + i)<0){//It goes negative from overflow
			  IO.setText("Too Big");
			  return -1;
		  }
			else
				runningSum += i;
		}
		return runningSum;
	}
	
	
	private int Prod(int value){
		int runningSum = 1;
    for(int i = 1; i <= value; i++)
		{	
			if ((runningSum * i)<0){//It goes negative from overflow
			  IO.setText("Too Big");
			  return -1;
		  }
			else
				runningSum *= i;
		}
		return runningSum;
	}
	
  private int RFact(int value){//yes, I know the error checking code is a bit off, but at least it works.
		int temp = 1;
		
		if((value > 0)&&(value < 13)/* Value goes no higher, and without it there is a stack overflow error. */){
			temp = value * RFact(value - 1);
		  if(temp > 2147483647){
			  IO.setText("Too Big");
				return -1;
				}
		}
				
		if (value != -1)
			return (int)temp;
		else
		  return -1;
	}
	
	Stack stack = new Stack();
	
	private void Rec()
	{
	  if (stack.empty() == true)
		  IO.setText("END");
		else
		  IO.setText("" + stack.pop());
	}
	
	private void addRec(int number, String type, int value){
	  if (number == -1)
		stack.push(type + "(INVALID)");		
		else if (value != -1)
		stack.push(type + "(" + number + ")" + " = " + value);
		else
		stack.push(type + "(" + number + ")");
	}
}


