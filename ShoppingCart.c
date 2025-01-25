#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ShoppingCart.h"
#include "General.h"
#include "list.h"

void	initCart(ShoppingCart* pCart)
{
	if (!pCart)
		return;  

	if (!L_init(&(pCart->shoppingItemList)))
		{
			printf("Failed to initialize shopping cart!\n");
			return;
		}
}

float	getTotalPrice(const ShoppingCart* pCart)
{
	float price = 0;
	ShoppingItem* pItem;
	NODE* temp = pCart->shoppingItemList.head.next;	
	if (temp == NULL)
		return price;

	while (temp != NULL)
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

		NODE* tempNode = whereToPut(pCart, barcode);
		if (!L_insert(tempNode, pItem)) {
			freeShoppingCart(pCart);
			return 0;
		}
	}
	else {
		pItem->count += count;
	}
	return 1;
}

float	printShoppingCart(const ShoppingCart* pCart)                
{

	float price = 0;
	L_print(&(pCart->shoppingItemList), printItem);
	price = getTotalPrice(pCart);
	printf("Total bill to pay ");
	return price;
}


ShoppingItem*	getItemByBarocde(ShoppingCart* pCart, char* barcode)
{  
	NODE* temp = &pCart->shoppingItemList.head;
	if (!temp->next)
		return NULL;
	NODE* temp2 = L_find(temp->next,barcode, compareItemByBarcode);
	if (!temp2)
		return NULL;
	return temp2->key;
}


void	freeShoppingCart(ShoppingCart* pCart)
{
	L_free(&(pCart->shoppingItemList),NULL);
}


NODE* whereToPut(ShoppingCart* pCart, char* barcode)
{
	NODE* prev = &(pCart->shoppingItemList.head);
	NODE* temp = prev->next;

	while (temp != NULL)
	{
		ShoppingItem* pItem = temp->key;

		if (strcmp(pItem->barcode, barcode) > 0)
			break;

		prev = temp;
		temp = temp->next;
	}

	return prev; 
}



int compareItemByBarcode(void* item, void* barcode)
{
	const char* sIBarcode = ((ShoppingItem*)item)->barcode;
	const char* currBarcode = (char*)barcode;
	return strcmp(sIBarcode,currBarcode);
}