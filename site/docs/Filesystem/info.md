---
layout: page
title: kip_info
parent: Filesystem
---
This feature allows you to display info of your kip on one screen. 

Example (json file with data):
```json
{
		"name": "RAM MHz",
		"offset": "32",
		"length": "4",
		"extent": "MHz"
}
```
Meanings: \
offset - offset from cust, \
length - length of hex value to read, \
extent - word after value.

Config.ini
```
[*Current Configuration]
; Mariko
kip_info '/switch/.packages/4IFIR Wizard/2_About/Data/curconf_mariko.json' name

[*Current Configuration]
; Erista
kip_info '/switch/.packages/4IFIR Wizard/2_About/Data/curconf_erista.json' name

```








{: .note }
See [4ifir wizard](https://github.com/efosamark/4IFIR-Wizard) for a working example

{: .exclusive }
Exclusively for Uberhand