using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication2
{

    class Program
    {
        static void Main(string[] args)
        {
            TestArithmeticProgression();
        }
        static void TestArithmeticProgression()
        {
            foreach (int item in MakeArithmeticProgression(10, 100, 5))
            {
                Console.WriteLine(item);
            }
            //          var a = MakeArithmeticProgression(20, 5, 50).ToArray();
            //          Console.WriteLine();
        }

        static ArithmeticProgression MakeArithmeticProgression(int begin, int end, int step)
        {
            return new ArithmeticProgression(begin, end, step);
        }
    }
}
