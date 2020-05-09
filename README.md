# Joestar

## What

<p>Joestar is a fork of [Joe's Own Editor](https://github.com/jhallen/joe-editor) that seeks to add extensibility,
theming, and other features.</p>


# Building

<p>joestar requres a C99 compliant compiler (perferably gcc or clang), 
and `make`. Its only dependency at the moment is libc. Make sure you have
the basic development packages on your distro (or MinGW) on Windows.</p>

Then: 
```shell
git clone https://gitlab.com/Mallock/joestar
cd joestar
./autojoe
./configure
make
strip src/joestar
```    

The binary will be src/joestar.