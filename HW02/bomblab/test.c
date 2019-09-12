#include <stdio.h>
#include "defs.h"

int explode_bomb()
{
	printf("bomb exploded");
}

signed __int64 __fastcall fun7(__int64 a1, int a2)
{
  signed __int64 result; // rax

  if ( !a1 )
    return 0xFFFFFFFFLL;
  if ( *(_DWORD *)a1 > a2 )
    return 2 * (unsigned int)fun7(*(_QWORD *)(a1 + 8), a2);
  result = 0LL;
  if ( *(_DWORD *)a1 != a2 )
    result = 2 * (unsigned int)fun7(*(_QWORD *)(a1 + 16), a2) + 1;
  return result;
}

int main()
{
  const char *v0; // rdi
  unsigned int v1; // ebx
  __int64 v2; // rdx

  v0 = "1";
  v1 = strtol(v0, 0LL, 10);
  if ( v1 - 1 > 0x3E8 )
    explode_bomb();
  if ( (unsigned int)fun7((__int64)&n1, v1) != 5 )
    explode_bomb();
}
