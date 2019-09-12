/***************************************************************************
 * Dr. Evil's Insidious Bomb, Version 1.1
 * Copyright 2011, Dr. Evil Incorporated. All rights reserved.
 *
 * LICENSE:
 *
 * Dr. Evil Incorporated (the PERPETRATOR) hereby grants you (the
 * VICTIM) explicit permission to use this bomb (the BOMB).  This is a
 * time limited license, which expires on the death of the VICTIM.
 * The PERPETRATOR takes no responsibility for damage, frustration,
 * insanity, bug-eyes, carpal-tunnel syndrome, loss of sleep, or other
 * harm to the VICTIM.  Unless the PERPETRATOR wants to take credit,
 * that is.  The VICTIM may not distribute this bomb source code to
 * any enemies of the PERPETRATOR.  No VICTIM may debug,
 * reverse-engineer, run "strings" on, decompile, decrypt, or use any
 * other technique to gain knowledge of and defuse the BOMB.  BOMB
 * proof clothing may not be worn when handling this program.  The
 * PERPETRATOR will not apologize for the PERPETRATOR's poor sense of
 * humor.  This license is null and void where the BOMB is prohibited
 * by law.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* 
 * Note to self: Remember to erase this file so my victims will have no
 * idea what is going on, and so they will all blow up in a
 * spectaculary fiendish explosion. -- Dr. Evil 
 */

FILE *infile;
int array_3493[16] = { 2, 10, 6, 1, 12, 16, 9, 3, 4, 7, 14, 5, 11, 8, 15, 13 };
//nodes
//848 272 188 66 42 196
void phase_defused()
{
	printf("phase_defused\n");
}

int explode_bomb()
{
	printf("Bomb exploded\n");
	exit(8);
}

char st[64];
char* read_line()
{
	st[0]='\0';
    scanf( "%[^\n]", st );
    printf( "Input was:\n%s\n", st);
    return st;
}

// 返回0 -- input == dst; 返回1 input != dst
int strings_not_equal(const char *input, const char *dst)
{
	int result;
	int len1 = strlen(input);
	int len2 = strlen(dst);

	if (len1 != len2) {
		result = 1;
	} else if (input[0] == '\0') {
		result = 0;
	} else { 
		result = 1;
		while (*input++ == *dst++) {
			if (*input == '\0') { 
				result = 0;
				break;	
			}
		}
	}

	return (result);	
}

int phase_1(const char *input)
{
	const char *dst = "For NASA, space is still a high priority.";
	int result = strings_not_equal(input, dst);
	if (result != 0) {
		explode_bomb();	
	}

	return (result);
}

