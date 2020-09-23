Usage: `python3 pinboard-search.py <file>`

Get data from pinboard: `curl 'https://api.pinboard.in/v1/posts/all?auth_token=<token>&format=json' > <file>`

Send SIGHUP to reload - `kill -sighup $(cat $XDG_RUNTIME_DIR/pinboard-search.pid)`
