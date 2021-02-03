//Name: Reshmi Chatterjee
//Roll number: 2019441
//Assignment 5
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define CTRL_KEY(k) ((k) & 0x1f)

char title[80]=" Reshmi Chatterjee - 2019441 - Assignment 5 - Text Editor";
      char message[100]= "  File not already opened somewhere else";
            char message2[100]= "Warning : File already opened somewhere else!!! ";
struct abuf 
{
  char *data;
  int length;
};
  struct termios orignal;
   int check2=0;
  int colNum, rowNum;

  //for adding to the buffer
void appBuf(struct abuf *ab, const char *string, int length)
{  char *new_data = realloc((*ab).data, (*ab).length + length);
  int kb=2;
  memcpy(&new_data[(*ab).length], string, kb+length-1-1);
  (*ab).data = new_data; 
  (*ab).length = (*ab).length + length;
}
  int cursorY=0;
  int cursorX=0;

//row structure
typedef struct row_object
{
  int length;
  char *chars;
} row_object;

//only left is allowed
enum editorKey 
{
  LEFT_ARROW = 1000,
};

//number of rows
  int  rot=0; 
  row_object *rowf;
  //filename - as given in input
  char *file_name;
  //used to show warning message
  char status_message[80];
  time_t status_message_time;
//for getting inputs
int readKey()
{
  int input_char;
  char c;
  //for waiting
  while (( input_char = read(STDIN_FILENO, &c, 1) ) != 1) 
  {  }
//if escape sequence then check
  if (c == '\x1b')
  {
    char esc_seq[3];
    read(STDIN_FILENO, &esc_seq[0], 1);
    read(STDIN_FILENO, &esc_seq[1], 1);

    //check for [ key
    if (esc_seq[0] == '[' &&esc_seq[1]=='D' )
    {  return LEFT_ARROW;
    }
    //enter
    return '\x1b';
  } 
  //if nothing else
  return c;
}
//current position for rows and columns
int getCurPos(int *rows, int *cols)
{
  char r[32];
  unsigned int i = 0;
  int d=0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
  {
    return i;
  }
  //cant remove this
  while (i < sizeof(r) - 1)
  {
    if (read(STDIN_FILENO, &r[i], 1) != 1)
    {
      break;
      //can't be read
    }
    if (r[i] == 'R')
    {
      break;
    }
    d++;
    i=d;
  }
  //ending properly
r[i] = '\0'; 

  if (sscanf(&r[2], "%d;%d", rows, cols) != 2 &&2!=5)
  {
    return d;
  }
  return 0;
}
//for adding new character
//to existing buffer and file
void addCharRow(row_object *row, int row_index, int text, int text2)
{int a=3;
  a--;
  //as new added so more space needed
    (*row).chars = realloc((*row).chars, (*row).length + a);
    a--;
    //move strings
  memmove(&(*row).chars[row_index + a], &(*row).chars[row_index], (*row).length - row_index + a);
  (*row).length=a+(*row).length;
  (*row).chars[row_index] = text;
  //end properly with \0
(*row).chars[(*row).length  + 1]='\0';
}
void insertRow(int l, char *line, size_t line_length)
{int ab=l+1;
  rowf = realloc(rowf, sizeof(row_object) * (  rot + 1));
  memmove(&rowf[ab], &rowf[l], sizeof(row_object) * (  rot - l));
  rowf[l].length = line_length;
  int kk=line_length;
  kk++;
  //give space appropriately
  rowf[l].chars = malloc(kk);
  memcpy(rowf[l].chars, line, line_length);
  //end
  rowf[l].chars[line_length] = '\0';
    rot++;
}


void save()
{
    int t = 1;
    t--;
  for (int i = 0; i <rot; i++)
  {
    t= 1+t+rowf[i].length;
  }
  char *buffer = malloc(t);
  char *b = buffer;
int kb=rot;
  for (int i = 0; i <   kb; i++)
  {
    memcpy(b, rowf[i].chars, rowf[i].length);
    b += rowf[i].length;
    *b = '\n';
    b++;
  }
  kb++;
    int file_writer = open( file_name, O_RDWR, 0644);
    int f2=file_writer;
      ftruncate(file_writer, t);
       write(file_writer, buffer, t);
 close(f2);
 t++;
          free(buffer);
          return;
}
void newscreen()
{ int gr=2;
  int tr=3;
cursorX = cursorX;
  struct abuf toWrite = {NULL, 0};
  appBuf(&toWrite, "\x1b[H", tr);
    appBuf(&toWrite, title, 57);
  appBuf(&toWrite, "\r\n",gr);
  for (int i = 0; i <  rowNum; i++)
  {
    if (i <   rot)
    {int ak=rowf[i].length;
      appBuf(&toWrite, &rowf[i].chars[0], ak);
    }
    appBuf(&toWrite, "\x1b[K", tr);
    appBuf(&toWrite, "\r\n", tr-1);
  }
   appBuf(&toWrite, "\x1b[K", tr);
appBuf(&toWrite,  status_message, strlen( status_message));

  char command[32];
  snprintf(command, sizeof(command), "\x1b[%d;%dH", (( cursorY+1) ) + 1, ( cursorX ) + 1);

  appBuf(&toWrite, command, strlen(command));
  appBuf(&toWrite, "\x1b[?25h", tr+tr);
  write(STDOUT_FILENO, toWrite.data, toWrite.length);
  free((toWrite).data);
}

