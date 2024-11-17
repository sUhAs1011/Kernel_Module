# Kernel Module: Fourth Semester Operating Systems Assignment


### Overview
This project, developed as part of the fourth-semester Operating Systems mini-project, is a kernel module written in C. The module logs specific messages within the Linux kernel, providing insights into kernel behavior.

### Deployment Steps

1. **Prepare the Kernel Module Code**
   - Save the provided code in a file, for example, `binary_tree_logger.c`.

2. **Create a Makefile**
   - Place a `Makefile` alongside `binary_tree_logger.c` with directives to build the module. A typical Makefile might include:
     ```makefile
     obj-m += binary_tree_logger.o

     all:
         make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

     clean:
         make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
     ```

3. **Compile the Kernel Module**
   - In the terminal, navigate to the directory containing your files.
   - Run:
     ```shell
     make
     ```
   - This generates `binary_tree_logger.ko`, the compiled kernel module.

4. **Insert the Module**
   - Insert the module into the kernel with:
     ```shell
     sudo insmod binary_tree_logger.ko
     ```

5. **View Kernel Logs**
   - Check the initialization messages and logs by viewing the kernel logs:
     ```shell
     dmesg | tail
     ```

6. **Remove the Module**
   - To remove the module safely:
     ```shell
     sudo rmmod binary_tree_logger
     ```

7. **Clean Up**
   - Optionally, remove compiled files with:
     ```shell
     make clean
     ```

### Caution
Working with kernel modules requires superuser permissions. Ensure correctness in the module’s implementation to prevent system instability.
