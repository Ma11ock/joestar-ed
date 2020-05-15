/* Main is separated from joemain.c so there is not a linker error while making the test library.  */

int main(int argc, char **argv, const char * const *envv)
{
    return joe_main(argc, argv, envv);
}
