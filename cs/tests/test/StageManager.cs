using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json.Bson;

namespace test
{
    class StageManager
    {
        private LinkedList<Stage> _stages;
        private int _number_of_conenctions;
        public StageManager(int numberOfConnections)
        {
            _number_of_conenctions = numberOfConnections;
            _stages = new LinkedList<Stage>();
        }

        public void Read()
        {
            //JsonReaderWriterFactory.CreateJsonReader()
            //BsonReader reader = new BsonReader()
            //bson
        }
        
    }
}
