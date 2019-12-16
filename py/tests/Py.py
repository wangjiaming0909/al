import sys
import http
from http import server
import socketserver


#class Handler(http.server.BaseHTTPRequestHandler):
#    def 

#handler = Handler()
#http.server.HTTPServer('localhost', )


class HTTPHandler(server.SimpleHTTPRequestHandler):
    def __init__(self, *args, directory = None, **kwargs):
        return super().__init__(*args, **kwargs)

    def do_GET(self):
        print(self.path)
        print(self.client_address)
        self.directory = "C://"
        print(self.directory)
        print('DO get')
        return super().do_GET()

port = 8080

handler = HTTPHandler
httpd = socketserver.TCPServer(("localhost", port), handler)


httpd.serve_forever()
