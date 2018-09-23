using System;
using System.Collections.Generic;
using System.Text;

namespace test.reactveTests
{
    public class MyEventArgs : EventArgs
    {
        public MyEventArgs(string name, string name2, int age)
        {
            this.Name = name;
            Name2 = name2;
            Age = age;
        }
        public string Name { get; set; }
        public string Name2 { get; set; }
        public int Age { get; set; }

        public override string ToString()
        {
            return Name + " " + Name2 + " " + Age;
        }
    }
}
