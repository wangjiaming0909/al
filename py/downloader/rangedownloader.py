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
    Not_Respond_To_Range = 6
    Range_Match = 7
    Range_Not_Match = 8

class RangeDownloader:
    start_ = 0
    end_ = 0
    status_ = Status.IDLE
    headerStatus_ = HeaderStatus.IDLE
    url_ = ''
    http_ = None
    response_ = None
    fileName_ = 'DefaultFile'
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
        transferEncoding = self.response_.headers.get('Transfer-Encoding')
        if transferEncoding != None and transferEncoding == 'chunked':
            self.headerStatus_ = HeaderStatus.Chunked
            return
        
        self.headerStatus_ = HeaderStatus.With_Content_Disposition
        contentDisposition = self.response_.headers.get('Content-Disposition')
        if contentDisposition == None:
            print('No content disposition')
            self.headerStatus_ = HeaderStatus.Without_Content_Disposition
            return 

        self.retriveFileName(contentDisposition)
        contentRange = self.response_.headers.get('Content-Range')
        if contentRange == None:
            print('server does not respond to range header')
            self.headerStatus_ = HeaderStatus.Not_Respond_To_Range
            return
        
        self.retriveFileSize(contentRange)
        if self.validRange(contentRange):
            self.headerStatus_ = HeaderStatus.Range_Match
        else:
            self.headerStatus_ = HeaderStatus.Range_Not_Match

    def validRange(self, contentRange):
        return True

    def retriveFileSize(self, contentRange):
        self.fileSize_ = 1;


    def retriveFileName(self, contentDispositionHeader):
        contentDispositionHeader.find('')
        self.fileName_ = ''
        

    def download(self):
        self.response_ = self.http_.request('GET', self.url_)
        self.updateStatus()
