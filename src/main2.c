/* Main is separated so there is not a linker error while making the test library.  */

int main(int argc, char **argv, const char * const *envv)
{
    joe_main(argc, argv, envv);
}
