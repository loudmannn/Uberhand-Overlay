---
layout: page
title: catch_errors
parent: Misc
---
Enables error handling in the current config before the ignore_errors command.
Catch_errors stops program execution, if a failure occurs at any step. \
Usage:
```
catch_errors
<some code in which errors are unacceptable>
ignore_errors
<some code in which errors should be ignored>
```

{: .exclusive }
Exclusively for Uberhand

{: .pro-tip }
One of the best use cases for catch_errors is to stop execution when a download or decompression fails.