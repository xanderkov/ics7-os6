#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/slab.h>


# define SLABNAME "SLABcache"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kovel Alexander");

static struct kmem_cache *cache = NULL;
static void **cache_mem_area = NULL;

# define MYFS_MAGIC_NUMBER 0x13131313

static void myfs_put_super(struct super_block * sb)
{
    printk(KERN_DEBUG "MYFS super block destroyed!\n" ) ;
}

static struct super_operations const myfs_super_ops = 
{
	.put_super = myfs_put_super,
	.statfs = simple_statfs, 
	.drop_inode = generic_delete_inode, 
};

static int myfs_fill_sb(struct super_block *sb, void *data, int silent) // 
{
    struct inode *root = NULL;
    sb->s_blocksize = PAGE_SIZE; 
    sb->s_blocksize_bits = PAGE_SHIFT; 
    sb->s_magic = MYFS_MAGIC_NUMBER; 
    sb->s_op = &myfs_super_ops;
    root = new_inode(sb); 
    if (!root)
    {
        printk(KERN_ERR "MYFS inode allocation failed !\n");
        return -ENOMEM;
    }
    root->i_mode = S_IFDIR | 0755; 
    root->i_op = &simple_dir_inode_operations; 
    root->i_fop = &simple_dir_operations; 
    sb->s_root = d_make_root(root); //
    if (!sb->s_root)
    {
        printk(KERN_ERR " MYFS root creation failed !\n");
        iput(root);
        return -ENOMEM;
    }
    return 0;
}

static struct dentry *my_mount(struct file_system_type *type, int flags, char const *dev, void *data)
{
    struct dentry* const root_dentry = mount_nodev(type,  flags,  data, myfs_fill_sb);
    if (IS_ERR(root_dentry)) 
        printk(KERN_ERR "MYFS mounting failed !\n");
    else 
        printk(KERN_DEBUG "MYFS mounted!\n");
    return root_dentry;
}

static void my_kill_block_super(struct super_block *sb)
{
    printk(KERN_DEBUG "kill");
    kill_anon_super(sb);
}

static struct file_system_type myfs_type = 
{
    .owner = THIS_MODULE,
    .name = "my_fs",
    .mount = my_mount,
    .kill_sb = my_kill_block_super,
};

static int __init my_init(void)
{
    int ret = register_filesystem(& myfs_type);
    if(ret != 0)
    {
        printk(KERN_ERR "MYFS_MODULE cannot register filesystem!\n");
        return ret;
    }
    if (!(cache_mem_area = kmalloc(sizeof(void *), GFP_KERNEL)))
    {
        printk(KERN_ERR "MYFS_MODULE cannot allocate memory for SLAB cache!\n");
        return ret;
    }
    if (!(cache = kmem_cache_create(SLABNAME, 7, 0, SLAB_HWCACHE_ALIGN, NULL)))
    {
        printk(KERN_ERR "MYFS_MODULE cannot use kmem_cache_create!\n");
        kfree(cache_mem_area);
        return ret;
    }
    if (!(*cache_mem_area = kmem_cache_alloc(cache, GFP_KERNEL)))
    {
        printk(KERN_ERR "MYFS_MODULE cannot use kmem_cache_alloc!\n");
        kfree(cache_mem_area);
        kmem_cache_destroy(cache);
        return ret;
    }
    printk(KERN_DEBUG "MYFS_MODULE loaded !\n");
    return 0;
}

static void __exit my_exit(void)
{
    int ret = unregister_filesystem(&myfs_type); 
    if (ret != 0)
        printk(KERN_ERR "MYFS_MODULE cannot unregister filesystem !\n");
    kmem_cache_free(cache, *cache_mem_area);
    kmem_cache_destroy(cache);
    kfree(cache_mem_area);
    printk(KERN_DEBUG "MYFS_MODULE unloaded !\n");
}

module_init(my_init);
module_exit(my_exit);