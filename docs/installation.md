# Installing the emulator

In order to successfully run the emulator, you'll need the following things:
* A decrypted PS4 5.05 module dump
* A fake self eboot.bin, where you'll need to remove the self-header and convert it to an ELF (hint: UnfSelf)

Next, place the module dump in the `bin/Debug(or Release)/modules` folder, and pass the path to your eboot via the command line.
If you encounter any errors feel free to ask.