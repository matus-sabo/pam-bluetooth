Intro
=====

Simple, low effort PAM module to use a bluetooth device to authenticate. No security implications have been considered. Tested on Fedora 38. 

The device needs to be paired for this module to work. The module will be ignored if the device is not found, and other authentication method can be used. 

Improvements are welcome.

Usage
-----
Enter the MAC address of the device within the double quotes on line 16 of pam_bluetooth.c

Build the module using the following commands.

linux

```bash
gcc -fPIC -fno-stack-protector -c pam_bluetooth.c -o pam_bluetooth.o
sudo ld -x --shared -o /lib64/security/pam_bluetooth.so pam_bluetooth.o
```

osx

```bash 
gcc -fPIC -fno-stack-protector -c pam_bluetooth.c -o pam_bluetooth.o
ld -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem -dylib -o pam_bluetooth.so pam_bluetooth.o
```

Add the following line at the top of the relevant config files in /etc/pam.d to use the module to authenticate.

# auth sufficient pam_bluetooth.so user = your_username

For eg. /etc/pam.d/gdm-password for gdm lockscreen, /etc/pam.d/sudo for sudo.

~/Sandbox/Personal/pam-bluetooth/keyring-unlock-gnome.sh


https://fostips.com/no-password-login-ubuntu-fedora-linux-mint/

