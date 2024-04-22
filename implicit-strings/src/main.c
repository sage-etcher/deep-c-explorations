#include <malloc.h>
#include <string.h>


static char *str_dup (const char *);

static void heap_test (void);
static void stack_test (void);
static void literal_test (void);

int 
main (void)
{
    /* literal_test() is the fun one, other 2 are normal-ish */

    heap_test ();       /* heap string literal */
    stack_test ();      /* stack array literal */
    literal_test ();    /* char* with string literal (undefined-behaviour) */

    return 0;
}


/* heap ******************************************************************** */
static char *
str_dup (const char *src)
{
    /* calculate the size of src (assuming 1 char == 1 byte)
     * then allocate atleast that amount of data on the stack
     * store the ptr in dest */
    char *dest = malloc (strlen (src) + 1);
    /* dont forget to check that (dest != NULL) in real programs */

    /* we know that src and dest should be the same size so we are going to 
     * use strcpy to "clone" src's (potentially) READ-ONLY data, into the 
     * READ-WRITE memory (heap) of dest. */
    strcpy (dest, src);
    /* shut-up Ale I know that the `Call to function 'strcpy' is insecure...` 
     * shush! >:pp */

    /* and finally we can return the cloned (heap allocated) string ptr */
    return dest;
}


static void
heap_test (void)
{
    /* create a local mutable variable called str_mutable
     * use the str_dup function to create a clone of "string" in the heap 
     * store the cloned str's ptr in str_mutable */
    char *str_mutable = str_dup ("string");

    /* the heap is READ-WRITE memory, so we can safely dereference str_mutable
     * and change the data without harm */
    str_mutable[0] = 'W';

    /* however, because the string is heap allocated we are in charge of
     * freeing that block of memory. forgetting to do so will cause a memory
     * leak */
    free (str_mutable);
}


/* stack ******************************************************************* */
static void
stack_test (void)
{
    /* here the "string" literal is pushed onto the stack durring the function
     * call, the stack is READ-WRITE so this works file. however this method
     * forces the string to a constant length, unlike with the heap you cant
     * `realloc` the string to make it longer, the array is 7 characters long
     * and that's that. */
    char str_mutable[] = "string";

    /* the stack is READ-WRITE memory, so this has no issues, so long as the
     * index is sane */
    str_mutable[0] = 'W';

}


/* string literal ********************************************************** */
static void
literal_test (void)
{
    /* now for the undefined behaviour =DD ie the fun stuff */
    /* the following is what this does on MY system amd64 linux vm 
     * the REAL outcomes may vary from a multitude of factors. */

    /* this time around the string literal "string" is first stored as a
     * GLOBAL CONST, along with the rest of the READ-ONLY data.
     *
     * from there str_mutable is allocated on the stack, before getting 
     * assigned a pointer to the aforementioned GLOBAL CONST string literal. 
     *
     * this means that where str_mutable points can be changed, however the 
     * IMPLICITLY defined string's data can not be (thats the individual 
     * characters of the string). */
    char *str_mutable = "string";

    /* because of this when we try to update a character in str_mutable
     * the variable's value is dereferenced, and then (in theory) written to.
     * however, because "string" is located in a READ-ONLY section of memoroy, 
     * this does not always go as planned */

    /* on my machine the following line instantly causes a seg fault,
     * however if your machine or implementation doesn't have or use READ-ONLY
     * memory for this, you likely wont have a seg fault. It may work as
     * expected or it may update other variables in the proccess, in the end,
     * it's undefined behaviour, so you'll just gotta explore and find out */
    str_mutable[0] = 'W';

    /* I would also like to point out, that with my versions of GCC and Clang,
     * neither of them complain about this, even with `-Wpedantic -Wall`
     * flags set */
}


/* end of file */
