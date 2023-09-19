---
layout: page
title: Separator
parent: Features
---

You can separate items in your package, by adding `--` and name of your section.

Example:
```
[*RAM Vdd2]
catch_errors
json_mark_current '/switch/.packages/4IFIR Wizard/1_Configure/RAM/json/Vdd2.json' name 16
hex-by-cust-offset /atmosphere/kips/loader.kip 16 {json_source(*,hex)}
back

-- Advanced settings

[*EMC DVB Table]
catch_errors
json_mark_current '/switch/.packages/4IFIR Wizard/1_Configure/RAM/json/EMC DVB Table.json' name 48
hex-by-cust-offset /atmosphere/kips/loader.kip 48 {json_source(*,hex)}
back
```