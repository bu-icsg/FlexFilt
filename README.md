# FlexFilt
Flexible Instruction Filtering for Security.

Note: FlexFilt will be officially released soon. This document is provided for ACSAC artifact evaluation.

## Overview
In this branch, we provide simplified version of our tests, where the source code of each test provides extensive comments.

### Functional Verification
We assume you have followed the guideline in the main branch and have a connection to the FPGA.
As a first step, each time before booting up Linux, please make sure to re-configure the FPGA:

```
  $ cat rocketchip_filter.bit.bin > /dev/xdevcfg
```

A new bbl has been added on ACM1 FPGA (called new_bbl_func). Please bootup the Linux with this new bbl:

```
  $ ./fesvr-zynq new_bbl_func
```

After the bootup is done, go to the home folder:

```
  $ cd home 
```

To run the first program, please run the following command:

```
  $ ./new_filter1.rv
```

The source code for this program is available at https://github.com/bu-icsg/FlexFilt/blob/new_code/code/new_filter1.c.
We use this test program to show that FlexFilt can prevent the execution of a target instruction in the default domain, which we consider as an untrusted domain.
Our test program configures one of our FlexibleFilters to prevent the execution of the target instruction (defined through komodo_wr, where we use [rocc-software](https://github.com/ibm/rocc-software/tree/fddb795a0b52e82f8f4ce9ead9b1428440a62ab0) repo to define the WRPKR custom instruction). Based on the values we have assigned to this custom instruction, the target instruction has the value of 0x20c5b02b. Please note that we have other custom instructions used in our design and our hardware implementation interprets an insrtuction with this specific value (0x20c5b02b) as a WRPKR instruction. Also, to understand the matching/masking mechanism used for configuring our FlexibleFilters please refer to Section 5.1.2 of the paper and Figure 1 in the manuscript.

To test the second program, please run the following command:

```
  $ LD_PRELOAD=./myfilter.so ./new_filter2.rv
```

The source code for this program is available at https://github.com/bu-icsg/FlexFilt/blob/new_code/code/new_filter2.c and https://github.com/bu-icsg/FlexFilt/blob/new_code/code/lib_filter.c.
As explained in the main branch, the second program follows the scenario described in Section 7.2 and Section 5.3 of the paper.
In this scenario, we have two trusted functions, i.e., good_code1 and good_code2, that are allowed to execute WRPKR extended instruction.
The execution of the target instruction should be prevented in the rest of the program.
As described in the last paragraph of Section 5.3 of the paper, we assign the same instruction protection key value to both trusted functions and then configure FlexFilt to prevent the execution of the target instruction in the default domain.
We use LD_PRELOAD to obtain the address range of the two trusted functions, and then invoke pkey_mprotect to associate them with the same instruction protection key.
Please note that both good_code1 and good_code2 belong to the same insrtuction protection domain, and they are both trusted (or good) code.
You can find our LD_PRELOAD function at https://github.com/bu-icsg/FlexFilt/blob/new_code/code/myfilter.c.
Please note that if new_filter2.rv gets executed without ld-preloading good_code1 and good_code2, these two functions by default belong to the untrusted default domain and as a result FlexFilt will prevent the execution of the target WRPKR instruction in them.
The pkey_mprotect function assigns the same instruction protection domain (domain1) to good_code1 and good_code2 (https://github.com/bu-icsg/FlexFilt/blob/new_code/code/myfilter.c#L46 and https://github.com/bu-icsg/FlexFilt/blob/new_code/code/myfilter.c#L76) and FlexFilt is not configured to prevent the execution of any instruction in domain1.

To test the buffer overflow example, please run the following command:

```
  $ cat new_payload.txt | LD_PRELOAD=./myfilter.so ./new_vuln.rv
```

The source code for this program is at https://github.com/bu-icsg/FlexFilt/blob/new_code/code/new_vuln.c.
To demonstrate FlexFilt's capability in preventing a security attack (Section 7.2 of the paper), we provide a sample program with buffer overflow vulnerabilities.
We provide an input for this program leveraging the buffer overflow vulnerability to execute a function that is never called in the program.
This function executes a target instruction (WRPKR) that can undermine an isolation in place by modifying the permission bits of a protection domain in Protection Key Register File (PKR).
As mentioned in our manuscript, for additional information about WRPKR and PKR please refer to [12] in our paper.
For simplicity, we do not include the code for allocating a memory protection domain, which will get affected by the execution of the target instruction. 
We Configure FlexFilt to prevent the execution of WRPKR instruction in the default domain and allow its execution in a trusted function (good_code1).
The function that gets called due to the buffer overflow vulnerability belongs to the undtrusted default domain (domain0); hence, FlexFilt can prevent the execution of the target instruction in this function.
As expected, FlexFilt prevents the execution of the target instruction in the untrusted function and allows its execution in the trusted function (good_code1).
