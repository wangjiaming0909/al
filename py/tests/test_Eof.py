import os.path;


fileNameToWrite = 'tests/eof.tmp';

toWritePyFile = open(fileNameToWrite, 'a');
toWritePyFile.write('123\n123\n123\n');
toWritePyFile.flush();