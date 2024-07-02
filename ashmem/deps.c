#include <linux/mm.h>
#include <linux/kallsyms.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
#define KPROBE_LOOKUP 1
#include <linux/kprobes.h>
static struct kprobe kp = {
	.symbol_name = "kallsyms_lookup_name"
};
#endif

typedef int (*shmem_zero_setup_ptr_t)(struct vm_area_struct *);
static shmem_zero_setup_ptr_t shmem_zero_setup_ptr = NULL;
int shmem_zero_setup(struct vm_area_struct *vma)
{
#ifdef KPROBE_LOOKUP
	typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
	kallsyms_lookup_name_t kallsyms_lookup_name;
	register_kprobe(&kp);
	kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
	unregister_kprobe(&kp);
#endif

	if (!shmem_zero_setup_ptr)
		shmem_zero_setup_ptr = (shmem_zero_setup_ptr_t) kallsyms_lookup_name("shmem_zero_setup");
	return shmem_zero_setup_ptr(vma);
}

