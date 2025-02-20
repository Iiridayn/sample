#!/bin/bash

DATA_PATH=""
AUTH_TOKEN=""
curl "https://api.pinboard.in/v1/posts/all?auth_token=$AUTH_TOKEN&format=json" > /tmp/pinboard.json
if [[ -f /tmp/pinboard.json && $(du /tmp/pinboard.json | cut -f 1) -gt $(du "$DATA_PATH"/pinboard.json | cut -f 1) ]]; then
	mv /tmp/pinboard.json "$DATA_PATH"/pinboard.json
	PID=$(systemctl show --property MainPID --value pinboard-search)
	kill -s HUP "$PID"
fi
