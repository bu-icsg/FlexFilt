#!/bin/bash
cat rocketchip_origin.bit.bin > /dev/xdevcfg
./fesvr-zynq bbl_spec
