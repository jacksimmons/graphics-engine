using System;


namespace Tank
{
    public class Main
    {
        public float FloatVar { get; set; }


        public Main()
        {
            Console.WriteLine("Main constructed!");
        }


        public void PrintMessage()
        {
            Console.WriteLine("Hello World");
        }


        public void PrintCustomMessage(string message)
        {
            Console.WriteLine(message);
        }
    }
}