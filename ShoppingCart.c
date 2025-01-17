#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ShoppingCart.h"
#include "General.h"
#include "list.h"

void	initCart(ShoppingCart* pCart)
{
	L_init(&(pCart->shoppingItemList));
}

float	getTotalPrice(const ShoppingCart* pCart)
{
	float price = 0;
	ShoppingItem* pItem;
	NODE* temp = pCart->shoppingItemList.head.next;		//??are we point on the first??
	while (temp != NULL);
	{
		pItem = temp->key;
		price += (pItem->price * pItem->count);
		temp = temp->next;
	}
	return price;
}

int		addItemToCart(ShoppingCart* pCart, char* barcode, float price, int count)
{
	ShoppingItem* pItem = getItemByBarocde(pCart, barcode);
	if (!pItem) //new item
	{
		pItem = createItem(barcode, price, count);
		if (!pItem)
			return 0;

		//ShoppingItem** tempArr = (ShoppingItem**)realloc(pCart->itemArr, (pCart->count + 1) * sizeof(ShoppingItem*));
		NODE* tempNode = whereToPut(pCart, barcode);
		if (!L_insert(tempNode, pItem)) {
			freeShoppingCart(pCart);
			return 0;
		}
		//pCart->itemArr = tempArr;

		//pCart->itemArr[pCart->count] = pItem;
		//pCart->count++;
		

	}
	else {
		pItem->count += count;
	}
	return 1;
}

float	printShoppingCart(const ShoppingCart* pCart)
{
	//ShoppingItem* pItem;
	//NODE* pCartTemp = pCart->shoppingItemList.head.next;
	float price = 0;
	L_print(&(pCart->shoppingItemList), printItem);
	/*
	while (pCartTemp != NULL)
	{
		pItem = pCartTemp->key;
		printItem(pItem);
		price += (pItem->price * pItem->count);
		pCartTemp = pCartTemp->next;
	}
	*/
	price = getTotalPrice(pCart);
	printf("Total bill to pay: %.2f\n", price);
	return price;
}


ShoppingItem*	getItemByBarocde(ShoppingCart* pCart, char* barcode)
{
	NODE* temp = L_find(&(pCart->shoppingItemList.head),barcode, compareItemByBarcode);

	return temp->key;
	/*
	for (int i = 0; i < pCart->count; i++)
	{
		if (strcmp(pCart->itemArr[i]->barcode,barcode) == 0)
			return pCart->itemArr[i];
	}
	return NULL;
	*/
}


void	freeShoppingCart(ShoppingCart* pCart)
{
	L_free(&(pCart->shoppingItemList),NULL);
}


NODE* whereToPut(ShoppingCart* pCart, char* barcode)
{
	NODE* temp = pCart->shoppingItemList.head.next;
	ShoppingItem* pItem = temp->key;

	if (!temp)
	{
		//private case for 1 place
		if (strcmp(pItem->barcode, barcode) > 0)
			return &(pCart->shoppingItemList.head);
		while (!(temp->next))
		{
			pItem = temp->next->key;
			if (strcmp(pItem->barcode, barcode) > 0)
				break;
			temp = temp->next;
		}
	}
	return temp;
}


int compareItemByBarcode(void* item, void* barcode)
{
	const char* sIBarcode = ((ShoppingItem*)item)->barcode;
	const char* currBarcode = (char*)barcode;
	return strcmp(sIBarcode,currBarcode);
}