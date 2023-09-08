---
layout: page
title: ignore_errors
parent: Misc
---
Disables error handling in the current config before the catch_errors command.\
Usage:
```
<some code in which errors should be ignored>
catch_errors
<some code in which errors are unacceptable>
ignore_errors
<some code in which errors should be ignored>
```

{: .exclusive }
Exclusively for Uberhand

{: .pro-tip }
By default, error handling is **disabled** for backwards compatibility, so you don't need to use ```ignore_errors``` in every script.