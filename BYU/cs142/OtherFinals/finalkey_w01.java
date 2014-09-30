/*
// header - edit "Data/yourJavaAppletHeader" to customize
// contents - edit "EventHandlers/Java Applet/onCreate" to customize
*/
import java.awt.*;
import java.awt.event.*;

public class finalkey_w01 extends java.applet.Applet implements ActionListener
{
        node head = null;
        node current;
        int x=50,y=400;

        Label curr_label = new Label("person");
        TextField curr_text = new TextField(10);
        Button traverseButton = new Button("Traverse");
        Button toChildButton = new Button("Move to Child");
        Button toMomButton = new Button("Add Mom");
        Button toDadButton = new Button("Add Dad");


        public void init()
        {
                resize(700,700);
                setLayout(null);
                curr_label.setBounds(50,50,80,20);
                curr_text.setBounds(140,50,150,20);
                traverseButton.setBounds(430,40,80,20);
                toMomButton.setBounds(530, 10, 80,20);
                toDadButton.setBounds(530, 70, 80,20);
                toChildButton.setBounds(530, 40, 80,20);
                add(curr_label);
                add(curr_text);
                
                add(traverseButton);
                add(toChildButton);
                add(toMomButton);
                add(toDadButton);
                
                traverseButton.addActionListener(this);
                toChildButton.addActionListener(this);
                toMomButton.addActionListener(this);
                toDadButton.addActionListener(this);
                
                //program the head node in
                node temp = new node("Jason");
                head = temp;
                current = head;
        }

        public void actionPerformed(ActionEvent e){
                String command = e.getActionCommand();
                if(command.equals("Move to Child")){//do restart stuff
                        System.out.println("Move to Child pressed");
                        if(current.child == null)
                                curr_text.setText("child is null");
                        else{
                                current = current.child;
                                curr_text.setText(current.name);
                        }//end else
                }//end if name = to child
                if(command.equals("Traverse")){//do update stuff
                        System.out.println("traverse pressed");
                        repaint();
                }
                if(command.equals("Add Mom")){//do mom stuff
                        System.out.println("Add Mom pressed");
                        String curr = curr_text.getText();
                        node myNewMom = new node(curr);
                        current.mom = myNewMom;
                        myNewMom.child = current;
                        current = current.mom;
                        curr_text.setText("current.name is " + current.name);
                }//end if adding Mom
                if(command.equals("Add Dad")){//do dad stuff
                        System.out.println("Add Dad pressed");
                        String curr = curr_text.getText();
                        node myNewDad = new node(curr);
                        current.dad = myNewDad;
                        myNewDad.child = current;
                        current = current.dad;
                        curr_text.setText("current.name is " + current.name);
                }
        }


        //can i recurse on the paint method???
        public void paint(Graphics g)
        {
                node temp = head;
                x=50;
                y=400;
                //drawTree(g,temp);
                drawOutLine(g,temp);
        }
        
        public void drawTree(Graphics g, node temp){
        
                if(temp != null){
                        g.drawString(temp.name, x, y);
                        if(temp.mom == null && temp.dad == null){
                                System.out.println("don't go there then");
                        }
                        else{
                                x = x + 100; //moving a generation so move some
                                y = y - 50; //move up 50;
                                drawTree(g,temp.dad);
                                y = y + 50; //move back to where i was
                                y = y + 50; //move down 50;
                                drawTree(g,temp.mom);
                                x= x-100;
                        }//end else
                }               
        }//end drawTree 

        public void drawOutLine(Graphics g, node temp){
                
                if(temp != null){
                        g.drawString(temp.name, x, y);
                        if(temp.mom == null && temp.dad == null)
                                System.out.println("don't go");
                        else{
                                x = x+70;
                                y = y+20;
                                drawOutLine(g,temp.dad);
                                y = y+20;
                                drawOutLine(g,temp.mom);
                                x = x-70;
                        }
                }//endif
        }//end drawOutLine
        
        
        
}

class node{
        
        String name;
        node mom;
        node dad;
        node child;
        
        public node (String N){
                name = N;
                mom = null;
                dad = null;
                child = null;
        }
}



