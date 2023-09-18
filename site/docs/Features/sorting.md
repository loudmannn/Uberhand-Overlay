---
layout: page
title: Sorting
parent: Features
---

Uberhand, as of **version 1.1.0** supports item sorting. 
- Sorting is divided into two types:
  - Sorting by prefix ("X_")
  - Sorting in json file

- To sort packages (.ovl or packages inside uberhand), use the first method and simply add 1_ to the beginning of the folder. When rendering, ```1_```, ```2_```, etc will be ignored.

- You can sort menu items created in a json file by swapping them directly inside the file.

Example (files structure):
```
- 1_package1
- 2_package2
- Data
- some.json
```
Output:
```
package1
package2
*buttons from some.json*
```


{: .note }
Mixing of two methods of menu creation is currently not possible due to peculiarities of rendering.

{: .exclusive }
Exclusively for Uberhand