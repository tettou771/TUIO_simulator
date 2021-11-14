# TUIO_simulator

This is openFrameworks project.
TUIO cursor auto generator.

# Usage

Send address and screen size settings is bin/data/config.xml
Screen size is virtual TUIO generator's size. 
If you wanna 16:9 aspect, set as width:270 height:480.
Like this.
```xml
<config>
	<tuio address="127.0.0.1" port="3333" />
	<size_pix width="270" height="480" />
</config>
```


# Tested system

Windows10 + openFrameworks 0.11.2

# Addons

- ofxTuioWrapper https://github.com/arturoc/ofxTuioWrapper
- ofxUI https://github.com/tettou771/ofxUI

# Lisence

MIT

# Author

Toru Takata
https://github.com/tettou771
