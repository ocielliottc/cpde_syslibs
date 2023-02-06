# Building with MPC

First, set the required environment variables.  op_lite requires msgpack-c and boost.  Set the following to values for your build system, for example:

```
export BOOST_ROOT=/c/Users/elliottc/boost_1_70_0
export MSGPACK_ROOT=/c/Users/elliottc/msgpack-c
```
Next, run MPC in the op_lite directory.

```
cd cpde_syslibs/op_lite
$MPC_ROOT/mwc.pl -features boost=1 -genins -type vs2019
```
Then build with Visual Studio 2019.

# Installing with MPC

Install op_lite using the project installer that comes with MPC.

```
$MPC_ROOT/prj_install.pl <full install path> op_lite.ins -b header_files=include -b lib_output=lib -a lib_output
```
