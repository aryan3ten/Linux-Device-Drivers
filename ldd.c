#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

// Module Metadata - Embeds information into the module
MODULE_LICENSE("GPL"); //Important to specify license that we are using 
MODULE_AUTHOR("Aryan S"); // Author's Name 
MODULE_DESCRIPTION("Our first dynamically loadable kernel module"); //Small Description Of The Module 

// This pointer stores the address of the proc file entry 
static struct proc_dir_entry *custom_proc_node;

// Function gets called whenever somebody reads > cat/proc/Aryans_Driver
//   > A message is created [Ack!]
//   > Kernel memory now contains A C K ! \n \0 (marks the end of the string)
//   > Strlen finds the length of the msg string (len = 5)
//   > Prints out pyjama_read onto the kernel log buffer
//   > The if statement ensures that the whole msg is read
//   > copy_to_user function allows the kernel to explicitly copy data onto the userspace
// 


static ssize_t pyjama_read(struct file* file_pointer,
                    char *user_space_buffer,
                    size_t count,
                    loff_t* offset){ //Offset exsists becuase linux treats proc entry like a file, and files have a current position 
    char msg[] = "Ack!\n";
    size_t len = strlen(msg);
    int result;
    printk ("pyjama_read\n");

    if (*offset >= len){
        return 0;}


    result = copy_to_user(user_space_buffer, msg, len);
    *offset += len;
    
    return len; //Tells kernel that len bytes were successfully copied to the user space
}



// pyjama_read function called when there is an attempt to read the proc file
struct proc_ops driver_proc_ops = {
    .proc_read = pyjama_read
};

// This is the standard init_function 
//Returns a 0 (that's why it is int.)
//static keywords are only accessible from within the file whereas normal int can be accessed by extern int

static int pyjama_module_init(void){
    printk("pyjama_module_init: entry\n"); //Prints this to the kernel's internal log

    custom_proc_node = proc_create("Aryans_Driver", // Creates proc entry
                                 0, 
                                NULL, 
                                &driver_proc_ops);

    printk("pyjama_module_init: exit\n");
    return 0;
} 




// This is the standard exit_function
// Does not return anything and does not take any parameters
static void pyjama_module_exit(void){

    printk("pyjama_module_exit: entry\n");

    // Removes the proc entry
    proc_remove(custom_proc_node);

    printk("pyjama_module_exit: exit\n");
}





// These both are macros
module_init(pyjama_module_init); //Function pointer to perform init function
module_exit(pyjama_module_exit); //Function pointer to perform exit function 