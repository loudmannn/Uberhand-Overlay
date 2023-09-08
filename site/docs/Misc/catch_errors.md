---
layout: page
title: catch_errors and ignore_errors
parent: Misc
---
Toggles error handling in the current config before the opposite command.
Catch_errors stops program execution, if a failure occurs at any step. \
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
One of the best use cases for ```catch_errors``` is to stop execution when a download or decompression fails.

{: .pro-tip }
By default, error handling is **disabled** for backwards compatibility, so you don't need to use ```ignore_errors``` in every script.