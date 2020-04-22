# learn about Linux file IO

./readtest 配下の test.c をコンパイルして動きを確認してみましょう。

```
$ gcc test.c -o test
$ ./test
```

test.c の処理を良く読んで、変数 fd1 に `0` を代入して、もう一度コンパイル。

動きを確認してみましょう。

```
$ gcc test.c -o test
$ ./test
```

キーボードから `LETMEWIN` と入力して、 file2.txt の内容を見てみましょう。

あとは、問題の fd.c を良く読んで、Capture the Flag !!
