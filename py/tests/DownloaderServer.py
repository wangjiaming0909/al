from http import server
import socketserver

if __name__ != '__main__':
    pass

class Handler(server.BaseHTTPRequestHandler):
    def __init__(self, request, client_address, server):
        self.wbufsize = 4096
        return super().__init__(request, client_address, server)

    def do_GET(self):
        if self.command != 'GET':
            self.send_error(403, 'forbidden')
            return

        mess = self.command + " " + self.path
        mess = mess.encode('utf-8')
        super().send_response(server.HTTPStatus.OK)
        super().send_header("Connection", "Keep-Alive")
        super().send_header("Content-type", "text/html; charset=utf-8")
        super().send_header("Server", "nginx")
        super().send_header("Content-Length", str(len(mess)))
        super().end_headers()
        self.wfile.write(mess)


handler = Handler

port = 8000
httpd = socketserver.TCPServer(("localhost", port), handler)


httpd.serve_forever()

