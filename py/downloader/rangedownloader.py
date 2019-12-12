import urllib3
from enum import Enum

if __name__ != '__main__':
    pass

class Status(Enum):
    IDLE = 0
    Received200 = 1
    Received206 = 2
    NOT_FOUND = 3

class HeaderStatus(Enum):
    IDLE = 0
    Chunked = 1
    With_Content_Disposition = 2
    Without_Content_Disposition = 3
    With_Content_Length = 4
    Without_Content_Length = 5

class RangeDownloader:
    start_ = 0
    end_ = 0
    status_ = Status.IDLE
    headerStatus_ = HeaderStatus.IDLE
    url_ = ''
    http_ = None
    response_ = None
    fileName_ = ''
    fileSize_ = 0
    isPartialData = False

    def __init__(self, s, e, url):
        self.start_ = s
        self.end_ = e
        self.url_ = url
        rangeHeader = {'Range' : 'bytes=' + str(s) + '-' + str(e)}
        self.http_ = urllib3.PoolManager(10, rangeHeader)

    def updateStatus(self):
        status = self.response_.status
        if status == 200:
            self.status_ = Status.Received200
        elif status == 206:
            self.status_ = Status.Received206
        elif status == 404:
            self.status_ = Status.NOT_FOUND

    def updateHeaderStatus(self):
        contentEncoding = self.response_.headers.get('Content-Encoding')
        if(contentEncoding != None and contentEncoding == 'chunked'):
            self.headerStatus_ = HeaderStatus.Chunked

    def download(self):
        self.response_ = self.http_.request('GET', self.url_)
        self.updateStatus()
