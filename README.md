# Intro

Simple, low effort PAM module to use a bluetooth device to authenticate.
No security implications have been considered.
Tested on Fedora 38 and OSX Ventura.

The device needs to be paired for this module to work.
The module will be ignored if the device is not found, and other authentication method can be used.

# Usage

## Fedora

```bash
sudo dnf install bluez openssl
```

```bash
gcc -fPIC -fno-stack-protector -c pam_bluetooth.c -o pam_bluetooth.o
ld -x --shared -o pam_bluetooth.so pam_bluetooth.o
sudo cp pam_bluetooth.so /lib64/security/pam_bluetooth.so
```

Generate pam config line

```bash
# controller_mac_address
pam-bluetooth]$ bluetoothctl list
# paired_device_mac_address
bluetoothctl devices

./pam_line.sh [controller_mac_address] [paired_device_mac_address]
./pam_line.sh 84:1B:77:15:0E:AE D4:3A:2C:A4:99:FF
```

Add line at the top of the relevant config files in /etc/pam.d to use the module to authenticate.

modules

- `/etc/pam.d/gdm-password`

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

- `/etc/pam.d/authorization` - ( TODO fix keyring )
- `/etc/pam.d/screensaver`
