#!/bin/bash
base64 -d | sha1sum -b | cut -f 1 -d' ' | head -c $1
printf "\n"
