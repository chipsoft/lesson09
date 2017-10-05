#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/delay.h>

// Число итераций цикла
#define CYCLES_CNT 1000000

MODULE_AUTHOR("Denis Suprunenko <chipsoft@gmail.com");
MODULE_DESCRIPTION("Thread demo, Module 09");
MODULE_LICENSE("Dual BSD/GPL");

u32 global_counter = 0;
volatile unsigned long addr = 0;

void lock(void)
{
	while (test_and_set_bit(0, &addr) != 0);
}

void unlock(void)
{
	if (!test_and_clear_bit(0, &addr)) printk(KERN_ERR "Something went wrong");
}

// Функция, которая выполняется в потоке
static int thread_func2( void* data ) {
	u32 num = CYCLES_CNT;
	printk(KERN_INFO "thread_func2: start");
	u32 i;
	for(i = 0; i < num; ++i) {
		lock();
		global_counter++;
		unlock();
		if (global_counter % 10000 == 0) mdelay(1);
	}
	//printk(KERN_INFO "thread_func2, global_counter = %d\n", global_counter);
	//printk(KERN_INFO "thread_func2 exit\n");
	return 0;
}

// Функция, которая выполняется в потоке
static int thread_func1( void* data ) {
	u32 num = CYCLES_CNT;
	printk(KERN_INFO "thread_func1: start");
	u32 i;
	for(i = 0; i < num; ++i) {
		lock();
		global_counter++;
		unlock();
		if (global_counter % 10000 == 0) mdelay(1);
	}
	//printk(KERN_INFO "thread_func1, global_counter = %d\n", global_counter);
	//printk(KERN_INFO "thread_func1 exit\n");
	return 0;
}

static int __init hello_init(void)
{
	printk(KERN_INFO "-> Module 09 load!\n");
	kthread_run(thread_func1, NULL, "cycle1");
	kthread_run(thread_func2, NULL, "cycle2");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "global_counter = %d\n", global_counter);
	printk(KERN_INFO "<- Module 09 unload!\n");
}

module_init(hello_init);
module_exit(hello_exit);
