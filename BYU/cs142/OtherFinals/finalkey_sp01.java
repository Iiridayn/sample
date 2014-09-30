import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

/*
// header - edit "Data/yourJavaAppletHeader" to customize
// contents - edit "EventHandlers/Java Applet/onCreate" to customize
*/

public class finalkey_sp01 extends java.applet.Applet implements ActionListener
{
        node queueHead = null;
        node stackHead = null;
        int stackSize = 0;
        int queueSize = 0;
        TextField field; 
        Label field_label;
        Checkbox displayOn,displayOff;
        CheckboxGroup myDisplay;
        Checkbox useQueue, useStack;
        CheckboxGroup myChoice;
        Button addButton,removeButton,displayButton;
        public void init()
        {
                resize(400,400);
                setLayout(null);
                field = new TextField(5);
                field_label = new Label("Enter the number here:");
                myDisplay = new CheckboxGroup();
                displayOn = new Checkbox ("display on", myDisplay, true);
                displayOff = new Checkbox ("display off", myDisplay, false);
                myChoice = new CheckboxGroup();
                useQueue = new Checkbox("use Queue", myChoice, true);
                useStack = new Checkbox("use Stack", myChoice, false);
                addButton = new Button("Add");
                removeButton = new Button("Remove");
                displayButton = new Button("Print");
                field.setBounds(120,60,40,20);
                field_label.setBounds(20,60,100,20);
                displayOn.setBounds(20,10,78,20);
                displayOff.setBounds(20,32,85,20);
                useQueue.setBounds(120,10,85,20);
                useStack.setBounds(120,32,85,20);
                addButton.setBounds(190,60, 40, 30);
                removeButton.setBounds(245, 60, 63, 30);
                displayButton.setBounds(320,60,65,30);
                add(field);
                add(field_label);
                add(displayOn);
                add(displayOff);
                add(useQueue);
                add(useStack);
                add(addButton);
                add(removeButton);
                add(displayButton);
                displayButton.addActionListener(this);
                addButton.addActionListener(this);
                removeButton.addActionListener(this);
        }

        public void paint(Graphics g)
        {
                int startx = 20;
                boolean doQueue = useQueue.getState(); // get if the Queue checkBox is filled or not
                if(doQueue){
                        if(queueHead != null){
                                node current = queueHead;
                                g.drawString("Your Queue: ", 100,120);
                                while(current != null){
                                        g.drawString(Integer.toString(current.num), startx, 200);
                                        startx += 30;
                                        current = current.next;
                                } //end while loop for all items in queue
                        }//end if queueHead != null
                }//end if they wanted to print the Queue
                if(!doQueue){
                        if(stackHead != null){
                                node current = stackHead;
                                g.drawString("Your Stack: ", 100,120);
                                while(current != null){
                                        g.drawString(Integer.toString(current.num), startx, 200);
                                        startx += 30;
                                        current = current.next;
                                } //end while loop for all items in stack
                        }//end if stackHead != null
                }//end if they wanted to print the Stack                        
        }//end the paint method
        
        public void actionPerformed(ActionEvent event){
                if(event.getSource() == displayButton)
                        repaint();
                if(event.getSource() == addButton){
                        boolean displayIt = displayOn.getState();
                        boolean doQueue = useQueue.getState();
                        int temp_num=0;
                        try{
                                temp_num = Integer.parseInt(field.getText());
                                if(doQueue){
                                        if(queueSize < 10)
                                                addQueue(temp_num);
                                        else
                                                field.setText("Full");
                                }
                                else{
                                        if(stackSize < 10)
                                                addStack(temp_num);
                                        else
                                                field.setText("Full");
                                }
                                if(displayIt)
                                        repaint();
                        }
                        catch (NumberFormatException exception){
                                field.setText("error");
                        }
                        
                }//end if addButton pushed
                
                if(event.getSource() == removeButton){
                        boolean displayIt = displayOn.getState();
                        boolean doQueue = useQueue.getState();
                        if(doQueue)
                                removeQueue();
                        else
                                removeStack();
                        if(displayIt)
                                repaint();
                }//end removeButton pushed
        }//end actionPerformed
        
        public void addStack(int temp_num){
                stackSize++;
                System.out.println("in the add Stack method");
                node newnode = new node(temp_num);
                newnode.next = stackHead;
                stackHead = newnode;
        }//end addStack
        
        public void removeStack(){
                stackSize--;
                System.out.println("in the remove from Stack method");
                if(stackHead != null)
                        stackHead = stackHead.next;
                else
                        field.setText("empty");
        }//end removeStack
        
        public void addQueue(int temp_num){
                queueSize++;
                System.out.println("in the add to Queue method");
                node temp = queueHead;
                node newnode = new node(temp_num);
                if (queueHead == null)
                        queueHead = newnode;
                else{
                        while (temp.next != null){
                                temp = temp.next;
                        }
                        temp.next = newnode;
                }
        }
        
        public void removeQueue(){
                queueSize--;
                System.out.println("in the remove from Queue method");
                node temp = queueHead;
                if(queueHead == null){
                        field.setText("empty");
                }
                else queueHead = queueHead.next;
        }//end removeQueue
        
}

class node
{
        int num;
        node next;
        
        public node(int temp_num){
                num = temp_num;
                next = null;
        }//end node constructor
}//end node class


