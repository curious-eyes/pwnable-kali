# pwnable-kali
pwnable.kr を楽しむためのKaliLinux環境

## Install Docker

Docker, Docker Compose をインストールしてください。

https://docs.docker.com/get-docker/

https://docs.docker.com/compose/install/


## Build and Run Your Docker Container

```
docker-compose build
docker-compose up -d
```

kaliにroot権限でログイン（exitしたらコンテナは削除されます）

```
docker-compose exec play bash
```

## Play Game!

https://pwnable.kr/play.php


## Clean Up Your Docker Images After Enjoying Pwnable

exit で抜けたあと、Docker imageが残ってしまうので削除します。

削除しても、 /usr/src/app に作成したディレクトリ・ファイルは残ります。

```
docker system prune
docker rmi $(docker images -qa)
docker system df
```
