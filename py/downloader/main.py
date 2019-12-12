import rangedownloader
import sys

print('hello world')

url = sys.argv[1]

# rd = rangedownloader.RangeDownloader(0, 99, 'http://d1.music.126.net/dmusic/netease-cloud-music_1.2.1_amd64_ubuntu_20190428.deb')
rd = rangedownloader.RangeDownloader(0, 99, url)

fileNames = []

def writeData(data, fileName):
    fileNames.append(fileName)
    pass

if rd.headerStatus_ == rangedownloader.HeaderStatus.Chunked or rd.headerStatus_ == rangedownloader.HeaderStatus.Not_Respond_To_Range:
    writeData(rd.response_.data, rd.fileName_)
    exit()

if(rd.headerStatus_ == rangedownloader.HeaderStatus.Range_Match):
    pass