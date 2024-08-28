# タイピングゲーム

アルファベット大文字小文字、数字、記号
表示された流れる文字をタイピング

機能？
- エスケープシーケンスを使ったアニメーション
- 正解不正解の文字色変化、点滅など
- 各文字の正解率、所要時間統計
- それを元に出題傾向を調整
- 難易度別モード
- 画面幅いっぱい使って、どこからでてくるかわからないモード、動きも消え方もランダム
- 早めに消さないと分裂して増える、例えば記号は記号、数字は数字が増えるようにすれば記号を後回しにもできない
- 文字の滞留時間に応じて文字色変化
- ステージクリアごとに難易度あがる
- 入力文字を配列に入れれば文字列判定もできるか
- 色の濃さで浮き出てくるような文字列、消えるときはだんだん薄く
- ひらがなモード、漢字モード
- ゲームサーバーに複数人で接続して対戦



### 開始画面

ランダム発生した文字が流れる
```
 t   8 #  {  1  .. e     }
```


### ゲーム中

複数のターゲット
順番は前後しても構わないから流れる文字をうつ
```
 a   8   # % 
```
正解とタイプミスを集計

流れる方向
左から、右から、ランダムな位置から
複数行、複数文字の場合は重複しないようにする、判定がややこしくなるから
```
   8 2d 8     o  
q   w r     
        o
            {
                j I
```
これは大変。。。
記号とかむずいの捨てて、得点稼ぐか

### title

```
typotypotypo   typo    typo   typotypo         typotypo   
    typo        typo  typo    typo   typo    typo    typo 
    typo         typotypo     typo   typo   typo      typo
    typo           typo       typotypo      typo      typo
    typo           typo       typo           typo    typo
    typo           typo       typo             typotypo         

                          no

typotypotypo        typo        typotypotypo     typotyp    typo    typo      typotypo   typotypo     typot   typo 
    typo          typotypo          typo        typo   po   typo    typo        typo       typo       typoty  typo 
    typo         typo  typo         typo          typo      typo    typo        typo       typo       typotty typo 
    typo        typotypotypo        typo            typo    typo    typo        typo       typo       typo ttyptyp
    typo       typo      typo       typo       typ   typo    typo  typo   typ   typo       typo       typo  tttypo
    typo       typo      typo       typo        typotypo        typo       typotypo      typotypo     typo   ttypo   


```

### 終了画面

得点　平均所要時間
終了アニメーション
```
  GAME OVER
```


### 関数分割、ファイル分割

ターミナル設定
アニメーション
データ集計
タイマー
ゲームモード

