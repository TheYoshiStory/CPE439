import java.lang.Math;
import java.util.HashMap;
import java.util.ArrayList;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.net.Socket;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;
import javax.swing.JComponent;
import javax.swing.JFrame;

public class RobotClient
{
   private static final int PORT = 23;
   private static final int SIZE = 500;
   private static final int HIGH_SPEED = 75;
   private static final int MID_SPEED = 50;
   private static final int LOW_SPEED = 25;
   private static final int STOP_SPEED = 0;

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
      private PrintWriter out;
      private BufferedReader in;
      private HashMap<Integer, Boolean> event;
      private ArrayList<Integer> data;
      private Picture gui;
      private int offset;

      public Window(String ip)
      {
         try
         {
            System.out.println("connecting to address " + ip + " on port " + PORT + "...");
            this.socket = new Socket(ip, PORT);
            this.out = new PrintWriter(this.socket.getOutputStream(), true);
            this.in = new BufferedReader(new InputStreamReader(this.socket.getInputStream()));
            this.in.readLine();
            this.in.readLine();
            this.in.readLine();
            System.out.println("robot-distance");
            this.out.println("robot-distance");
            this.offset = Integer.parseInt(this.in.readLine().substring(1));
            this.in.readLine();
            System.out.println("socket connection established");
         }
         catch(IOException e)
         {
            System.err.println("ERROR: failed to open socket connection");
            System.exit(1);
         }
         
         this.event = new HashMap<Integer, Boolean>();
         this.data = new ArrayList<Integer>();
         this.gui = new Picture(event, data);
         
         this.event.put(KeyEvent.VK_UP, false);
         this.event.put(KeyEvent.VK_DOWN, false);
         this.event.put(KeyEvent.VK_RIGHT, false);
         this.event.put(KeyEvent.VK_LEFT, false);
         this.data.add(0);

         addKeyListener(this);
         setFocusable(true);
         setFocusTraversalKeysEnabled(false);
      }

      public void sendControlCommand()
      {
         boolean up;
         boolean down;
         boolean right;
         boolean left;
         int[] speed;
         
         up = this.event.get(KeyEvent.VK_UP);
         down = this.event.get(KeyEvent.VK_DOWN);
         right = this.event.get(KeyEvent.VK_RIGHT);
         left = this.event.get(KeyEvent.VK_LEFT);
         speed = new int[2];

         if((up && !down && !right && !left) || (up && !down && right && left))
         {
            speed[0] = MID_SPEED;
            speed[1] = MID_SPEED;
         }
         else if(up && !down && right && !left)
         {
            speed[0] = MID_SPEED;
            speed[1] = LOW_SPEED;
         }
         else if((!up && !down && right && !left) || (up && down && right && !left))
         {
            speed[0] = MID_SPEED;
            speed[1] = MID_SPEED * -1;
         }
         else if(!up && down && right && !left)
         {
            speed[0] = LOW_SPEED * -1;
            speed[1] = MID_SPEED * -1;
         }
         else if((!up && down && !right && !left) || (!up && down && right && left))
         {
            speed[0] = MID_SPEED * -1;
            speed[1] = MID_SPEED * -1;
         }
         else if(!up && down && !right && left)
         {
            speed[0] = MID_SPEED * -1;
            speed[1] = LOW_SPEED * -1;
         }
         else if((!up && !down && !right && left) || (up && down && !right && left))
         {
            speed[0] = MID_SPEED * -1;
            speed[1] = MID_SPEED;
         }
         else if(up && !down && !right && left)
         {
            speed[0] = LOW_SPEED;
            speed[1] = MID_SPEED;
         }
         else
         {
            speed[0] = STOP_SPEED;
            speed[1] = STOP_SPEED;
         }
         
         try
         {
            System.out.println("robot-control " + speed[0] + "  " + speed[1]);
            this.out.println("robot-control " + speed[0] + "  " + speed[1]);
            this.in.readLine();
         }
         catch(IOException e)
         {
            System.err.println("ERROR: I/O error occurred");
            System.exit(1);
         }
      }

      public void sendDistanceCommand()
      {  
         try
         {
            System.out.println("robot-distance");
            this.out.println("robot-distance");
            this.data.set(0, Integer.parseInt(this.in.readLine().substring(1)) - this.offset);
            this.in.readLine();
         }
         catch(IOException e)
         {
            System.err.println("ERROR: I/O error occurred");
            System.exit(1);
         }
      }

      public void keyPressed(KeyEvent input)
      {
         if(input.getKeyCode() == KeyEvent.VK_ESCAPE)
         {
            this.event.put(KeyEvent.VK_UP, false);
            this.event.put(KeyEvent.VK_DOWN, false);
            this.event.put(KeyEvent.VK_RIGHT, false);
            this.event.put(KeyEvent.VK_LEFT, false);
            sendControlCommand();
            
            try
            {
               System.out.println("closing socket connection...");
               this.socket.close();
               this.out.close();
               this.in.close();
               System.out.println("socket connection closed");
            }
            catch(IOException e)
            {
               System.err.println("ERROR: failed to close socket connection");
               System.exit(1);
            }
            
            System.exit(0);
         }
         else if(this.event.containsKey(input.getKeyCode()))
         {
            if(!this.event.get(input.getKeyCode()))
            {
               this.event.put(input.getKeyCode(), true);
               sendControlCommand();
            }
         }

         repaint();
      }
   
      public void keyReleased(KeyEvent input)
      {
         if(this.event.containsKey(input.getKeyCode()))
         {
            if(this.event.get(input.getKeyCode()))
            {
               this.event.put(input.getKeyCode(), false);
               sendControlCommand();
               sendDistanceCommand();
            }
         }
         
         repaint();
      }

      public void keyTyped(KeyEvent input){}
   }

   private static class Picture extends JComponent
   {
      private HashMap<Integer, Boolean> event;
      private ArrayList<Integer> data;

      public Picture(HashMap<Integer, Boolean> event, ArrayList<Integer> data)
      {
         this.event = event;
         this.data = data;
      }

      public void paintComponent(Graphics g)
      {
         int xMid;
         int yMid;
         
         xMid = SIZE / 2;
         yMid = SIZE / 2;

         super.paintComponent(g);
         g.setColor(Color.BLUE);
         g.drawString("ODOMETER: " + String.format("%d", data.get(0) / 1000) + "." + String.format("%03d", data.get(0) % 1000)+ "m", 5, SIZE - 10);

         if(this.event.get(KeyEvent.VK_UP))
         {
            g.fillRoundRect(xMid - 50, yMid - 105, 100, 100, 25, 25);
         }
         
         if(this.event.get(KeyEvent.VK_DOWN))
         {
            g.fillRoundRect(xMid - 50, yMid, 100, 100, 25, 25);
         }
         
         if(this.event.get(KeyEvent.VK_RIGHT))
         {
            g.fillRoundRect(xMid + 55, yMid, 100, 100, 25, 25);
         }
         
         if(this.event.get(KeyEvent.VK_LEFT))
         {
            g.fillRoundRect(xMid - 155, yMid, 100, 100, 25, 25);
         }
         
         g.setColor(Color.BLACK);
         g.drawRoundRect(xMid - 50, yMid - 105, 100, 100, 25, 25);
         g.drawRoundRect(xMid - 50, yMid, 100, 100, 25, 25);
         g.drawRoundRect(xMid + 55, yMid, 100, 100, 25, 25);
         g.drawRoundRect(xMid - 155, yMid, 100, 100, 25, 25);
      }
   }
}

