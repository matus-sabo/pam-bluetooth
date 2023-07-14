Intro
=====

Simple, low effort PAM module to use a bluetooth device to authenticate. No security implications have been considered. Tested on Fedora 38. 

The device needs to be paired for this module to work. The module will be ignored if the device is not found, and other authentication method can be used. 

Improvements are welcome.

Usage
-----
Enter the MAC address of the device within the double quotes on line 16 of pam_bluetooth.c

Build the module using the following commands.

## Linux

```bash
gcc -fPIC -fno-stack-protector -c pam_bluetooth.c -o pam_bluetooth.o
ld -x --shared -o pam_bluetooth.so pam_bluetooth.o
sudo cp pam_bluetooth.so /lib64/security/pam_bluetooth.so
```

Add the following line at the top of the relevant config files in /etc/pam.d to use the module to authenticate.
# auth sufficient pam_bluetooth.so
/etc/pam.d/gdm-password

## OSX

```bash 
brew install blueutil coreutils
gcc -fPIC -fno-stack-protector -c pam_bluetooth.c -o pam_bluetooth.o
ld -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem -dylib -o pam_bluetooth.so pam_bluetooth.o
mkdir -p /usr/local/lib/pam
cp pam_bluetooth.so /usr/local/lib/pam/pam_bluetooth.so
```

Add the following line at the top of the relevant config files in /etc/pam.d to use the module to authenticate.
# auth sufficient pam_bluetooth.so
/etc/pam.d/authorization
/etc/pam.d/screensaver
