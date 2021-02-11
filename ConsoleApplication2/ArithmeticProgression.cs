namespace ConsoleApplication2
{
    class ArithmeticProgression //: IEnumerable<int>
   {
      int begin;
      int end;
      int step;

      public ArithmeticProgression(int begin, int end, int step)
      {
         this.begin = begin;
         this.end = end;
         this.step = step;
      }

      public Iterator GetEnumerator() { return new Iterator(this); }
      //IEnumerator IEnumerable.GetEnumerator() { return GetEnumerator(); }

      public class Iterator //: IEnumerator<int>
      {
         ArithmeticProgression arithmeticProgression;

         public Iterator(ArithmeticProgression arithmeticProgression)
         {
            this.arithmeticProgression = arithmeticProgression;
            Current = arithmeticProgression.begin;
         }

         public int Current { get; private set; }

         //object IEnumerator.Current { get { return Current; } }

         public void Dispose() { }

         public bool MoveNext()
         {
            Current += arithmeticProgression.step;
            return Current <= arithmeticProgression.end;
         }

         public void Reset() { Current = arithmeticProgression.begin; }
      }
   }
}
