using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Reactive;
using System.Reactive.Linq;
using System.Xml.Linq;
using test.reactveTests;

namespace test
{
    class Program
    {
        static void Main(string[] args)
        {
            //SubscribeTest.test();
            //var processes = Process.GetProcessesByName("PlanckSimulator.exe");
            //var processes2 = Process.GetProcessesByName("PlanckSimulator");

            //string str = File.ReadAllText(@"C:\\getcomputerinfo.xml");
            //var doc = XDocument.Parse(str);
            //Console.WriteLine(doc.Element("Name").Value);
            HttpWebRequest request = WebRequest.Create("https://github.com/wangjiaming0909/thread_pool/blob/master/configure") as HttpWebRequest;
            request.ProtocolVersion = HttpVersion.Version11;
            request.KeepAlive = true;
            request.Method = "GET";
            request.UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Safari/537.36";
            request.Host = "github.com";
            var response = request.GetResponseAsync();
            var res = response.Result as HttpWebResponse;
            Stream s = res.GetResponseStream();
            Byte[] bs = new Byte[2048];
            s.Read(bs, 0, 1024);
            char[] cs = new char[2048];
            int i = 0;
            foreach(var b in bs)
            {
                cs[i] = Convert.ToChar(b);
                i++;
            }
            Console.WriteLine(cs);
            s.Read(bs, 1024, 1024);
            i = 0;
            foreach(var b in bs)
            {
                cs[i] = Convert.ToChar(b);
                i++;
            }
            Console.WriteLine(cs);
        }
    }
}
