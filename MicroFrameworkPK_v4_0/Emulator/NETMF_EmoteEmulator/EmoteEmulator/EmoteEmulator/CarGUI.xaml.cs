using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Media.Animation;

namespace EmoteEmulator
{
    /// <summary>
    /// Interaction logic for CarGUI.xaml
    /// </summary>
    public partial class CarGUI : UserControl 
    {
        Storyboard moveStory;
        //Image car = new Image();
        public Boolean isAnimating = false;
        Vector IntialOffset;
        Vector previousOffset;
        Timer testTimer;
        double X, Y = 0;

        public CarGUI()
        {
            InitializeComponent();
            /*BitmapImage bi3 = new BitmapImage();
            bi3.BeginInit();
            bi3.UriSource = new Uri("Redcar.jpg", UriKind.Relative);
            bi3.EndInit();
            //car.Stretch = Stretch.Fill;
            //car.Height = 150;
            //car.Width = 50;
            car.Source = bi3;
            int c_int = canvas1.Children.Add(car);
            */

            
            double cWidth = canvas1.Width;
            double cHeight = canvas1.Height;
            previousOffset.X = canvas1.Width / 2;
            previousOffset.Y = canvas1.Width / 2;

            Console.WriteLine("Canvas Width: " + cWidth.ToString() + ", Length: " + cHeight.ToString() );
            Vector IntialOffset = VisualTreeHelper.GetOffset(car);
            //car.SetValue(Canvas.LeftProperty, canvas1.Width / 2);
            //car.SetValue(Canvas.TopProperty, canvas1.Height/2);
            
            //canvas1.Children[c_int].
            testTimer = new Timer(new TimerCallback(testTimerCallback), null, 0, 10000);
            

            moveCar(cWidth/2, cHeight/2, 1000);

            //moveCar(car, cWidth/4, 0, 4000);
        }

        public void moveCar( double newX, double newY, int duration)
        {

            Vector offset = VisualTreeHelper.GetOffset(car);
            var top = offset.Y;
            var left = offset.X ;
            
            //var top = Canvas.GetTop(car);
            //var left = Canvas.GetLeft(car);
            TimeSpan durationMS = TimeSpan.FromMilliseconds(duration);
            //TimeSpan durationY = TimeSpan.FromMilliseconds(duration);
            
            /*
            moveStory = new Storyboard();
            //if (callback != null) _moveStoryboard.Completed += callback;


            //Console.WriteLine("Current Location, X: " + previousOffset.X.ToString() + " , Y: " + previousOffset.Y.ToString());
            Console.WriteLine("Current Location, X: " + left.ToString() + " , Y: " + top.ToString());
            Console.WriteLine("Target Location, X: " + newX.ToString() + " , Y: " + newY.ToString());
            //TranslateTransform trans = new TranslateTransform();
            //car.RenderTransform = trans;

            
            DoubleAnimation anim1 = new DoubleAnimation(newY , durationMS);
            Storyboard.SetTargetProperty(anim1, new PropertyPath("(Canvas.Top)"));

            Console.WriteLine("Done 1");

            DoubleAnimation anim2 = new DoubleAnimation(newX , durationMS);
            Storyboard.SetTargetProperty(anim2, new PropertyPath("(Canvas.Left)"));

            Console.WriteLine("Done 2");

            
            //trans.BeginAnimation(TranslateTransform.YProperty, anim1);
            //trans.BeginAnimation(TranslateTransform.XProperty, anim2);

            moveStory.Completed += new EventHandler(moveStory_Completed);
            moveStory.Children.Add(anim1);
            moveStory.Children.Add(anim2);

            Console.WriteLine("Done 3");

            moveStory.Begin(car);
            */
            

            Console.WriteLine("Done 3");
            car.SetValue(Canvas.TopProperty, newY);
            car.SetValue(Canvas.LeftProperty, newX);
            Console.WriteLine("Done 4");
            isAnimating = true;

            previousOffset.X = newX;
            previousOffset.Y = newY;
            Console.WriteLine("Done 5");
            //Canvas.SetTop(car, newY);
            //Canvas.SetLeft(car, newX);

            
        }

        void moveStory_Completed(object sender, EventArgs e)
        {
            //if (moveStory != null)
            //{
                Console.WriteLine("Done with move");
                Vector offset = VisualTreeHelper.GetOffset(car);
                var top = offset.Y;
                var left = offset.X;
                Console.WriteLine("Final Location, X: " + left.ToString() + " , Y: " + top.ToString());
                isAnimating = false;
            //}
            
            
        }
        public void testTimerCallback (object state){
            X += 2;
            Y += 4;
            moveCar(X, Y, 750);
        }
        public void UpdateLocation(double X, double Y, int duration)
        {
            if (!isAnimating)
            {
                moveCar(X, Y, duration);
            }

        }

       
    }
    
}
