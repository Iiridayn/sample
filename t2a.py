#!/usr/bin/python
import sys; [print("".join([chr(int("".join(j), 3)) for j in [line.strip()[i:i+6] for i in range(0, len(line.strip()), 6)]])) for line in sys.stdin]
