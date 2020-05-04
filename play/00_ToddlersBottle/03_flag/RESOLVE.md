# flag

`flag` というバイナリファイルがあるだけの問題。

ヒントは

This is reversing task. all you need is binary


## 動作確認

まずは、このファイルが何者なのかを確認するため `file` コマンドを実行。

```bash
$ file ./flag
flag: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, no section header
```

実行ファイルなので、実行してみましょう。

```bash
$ ./flag
I will malloc() and strcpy the flag there. take it.
```

malloc()は、メモリ領域を確保するための関数。

`その領域にフラグをコピー (strcpy) しているから、取ってね。` とのこと。


## 解析

デバッグしてみます。が、エラーが出てうまくいきません。

```bash
$ gdb ./flag
(gdb) disas main
No symbol table is loaded.  Use the "file" command.
```

すでに `file` コマンドで確認しましたが、内容をもう一度見てみると、確かに通常の実行ファイルとは少し違うようです。

たとえば、前の問題 `bof` の実行ファイルを `file` コマンドで確認した結果と比べると...

```bash
$ file ../02_bof/bof
bof: ELF 32-bit LSB shared object, Intel 80386, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.2, for GNU/Linux 2.6.24, BuildID[sha1]=ed643dfe8d026b7238d3033b0d0bcc499504f273, not stripped
$ file ./flag
flag: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, no section header
```

今回の実行ファイル `flag` は最後に `no section header` とあります。

何か加工されているようです。


バイナリエディタで中身を見たり、`strings`コマンドで文字列を探したりしてみます。

```bash
$ hexedit ./flag
```
( ctrl + c で終了できます)


```bash
$ strings ./flag | less
UPX!
@/x8
gX lw_
H/\_@
Kl$
H9\$(t
[]]y
nIV,Uh
AWAVAUATS
uSL9
...(以下、略)...
```

先頭に、`UPX!`と表示されています。気になるので、`grep`してみます。

```bash
$ strings ./flag | grep UPX
UPX!
$Info: This file is packed with the UPX executable packer http://upx.sf.net $
$Id: UPX 3.08 Copyright (C) 1996-2011 the UPX Team. All Rights Reserved. $
UPX!
UPX!
```

UPX という圧縮ツールを使っていました。

記載のURLからツールをダウンロードして、解凍するとデバッグ可能になります。

```bash
$ upx -d ./flag -o unpacked

$ file ./unpacked
./unpacked: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, for GNU/Linux 2.6.24, BuildID[sha1]=96ec4cc272aeb383bd9ed26c0d4ac0eb5db41b16, not stripped
```


## さらに解析

http://kent056-n.hatenablog.com/entry/2017/06/22/233747

```bash
$ gdb ./unpacked
(gdb) disas main
Dump of assembler code for function main:
   0x0000000000401164 <+0>:	push   %rbp
   0x0000000000401165 <+1>:	mov    %rsp,%rbp
   0x0000000000401168 <+4>:	sub    $0x10,%rsp
   0x000000000040116c <+8>:	mov    $0x496658,%edi
   0x0000000000401171 <+13>:	callq  0x402080 <puts>
   0x0000000000401176 <+18>:	mov    $0x64,%edi
   0x000000000040117b <+23>:	callq  0x4099d0 <malloc>
   0x0000000000401180 <+28>:	mov    %rax,-0x8(%rbp)
   0x0000000000401184 <+32>:	mov    0x2c0ee5(%rip),%rdx        # 0x6c2070 <flag>
   0x000000000040118b <+39>:	mov    -0x8(%rbp),%rax
   0x000000000040118f <+43>:	mov    %rdx,%rsi
   0x0000000000401192 <+46>:	mov    %rax,%rdi
   0x0000000000401195 <+49>:	callq  0x400320
   0x000000000040119a <+54>:	mov    $0x0,%eax
   0x000000000040119f <+59>:	leaveq
   0x00000000004011a0 <+60>:	retq
End of assembler dump.
```

`<+23>`で、`malloc`していて、`<+32>`の行に `# 0x6c2070 <flag>` と記述している箇所が怪しいです。

`rdx`にフラグを`strcpy`しているということのようです。

該当箇所にブレークポイント(`b`)を設置して、実行(`r`)してみます。

```bash
(gdb) b *main+32
Breakpoint 1 at 0x401184
(gdb) r
Starting program: /usr/src/app/00_ToddlersBottle/03_flag/unpacked
I will malloc() and strcpy the flag there. take it.

Breakpoint 1, 0x0000000000401184 in main ()
```

ブレークポイントの箇所では、まだ`rdx`にはコピーされていません。

ステップ実行(`nexti`)することで、該当行(`main<+32>`)が実行されます。

```bash
(gdb) nexti
0x000000000040118b in main ()
```

`rdx`のメモリ値を出力。

```bash
(gdb) x/24xh $rdx
0x496628:	0x5055	0x2e58	0x2e2e	0x203f	0x6f73	0x6e75	0x7364	0x6c20
0x496638:	0x6b69	0x2065	0x2061	0x6564	0x696c	0x6576	0x7972	0x7320
0x496648:	0x7265	0x6976	0x6563	0x3a20	0x0029	0x0000	0x0000	0x0000
```

ASCII文字列(Little Endian)なので頑張って読むこともできますが、下記の方法で文字列として出力できます。



## Capture the Flag

`x`コマンドで`rdx`の中身（フラグ）を文字列(`s`)として出力して終了。

```bash
(gdb) x/s $rdx
```

`x`コマンドは出力フォーマットが選べるので、いろいろ試すことができます。

https://flex.phys.tohoku.ac.jp/texi/gdb-j/gdb-j_41.html


## Incidentally

`UPX` が気になります。

通常の実行ファイルでは `gdb` コマンドでデバッグが可能ですが、圧縮すると `no section header` になってステップ実行などができなくなるようです。

つまり、実行できる状態のまま、処理を難読化できる。


ファイルサイズも軽量化されるし、実行時に余計なメモリ割り当てを要求しないので、処理への影響も少ない。

https://ja.wikipedia.org/wiki/UPX

良いことだらけのような気がします。
