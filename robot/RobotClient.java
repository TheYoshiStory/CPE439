import java.lang.Math;
import java.util.HashMap;
import java.io.PrintWriter;
import java.io.IOException;
import java.net.Socket;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JComponent;
import javax.swing.JFrame;

public class RobotClient
{
   private static enum State{IDLE, RUNNING, EXIT, STOP, FORWARD, BACKWARD, RIGHT, LEFT};
   private static final int PORT = 23;
   private static final int SIZE = 500;

   public static void main(String[] args)
   {
      Window window;
 
      if(args.length != 1)
      {
         System.err.println("ERROR: invalid number of arguments");
         System.exit(1);
      }

      window = new Window(args[0]);
      window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      window.setResizable(false);
      window.setMinimumSize(new Dimension(SIZE, SIZE));
      window.setSize(SIZE, SIZE);
      window.getContentPane().add(window.gui);
      window.pack();
      window.setVisible(true);
   }

   private static class Window extends JFrame implements KeyListener
   {
      private Socket socket;
      private PrintWriter output;
      private Picture gui;
      private HashMap<Integer, Boolean> event;

      public Window(String ip)
      {
         /*
         try
         {
            System.out.println("connecting to address " + ip + " on port " + PORT + "...");
            this.socket = new Socket(ip, PORT);
            this.output = new PrintWriter(this.socket.getOutputStream(), true);
            System.out.println("socket connection established");
         }
         catch(IOException e)
         {
            System.err.println("ERROR: failed to open socket connection");
            System.exit(1);
         }
         */

         this.gui = new Picture();
         this.event = new HashMap<Integer, Boolean>();

         this.event.put(KeyEvent.VK_UP, false);
         this.event.put(KeyEvent.VK_DOWN, false);
         this.event.put(KeyEvent.VK_RIGHT, false);
         this.event.put(KeyEvent.VK_LEFT, false);

         addKeyListener(this);
         setFocusable(true);
         setFocusTraversalKeysEnabled(false);
      }
      
      public void keyPressed(KeyEvent input)
      {
         int[] speed = {0, 0};

         if(input.getKeyCode() == KeyEvent.VK_ESCAPE)
         {
            //this.output.println("robot-control " + speed[0] + "  " + speed[1]);
            System.out.println("robot-control " + speed[0] + "  " + speed[1]);
            
            /*
            try
            {
               System.out.println("closing socket connection...");
               this.socket.close();
               this.output.close();
               System.out.println("socket connection closed");
            }
            catch(IOException e)
            {
               System.err.println("ERROR: failed to close socket connection");
               System.exit(1);
            }
            */
            
            System.exit(0);
         }
         else if(this.event.containsKey(input.getKeyCode()))
         {
            if(!this.event.get(input.getKeyCode()))
            {
               this.event.put(input.getKeyCode(), true);

               if(this.event.get(KeyEvent.VK_UP))
               {
                  speed[0] += 50;
                  speed[1] += 50;
               }
               
               if(this.event.get(KeyEvent.VK_DOWN))
               {
                  speed[0] -= 50;
                  speed[1] -= 50;
               }
               
               if(this.event.get(KeyEvent.VK_RIGHT))
               {
                  speed[0] += 25;
                  speed[1] -= 25;
               }
               
               if(this.event.get(KeyEvent.VK_LEFT))
               {
                  speed[0] -= 25;
                  speed[1] += 25;
               }
               
               //this.output.println("robot-control " + speed[0] + "  " + speed[1]);
               System.out.println("robot-control " + speed[0] + "  " + speed[1]);
               repaint();
            }
         }
      }
   
      public void keyReleased(KeyEvent input)
      {
         int[] speed = {0, 0};
         
         if(this.event.containsKey(input.getKeyCode()))
         {
            if(this.event.get(input.getKeyCode()))
            {
               this.event.put(input.getKeyCode(), false);

               if(this.event.get(KeyEvent.VK_UP))
               {
                  speed[0] += 50;
                  speed[1] += 50;
               }
               
               if(this.event.get(KeyEvent.VK_DOWN))
               {
                  speed[0] -= 50;
                  speed[1] -= 50;
               }
               
               if(this.event.get(KeyEvent.VK_RIGHT))
               {
                  speed[0] += 25;
                  speed[1] -= 25;
               }
               
               if(this.event.get(KeyEvent.VK_LEFT))
               {
                  speed[0] -= 25;
                  speed[1] += 25;
               }
               
               //this.output.println("robot-control " + speed[0] + "  " + speed[1]);
               System.out.println("robot-control " + speed[0] + "  " + speed[1]);
               repaint();
            }
         }
      }

      public void keyTyped(KeyEvent input){}
   }

   private static class Picture extends JComponent
   {
      private State state;

      public Picture()
      {
         state = State.IDLE;
      }

      public void paintComponent(Graphics g)
      {
         int xMid;
         int yMid;
         
         xMid = SIZE / 2;
         yMid = SIZE / 2;
         
         super.paintComponent(g);
         
         g.setColor(Color.BLACK);
         g.drawRoundRect(xMid - 50, yMid - 105, 100, 100, 25, 25);
         g.drawRoundRect(xMid - 50, yMid, 100, 100, 25, 25);
         g.drawRoundRect(xMid - 155, yMid, 100, 100, 25, 25);
         g.drawRoundRect(xMid + 55, yMid, 100, 100, 25, 25);

         switch(this.state)
         {
            case STOP:
               this.state = State.IDLE;
               break;

            case FORWARD:
               this.state = State.RUNNING;
               g.setColor(Color.BLUE);
               g.fillRoundRect(xMid - 50, yMid - 105, 100, 100, 25, 25);
               break;

            case BACKWARD:
               this.state = State.RUNNING;
               g.setColor(Color.BLUE);
               g.fillRoundRect(xMid - 50, yMid, 100, 100, 25, 25);
               break;

            case RIGHT:
               this.state = State.RUNNING;
               g.setColor(Color.BLUE);
               g.fillRoundRect(xMid + 55, yMid, 100, 100, 25, 25);
               break;

            case LEFT:
               this.state = State.RUNNING;
               g.setColor(Color.BLUE);
               g.fillRoundRect(xMid - 155, yMid, 100, 100, 25, 25);
               break;
         }
      }
   }
}

