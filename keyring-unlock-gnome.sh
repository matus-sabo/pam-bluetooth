#!/bin/sh

PASSWORD=`cat ~/.keyring_unlock_password | base64 --decode`
echo "$PASSWORD" | gnome-keyring-daemon --replace --unlock