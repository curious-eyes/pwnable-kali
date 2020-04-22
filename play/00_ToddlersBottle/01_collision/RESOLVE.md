# check_password

## 動作確認

まずは実行してみましょう。

```
$ ./col
usage : col [passcode]
```

引数が必要ということですね。

引数を追加してもう一度実行してみましょう。

```
$ ./col aaaaaa
passcode length should be 20 bytes
```

引数は、20バイト必要だそうです。

python で、20文字の引数を作って渡してみましょう。

```
$ ./col `python3 -c 'print("a"*20)'`
wrong passcode.
```

20バイトのデータを渡すと、何らかのチェック処理が動いているようです。


## 処理内容

col.c の処理を確認してみましょう。

上記で試した動作は、`main`関数で制御しているようです。

`check_password` 関数のreturn値が 変数 `hashcode` の値と同じであれば、flagを取得できそうです。

`check_password` の１行目を見ると

```c
	int* ip = (int*)p;
```

引数のデータ型を変換しています (charポインタ -> intポインタ)

20バイトのchar配列を、4バイト(int)の配列とみなして処理しようということです。


```c
	for(i=0; i<5; i++){
		res += ip[i];
	}
```

4バイトずつ５回に分けて（計20バイト）足しています。

５つの値を足した結果が 変数 `hashcode` の値 `0x21DD09EC` になれば良いわけです。


## Capture the Flag

python の pwntools を使って計算してみましょう。

hashcode を 5 で割ると余りがでるので、５つ目の値に余りを足しておきます。

```python
python
>>> from pwn import *
>>> hashcode = 0x21DD09EC
>>> pack(hashcode//5)*4
b'\xc8\xce\xc5\x06'
>>> pack(hashcode // 5 + hashcode % 5)
b'\xcc\xce\xc5\x06'
```

ということで、

```bash
ssh col@pwnable.kr -p2222
$ ./col `python -c 'print "\xc8\xce\xc5\x06" * 4 + "\xcc\xce\xc5\x06"'`
```

python3 の場合

```bash
$ ./col `python3 -c 'import sys; sys.stdout.buffer.write(b"\xc8\xce\xc5\x06" * 4 + b"\xcc\xce\xc5\x06")'`
```
