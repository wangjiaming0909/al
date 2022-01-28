.global t
t:
  .ascii "asdasd"

entry _start
_start:
  mov dx, t
  mov ah, 0x9
  int 0x21

