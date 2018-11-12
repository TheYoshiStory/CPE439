import java.lang.Math;
import java.util.HashMap;
import java.io.PrintWriter;
import java.io.IOException;
import java.net.Socket;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JComponent;
import javax.swing.JFrame;

public class RobotClient
{
   private static enum State{IDLE, RUNNING, STOP, FORWARD, BACKWARD, RIGHT, LEFT, RESET, EXIT};
   private static final int[][] IMAGE = {{ 20,  25,  25,  25,  35, 35, 25, 25, 20, 5,   0, -5, -20, -25, -25, -35, -35, -25, -25, -25, -20},
                                         {-50, -45, -25, -15, -15, 15, 15, 45, 50, 50, 35, 50,  50,  45,  15,  15, -15, -15, -25, -45, -50}};
   private static final int PORT = 23;
   private static final int SIZE = 1000;
   private static final int SPEED = 10;
   
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
      window.getContentPane().add(window.robot);
      window.pack();
      window.setVisible(true);
   }

   private static class Window extends JFrame implements KeyListener
   {
      private Socket socket;
      private PrintWriter output;
      private Picture robot;
      private HashMap<Integer, State> events;
      private HashMap<State, String> commands;

      public Window(String ip)
      {
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

         this.robot = new Picture();
         this.events = new HashMap<Integer, State>();
         this.commands = new HashMap<State, String>();

         this.events.put(KeyEvent.VK_UP, State.FORWARD);
         this.events.put(KeyEvent.VK_DOWN, State.BACKWARD);
         this.events.put(KeyEvent.VK_RIGHT, State.RIGHT);
         this.events.put(KeyEvent.VK_LEFT, State.LEFT);
         this.events.put(KeyEvent.VK_SPACE, State.RESET);
         this.events.put(KeyEvent.VK_ESCAPE, State.EXIT);

         this.commands.put(State.STOP, "robot-stop");
         this.commands.put(State.FORWARD, "robot-forward 25");
         this.commands.put(State.BACKWARD, "robot-backward 25");
         this.commands.put(State.RIGHT, "robot-right 25");
         this.commands.put(State.LEFT, "robot-left 25");

         addKeyListener(this);
         setFocusable(true);
         setFocusTraversalKeysEnabled(false);
      }
      
      public void keyPressed(KeyEvent input)
      {
         if((this.robot.state == State.IDLE) && (this.events.containsKey(input.getKeyCode())))
         {
            this.robot.state = this.events.get(input.getKeyCode());

            switch(this.robot.state)
            {
               case RESET:
                  this.robot.reset();
                  break;

               case EXIT:
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
               
                  System.exit(0);

               default:
                  this.output.println(this.commands.get(this.robot.state));
            }
         }

         repaint();
      }
   
      public void keyReleased(KeyEvent input)
      {
         if(this.robot.state == State.RUNNING)
         {
            this.robot.state = State.STOP;
            this.output.println("robot-stop");
         }
         
         repaint();
      }

      public void keyTyped(KeyEvent input){}
   }

   private static class Picture extends JComponent
   {
      private double[] x;
      private double[] y;
      private double theta;
      private State state;
      private State action;

      public Picture()
      {
         this.x = new double[IMAGE[0].length];
         this.y = new double[IMAGE[1].length];
         reset();
      }

      public void paintComponent(Graphics g)
      {
         int[] xPoints = new int[IMAGE[0].length];
         int[] yPoints = new int[IMAGE[1].length]; 
         int i;

         switch(this.state)
         {
            case STOP:
               this.state = State.IDLE;
               this.action = State.STOP;
               break;

            case FORWARD:
               this.state = State.RUNNING;
               this.action = State.FORWARD;
               break;

            case BACKWARD:
               this.state = State.RUNNING;
               this.action = State.BACKWARD;
               break;

            case RIGHT:
               this.state = State.RUNNING;
               this.action = State.RIGHT;
               break;

            case LEFT:
               this.state = State.RUNNING;
               this.action = State.LEFT;
               break;
         }

         switch(this.action)
         {
            case FORWARD:
               forward();
               break;
            
            case BACKWARD:
               backward();
               break;

            case RIGHT:
               right();
               break;

            case LEFT:
               left();
               break;
         }

         for(i=0; i<x.length; i++)
         {
            xPoints[i] = (int)Math.round(this.x[i]);
            yPoints[i] = (int)Math.round(this.y[i]);
         }

         super.paintComponent(g);
         g.drawPolygon(xPoints, yPoints, xPoints.length);
         g.fillPolygon(xPoints, yPoints, xPoints.length);
         g.setColor(Color.BLACK);
      }

      public void forward()
      {
         int i;
         
         for(i=0; i<this.x.length; i++)
         {
            this.x[i] += SPEED * Math.cos(this.theta);
            this.y[i] += SPEED * Math.sin(this.theta);
         }
      }

      public void backward()
      {
         int i;
         
         for(i=0; i<this.x.length; i++)
         {
            this.x[i] -= SPEED * Math.cos(this.theta);
            this.y[i] -= SPEED * Math.sin(this.theta);
         }
      }

      public void right()
      {
         double xMid;
         double yMid;
         double radius;
         double angle;
         int i;
 
         xMid = 0;
         yMid = 0;

         for(i=0; i<this.x.length; i++)
         {
            xMid += this.x[i];
            yMid += this.y[i];
         }

         xMid /= x.length;
         yMid /= y.length;

         for(i=0; i<this.x.length; i++)
         {
            radius = Math.sqrt(((this.x[i] - xMid) * (this.x[i] - xMid)) + ((this.y[i] - yMid) * (this.y[i] - yMid)));
            angle = Math.atan2(this.y[i] - yMid, this.x[i] - xMid) + Math.toRadians(SPEED);

            this.x[i] = (radius * Math.cos(angle)) + xMid;
            this.y[i] = (radius * Math.sin(angle)) + yMid;
         }

         this.theta += Math.toRadians(SPEED);
      }

      public void left()
      {
         double xMid;
         double yMid;
         double radius;
         double angle;
         int i;
       
         xMid = 0;
         yMid = 0;

         for(i=0; i<this.x.length; i++)
         {
            xMid += this.x[i];
            yMid += this.y[i];
         }

         xMid /= x.length;
         yMid /= y.length;

         for(i=0; i<this.x.length; i++)
         {
            radius = Math.sqrt(((this.x[i] - xMid) * (this.x[i] - xMid)) + ((this.y[i] - yMid) * (this.y[i] - yMid)));
            angle = Math.atan2(this.y[i] - yMid, this.x[i] - xMid) - Math.toRadians(SPEED);
            
            this.x[i] = (radius * Math.cos(angle)) + xMid;
            this.y[i] = (radius * Math.sin(angle)) + yMid;
         }

         this.theta -= Math.toRadians(SPEED);
      }

      public void reset()
      {
         int xMid;
         int yMid;
         int i;

         xMid = SIZE / 2;
         yMid = SIZE / 2;

         for(i=0; i<IMAGE[0].length; i++)
         {
            this.x[i] = IMAGE[0][i] + xMid;
            this.y[i] = IMAGE[1][i] + yMid;
         }

         this.theta = -1 * Math.PI / 2;
         this.state = State.IDLE;
         this.action = State.STOP;
      }
   }
}

