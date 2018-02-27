using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication2
{
   class Program
   {
      public static double Multiply <T>(T x, T y)
      {
         return Convert.ToDouble(x) * Convert.ToDouble(y);
      }

      static void Main(string[] args)
      {
         var res = Multiply(1, 2);
         Console.WriteLine(res);
         Console.WriteLine(res.GetType());
      }
   }
}
