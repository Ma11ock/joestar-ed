# Joestar

[Joe's Own Editor](https://github.com/jhallen/joe-editor)

## What

<p>Joestar is a fork of Joe's Own Editor that seeks to add extensibility,
theming, and other features.</p>


# Building

<p>joestar requres a C99 compliant compiler (perferably gcc or clang), 
`cmake`, and `make`. Its only dependency at the moment is libc.</p>

Then: 
```shell
git clone https://gitlab.com/Mallock/joestar
cd joestar
mkdir bin
cd bin
cmake ..
make joestar
```    

The binary will be (from the project root) bin/src/joestar

<p>Clang users will have to strip the binary manually. GCC users get that for
free!</p>
