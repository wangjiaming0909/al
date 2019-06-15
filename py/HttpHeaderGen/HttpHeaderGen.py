import os.path;
import shutil;

inputFileName = 'HttpCommomHeaders.txt';
outputFileForGperfName = 'headersForGperf.txt';
outputFileForHeadersName = 'headers.txt'

endOfFile = '\n';

def getFileFullName(fileName):
    return 'HttpHeaderGen/' + fileName;

def genHttpHeaderForGperf(line):
    line = line + '';
    line = line.strip();
    line = line.upper();
    line = line.replace(':', 'COLON_');
    line = line.replace('-', '_');
    line = 'HTTP_HEADER_' + line;
    line = line + ', HttpHeaderCode::' + line;
    return line
    

def genHttpHeaders(line):
    line = line + '';
    line = line.strip();
    line = line.upper();
    line = line.replace(':', 'COLON_');
    line = line.replace('-', '_');
    line = 'HTTP_HEADER_' + line;
    return line;



if(not os.path.exists(getFileFullName(inputFileName))):
    print('file not exist: ' + getFileFullName(inputFileName));
    exit();


file = open(getFileFullName(inputFileName), 'r');
newFileForGperf = '';
newFileForHeaders = '';

while True:
    line = file.readline();
    if(line == ''): break;
    lineForGperf = genHttpHeaderForGperf(line);
    lineForHeaders = genHttpHeaders(line);
    newFileForGperf += lineForGperf + endOfFile;
    newFileForHeaders += lineForHeaders + endOfFile;

outputForGperfFile = open(getFileFullName(outputFileForGperfName), 'w');
outputForGperfFile.write(newFileForGperf);
outputForGperfFile.flush();

outputForHeadersFile = open(getFileFullName(outputFileForHeadersName), 'w');
outputForHeadersFile.write(newFileForHeaders);
outputForHeadersFile.flush();

if(not os.path.exists(getFileFullName(outputFileForGperfName))):
    print('file not exist: ' + outputFileForGperfName);
    exit();

if(not os.path.exists(getFileFullName(outputFileForHeadersName))):
    print('file not exist: ' + outputFileForHeadersName);
    exit();


cppFolder = '../cpp/recipes/src/gperf/';
newFileInputFileInCppFolder = cppFolder + inputFileName;
newFileForGperfFullNameInCppFolder = cppFolder+ outputFileForGperfName;
newFileForHeadersInCppFolder = cppFolder + outputFileForHeadersName;

if(not os.path.exists(cppFolder)):
    print('cpp folder not exists: ' + cppFolder);
    exit();

shutil.copyfile(getFileFullName(inputFileName), newFileInputFileInCppFolder);
shutil.copyfile(getFileFullName(outputFileForGperfName), newFileForGperfFullNameInCppFolder);
shutil.copyfile(getFileFullName(outputFileForHeadersName), newFileForHeadersInCppFolder);



