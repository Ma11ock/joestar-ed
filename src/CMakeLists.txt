set(BINARY ${CMAKE_PROJECT_NAME})

set(
    SRC_SOURCES    
    b.h 
    blocks.h 
    bw.h 
    cmd.h 
    config.h 
    hash.h 
    help.h 
    kbd.h 
	macro.h 
    main.h 
    menu.h 
    path.h 
    poshist.h 
    pw.h 
    queue.h 
    qw.h 
    rc.h 
    regex.h 
	scrn.h 
    tab.h 
    termcapj.h 
    tty.h 
    tw.h 
    types.h 
    ublock.h 
    uedit.h 
    uerror.h 
	ufile.h 
    uformat.h 
    uisrch.h 
    umath.h 
    undo.h 
    usearch.h 
    ushell.h 
    utag.h 
	utils.h 
    va.h 
    vfile.h 
    vs.h 
    w.h 
    utf8.h 
    syntax.h 
    charmap.h 
    mouse.h 
	lattr.h 
    gettext.h 
    builtin.h 
    vt.h 
    mmenu.h 
    state.h 
    options.h 
    selinux.h 
	unicode.h 
    cclass.h 
    frag.h 
    colors.h
    jlua.h
    joestar.h
    bridge.h

    # Sources
    b.c 
    blocks.c 
    bw.c 
    cmd.c 
    hash.c 
    help.c 
    kbd.c 
    macro.c 
    menu.c 
	path.c 
    poshist.c 
    pw.c 
    queue.c 
    qw.c 
    rc.c 
    regex.c 
    scrn.c 
    tab.c 
    termcap.c 
	tty.c 
    tw.c 
    ublock.c 
    uedit.c 
    uerror.c 
    ufile.c 
    uformat.c 
    uisrch.c 
    umath.c 
	undo.c 
    usearch.c 
    ushell.c 
    utag.c 
    va.c 
    vfile.c 
    vs.c 
    w.c 
    utils.c 
    syntax.c 
	utf8.c 
    selinux.c 
    charmap.c 
    mouse.c 
    lattr.c 
    gettext.c 
    builtin.c 
	builtins.c 
    vt.c 
    mmenu.c 
    state.c 
    options.c 
    unicode.c 
	cclass.c 
    frag.c 
    colors.c 
    unicat-9.0.0.c
    jlua.c
    joemain.c
    bridge.c
    joestar.cpp
    # Files and dirs for installation
    ${PROJECT_SOURCE_DIR}/syntax
    ${PROJECT_SOURCE_DIR}/colors
    ${PROJECT_SOURCE_DIR}/rc
)

list(APPEND CMAKE_REQUIRED_LIBRARIES ncurses)
list(APPEND CMAKE_REQUIRED_LIBRARIES m)
list(APPEND CMAKE_REQUIRED_LIBRARIES util)
list(APPEND CMAKE_REQUIRED_LIBRARIES lua5.3)

