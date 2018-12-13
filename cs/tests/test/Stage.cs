using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

namespace test
{
    class Stage
    {
        private HttpWebRequest _request;
        private Byte[] _data;
        private int _bufferSize;
        private int _offset;
        public Stage(string uri, int bufferSize, int offset)
        {
            _bufferSize = bufferSize;
            _data = new byte[_bufferSize];
            _offset = offset;
            _request = WebRequest.Create(uri) as HttpWebRequest;
            _request.KeepAlive = true;
            _request.ProtocolVersion = HttpVersion.Version11;
            _request.Method = "GET";
        }

        public Byte[] GetStream()
        {
            Stream s = (_request.GetResponse() as HttpWebResponse).GetResponseStream();
            s.Read(_data, _offset, _bufferSize);
            return _data;
        }

        public void SetUserAgent(string agent)
        {
            _request.UserAgent = agent;
        }
        public void SetHost(string host)
        {
            _request.Host = host;
        }
        public void SetCookie(string name, string value, string domain)
        {
            Cookie c = new Cookie(name, value);
            c.Domain = domain;
            _request.CookieContainer.Add(c);
        }
        public void SetReferer(string referer)
        {
            _request.Referer = referer;
        }
    }
}
