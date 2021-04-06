#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int main(int argc, char* argv[])
{
  int ret_offset = -1;
  int fd;

/*
  if(argc != 4){
    printf(2, "Usage: ./testlseek <path> <offset> <whence>\n");
    exit();
  }

  strcpy(path, argv[1]);
  offset = atoi(argv[2]);
  whence = atoi(argv[3]);
*/

  fd = open("testfile", O_RDWR | O_CREATE);
  write(fd, "0123456789 Hello :)", strlen("0123456789 Hello :)"));  

  ret_offset = lseek(fd, 2, SEEK_SET);
  printf(1, "SET Off: 2\t Ret: %d\t Exp: 2\n", ret_offset);
   
  ret_offset = lseek(fd, 0, SEEK_CUR);
  printf(1, "CUR Off: 0\t Ret: %d\t Exp: 2\n", ret_offset);

  ret_offset = lseek(fd, -2, SEEK_CUR);
  printf(1, "CUR Off: -2\t Ret: %d\t Exp: 0\n", ret_offset);

  ret_offset = lseek(fd, -2, SEEK_END);
  printf(1, "END Off: -2\t Ret: %d\t Exp: 17\n", ret_offset);

  ret_offset = lseek(fd, -3, SEEK_END);
  printf(1, "END Off: -3\t Ret: %d\t Exp: 16\n", ret_offset);

  ret_offset = lseek(fd, 11, SEEK_SET);
  printf(1, "SET Off: 11\t Ret: %d\t Exp: 11\n", ret_offset);

  close(fd);
  exit();
}
