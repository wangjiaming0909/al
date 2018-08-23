using System;

namespace cs
{
    class Program
    {
        static void Main(string[] args) {
            int[] arr = {1,2,3,5,4};
            foreach (var item in TwoSum.SumOfTwo(arr, 6)){
             System.Console.WriteLine(item);   
            }

            foreach (var item in TwoSum.SumOfTwo2(arr, 6)){
                System.Console.WriteLine(item);
            }
            System.Console.ReadKey();
        }
    }
}
