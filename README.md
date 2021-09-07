# FlexFilt
Flexible Instruction Filtering for Security.

## Overview
In this document, we provide a guideline for running experiments on FlexFilt. For more details on FlexFilt please refer to our paper.
We run our experiments on the Xilinx Zynq Zedboard FPGA and use a modified Linux kernel 4.15 to provide the support for our hardware.

## Accessing the experiment environment
To simplify the process of testing FlexFilt, we provide you with access to our FPGA boards.
We have already created a guest account on one of our machines and provided you the private ssh key (please refer to the “Additional Comments” in the artifact evaluation submission website).
You can ssh to our machine after configuring the private ssh key:

```  
  $ ssh flexfilt@icsg-04.bu.edu
```

You have access to two of our FPGAs available at /dev/ttyACM0 and /dev/ttyACM1.
For simplicity, in the rest of this document we call the FPGA connected to /dev/ttyACM0 as ACM0 and the one connected to /dev/ttyACM1 as ACM1.
We suggest you to use screen for connecting to these two FPGAs (we have provided you screen access to these two nodes).
However, if you want to do ssh or scp to ACM0 and ACM1, their ip addresses are 192.168.1.30 and 192.168.1.31, respectively.
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
The first program prevents the execution of WRPKRU instruction anywhere in the default domain.
The second program follows the scenario described in Section 7.2 of the paper.
In this scenario, we have two trusted functions, i.e., good_code1 and good_code2, that are allowed to execute WPKRU extended instruction.
The execution of WRPKR instruction should be prevented in the rest of the program.
As described in Section 5.3 of the paper, we assign the same ipkey value to both trusted functions and then configure FlexFilt to prevent the executoin of WRPKRU in the default domain.
We use LD_PRELOAD to obtain the address range of the two trusted functions, and then invok pkey_mprotect to associate them with the same instruction protection key.

To run these two test programs, please run the filter script (filter.sh) on ACM1:

```
  $ ./filter.sh
```

To demonstrate FlexFilt's capability in preventing a security attack (Section 7.2 of the paper), we provide two sample programs with buffer overflow vulnerabilities.
We provide an input for these two programs leveraging the buffer overflow vulnerabilities to execute a function that is never called in the program.
This function executes a WRPKR instruction to modify the permission bits of a protection domain.
We Configure FlexFilt to prevent the exuction of WRPKR instruction in the default domain.

To run these two test programs with FlexFilt's protection, run the script (vuln.sh) on ACM1:

```
  $ ./vuln.sh
```

### Performance Evaluation
To demonstrate the negligble performance overhead of FlexFilt, we provide the executables for running two spec2000 benchmark applications (i.e., bzip2 and gcc) on the baseline Rocket core and the core enhanced with FlexFilt.
To run the baseline experiments, run the spec script on ACM0:

```
  $ ./spec_baseline.sh
```

To run the FlexFilt experiments, run the spec script on ACM1:

```
  $ ./spec_flexfilt.sh
```

