---
layout: page
title: SoC-based Commands
parent: Features
---
Allows show some items only for specified soc type.

Example:
```
[*CPU Voltage Limit]
; Mariko
catch_errors
json_mark_current '/switch/.packages/4IFIR Wizard/1_Configure/CPU/json/voltage_mariko.json' name 28
hex-by-cust-offset /atmosphere/kips/loader.kip 28 {json_mark_current(*,hex)}
back

[*CPU Voltage Limit]
; Erista
catch_errors
json_mark_current '/switch/.packages/4IFIR Wizard/1_Configure/CPU/json/voltage_erista.json' name 20
hex-by-cust-offset /atmosphere/kips/loader.kip 20 {json_mark_current(*,hex)}
back
```

{: .exclusive }
Exclusively for Uberhand