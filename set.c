#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
 
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
	int  (*RemoveItem)(void* item,unsigned int sizeOfItem);
	int  (*Contains)(void* item, unsigned int sizeOfItem);
	int  (*Count)(void);

    	void (*Union)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet );
        void (*Intersection)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet);
	void (*Difference)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet);
	void (*SymmetricDifference)(void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet);

	struct _DataSet {
		void* item;
		unsigned int size;
		struct list_head;
	}DataSet;
} Set;


typedef struct _Data {
	void* item;
	unsigned int size;
	struct list_head list;
} Data;

int Ctor(Data* dataSet)
{
	INIT_LIST_HEAD(&dataSet->list);     
	return 0;
}

void Add(Data* dataSet, void* item, unsigned int sizeOfItem)
{
	Data *aNewData;

 	aNewData = kmalloc(sizeof(*aNewData), GFP_KERNEL);

	aNewData->item = item;
  	aNewData->size = sizeOfItem;

  	INIT_LIST_HEAD(&aNewData->list);
 	list_add_tail(&(aNewData->list), &(dataSet->list));
}

void AddRange(Data* dataSet, void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet )
{
	Data *aNewData;

	unsigned int i;
	for(i=0; i<sizeOfSet; ++i){
		Add(dataSet, *(items + i*sizeOfOneItem), sizeOfOneItem);
	}
}

int  RemoveItem(Data* dataSet, void* item, unsigned int sizeOfItem)
{
	Data *aData, *tmp;

	list_for_each_entry_safe(aData, tmp, &dataSet->list, list){
		if (aData->item == item) {
			list_del(&aData->list);
 			kfree(aData);
		}
	 }

	return true;
}

int  Contains(Data* dataSet, void* item, unsigned int sizeOfItem)
{

	Data *aData, *tmp;

	list_for_each_entry_safe(aData, tmp, &dataSet->list, list){
		if (aData->item == item) {
			return true;
		}
	 }

	return false;
}

int  Count(Data* dataSet) 
{

	Data *aData, *tmp;
	unsigned int counter = 0;

	list_for_each_entry_safe(aData, tmp, &dataSet->list, list){
		counter++;
	 }

	return counter;
}

void Dtor(Data* dataSet)
{
	
	Data *aData, *tmp;
 	printk(KERN_INFO "deleting the list using list_for_each_entry_safe()n");

	list_for_each_entry_safe(aData, tmp, &dataSet->list, list){
		printk(KERN_INFO "freeing node %dn", aData->item);
		list_del(&aData->list);
 		kfree(aData);
	 }
}

int set_copy(Data *dstSet, Data *srcSet)
{
	
	Data *aData, *tmp;
	list_for_each_entry_safe(aData, tmp, &srcSet->list, list){
		Add(dstSet, srcSet->item, srcSet->size);	
	}

	return true;
}


Data* Union(Data* dataSet, void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet )
{
	Data  *unionSet;
	unsigned int i;
	
	Ctor(unionSet);

	for(i=0; i<sizeOfSet; i++){
		if (!Contains(dataSet, *(items + i*sizeOfOneItem), sizeOfOneItem)) {
			Add(unionSet, *(items + i*sizeOfOneItem), sizeOfOneItem);	
		}
	 }

	return unionSet;
}

Data* Intersection(Data* dataSet, void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet)
{
    	Data* intersectionSet;

	unsigned int i;

	Ctor(intersectionSet);

	for(i=0; i<sizeOfSet; i++){
		if (Contains(dataSet, *(items + i*sizeOfOneItem), sizeOfOneItem)) {
			Add(intersectionSet, *(items + i*sizeOfOneItem), sizeOfOneItem);	
		}
	 }
 
    return intersectionSet;    
}

Data* Difference(Data* dataSet, void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet)
{
	Data *differenceSet;

	Ctor(differenceSet);

	set_copy(differenceSet, dataSet);
 
	unsigned int i;
	for(i=0; i<sizeOfSet; i++){
		RemoveItem(differenceSet, *(items + i*sizeOfOneItem), sizeOfOneItem);	
	 }
 
        return differenceSet;
}

Data* SymmetricDifference(Data* dataSet, void** items, unsigned int sizeOfOneItem, unsigned int sizeOfSet)
{

    Data *intersection = Intersection(dataSet, items, sizeOfOneItem, sizeOfSet);
    
 
    return Difference(intersection, items, sizeOfOneItem, sizeOfSet);    
}

int init_module() {
 
	printk(KERN_INFO "initialize kernel modulen");
  	printk(KERN_INFO "n");
         
   return 0;
 }
                 
void cleanup_module() {

	printk(KERN_INFO "kernel module unloaded.n");
}
