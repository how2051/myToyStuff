### Notes
This toy project is use to encrypt files
it use simple xor to do this
and I add a twist, swaping every byte's first 2 bits and last 2 bits
make it look less weak lol
but it just a toy project, don't use it to encrypt some very important files


### how to use
1. for linux
```shell
./build_linux.sh
./mycrypt -k yourPassword -e yourFiletobeEncrypted
./mycrypt -k yourPassword -d yourFiletobeDecrypted
```

2. for windows
```shell
rm -rf build
./build_windows.sh
./build/mycrypt.exe -k yourPassword -e yourFiletobeEncrypted
./build/mycrypt.exe -k yourPassword -d yourFiletobeDecrypted
```
