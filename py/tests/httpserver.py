#!/bin/python
from http import server
import socketserver

class HTTPHandler(server.SimpleHTTPRequestHandler):
    def do_GET(self):
        self.directory = "/"
        return super().do_GET()


port = 8080

handler = HTTPHandler
httpd = socketserver.TCPServer(("163.184.56.140", port), handler)

httpd.serve_forever()

