/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby,
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
struct s {
    int a;
    int b : 1;
};
void f(struct s *encode){
    printf("%d", encode->b);
}

int main()
{
    struct s bs;
            printf("%d", bs.b);

    f(&bs);
        printf("%d", bs.b);

    printf("Hello World");

    return 0;
}
