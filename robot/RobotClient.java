import java.net.Socket;
import java.io.PrintWriter;
import java.io.IOException;
import java.lang.Math;
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
   private static final String ip = "192.168.0.100";
   private static final int port = 23;
   private static final int size = 500;
   private static final int speed = 10;

   public static void main(String[] args)
   {
      Window window;
 
      window = new Window();
      window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      window.setResizable(true);
      window.setMinimumSize(new Dimension(size, size));
      window.setSize(size * 2, size * 2);
      window.getContentPane().add(window.robot);
      window.pack();
      window.setVisible(true);
   }

   private static class Window extends JFrame implements KeyListener
   {
      private Socket socket;
      private PrintWriter output;
      private Picture robot;

      public Window()
      {
         try
         {
            this.socket = new Socket(ip, port);
            this.output = new PrintWriter(this.socket.getOutputStream(), true);
         }
         catch(IOException e)
         {
            System.exit(1);
         }

         this.robot = new Picture();
         addKeyListener(this);
         setFocusable(true);
         setFocusTraversalKeysEnabled(false);
      }
      
      public void keyPressed(KeyEvent input)
      {
         switch(input.getKeyCode())
         {
            case KeyEvent.VK_UP:
               this.output.println("robot-control f 25 10");
               this.robot.forward();
               break;
         
            case KeyEvent.VK_DOWN:
               this.output.println("robot-control b 25 10");
               this.robot.backward();
               break;
         
            case KeyEvent.VK_RIGHT:
               this.output.println("robot-control r 25 10");
               this.robot.right();
               break;
         
            case KeyEvent.VK_LEFT:
               this.output.println("robot-control l 25 10");
               this.robot.left();
               break;

            case KeyEvent.VK_ESCAPE:
               try
               {
                  this.socket.close();
                  this.output.close();
               }
               catch(IOException e)
               {
                  System.exit(1);
               }
               
               System.exit(0);
               break;
         }

      }
   
      public void keyReleased(KeyEvent input){}

      public void keyTyped(KeyEvent input){}
   }

   private static class Picture extends JComponent
   {
      private double[] x = {100, 125, 150, 150, 100};
      private double[] y = {100, 75, 100, 150, 150};
      private double ang = -1 * Math.PI / 2;

      public void paintComponent(Graphics g)
      {
         int[] xPoints = new int[x.length];
         int[] yPoints = new int[y.length]; 
         int i;

         for(i=0; i<x.length; i++)
         {
            xPoints[i] = (int)Math.round(x[i]);
            yPoints[i] = (int)Math.round(y[i]);
         }

         super.paintComponent(g);
         g.drawPolygon(xPoints, yPoints, xPoints.length);
         g.fillPolygon(xPoints, yPoints, xPoints.length);
         g.setColor(Color.BLACK);
      }

      public void forward()
      {
         int i;
         
         for(i=0; i<x.length; i++)
         {
            x[i] += speed * Math.cos(ang);
            y[i] += speed * Math.sin(ang);
         }

         repaint();
      }

      public void backward()
      {
         int i;
         
         for(i=0; i<x.length; i++)
         {
            x[i] -= speed * Math.cos(ang);
            y[i] -= speed * Math.sin(ang);
         }

         repaint();
      }

      public void right()
      {
         double xMid;
         double yMid;
         double r;
         double a;
         int i;
 
         xMid = 0;
         yMid = 0;

         for(i=0; i<x.length; i++)
         {
            xMid += x[i];
            yMid += y[i];
         }

         xMid /= x.length;
         yMid /= y.length;

         for(i=0; i<x.length; i++)
         {
            r = Math.sqrt(((x[i] - xMid) * (x[i] - xMid)) + ((y[i] - yMid) *(y[i] - yMid)));
            a = Math.atan2(y[i] - yMid, x[i] - xMid) + Math.toRadians(speed);

            x[i] = (r * Math.cos(a)) + xMid;
            y[i] = (r * Math.sin(a)) + yMid;
         }

         ang += Math.toRadians(speed);
         repaint();
      }

      public void left()
      {
         double xMid;
         double yMid;
         double r;
         double a;
         int i;
       
         xMid = 0;
         yMid = 0;

         for(i=0; i<x.length; i++)
         {
            xMid += x[i];
            yMid += y[i];
         }

         xMid /= x.length;
         yMid /= y.length;

         for(i=0; i<x.length; i++)
         {
            r = Math.sqrt(((x[i] - xMid) * (x[i] - xMid)) + ((y[i] - yMid) *(y[i] - yMid)));
            a = Math.atan2(y[i] - yMid, x[i] - xMid) - Math.toRadians(speed);
            
            x[i] = (r * Math.cos(a)) + xMid;
            y[i] = (r * Math.sin(a)) + yMid;
         }

         ang -= Math.toRadians(speed);
         repaint();
      }
   }
}

