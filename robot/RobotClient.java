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
   private static final int HIGH_SPEED = 50;
   private static final int LOW_SPEED = 25;
   private static final int STOP_SPEED = 0;
   private static final int MAX_RANGE = 10000;

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
         this.data.add(0);
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
            speed[0] = HIGH_SPEED;
            speed[1] = HIGH_SPEED;
         }
         else if(up && !down && right && !left)
         {
            speed[0] = HIGH_SPEED;
            speed[1] = LOW_SPEED;
         }
         else if((!up && !down && right && !left) || (up && down && right && !left))
         {
            speed[0] = HIGH_SPEED;
            speed[1] = HIGH_SPEED * -1;
         }
         else if(!up && down && right && !left)
         {
            speed[0] = LOW_SPEED * -1;
            speed[1] = HIGH_SPEED * -1;
         }
         else if((!up && down && !right && !left) || (!up && down && right && left))
         {
            speed[0] = HIGH_SPEED * -1;
            speed[1] = HIGH_SPEED * -1;
         }
         else if(!up && down && !right && left)
         {
            speed[0] = HIGH_SPEED * -1;
            speed[1] = LOW_SPEED * -1;
         }
         else if((!up && !down && !right && left) || (up && down && !right && left))
         {
            speed[0] = HIGH_SPEED * -1;
            speed[1] = HIGH_SPEED;
         }
         else if(up && !down && !right && left)
         {
            speed[0] = LOW_SPEED;
            speed[1] = HIGH_SPEED;
         }
         else
         {
            speed[0] = STOP_SPEED;
            speed[1] = STOP_SPEED;
         }
         
         this.data.set(1, speed[0]);
         this.data.set(2, speed[1]);
         
         try
         {
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
         int distance;
         int left;
         int right;
         
         super.paintComponent(g);
         
         xMid = SIZE / 2;
         yMid = SIZE / 2;
         distance = data.get(0);
         left = data.get(1);
         right = data.get(2);

         g.setColor(Color.BLUE);

         if(this.event.get(KeyEvent.VK_UP))
         {
            g.fillRoundRect(xMid - 50, yMid - 5, 100, 100, 25, 25);
         }
         
         if(this.event.get(KeyEvent.VK_DOWN))
         {
            g.fillRoundRect(xMid - 50, yMid + 100, 100, 100, 25, 25);
         }
         
         if(this.event.get(KeyEvent.VK_RIGHT))
         {
            g.fillRoundRect(xMid + 55, yMid + 100, 100, 100, 25, 25);
         }
         
         if(this.event.get(KeyEvent.VK_LEFT))
         {
            g.fillRoundRect(xMid - 155, yMid + 100, 100, 100, 25, 25);
         }
         
         g.setColor(Color.GREEN);

         if(distance > MAX_RANGE)
         {
            g.fillRect(xMid - 175, yMid - 180, 75, 25);
         }

         if(distance > MAX_RANGE * 0.8)
         {
            g.fillRect(xMid - 175, yMid - 155, 75, 25);
         }

         if(distance > MAX_RANGE * 0.6)
         {
            g.fillRect(xMid - 175, yMid - 130, 75, 25);
         }

         if(distance > MAX_RANGE * 0.4)
         {
            g.fillRect(xMid - 175, yMid - 105, 75, 25);
         }

         if(distance > MAX_RANGE * 0.2)
         {
            g.fillRect(xMid - 175, yMid - 80, 75, 25);
         }

         g.setColor(Color.YELLOW);
         g.fillRect(xMid + 100, yMid - 130, 50, 25);
         g.fillRect(xMid + 170, yMid - 130, 50, 25);

         switch(left)
         {
            case HIGH_SPEED:
               g.setColor(Color.RED);
               g.fillRect(xMid + 100, yMid - 180, 50, 25);
               g.setColor(Color.ORANGE);
               g.fillRect(xMid + 100, yMid - 155, 50, 25);
               break;
            
            case LOW_SPEED:
               g.setColor(Color.ORANGE);
               g.fillRect(xMid + 100, yMid - 155, 50, 25);
               break;
         
            case LOW_SPEED * -1:
               g.setColor(Color.ORANGE);
               g.fillRect(xMid + 100, yMid - 105, 50, 25);
               break;
            
            case HIGH_SPEED * -1:
               g.setColor(Color.ORANGE);
               g.fillRect(xMid + 100, yMid - 105, 50, 25);
               g.setColor(Color.RED);
               g.fillRect(xMid + 100, yMid - 80, 50, 25);
               break;
         }
         
         switch(right)
         {
            case HIGH_SPEED:
               g.setColor(Color.RED);
               g.fillRect(xMid + 170, yMid - 180, 50, 25);
               g.setColor(Color.ORANGE);
               g.fillRect(xMid + 170, yMid - 155, 50, 25);
               break;
            
            case LOW_SPEED:
               g.setColor(Color.ORANGE);
               g.fillRect(xMid + 170, yMid - 155, 50, 25);
               break;
         
            case LOW_SPEED * -1:
               g.setColor(Color.ORANGE);
               g.fillRect(xMid + 170, yMid - 105, 50, 25);
               break;
            
            case HIGH_SPEED * -1:
               g.setColor(Color.ORANGE);
               g.fillRect(xMid + 170, yMid - 105, 50, 25);
               g.setColor(Color.RED);
               g.fillRect(xMid + 170, yMid - 80, 50, 25);
               break;
         }

         g.setColor(Color.BLACK);
         g.drawRoundRect(xMid - 50, yMid - 5, 100, 100, 25, 25);
         g.drawRoundRect(xMid - 50, yMid + 100, 100, 100, 25, 25);
         g.drawRoundRect(xMid + 55, yMid + 100, 100, 100, 25, 25);
         g.drawRoundRect(xMid - 155, yMid + 100, 100, 100, 25, 25);
         g.drawString("DISTANCE", xMid - 166, yMid - 185);
         g.drawRect(xMid - 175, yMid - 180, 75, 25);
         g.drawRect(xMid - 175, yMid - 155, 75, 25);
         g.drawRect(xMid - 175, yMid - 130, 75, 25);
         g.drawRect(xMid - 175, yMid - 105, 75, 25);
         g.drawRect(xMid - 175, yMid - 80, 75, 25);
         g.drawString("LEFT", xMid + 110, yMid - 185);
         g.drawRect(xMid + 100, yMid - 180, 50, 25);
         g.drawRect(xMid + 100, yMid - 155, 50, 25);
         g.drawRect(xMid + 100, yMid - 130, 50, 25);
         g.drawRect(xMid + 100, yMid - 105, 50, 25);
         g.drawRect(xMid + 100, yMid - 80, 50, 25);
         g.drawString("RIGHT", xMid + 177, yMid - 185);
         g.drawRect(xMid + 170, yMid - 180, 50, 25);
         g.drawRect(xMid + 170, yMid - 155, 50, 25);
         g.drawRect(xMid + 170, yMid - 130, 50, 25);
         g.drawRect(xMid + 170, yMid - 105, 50, 25);
         g.drawRect(xMid + 170, yMid - 80, 50, 25);
      }
   }
}

