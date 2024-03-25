# Kernel_Module

My fourth semester operating systems mini-project where made a kernel module using the C programming language


To deploy the code provided, you'll first need to compile it into a kernel module and then insert it into a running Linux kernel. Here's a breakdown of the process:

1. **Create Kernel Module File**: Copy the supplied code into a file, for example, binary_tree_logger.c.

2. **Makefile Creation**: You'll also require a Makefile to build the module. Create a file named Makefile alongside binary_tree_logger.c, containing the necessary directives for compilation.

3. **Compilation**: Navigate to the directory containing your files and execute the make command. This will compile the kernel module, generating a file named binary_tree_logger.ko if successful.

4. **Module Insertion**: Once compiled, insert the module into the kernel using the insmod command prefixed with sudo for appropriate permissions.

5. **Kernel Log Viewing**: Check the kernel logs using the dmesg command to observe the initialization messages printed by the module.

6. **Module Removal**: When done, remove the module from the kernel with sudo rmmod binary_tree_logger.

7. **Cleaning Up**: Optionally, to remove compiled files, execute make clean.

It's vital to exercise caution when working with kernel modules, ensuring appropriate permissions and attentiveness to prevent potential system crashes due to incorrect implementation.
