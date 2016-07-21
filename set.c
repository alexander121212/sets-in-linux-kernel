#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Set");
MODULE_AUTHOR("Nester");

#define true  1
#define false 0

typedef struct _Set{

	void (*Ctor)(void);
	void (*Dtor)(void);

	void (*Add)(void* item, unsigned int sizeOfItem);
	void (*AddRange)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet );
	int  (*RemoveItem)(void* item, sizeOfItem);
	int  (*Contains)(void* item, unsigned int sizeOfItem);
	int  (*Count)(void);

    	void (*Union)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet );
        void (*Intersection)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet);
	void (*Difference)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet);
	void (*SymmetricDifference)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet);

	struct _Data {
		void* item;
		unsigned int size;
		struct list_head;
	}Data;
} Set;


typedef struct _Data {
	void* item;
	unsigned int size;
	struct list_head;
} Data;

Data *g_DataSet;

int Ctor(void)
{
	INIT_LIST_HEAD(&g_DataSet.list);     
	return 0;
}

void Add(void* item, unsigned int sizeOfItem)
{
	Data *aNewData;

 	aNewData = kmalloc(sizeof(*aNewData), GFP_KERNEL);

	aNewData->item = item;
  	aNewData->size = sizeOfItem;

  	INIT_LIST_HEAD(&aNewData->list);
 	list_add_tail(&(aNewData->list), &(g_DataSet.list));
}

void AddRange(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet )
{
	Data *aNewData;

	for(i=0; i<sizeOfSet; ++i){
       		aNewData = kmalloc(sizeof(*aNewData), GFP_KERNEL);

		aNewData->item = *items[i];
  		aNewData->size = sizeOfItem;

       		INIT_LIST_HEAD(&aNewData->list);
 		list_add_tail(&(aNewData->list), &(g_DataSet.list));
	}

}

int  RemoveItem(void* item, sizeOfItem)
{
	Data *aData, *tmp;

	list_for_each_entry_safe(aData, tmp, &g_DataSet.list, list){
		if (aData->item == item) {
			list_del(&aData->list);
 			kfree(aData);
		}
	 }

	return true;
}

int  Contains(void* item, unsigned int sizeOfItem)
{

	Data *aData, *tmp;

	list_for_each_entry_safe(aData, tmp, &g_DataSet.list, list){
		if (aData->item == item) {
			return true;
		}
	 }

	return false;
}

int  Count(void)
{

	Data *aData, *tmp;
	unsigned int counter = 0;

	list_for_each_entry_safe(aData, tmp, &g_DataSet.list, list){
		counter++;
	 }

	return counter;
}

void Dtor(void)
{
	
	Data *aData, *tmp;
 	printk(KERN_INFO "deleting the list using list_for_each_entry_safe()n");

	list_for_each_entry_safe(aData, tmp, &g_DataSet.list, list){
		printk(KERN_INFO "freeing node %sn", aData->name);
		list_del(&aData->list);
 		kfree(aData);
	 }
}


void Union(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet )
{
	Data  *aData, *tmp;
	unsigned int i;

	for(i=0; i<sizeOfSet)
		if (!Contains(*item[i])) {
			Add(*item, sizeOfOneItem);	
		}
	 }
 
}

void Intersection(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet)
{
	
}

void Difference(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet)
{
	
}

void SymmetricDifference(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet)
{
	
}

int init_module() {
 
	printk(KERN_INFO "initialize kernel modulen");
	Ctor(); 


 	list_for_each_entry(aData, &g_DataSet.list, list) {
	 	printk(KERN_INFO "Data: %s; weight: %d; gender: %sn", aData->name, aData->weight, aData->gender==0?"Female":"Male");
	}

  	printk(KERN_INFO "n");
         
   return 0;
 }
                 
void cleanup_module() {

	printk(KERN_INFO "kernel module unloaded.n");
	Dtor();
}
