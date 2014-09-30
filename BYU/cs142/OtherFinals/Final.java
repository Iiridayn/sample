/*
Final Key: Summer 2000

Jared Clark
08/09/00
CS 142 Head TA

*** Not included: Pre, Postconditions
*/


import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;


class Final
{
        public static JFrame MyWindow=new JFrame("Final Key");
        public static Action ActionRef=new Action();
        
        public static JLabel L_Input=new JLabel("Input:");
        public static JLabel L_Stack=new JLabel("Stack:");
        public static JLabel L_Queue=new JLabel("Queue:");
        public static JLabel L_Spacer=new JLabel("----------------");
        
        public static JTextField T_Input=new JTextField(5);
        public static TextArea T_Stack=new TextArea(20,10);
        public static TextArea T_Queue=new TextArea(20,10);
        
        public static JButton B_AddStack=new JButton("Add");
        public static JButton B_RemoveStack=new JButton("Remove");
        public static JButton B_AddQueue=new JButton("Add");
        public static JButton B_RemoveQueue=new JButton("Remove");
        public static JButton B_Quit=new JButton("Quit");
        
        
        public static void main(String[] args)
        {
                MyWindow.setSize(500,500);
                
                Panel Top=new Panel();
                Top.setLayout(new BorderLayout());
                Top.add(BorderLayout.WEST,L_Input);
                Top.add(BorderLayout.CENTER,T_Input);
                
                Panel S_Commands=new Panel();
                S_Commands.setLayout(new GridLayout(3,1));
                S_Commands.add(L_Stack);
                S_Commands.add(B_AddStack);
                S_Commands.add(B_RemoveStack);
                
                Panel Q_Commands=new Panel();
                Q_Commands.setLayout(new GridLayout(3,1));
                Q_Commands.add(L_Queue);
                Q_Commands.add(B_AddQueue);
                Q_Commands.add(B_RemoveQueue);
                
                Panel Middle=new Panel();
                Middle.setLayout(new GridLayout(1,5));
                Middle.add(new JLabel("      "));
                Middle.add(S_Commands);
                Middle.add(new JLabel("      "));
                Middle.add(Q_Commands);
                Middle.add(new JLabel("      "));
                
                Panel Output=new Panel();
                Output.setLayout(new GridLayout(1,2));
                Output.add(T_Stack);
                Output.add(T_Queue);
                
                Panel Bottom=new Panel();
                Bottom.setLayout(new BorderLayout());
                Bottom.add(BorderLayout.CENTER,Output);
                Bottom.add(BorderLayout.SOUTH,B_Quit);
                
                MyWindow.getContentPane().setLayout(new BorderLayout());
                MyWindow.getContentPane().add(BorderLayout.NORTH,Top);
                MyWindow.getContentPane().add(BorderLayout.CENTER,Middle);
                MyWindow.getContentPane().add(BorderLayout.SOUTH,Bottom);
                
                MyWindow.show();
                
                B_AddStack.addActionListener(ActionRef);
                B_AddQueue.addActionListener(ActionRef);
                B_RemoveStack.addActionListener(ActionRef);
                B_RemoveQueue.addActionListener(ActionRef);
                B_Quit.addActionListener(ActionRef);
                
        } // End method Main
        
} // End Class Final




class Action implements ActionListener
{
        private ParentList QueueHandler,StackHandler;
        
        public Action()
        {
                QueueHandler = new QueueList();
                StackHandler = new StackList();
        } // End Constructor
        
        public void actionPerformed(ActionEvent event)
        {
                int Num=0;
                
                if(event.getSource()==Final.B_Quit)
                {
                        Final.MyWindow.dispose();
                        System.exit(1);
                } // End If Quit
                
                if(event.getSource()==Final.B_RemoveStack)
                {
                        StackHandler.Remove();
                } // End if RemoveStack
                else
                {
                        if(event.getSource()==Final.B_RemoveQueue)
                        {
                                QueueHandler.Remove();
                        } // End if RemoveQueue                                         
                        else
                        {
                                try
                                {
                                        Num=Integer.parseInt(Final.T_Input.getText());
                                } 
                                catch(NumberFormatException Bob)
                                {
                                        Final.T_Input.setText("Number Required");
                                        return;
                                } // End Catch
                                
                                if(event.getSource()==Final.B_AddStack)
                                {
                                        StackHandler.Add(Num);
                                } // End if AddStack
                                else
                                {
                                        if(event.getSource()==Final.B_AddQueue)
                                        {
                                                QueueHandler.Add(Num);
                                        } // End if AddQueue
                                        else
                                        {
                                                Final.T_Input.setText("Unrecognized Action");
                                        } // End Else Not AddQueue
                                } // End Else Not AddStack
                        } // End Else Not RemoveQueue
                } // End Else Not RemoveStack
                
                Final.T_Input.setText("");
                StackHandler.Display(Final.T_Stack);
                QueueHandler.Display(Final.T_Queue);    
        } // End method ActionPerformed
        
} // End Class Action





class ParentList
{
        protected NumberNode First;
        
        public ParentList()
        {
                First=null;
        } // End Constructor
        
        public void Add(int InValue)
        {
                if(First==null)
                {
                        First=new NumberNode(InValue);
                } // End if null
                else
                {
                        NumberNode Current=First;
                        while(Current.Next!=null) Current=Current.Next;
                        Current.Next=new NumberNode(InValue);
                } // End Else Not Null
        } // End Method Add
        
        public void Remove() {} // Implemented in Child Classes
        
        public void Display(TextArea Screen)
        {
                NumberNode Current=First;
                
                Screen.setText("Contents:\n");                  
                while(Current!=null)
                {
                        Screen.append(Current.Value+"\n");
                        Current=Current.Next;
                } // End While Print
                
        } // End Method Display
        
} // End Class ParentList




class StackList extends ParentList
{
        public StackList()
        {
                super();
        } // End Constructor
        
        public void Remove()
        {
                if(First==null) return;
                if(First.Next==null) First=null;
                else
                {
                        NumberNode Prev=First;
                        NumberNode Current=First.Next;
                        while(Current.Next!=null)
                        {
                                Prev=Current;
                                Current=Current.Next;
                        } // End Find End
                        Prev.Next=null;
                } // End Else 
        } // End Method Remove
        
} // End Class StackList




class QueueList extends ParentList
{
        public QueueList()
        {
                super();
        } // End Constructor
        
        public void Remove()
        {
                if(First==null) return;
                First=First.Next;
        } // End Method Remove
        
} // End Class QueueList




class NumberNode
{
        public int Value;
        public NumberNode Next;
        
        public NumberNode(int InV)
        {
                Value=InV;
                Next=null;
        } // End Constructor
        
} // End Class NumberNode
        
