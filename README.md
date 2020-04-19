# pwnable-kali
pwnable.kr を楽しむためのKaliLinux環境

## Install Docker

Docker をインストールしてください。

https://docs.docker.com/get-docker/


## Build and Run Your Docker Container

```
docker build -t kali/rolling .
```

kaliにroot権限でログイン（exitしたらコンテナは削除されます）

```
docker run --cap-add=SYS_PTRACE --security-opt="seccomp=unconfined" --name ctf-kali -v $(pwd):/usr/src/app -w /usr/src/app -it --rm kali/rolling /bin/bash
```

## Play Game!

https://pwnable.kr/play.php


## Clean Up Your Docker Images After Enjoying Pwnable

exit で抜けたあと、Docker imageが残ってしまうので、削除します。
削除しても、 /usr/src/app に作成したディレクトリ・ファイルは残ります。

```
docker system prune
docker rmi $(docker images -qa)
docker system df
```