include( CheckSymbolExists )
include( CheckIncludeFile )

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Replacement for the autoconf.h file
# Getting size of types
include(CheckTypeSize)
CHECK_TYPE_SIZE(off_t SIZEOF_OFF_T)
CHECK_TYPE_SIZE(char SIZEOF_CHAR)
CHECK_TYPE_SIZE(int SIZEOF_INT)
CHECK_TYPE_SIZE(short SIZEOF_SHORT)
CHECK_TYPE_SIZE(long SIZEOF_LONG)
CHECK_TYPE_SIZE("long long" SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("void*" SIZEOF_VOID_P)

check_symbol_exists(TIOCGWINSZ "sys/ioctl.h" GWINSZ_IN_SYS_IOCTL)
# trig arc
check_symbol_exists(acos "math.h" HAVE_ACOS)
check_symbol_exists(asin "math.h" HAVE_ASIN)
check_symbol_exists(atan "math.h" HAVE_ATAN)
# trig arc hyperbolic
check_symbol_exists(atanh "math.h" HAVE_ATANH)
check_symbol_exists(asinh "math.h" HAVE_ASINH)
check_symbol_exists(acosh "math.h" HAVE_ACOSH)
# trig
check_symbol_exists(cos "math.h" HAVE_COS)
check_symbol_exists(sin "math.h" HAVE_SIN)
check_symbol_exists(tan "math.h" HAVE_TAN)
# trig hyperbolic
check_symbol_exists(tanh "math.h" HAVE_TANH)
check_symbol_exists(cosh "math.h" HAVE_COSH)
check_symbol_exists(sinh "math.h" HAVE_SINH)
# misc math
check_symbol_exists(ceil "math.h" HAVE_CEIL)
check_symbol_exists(cbrt "math.h" HAVE_CBRT)
check_symbol_exists(erfc "math.h" HAVE_ERFC)
check_symbol_exists(exp "math.h" HAVE_EXP)
check_symbol_exists(fabs "math.h" HAVE_FABS)
check_symbol_exists(floor "math.h" HAVE_FLOOR)
check_symbol_exists(hypot "math.h" HAVE_HYPOT)
check_symbol_exists(j0 "math.h" HAVE_J0)
check_symbol_exists(j1 "math.h" HAVE_J1)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(log10 "math.h" HAVE_LOG10)
check_symbol_exists(y0 "math.h" HAVE_Y0)
check_symbol_exists(y1 "math.h" HAVE_Y1)
# Unix headers
check_include_file("curses.h" HAVE_CURSES_H)
check_include_file("dirent.h" HAVE_DIRENT_H)
check_include_file("errno.h" HAVE_ERRNO_H)
check_include_file("fcntl.h" HAVE_FCNTL_H)
check_include_file("inttypes.h" HAVE_INTTYPES_H)
check_include_file("libutil.h" HAVE_LIBUTIL_H)
check_include_file("limits.h" HAVE_LIMITS_H)
check_include_file("locale.h" HAVE_LOCALE_H)
check_include_file("memory.h" HAVE_MEMORY_H)
check_include_file("ndir.h" HAVE_NDIR_TMP)
if(HAVE_NDIR_TMP)
    check_symbol_exists(DIR "ndir.h" HAVE_NDIR_H)
endif()

check_include_file("paths.h" HAVE_PATHS_H)
check_include_file("termios.h" HAVE_TERMIOS_TMP)
if(HAVE_TERMIOS_TMP)
    check_include_file("sys/termios.h" HAVE_POSIX_TERMIOS)
endif()

check_include_file("pty.h" HAVE_PTY_H)
check_include_file("pwd.h" HAVE_PWD_H)
check_include_file("sgtty.h" HAVE_SGTTY_H)
check_include_file("signal.h" HAVE_SIGNAL_H)
check_include_file("stddef.h" HAVE_STDDEF_H)
check_include_file("stdint.h" HAVE_STDINT_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)
check_include_file("strings.h" HAVE_STRINGS_H)
check_include_file("string.h" HAVE_STRING_H)
check_include_file("termio.h" HAVE_TERMIO_TMP)
if(HAVE_TERMIO_TMP)
    check_include_file("sys/termio.h" HAVE_SYSV_TERMIO)
endif()

check_include_file("sys/dir.h" HAVE_DIR_TMP)
if(HAVE_DIR_TMP)
    check_symbol_exists(DIR "sys/dir.h" HAVE_SYS_DIR_H)
endif()

check_include_file("sys/dirent.h" HAVE_SYS_DIRENT_H)
check_include_file("sys/ioctl.h" HAVE_SYS_IOCTL_H)

check_include_file("sys/ndir.h" HAVE_SYS_NDIR_TMP)
if(HAVE_NDIR_TMP)
    check_symbol_exists(DIR "sys/ndir.h" HAVE_SYS_NDIR_H)
endif()

check_include_file("sys/param.h" HAVE_SYS_PARAM_H)
check_include_file("sys/stat.h" HAVE_SYS_STAT_H)
check_include_file("sys/termio.h" HAVE_SYS_TERMIO_H)
check_include_file("sys/time.h" HAVE_SYS_TIME_H)
check_include_file("sys/types.h" HAVE_SYS_TYPES_H)
check_include_file("sys/utime.h" HAVE_SYS_UTIME_H)
check_include_file("sys/wait.h" HAVE_SYS_WAIT_H)
#check_include_file("termio.h" HAVE_TERMIO_H)
check_include_file("term.h" HAVE_TERM_H)
check_include_file("time.h" HAVE_TIME_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("utime.h" HAVE_UTIME_H)
check_include_file("utmp.h" HAVE_UTMP_H)
check_include_file("vfork.h" HAVE_VFORK_H)
check_include_file("termcap.h" TERMINFO)
check_include_file("lua5.3/lua.h" HAVE_LUA_H)
check_include_file("lua5.3/lauxlib.h" HAVE_LAUXLIB_H)
check_include_file("lua5.3/lualib.h" HAVE_LUALIB_H)

if(NOT HAVE_LUA_H OR NOT HAVE_LAUXLIB_H OR NOT HAVE_LUALIB_H)
    # TODO Error
endif()

# TODO selinux
# Unix functions 
check_symbol_exists(fork "unistd.h" HAVE_FORK)
check_symbol_exists(getcwd "unistd.h" HAVE_GETCWD)
check_symbol_exists(getwd "unistd.h" HAVE_GETWD)
check_symbol_exists(isblank "ctype.h" HAVE_ISBLANK)
check_symbol_exists(login_tty "utmp.h" HAVE_LOGIN_TTY)
check_symbol_exists(mkdir "sys/stat.h" HAVE_MKDIR)
check_symbol_exists(mkstemp "stdlib.h" HAVE_MKSTEMP)
check_symbol_exists(openpty "pty.h" HAVE_OPENPTY)
check_symbol_exists(putenv "stdlib.h" HAVE_PUTENV)
check_symbol_exists(setitimer "sys/time.h" HAVE_SETITIMER)
check_symbol_exists(setlocale "locale.h" HAVE_SETLOCALE)
check_symbol_exists(setpgid "unistd.h" HAVE_SETPGID)
check_symbol_exists(sigaction "signal.h" HAVE_SIGACTION)
check_symbol_exists(siginterrupt "signal.h" HAVE_SIGINTERRUPT)
check_symbol_exists(sigprocmask "signal.h" HAVE_SIGPROCMASK)
check_symbol_exists(sigvec "signal.h" HAVE_SIGVEC)
check_symbol_exists(snprintf "stdio.h" HAVE_SNPRINTF)
check_symbol_exists(strchr "string.h" HAVE_STRCHR)
check_symbol_exists(strdup "string.h" HAVE_STRDUP)
check_symbol_exists(utime "utime.h" HAVE_UTIME)
check_symbol_exists(vfork "unistd.h" HAVE_VFORK)

if(SIZEOF_LONG_LONG GREATER_EQUAL "8")
    set(HAVE_LONG_LONG 1)
endif()

add_compile_definitions(PACKAGE="joestar")
add_compile_definitions(PACKAGE_BUGREPORT="https://gitlab.com/Mallock/joestar")
add_compile_definitions(PACKAGE_NAME="joestar")
add_compile_definitions(PACKAGE_STRING="joestar 0.4")
add_compile_definitions(PACKAGE_TARNAME="joestar")
add_compile_definitions(PACKAGE_URL="https://gitlab.com/Mallock/joestar")
add_compile_definitions(PACKAGE_VERSION="0.4")
add_compile_definitions(VERSION="0.4")

add_compile_definitions(SIZEOF_CHAR=${SIZEOF_CHAR})
add_compile_definitions(SIZEOF_SHORT=${SIZEOF_SHORT})
add_compile_definitions(SIZEOF_INT=${SIZEOF_INT})
add_compile_definitions(SIZEOF_LONG=${SIZEOF_LONG})
add_compile_definitions(SIZEOF_LONG_LONG=${SIZEOF_LONG_LONG})
add_compile_definitions(SIZEOF_VOID_P=${SIZEOF_VOID_P})
add_compile_definitions(SIZEOF_OFF_T=${SIZEOF_OFF_T})
add_compile_definitions(__PROTOTYPES=1)
 
add_executable(
    ${BINARY}
    ${SRC_SOURCES}
    main.c
)

add_compile_definitions(JOES_TEST=1)
add_library(${BINARY}_lib STATIC ${SRC_SOURCES})

configure_file(config.h.in config.h)

# Linking
target_link_libraries(
    ${BINARY}
    ncurses
    m
    util
    lua5.3
)

target_link_libraries(
    ${BINARY}_lib
    ncurses
    m
    util
    lua5.3
)

install(TARGETS ${BINARY} 

        DESTINATION bin/
        
        PERMISSIONS
                    OWNER_READ
                    OWNER_WRITE
                    OWNER_EXECUTE
                    
                    GROUP_READ
                    GROUP_EXECUTE
                    
                    WORLD_READ
                    WORLD_EXECUTE
)

# For the install script
install(DIRECTORY   
                    ${PROJECT_SOURCE_DIR}/rc
                    ${PROJECT_SOURCE_DIR}/colors
                    ${PROJECT_SOURCE_DIR}/syntax
        DESTINATION etc/${BINARY}
        
        COMPONENT config
        
        DIRECTORY_PERMISSIONS 
                                OWNER_READ 
                                OWNER_WRITE 
                                OWNER_EXECUTE
                                
                                GROUP_READ
                                GROUP_EXECUTE
                                
                                WORLD_READ
                                WORLD_EXECUTE
        FILE_PERMISSIONS        
                                OWNER_READ
                                OWNER_WRITE
                                
                                GROUP_READ
                                
                                WORLD_READ
)
