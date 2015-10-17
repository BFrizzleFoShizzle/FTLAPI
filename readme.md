This is a mod/hack for FTL: Faster Than Light that aims to allow for complex interaction with the FTL process using ChaiScript. FTLAPI currently only works with the latest Windows Steam version (1.5.13) of FTL.
#Compiling/running
This project uses VC++2015. It contains inline assembler which will likely break if compiled using a different compiler.
Compilation steps:

1. Download [ChaiScript](http://chaiscript.com/) (tested with 5.7.1)
2. Download [SDL 1.2](https://www.libsdl.org/download-1.2.php) (NOT compatible with 2.0 as FTL uses 1.2)
3. Add the ChaiScript and SDL include directories to the project includes
4. Compile!

In order to run this, compile it into a DLL, place the fonts image in the same directory as the DLL, then inject the DLL into FTLGame.exe using your favourite DLL injector.
It currently loads and runs test.chai from FTL's directory.
