---
layout: page
title: Back
parent: Misc
---
The back command allows you to exit the current submenu after selecting an option. Example:
```
[*Backup]
json_source '/switch/.packages/package/Backup/Backup/Backup.json' name
del {json_source(*,dir)}
mkdir {json_source(*,dir)}
cp {json_source(*,from)} {json_source(*,to)}
back
```
This code snippet backup files specified in `Backup.json` and exit submenu after selecting option.

{: .exclusive }
Exclusively for Uberhand
