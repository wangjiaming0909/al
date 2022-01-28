.section .data
  .lcomm buffer, 10000
buf2:
  .ascii "asdasd"
local_buffer:
  .fill 10000
.section .text
.global _start
_start:
  movl $buf2, %edx
  movl 0x9, %eax
  int $0x21
  movl $1, %eax
  movl $0, %ebx
  int $0x80
