
fileNameToWrite = 'tmpFile.txt';

toWritePyFile = open(fileNameToWrite, 'w');
toWritePyFile.write('123\n123\n123\n');
toWritePyFile.flush();