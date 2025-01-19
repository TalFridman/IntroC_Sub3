#pragma once
#include "ShoppingItem.h"
#include "list.h"

typedef struct
{
	LIST	shoppingItemList;
}ShoppingCart;


void			initCart(ShoppingCart* pCart);
float			getTotalPrice(const ShoppingCart* pCart);
ShoppingItem*	getItemByBarocde(ShoppingCart* pCart, char* barcode);
int				addItemToCart(ShoppingCart* pCart, char* name, float price, int count);
float			printShoppingCart(const ShoppingCart* pCart);

NODE* whereToPut(ShoppingCart* pCart, char* barcode);
int compareItemByBarcode(void* item, void* barcode);

void			freeShoppingCart(ShoppingCart* pCart);