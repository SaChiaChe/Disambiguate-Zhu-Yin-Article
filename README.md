# Disambiguate Zhu Yin Article
Disambiguate Zhu Yin Article(注音文) with SRILM and self implemented disambig.

## How to run

First, we have to segment our test data.
```
./separator_big5.pl corpus.txt > corpus_seg.txt
./separator_big5.pl testdata/xx.txt > testdata/seg_xx.txt
```

Then get counts and generate bigram model using SRILM
```
./ngram-count -text corpus_seg.txt -write lm.cnt -order 2
./ngram-count -read lm.cnt -lm bigram.lm -unk -order 2
```

Generate ZhuYin to Big5 map
```
make map
```

Try disambiguate implemented by SRILM
```
./disambig -text testdata/xx.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > $output
```

Now, lets try our own disambig!

Compile
```
make MACHINE_TYPE={$your machine type$} SRIPATH={$your SRILM path$}
```

Generate ZhuYin to Big5 map
```
make map
```

Run our disambig
```
make MACHINE_TYPE=i686-m64 SRIPATH=/home/ta/srilm-1.5.10 LM=bigram.lm run
```

## Description

### mapping.py

Simply converting the Big5-ZhuYin.map to ZhuYin-Big5.map.

### mydisambig.cpp

Uses bigram and Viterbi to find the most possible sentence constructed from the Zhu Yin article(注音文)

## Built With

* Python 3.6.0 :: Anaconda custom (64-bit)
* C++

## Authors

* **SaKaTetsu** - *Initial work* - [SaKaTetsu](https://github.com/SaKaTetsu)