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
docker-compose exec play bash
```

## Play Game!

https://pwnable.kr/play.php


## Clean Up Your Docker Images After Enjoying Pwnable

exit で抜けたあと、Docker imageが残ってしまうので削除します。

Docker imageを削除しても /usr/src/app に作成した作業内容は残りますので、ノウハウをためていけます。

```
docker stop $(docker ps -q)
docker system prune
docker rmi $(docker images -qa)
docker system df

TYPE                TOTAL               ACTIVE              SIZE                RECLAIMABLE
Images              0                   0                   0B                  0B
Containers          0                   0                   0B                  0B
Local Volumes       0                   0                   0B                  0B
Build Cache         0                   0                   0B                  0B
```
