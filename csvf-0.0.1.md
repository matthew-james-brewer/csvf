# CSVF specs 0.0.1

CSVF is a textual file format which holds simple CSV data and row/column thickness annotations.

## syntax

```
thickness = "1" | "2"; (* 1 means single thin line, 2 means double thin line *)
col defs = thickness, ",", thickness, { ",", thickness };
item = ? not newline or comma ?, { ? not newline or comma ? };
row = item, { ",", item };
csvf = col defs, "\n", thickness, row, { "\n", thickness, row }, "\n", thickness;
```

### notes

The number of `item`s in each `row` must be the number of `thickness`es in the `col defs` minus one.

## examples

### simple csvf file with no formatting added

```
1,1,1
1
name,cost
1
socks,$7.00
1
pants,$22.00
1
shoes,$39.00
1
```

### csvf file with header seperated

```
1,1,1,1
1
expression,result,note
2
5+7,12,it's simple math
1
41*62,2542,multiplication isn't hard
1
6480/60,108,just use long division
1
```

### example with complex formatting

```
1,2,1,2,1,1,1
1
url,scheme,host,path,query,fragment
2
https://www.google.com/search?q=hello,https,www.google.com,/search,q=hello, 
1
https://en.m.wikipedia.org/wiki/Extended_Backus–Naur_form,https,en.m.wikipedia.org,/wiki/Extended_Backus–Naur_form, , 
1
https://game8.co/games/Minecraft/archives/377719#hm_3,https,game8.co,/games/Minecraft/archives/377719, ,hm_3
1
```