#!/usr/bin/env bash

read -p "Please select the length of your password: " length
len=$((length))

characters=()
for ch in {a..z}
do
    characters+=("$ch")
done
for ch in {A..Z}
do
    characters+=("$ch")
done
for ch in {0..9}
do
    characters+=("$ch")
done
symbols=("!" "@" "#", "$" "%" "^" "&")
for sym in ${symbols[*]}
do
    characters+=("$sym")
done

pass=""
chars=( $(shuf -e "${characters[@]}") )

for (( i=0; i<len; i++ )); do
	pass+=${chars[i]}
done

echo "Your password is:" $pass
read -p "Please select your desired hashing method (md5/sha256 only): " hash
case $hash in
        md5 | MD5)
                echo "This is your hashed password in MD5: " -n $pass | md5sum
                ;;
        sha256 | SHA256)
                echo "This is your hashed password in SHA256: " -n $pass | sha256sum
                ;;
        esac
