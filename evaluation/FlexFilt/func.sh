#!/bin/bash
cat rocketchip_filter.bit.bin > /dev/xdevcfg
./fesvr-zynq bbl_func
