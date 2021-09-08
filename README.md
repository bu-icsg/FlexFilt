# FlexFilt
Flexible Instruction Filtering for Security.

Note: FlexFilt will be officially released soon. This document is provided for ACSAC artifact evaluation.

## Overview
In this document, we provide a guideline for running experiments on FlexFilt. For more details on FlexFilt please refer to our paper.
We run our experiments on the Xilinx Zynq Zedboard FPGA and use a modified Linux kernel 4.15 to provide the support for our hardware.
You can find the source code for our test programs in the code folder.
The evaluation folder includes the binaries for running the tests on the baseline and FlexFilt configurations.

## Accessing the experiment environment
To simplify the process of testing FlexFilt, we provide you with access to our FPGA boards.
We have already created a guest account on one of our machines and provided you the private ssh key (please refer to the “Additional Comments” in the artifact evaluation submission website).
You can ssh to our machine after configuring the private ssh key:

```  
  $ ssh flexfilt@icsg-04.bu.edu
```

You have access to two of our FPGAs available at /dev/ttyACM0 and /dev/ttyACM1.
For simplicity, in the rest of this document we call the FPGA connected to /dev/ttyACM1 as ACM1 and the one connected to /dev/ttyACM1 as ACM1.
We suggest you to use screen for connecting to these two FPGAs (we have provided you screen access to these two nodes).
However, if you want to do ssh or scp to ACM0 and ACM1, their ip addresses are 192.168.1.33 and 192.168.1.31, respectively.
The username and password of the FPGAs are both root.
We have already configured ACM0 and ACM1 with necessary files to run the baseline and FlexFilt experiments, respectively.
To access the experiment environment of the baseline, connect to ACM0 using screen:

```
  $ screen -S ACM0 /dev/ttyACM0 115200
```

Please make sure to use 115200 as the baud rate in the screen command.
After connecting to ACM0 FPGA, you have access to the Processing System (PS) side of our zedboard.
If you run ls here, you will see all the required files to configure the Programmable Logic (PL) side of the zedboard with
a baseline Rocketchip processor and to boot up the Linux kernel.
Similarly, you can connect to ACM1, the experiment environment for FlexFilt:

```
  $ screen -S ACM1 /dev/ttyACM1 115200
```

Here, you have all the required files to configure the PL side of the zedboard with a Rocket
processor integrated with PHMon and to run the Linux kernel.
For more information about useful screen commands, please refer to the screen man page
(https://linux.die.net/man/1/screen).
We suggest you to detatch (ctrl a + d) from the screen session when you do not run the experiments.
To reattach to the screen session use the following
command:

```
  $ screen -rd -S ACM0
  $ screen -rd -S ACM1
```

As mentioned before, we provide the necessary files and scripts to configure the FPGA, boot up the
Linux kernel, and run the experiments.
However, if you accidentally remove these files, you can get them from this repository and scp them to the FPGA.
The FlexFilte files and baseline files are in the FlexFilt and baseline folders of this repository, respectively.
Additionally, if the FPGA becomes unresponsive, you can reboot it using the reboot command.
Please note that after a reboot, you need to remove all the default files on the PS side of the FPGA and replace them with the files provided in this repository.

For each use case, we provide a script to boot up Linux.
Because of our FPGA limitations, it takes about 2 minutes for the Linux to boot up (please be patient).
After you are done with one set of experiments, the easiest way to exit the Linux environment is just by terminating the process using ctrl+c.

## Running the experiments

### Functional Verification
For functional verification, we provide two sample programs.
The first program prevents the execution of WRPKR instruction (the equivalent of WRPKR in our RISC-V prototype) anywhere in the default domain.
The second program follows the scenario described in Section 7.2 of the paper.
In this scenario, we have two trusted functions, i.e., good_code1 and good_code2, that are allowed to execute WRPKR extended instruction.
The execution of WRPKR instruction should be prevented in the rest of the program.
As described in Section 5.3 of the paper, we assign the same ipkey value to both trusted functions and then configure FlexFilt to prevent the execution of WRPKR in the default domain.
We use LD_PRELOAD to obtain the address range of the two trusted functions, and then invoke pkey_mprotect to associate them with the same instruction protection key.

To demonstrate FlexFilt's capability in preventing a security attack (Section 7.2 of the paper), we provide a sample program with buffer overflow vulnerabilities.
We provide an input for this program leveraging the buffer overflow vulnerability to execute a function that is never called in the program.
This function executes a WRPKR instruction to modify the permission bits of a protection domain.
We Configure FlexFilt to prevent the execution of WRPKR instruction in the default domain and allow its execution in a trusted function.

We provide the binaries for these programs as part of our bbl. 
To run these test programs, you first need to bootup the kernel using bbl_func. To do this, please run func.sh script after connecting to ACM1:

```
  $ ./func.sh
```

After the kernel boots up, you can run each of the above-mentioned test programs. First, you need to go to the home folder.

```
  $ cd home 
```

Run the first test program:
```
  $ ./vuln1.rv 
```

As expected, FlexFilt prevents the execution of the unsafe instruction in the default domain.

Run the second test program:
```
  $ LD_PRELOAD=./myfilter.so ./filter2.rv
```

As expected, FlexFilt prevents the execution of the target instruction in the untrusted function and allows its execution in the two trusted functions.

Run the buffer overflow example:

```
  $ cat payload1.txt | LD_PRELOAD=./myfilter.so ./vuln1.rv
```
As expected, FlexFilt allows the execution of the target instruction in the trusted function and prevents its execution in the function executed by leveraging the buffer overflow vulnerability.

### Performance Evaluation
To demonstrate the negligible performance overhead of FlexFilt, we provide the executables for running a spec2000 benchmark application (i.e., bzip2) on the baseline Rocket core and the core enhanced with FlexFilt.
To run the baseline experiments, run the spec script on ACM0:

```
  $ ./spec_baseline.sh
```

To run the FlexFilt experiments, run the spec script on ACM1:

```
  $ ./spec_flexfilt.sh
```

After the kernel boots up, you can run and time the execution of the benchmark:

```
  $ cd home/256.bzip2
  $ time ./bzip2.rv data/test/input/input.random 2
```

