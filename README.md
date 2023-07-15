Intro
=====

Simple, low effort PAM module to use a bluetooth device to authenticate. 
No security implications have been considered. 
Tested on Fedora 38 and OSX Ventura.

The device needs to be paired for this module to work. 
The module will be ignored if the device is not found, and other authentication method can be used. 

Usage
-----
Set the MAC addresses of allowed paired devices 
Set your password

## Linux

```bash
gcc -fPIC -fno-stack-protector -c pam_bluetooth.c -o pam_bluetooth.o
ld -x --shared -o pam_bluetooth.so pam_bluetooth.o
sudo cp pam_bluetooth.so /lib64/security/pam_bluetooth.so
```

Add the following line at the top of the relevant config files in /etc/pam.d to use the module to authenticate.
`-auth       optional      pam_bluetooth.so`

modules
`/etc/pam.d/gdm-password`

## OSX

```bash 
brew install blueutil coreutils
gcc -fPIC -fno-stack-protector -c pam_bluetooth.c -o pam_bluetooth.o
ld -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem -lpam -dylib -o pam_bluetooth.so pam_bluetooth.o
mkdir -p /usr/local/lib/pam
cp pam_bluetooth.so /usr/local/lib/pam/pam_bluetooth.so
```

Add the following line at the top of the relevant config files in /etc/pam.d to use the module to authenticate.
`auth       optional       pam_bluetooth.so`

modules
`/etc/pam.d/authorization`
`/etc/pam.d/screensaver`
