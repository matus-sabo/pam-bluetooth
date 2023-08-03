#!/usr/bin/env bash

CONTROLLER_MAC_ADDRESS="$1"
ALLOW_MAC_ADDRESS="$2"
LINK_KEY=$(sudo cat /var/lib/bluetooth/$CONTROLLER_MAC_ADDRESS/$ALLOW_MAC_ADDRESS/info | grep -A1 '\[LinkKey\]' | tail -n 1)

echo "Enter password for user $USER" 
read -sp "> " PASSWORD 
ENCRYPTED_PASSWORD=$(echo "$PASSWORD" | openssl enc -base64 -e -aes-256-cbc -salt -pass pass:$LINK_KEY -pbkdf2)
echo "-auth       optional      pam_bluetooth.so allow=$CONTROLLER_MAC_ADDRESS/$ALLOW_MAC_ADDRESS" user=$USER encrypted_authtok=$ENCRYPTED_PASSWORD
