This is a mod/hack for FTL: Faster Than Light that aims to allow for complex interaction with the FTL process using ChaiScript. FTLAPI currently only works with the latest Windows Steam version (1.5.13) of FTL.
#Compiling/running
This project uses VC++2015. It contains inline assembler which will likely break if compiled using a different compiler.
In order to run this, compile it into a DLL, place the fonts image in the same directory as the DLL, then inject the DLL into FTLGame.exe using your favourite DLL injector.
It currently loads and runs test.chai from the DLL's directory.
