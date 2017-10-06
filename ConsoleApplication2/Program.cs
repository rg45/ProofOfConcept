using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication2
{
    class Program
    {
        [Flags]
        enum MyEnum
        {
            zero = 0,
            one = 1,
            two = 2,
            seven = 7,
            eight = 8,
            twelve = 12,
        };

        static void Main(string[] args)
        {
            MyEnum a = (MyEnum)15;
            Console.WriteLine(a);
        }
    }
}