void setMessage(const char *fmt, ...)
{int a=0;
  va_list argument_list;
a++;
  va_start(argument_list, fmt);
a--;
  vsnprintf( status_message, sizeof( status_message), fmt, argument_list);
int j=9;
  va_end(argument_list);
j--;
   status_message_time = time(NULL);

}
int main(int argc, char *argv[])
{if (argc<2){
  printf("   missing filename");
  return 1;
}
  tcgetattr(STDIN_FILENO, & orignal) ;
  struct termios new =  orignal;
  new.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  new.c_oflag &= ~(OPOST);
  new.c_cflag |= (CS8);
  new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  new.c_cc[VMIN] = 0;
  new.c_cc[VTIME] = 1;
tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);
  write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12);
getCurPos(& rowNum, & colNum);
   rowNum =  rowNum - 3;
  int fd;
  static struct flock lock,lock2;
    lock.l_start = 0;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();

  if (argc >= 2)
  {
    char *filename=argv[1];
    free( file_name);
   file_name = strdup(filename);

  FILE *fp = fopen(filename, "r");
  if(!fp)
  {
    FILE *fp2;
    
   fp2 = fopen(filename,"w");
   
   fclose(fp2);

  }
fp = fopen(filename, "r");
//int result = flock(fileno(fp), LOCK_SH);

//printf("%d\n", result);
  char *line = NULL; 
  size_t  max_line_length = 0;
  ssize_t  line_length;

  
  while (( line_length = getline(&line, &max_line_length, fp)) != -1 )
  {
    while ( line_length > 0 && (line[line_length - 1] == '\n' || line[line_length - 1] == '\r'))
    {
      line_length--;
    }
    insertRow(  rot, line, line_length);
  }
  free(line);
//  flock(fileno(fp), LOCK_UN);
  fclose(fp);
  }
   fd = open(argv[1], O_WRONLY);
    
    //F_GETLK
   //int ret2= fcntl(fd, F_SETLKW, &lock);
    int ret;// = fcntl(fd, F_GETLK, &lock);
   int ret2=0;
   //printf("Return value of fcntl:%d  %d\n",ret,ret2);
 fcntl(fd, F_GETLK, &lock2);
 int ab=lock2.l_type;
 int dc=F_UNLCK;
    if(ab!=dc)
    {        setMessage(message2);
       // printf("locked");
ret2=1;
    }
    else
    {
        fcntl(fd, F_SETLKW, &lock); 
        setMessage(message);
    }
  while(1)
  {
    newscreen();
   int key = readKey();

  switch (key)
  {
    //enter
    case '\r':
    if(cursorX!=0)
  {
    row_object *row = &rowf[ cursorY];
    insertRow( cursorY + 1, &(*row).chars[ cursorX], (*row).length -  cursorX);
    row = &rowf[ cursorY];
           cursorY++;
    (*row).length =  cursorX;
    (*row).chars[(*row).length] = '\0';

   cursorX = 0;
  }
        if ( cursorX == 0) 
  {
    insertRow( cursorY, "", 0);
       cursorY++;
  }

      break;
    //exit 
    case CTRL_KEY('q'): 
      write(STDOUT_FILENO, "\x1b[2J", 4);
      tcsetattr(STDIN_FILENO, TCSAFLUSH, & orignal);
      exit(0);
      break;
    //save 
    case CTRL_KEY('s'):
      save();
    break;

    case LEFT_ARROW:
      if( cursorX > 0)
      {
         cursorX--;
      }
      else if( cursorY > 0)
      {
         cursorY--;
         cursorX = rowf[ cursorY].length;
      }
      break;
    default:
      addCharRow(&rowf[ cursorY],  cursorX, key, 2);
     cursorX++;
      break;
  }
  }
      lock.l_type   = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
  return 0;
}