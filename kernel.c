#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/signal.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suhas Venkata");
MODULE_DESCRIPTION("Binary Tree Process Logger Kernel Module");

#define MAX_LEVELS 3

struct tree_node {
    int pid;
    char name[16];
    struct list_head children;
    struct list_head sibling;
};

static struct task_struct *root_thread;
static struct tree_node *root_thread_data;
static int module_initialized = 0; // Flag to check if the module is initialized

static int child_function(void *data) {
    allow_signal(SIGKILL);
    set_current_state(TASK_INTERRUPTIBLE);

    printk(KERN_INFO "Entering child_function\n");

    while (!kthread_should_stop()) {
        schedule();
    }

    set_current_state(TASK_RUNNING);
    printk(KERN_INFO "Exiting child_function\n");

    return 0;
}

static void print_tree(struct tree_node *root, int level) {
    struct tree_node *node;
    struct list_head *pos, *q;

    printk(KERN_INFO "%*s├── %s(%d)\n", level * 4, "", root->name, root->pid);

    list_for_each_safe(pos, q, &root->children) {
        node = list_entry(pos, struct tree_node, sibling);

        // Recursively print the tree structure
        print_tree(node, level + 1);

        // Remove the printed node from the list
        list_del(pos);
        kfree(node);
    }
}

static int create_binary_tree(int level, struct task_struct *parent, struct tree_node *parent_node) {
    int i;
    char thread_name[16];

    if (level >= MAX_LEVELS) {
        return 0;
    }

    for (i = 0; i < 2; ++i) {
        struct task_struct *thread;
        struct tree_node *thread_node;

        snprintf(thread_name, sizeof(thread_name), "thread_%d_%d", level, i);

        // Create a child thread
        thread = kthread_run(child_function, NULL, thread_name);
        if (IS_ERR(thread)) {
            printk(KERN_ERR "Failed to create child thread\n");
            return PTR_ERR(thread); // Return error code
        }

        // Log information about the created process/thread
        printk(KERN_INFO "Created thread: PID=%d, Parent PID=%d, Level=%d\n", thread->pid, parent->pid, level);

        // Create a tree node for the child
        thread_node = kmalloc(sizeof(struct tree_node), GFP_KERNEL);
        if (!thread_node) {
            // Handle memory allocation failure
            return -ENOMEM;
        }
        thread_node->pid = thread->pid;
        snprintf(thread_node->name, sizeof(thread_node->name), "%s", thread_name);
        INIT_LIST_HEAD(&thread_node->children);

        // Add the child to the parent's list
        list_add_tail(&thread_node->sibling, &parent_node->children);

        // Recursively create the thread tree
        create_binary_tree(level + 1, thread, thread_node);
    }

    return 0;
}

static int __init binary_tree_logger_init(void) {
    int ret;

    if (module_initialized) {
        printk(KERN_INFO "Module already initialized\n");
        return 0;
    }

    printk(KERN_INFO "Binary Tree Logger Module: Initialization\n");

    // Create a root process/thread for the binary tree
    root_thread = kthread_run(child_function, NULL, "root_thread");
    if (IS_ERR(root_thread)) {
        printk(KERN_ERR "Failed to create root thread\n");
        return PTR_ERR(root_thread);
    }

    // Create a tree node for the root
    root_thread_data = kmalloc(sizeof(struct tree_node), GFP_KERNEL);
    if (!root_thread_data) {
        // Handle memory allocation failure
        kthread_stop(root_thread);
        return -ENOMEM;
    }
    root_thread_data->pid = root_thread->pid;
    snprintf(root_thread_data->name, sizeof(root_thread_data->name), "root_thread");
    INIT_LIST_HEAD(&root_thread_data->children);

    // Log information about the root thread
    printk(KERN_INFO "Created root thread: PID=%d\n", root_thread->pid);

    // Create a binary tree
    ret = create_binary_tree(1, root_thread, root_thread_data);
    if (ret) {
        // Stop the root thread and free allocated memory in case of error
        kthread_stop(root_thread);
        kfree(root_thread_data);
        return ret;
    }

    // Print the binary tree structure
    printk(KERN_INFO "Process Tree Structure:\n");
    print_tree(root_thread_data, 0);

    module_initialized = 1;

    return 0;
}

static void __exit binary_tree_logger_exit(void) {
    // Stop the entire process tree starting from the root thread
    kthread_stop(root_thread);

    printk(KERN_INFO "Binary Tree Logger Module: Cleanup\n");
}

module_init(binary_tree_logger_init);
module_exit(binary_tree_logger_exit);