static void read_six_numbers(const char *input, int *a)
{
	//					%rdi	     %rsi			%rdx    %rcx   %r8   %r9    (%rsp)  *(%rsp)
	int result = sscanf(input, "%d %d %d %d %d %d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5]);
	if (result <= 5) {
		explode_bomb();	
	}
}

int __fastcall phase_2(const char* a1)
{
  int result; // rax
  signed int v2; // ebx	//循环变量，控制做六次 
  int *v3; // rbp	//数组指针 
  int v4; // [rsp+0h] [rbp-38h]	//读入六个数使用的辅助变量，统计读到数的个数 
  char v5; // [rsp+4h] [rbp-34h]	//数组[1]（初始下标[0]） 

  read_six_numbers(a1, &v4);
  if ( v4 < 0 )
    explode_bomb(a1, &v4);
  v3 = &v5;
  v2 = 1;
  do
  {
    result = (unsigned int)(*(v3 - 4) + v2);
    if ( *v3 != (int)result )
      explode_bomb(a1, &v4);
    ++v2;
    v3 += 4;
  }
  while ( v2 != 6 );
  return result;
}

int phase_3(const char *input)
//5 -412
{
  signed int v5; // eax
  int v6; // eax
  int v7; // eax
  int v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // eax
  int v13; // [rsp+8h] [rbp-10h]
  int v14; // [rsp+Ch] [rbp-Ch]
	//  0x8(%rsp)  0xc(%rsp)
	int num1, num2;
	//     %rdi     %rsi   %rdx   %rcx 
	int result = sscanf(input, "%d %d", &num1, &num2);
	if (result <= 1) {
		explode_bomb();	
	}

  switch ( num1 )
  {
    case 0:
      v5 = 893;
      goto LABEL_6;
    case 1:
      v5 = 0;
LABEL_6:
      v6 = v5 - 253;
      goto LABEL_8;
    case 2:
      v6 = 0;
LABEL_8:
      v7 = v6 + 524;
      goto LABEL_10;
    case 3:
      v7 = 0;
LABEL_10:
      v8 = v7 - 412;
      goto LABEL_12;
    case 4:
      v8 = 0;
LABEL_12:
      v9 = v8 + 412;
      goto LABEL_14;
    case 5:
      v9 = 0;
LABEL_14:
      v10 = v9 - 412;//v10 == -412
      goto LABEL_16;
    case 6:
      v10 = 0;
LABEL_16:
      v11 = v10 + 412;//v11 == 0
      break;
    case 7:
      v11 = 0;
      break;
    default:
      explode_bomb();
      return result;
  }
  result = (unsigned int)(v11 - 412);//-412
  if ( num1 > 5 || result != num2 )
    explode_bomb();
  return result;
}

int __fastcall func4(int a1, unsigned int a2)
{
  __int64 result; // rax
  int v3; // er12

  if ( a1 <= 0 )
    return 0LL;
  result = a2;
  if ( a1 != 1 )
  {
    v3 =func4(a1 - 1, a2) + a2;
    result = v3 + func4(a1 - 2, a2);
  }
  return result;
}

int __fastcall phase_4(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5)
{//80 4
//第二个输入<=4
//func4(6,第二个输入) == 第一个输入 

  __int64 v5; // rsi
  __int64 result; // rax
  unsigned int v7; // [rsp+8h] [rbp-10h]
  int v8; // [rsp+Ch] [rbp-Ch]

  if ( (unsigned int)__isoc99_sscanf(a1, (__int64)"%d %d", (__int64)&v8, (__int64)&v7, a5) != 2 || v7 - 2 > 2 )
    explode_bomb(a1, (__int64)"%d %d");
  v5 = v7;
  result = func4(6, v7);
  if ( (_DWORD)result != v8 )
    explode_bomb(6LL, v5);
  return result;
}

int __fastcall phase_5(_BYTE *a1, __int64 a2)
{//GKLMNO
  __int64 result; // rax
  int v3; // edx

  if ( (unsigned int)string_length(a1) != 6 )
    explode_bomb((__int64)a1, a2);
  result = 0LL;
  v3 = 0;
  do
    v3 += array_3493[a1[result++] & 0xF];
  while ( result != 6 );
  if ( v3 != 55 )
    explode_bomb((__int64)a1, a2);
  return result;
}

int __fastcall phase_6(__int64 a1)
{
  int *v1; // r13
  signed int v2; // er12
  signed int v3; // ebx
  char *v4; // rax
  unsigned __int64 v5; // rsi
  _QWORD *v6; // rdx
  signed int v7; // eax
  int v8; // ecx
  __int64 v9; // rbx
  char *v10; // rax
  __int64 i; // rcx
  __int64 v12; // rdx
  signed int v13; // ebp
  __int64 result; // rax
  int v15[6]; // [rsp+0h] [rbp-78h]
  char v16; // [rsp+18h] [rbp-60h]
  __int64 v17; // [rsp+20h] [rbp-58h]
  char v18; // [rsp+28h] [rbp-50h]
  char v19; // [rsp+50h] [rbp-28h]

  v1 = v15;
  read_six_numbers(a1, (__int64)v15);
  v2 = 0;
  while ( 1 )
  {
    if ( (unsigned int)(*v1 - 1) > 5 )
      explode_bomb(a1, (__int64)v15);
    if ( ++v2 == 6 )
      break;
    v3 = v2;
    do
    {
      if ( *v1 == v15[v3] )
        explode_bomb(a1, (__int64)v15);
      ++v3;
    }
    while ( v3 <= 5 );
    ++v1;
  }
  v4 = (char *)v15;
  do
  {
    *(_DWORD *)v4 = 7 - *(_DWORD *)v4;
    v4 += 4;
  }
  while ( v4 != &v16 );
  v5 = 0LL;
  do
  {
    v8 = v15[v5 / 4];
    if ( v8 <= 1 )
    {
      v6 = &node1;
    }
    else
    {
      v7 = 1;
      v6 = &node1;
      do
      {
        v6 = (_QWORD *)v6[1];
        ++v7;
      }
      while ( v7 != v8 );
    }
    *(__int64 *)((char *)&v17 + 2 * v5) = (__int64)v6;
    v5 += 4LL;
  }
  while ( v5 != 24 );
  v9 = v17;
  v10 = &v18;
  for ( i = v17; ; i = v12 )
  {
    v12 = *(_QWORD *)v10;
    *(_QWORD *)(i + 8) = *(_QWORD *)v10;
    v10 += 8;
    if ( v10 == &v19 )
      break;
  }
  *(_QWORD *)(v12 + 8) = 0LL;
  v13 = 5;
  do
  {
    result = **(unsigned int **)(v9 + 8);
    if ( *(_DWORD *)v9 < (signed int)result )
      explode_bomb(a1, (__int64)&v19);
    v9 = *(_QWORD *)(v9 + 8);
    --v13;
  }
  while ( v13 );
  return result;
}

int main(int argc, char *argv[])
{
    char *input;

    /* Note to self: remember to port this bomb to Windows and put a 
     * fantastic GUI on it. */

    /* When run with no arguments, the bomb reads its input lines 
     * from standard input. */
//    if (argc == 1) {  
//	infile = stdin;
//    } 
//
//    /* When run with one argument <file>, the bomb reads from <file> 
//     * until EOF, and then switches to standard input. Thus, as you 
//     * defuse each phase, you can add its defusing string to <file> and
//     * avoid having to retype it. */
//    else if (argc == 2) {
//	if (!(infile = fopen(argv[1], "r"))) {
//	    printf("%s: Error: Couldn't open %s\n", argv[0], argv[1]);
//	    exit(8);
//	}
//    }
//
//    /* You can't call the bomb with more than 1 command line argument. */
//    else {
//	printf("Usage: %s [<input_file>]\n", argv[0]);
//	exit(8);
//    }

    /* Do all sorts of secret stuff that makes the bomb harder to defuse. */
//    initialize_bomb();

    printf("Welcome to my fiendish little bomb. You have 6 phases with\n");
    printf("which to blow yourself up. Have a nice day!\n");

    /* Hmm...  Six phases must be more secure than one phase! */
    input = read_line();             /* Get input                   */
    phase_1(input);                  /* Run the phase               */
    phase_defused();                 /* Drat!  They figured it out!
				      * Let me know how they did it. */
    printf("Phase 1 defused. How about the next one?\n");

    /* The second phase is harder.  No one will ever figure out
     * how to defuse this... */
    input = read_line();
    phase_2(input);
    phase_defused();
    printf("That's number 2.  Keep going!\n");

    /* I guess this is too easy so far.  Some more complex code will
     * confuse people. */
    input = read_line();
    phase_3(input);
    phase_defused();
    printf("Halfway there!\n");

//    /* Oh yeah?  Well, how good is your math?  Try on this saucy problem! */
//    input = read_line();
//    phase_4(input);
//    phase_defused();
//    printf("So you got that one.  Try this one.\n");
//    
//    /* Round and 'round in memory we go, where we stop, the bomb blows! */
//    input = read_line();
//    phase_5(input);
//    phase_defused();
//    printf("Good work!  On to the next...\n");
//
//    /* This phase will never be used, since no one will get past the
//     * earlier ones.  But just in case, make this one extra hard. */
//    input = read_line();
//    phase_6(input);
//    phase_defused();

    /* Wow, they got it!  But isn't something... missing?  Perhaps
     * something they overlooked?  Mua ha ha ha ha! */
    
    return 0;
}
